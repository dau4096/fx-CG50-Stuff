extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}
#include "utils.h"
#include "graphics.h"
#include <string.h>
using namespace std;
using namespace utils;



namespace graphics {


Mat4 viewMatrix(Camera camera) {
	Vec3 forward = Vec3(
		sin(camera.angle.x) * cos(camera.angle.y),
		cos(camera.angle.x) * cos(camera.angle.y),
		sin(camera.angle.y)
	).normalise();

	Vec3 right = forward.cross(Vec3(0.0f, 0.0f, 1.0f)).normalise();
	Vec3 up = right.cross(forward).normalise();
	return Mat4(
		 right.x,						 up.x,						-forward.x,						0.0f, 
		 right.y,						 up.y,						-forward.y,						0.0f,
		 right.z,						 up.z,						-forward.z,						0.0f,
		-right.dot(camera.position),	-up.dot(camera.position),	-forward.dot(camera.position),	1.0f
	);
}


Mat4 projectionMatrix(Camera camera) {
	float F = 1.0f / tan(camera.fov / 2.0f);
	float dist = camera.near - camera.far;
	return Mat4(
		F/display::ASPECT_RATIO,	0.0f,	0.0f,								 0.0f,
		0.0f,						F,		0.0f,								 0.0f,
		0.0f,						0.0f,	(camera.far+camera.near)/dist,		-1.0f,
		0.0f,						0.0f,	(2.0f*camera.far*camera.near)/dist,	 0.0f
	);
}




Vec4 project(Vec3 vertex, Mat4& pvmMatrix) {
	Vec4 vertexV4 = Vec4(vertex, 1.0f);
	Vec4 NDC = vertexV4 * pvmMatrix;
	float NDCw = maths::max(1e-3f, NDC.w);
	NDC /= NDCw;
	return Vec4(
		(NDC.x + 1.0f) / 2.0f * LCD_WIDTH_PX,
		(1.0f - NDC.y) / 2.0f * LCD_HEIGHT_PX,
		NDC.z * NDCw, NDCw
	);
}



void projectVertices(Mat4& pvmMatrix) {
	//Project vertices.
	for (unsigned int vIndex=0u; vIndex<=display::MAX_VERTICES; vIndex++) {
		Vec3 vertex = data::vertices[vIndex];
		Vec4 proj = project(vertex, pvmMatrix);
		data::projectedVertices[vIndex] = proj;
	}
}


void sortVerts(Vec4 (*verts)[3]) {
	//Very crude sorting of 3 vertices. Not intended to scale, just be fast and simplistic.
    Vec4& A = (*verts)[0];
    Vec4& B = (*verts)[1];
    Vec4& C = (*verts)[2];

    //Swap A and B if B is lower
    if (A.y > B.y) {
        Vec4 temp = A;
        A = B;
        B = temp;
    }

    //Same for A and C
    if (A.y > C.y) {
        Vec4 temp = A;
        A = C;
        C = temp;
    }

    //Same for B and C
    if (B.y > C.y) {
        Vec4 temp = B;
        B = C;
        C = temp;
    }
}



void handleEdges(
		Edge* (*edgeAdditions)[LCD_HEIGHT_PX][display::MAX_EDGES],
		Edge* (*edgeRemovals)[LCD_HEIGHT_PX][display::MAX_EDGES],
		Vec2 (*edgeCounts)[LCD_HEIGHT_PX]
	) {
	//Create edges.
	size_t tIndex = 0;
	for (Vec4 compIndex : data::indices) {
		Vec4 A = data::projectedVertices[static_cast<int>(compIndex.x)];
		Vec4 B = data::projectedVertices[static_cast<int>(compIndex.y)];
		Vec4 C = data::projectedVertices[static_cast<int>(compIndex.z)];
		int triIndex = static_cast<int>(compIndex.w);


		Vec2 AB = Vec2(B - A);
		Vec2 AC = Vec2(C - A);
		Vec2 An = Vec2(AC.y, -AC.x);
		bool isBackface = An.dot(AB) <= 0.0f;
		if (isBackface && !dev::DRAW_BACKFACES) {
			if constexpr (dev::REQUIRES_EDGES) {
				triIndex = -1u;
			} else {
				//This is a backface; and should not draw anything.
				Edge invalEdge = Edge(
					Vec3(),	Vec3(),	0u, false
				);
				unsigned int startIDX = (tIndex * 3u);
				data::edges[startIDX + 0u] = invalEdge;
				data::edges[startIDX + 1u] = invalEdge;
				data::edges[startIDX + 2u] = invalEdge;		
				tIndex++;
				continue;
			}
		}


		//Mark edges as screenspace left (starting a triangle) or right (ending a triangle)
		Vec4 verts[3] = {A,B,C};
		//std::sort(verts.begin(), verts.end(), [](auto& v1, auto& v2) {return v1.y < v2.y;});
		sortVerts(&verts);


		Edge longEdge = Edge(verts[0], verts[2], triIndex, true);
		Edge shortEdgeA = Edge(verts[0], verts[1], triIndex, false);
		Edge shortEdgeB = Edge(verts[1], verts[2], triIndex, false);

		float v1X = maths::round(verts[1].x);
		longEdge.calculateYScanValues(static_cast<size_t>(maths::round(verts[1].y)));
		if (v1X < longEdge.currentX) {
			shortEdgeA.isLeftEdge = true; 
			shortEdgeB.isLeftEdge = true;
			longEdge.isLeftEdge = false;
		} else {
			shortEdgeA.isLeftEdge = false; 
			shortEdgeB.isLeftEdge = false;
			longEdge.isLeftEdge = true;
		}

		unsigned int startIDX = (tIndex * 3u);
		data::edges[startIDX + 0u] = longEdge;
		data::edges[startIDX + 1u] = shortEdgeA;
		data::edges[startIDX + 2u] = shortEdgeB;

		tIndex++;
	}



	//Add edge start/ends to the relevant datasets.
	for (Edge& e : data::edges) {
		int yMin = maths::clamp(static_cast<int>(maths::round(e.start.y)), 0, LCD_HEIGHT_PX-1);
		int yMax = maths::clamp(static_cast<int>(maths::round(e.end.y + 1)), 0, LCD_HEIGHT_PX-1);
		if (yMin == yMax) {
			continue; //Ignore horizontal edges.
		}

		//Find start of edge and add to relevant line of the additions vector.
		if ((yMin >= 0) && (yMin < LCD_HEIGHT_PX)) {
			(*edgeAdditions)[yMin][static_cast<int>(((*edgeCounts)[yMin].x)++)] = &e;
		}

		//Find end of edge and add to relevant line of the additions vector.
		if ((yMax >= 0) && (yMax < LCD_HEIGHT_PX)) {
			(*edgeRemovals)[yMax][static_cast<int>(((*edgeCounts)[yMin].y)++)] = &e;
		}
	}
}


unsigned int triangleStackLength = 0u;
TriData triangleStack[display::MAX_TRIANGLES];
TriData* getActiveTriangle() {
	if (triangleStackLength > 0u) {
		return &triangleStack[0]; // pointer to real object in stack
	}
	return nullptr;
}

bool insert(unsigned int insIndex, const TriData& data) {
	if ((triangleStackLength >= display::MAX_TRIANGLES) || (insIndex > triangleStackLength)) {return false;}

	if (insIndex < triangleStackLength) {
		memmove(
			&triangleStack[insIndex + 1],	//Dest
			&triangleStack[insIndex],		//SRC
			sizeof(TriData) * (triangleStackLength - insIndex) //Size, Bytes.
		);
	}

	triangleStack[insIndex] = data;
	++triangleStackLength;
	return true;
}

bool remove(unsigned int rmIndex) {
	if (rmIndex >= triangleStackLength) {return false;}

	if (rmIndex < triangleStackLength - 1) {
		memmove(
			&triangleStack[rmIndex],         //Dest
			&triangleStack[rmIndex + 1],     //SRC
			sizeof(TriData) * (triangleStackLength - rmIndex - 1) //Size, Bytes.
		);
	}

	--triangleStackLength;
	return true;
}

bool manageStack(Edge* thisEdge, Span* thisSpan, unsigned int yScan) {
	TriData* activeTriangle = getActiveTriangle();
	bool hasActiveTriangle = activeTriangle != nullptr;
	if (thisEdge->isLeftEdge) {
		//Edge starts new triangle.
		TriData newTriangle = TriData(
			thisEdge->currentX, LCD_WIDTH_PX-1u,
			thisEdge->triIndex, thisEdge->currentZ
		);
		if (!hasActiveTriangle) { //No triangles are currently active.
			triangleStack[triangleStackLength] = newTriangle;
			triangleStackLength++;
		} else {
			//Must decide whether to occlude or be occluded by active triangle.
			if (newTriangle.depth < activeTriangle->depth) {
				activeTriangle->endX = thisEdge->currentX;
				*thisSpan = Span(
					*activeTriangle, yScan
				);

				//New triangle occludes old. Create span for old and add new to start of stack.
				insert(0u, newTriangle);
				return true;

			} else {
				//Add triangle to stack, in order of depth.
				bool didAddTriangle = false;
				for (unsigned int index=0u; index<display::MAX_TRIANGLES; index++) {
					if (triangleStack[index].depth > newTriangle.depth) {
						insert(index, newTriangle);
						didAddTriangle = true;
						break;
					}
				}

				if (!didAddTriangle) {
					triangleStack[triangleStackLength++] = newTriangle;
				}
			}
		}
	} else {
		//Edge ends a triangle.
		bool foundTriangle = false;
		for (unsigned int index=0u; index<display::MAX_TRIANGLES; index++) {
			TriData thisTri = triangleStack[index];
			if (static_cast<int>(thisTri.triIndex) == thisEdge->triIndex) {
				//The same triangle that this edge closes.
				if (index == 0u) {
					thisTri.endX = thisEdge->currentX;
					if ((index+1) < display::MAX_TRIANGLES) {
						triangleStack[index+1].startX = thisEdge->currentX;
					}
					*thisSpan = Span(
						thisTri, yScan
					);
					foundTriangle = true;
				}
				remove(index);
				break;
			}
		}
		return foundTriangle;
	}
	return false;
}


unsigned int latestRNG = 0u;
unsigned short rngColourList[display::MAX_TRIANGLES];
void createRandomColours() {
	for (unsigned int index=0u; index<display::MAX_TRIANGLES; index++) {
		rngColourList[index] = utils::createColour(
			utils::getRNG(), utils::getRNG(), utils::getRNG()
		);
	}
}

void createSpans(
		Edge* (*edgeAdditions)[LCD_HEIGHT_PX][display::MAX_EDGES],
		Edge* (*edgeRemovals)[LCD_HEIGHT_PX][display::MAX_EDGES],
		Vec2 (*edgeCounts)[LCD_HEIGHT_PX]
	) {
	unsigned int numActiveEdges = 0u, numSpans = 0u;
	Edge* activeEdgesList[display::MAX_EDGES]; //Active edges, based on the above 2 vectors.
	Span spanStack[LCD_WIDTH_PX];

	for (unsigned int yScan=0u; yScan<LCD_HEIGHT_PX; yScan++) {
		//Not identical to clearing them, but it acts like it did.
		triangleStackLength = 0u; numSpans = 0u;

		//Add new lines that start on this scanline.
		for (unsigned int idx=0u; idx<(*edgeCounts)[yScan].x; idx++) {
			activeEdgesList[numActiveEdges++] = (*edgeAdditions)[yScan][idx];
		}

		//Remove lines that stop on this scanline
		for (unsigned int idx=0u; idx<(*edgeCounts)[yScan].y; idx++) {
			Edge* thisEdge = (*edgeRemovals)[idx];
			auto it = std::find(activeEdgesList.begin(), activeEdgesList.end(), thisEdge);
			if (it != activeEdgesList.end()) {
				activeEdgesList.erase(it);
			}
		}

		//Handle the lines?
		if (numActiveEdges < 1u) {continue; /* No active edges for this scanline. */}

		for (Edge* thisEdge : activeEdgesList) {
			thisEdge->calculateYScanValues(yScan);
			if constexpr (dev::REQUIRES_EDGES) {
				utils::drawPixel(thisEdge->currentX, yScan, ((thisEdge->isLeftEdge) ? RGB565::EDGE_COLOUR_L : RGB565::EDGE_COLOUR_R));
			}
		}
		if constexpr (!(dev::DRAW_WIREFRAME)) {
			//Sort by left-to-right onscreen.
			std::sort(activeEdgesList.begin(), activeEdgesList.end(), compareEdges);


			for (Edge* thisEdge : activeEdgesList) {
				if ((thisEdge->triIndex == -1)) {continue;}
				Span thisSpan;
				bool success = manageStack(thisEdge, &thisSpan, yScan);
				if (!success) {continue; /* Tri was not in stack. */}
				spanStack[numSpans++] = thisSpan;
			}


			for (Span& thisSpan : spanStack) {
				utils::drawSpan(thisSpan, rngColourList[thisSpan.triIndex]);
			}
		}

		if constexpr (dev::REQUIRES_EDGES) {
			for (Edge* thisEdge : activeEdgesList) {				
				frameBuffer.setPX(
					thisEdge->currentX, yScan,
					((thisEdge->isLeftEdge) ? glm::uvec3(255u, 255u, 127u) : glm::uvec3(127u, 255u, 255u))
				);
			}
		}
	}
}



void drawScene(Camera& camera) {
	Mat4 pMatrix = projectionMatrix(camera);
	Mat4 vMatrix = viewMatrix(camera);
	Mat4 mMatrix = mat4Identity();

	Mat4 pvmMatrix = pMatrix * vMatrix * mMatrix;


	//Quake span-style rendering process
	projectVertices(pvmMatrix);

	std::array<std::vector<Edge*>, display::RENDER_RESOLUTION.y> edgeAdditions;
	std::array<std::vector<Edge*>, display::RENDER_RESOLUTION.y> edgeRemovals;
	handleEdges(&edgeAdditions, &edgeRemovals);
	createSpans(&edgeAdditions, &edgeRemovals);

}


}