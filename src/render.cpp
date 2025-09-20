extern "C" {
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/display.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/keyboard.h"
	#include "C:\Users\User\Documents\code\.cpp\PrizmSDK-win-0.6\include\fxcg/rtc.h"
}
#include "utils.h"
#include "render.h"
#include <string.h>
using namespace std;
using namespace utils;



namespace render {


Mat4 viewMatrix(Camera camera) {
	Vec3 forward = Vec3(
		sin(camera.angle.x) * cos(camera.angle.y),
		cos(camera.angle.x) * cos(camera.angle.y),
		sin(camera.angle.y)
	).normalise();

	Vec3 right = cross(forward, Vec3(0.0f, 0.0f, 1.0f)).normalise();
	Vec3 up = cross(right, forward).normalise();
	return Mat4(
		 right.x,						 up.x,						-forward.x,						0.0f, 
		 right.y,						 up.y,						-forward.y,						0.0f,
		 right.z,						 up.z,						-forward.z,						0.0f,
		-dot(right,camera.position),	-dot(up,camera.position),	-dot(forward,camera.position),	1.0f
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




Vec2 project(Vec3 vertex, Camera& camera, Mat4& pvmMat) {
	float w = pvmMat[3]*vertex.x+pvmMat[7]*vertex.y+pvmMat[11]*vertex.z+pvmMat[15];
	if (abs(w) < EPSILON) {return INVALIDv2;}
	Vec3 ndc = Vec3(
		(pvmMat[0]*vertex.x+pvmMat[4]*vertex.y+pvmMat[8]*vertex.z+pvmMat[12]) / w,
		(pvmMat[1]*vertex.x+pvmMat[5]*vertex.y+pvmMat[9]*vertex.z+pvmMat[13]) / w,
		(pvmMat[2]*vertex.x+pvmMat[6]*vertex.y+pvmMat[10]*vertex.z+pvmMat[14]) / w
	);
	if (ndc.x < -1 || ndc.x > 1 || ndc.y < -1 || ndc.y > 1 || ndc.z < -1 || ndc.z > 1) {return INVALIDv2;}
	return Vec2(
		(ndc.x + 1.0f) / 2.0f * LCD_WIDTH_PX,
		LCD_HEIGHT_PX * (1.0f - ((ndc.y + 1.0f) / 2.0f))
	);
}


TriScreen createTriScreen(size_t triIdx, Camera& camera, Mat4& pvmMat, Vec3 chosenCol=INVALIDv3) {
	Vec3 index = data::indices[triIdx];
	Vec2 projVertices[3];
	for (int vIdx=0; vIdx<3; vIdx++) {
		Vec3 thisVertex = data::vertices[int(index[vIdx])];
		projVertices[vIdx] = project(thisVertex, camera, pvmMat);
		if (projVertices[vIdx] == INVALIDv3) {
			return TriScreen();
		}
	}
	if (chosenCol == INVALIDv3) {
		chosenCol = Vec3(
			utils::getRNG(), utils::getRNG(), utils::getRNG()
		); //Randomised colour.
	}
	return TriScreen(projVertices, createColour(chosenCol));
}


bool inTri(Vec2 point, const TriScreen& triS) {
    float d1 = (point.x - triS.vertices[0].x) * triS.edges[0].y - (point.y - triS.vertices[0].y) * triS.edges[0].x;
    float d2 = (point.x - triS.vertices[1].x) * triS.edges[1].y - (point.y - triS.vertices[1].y) * triS.edges[1].x;
    float d3 = (point.x - triS.vertices[2].x) * triS.edges[2].y - (point.y - triS.vertices[2].y) * triS.edges[2].x;

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}




void drawScene(Camera& camera) {
	Mat4 pMat = projectionMatrix(camera);
	Mat4 vMat = viewMatrix(camera);
	Mat4 mMat = mat4Identity();

	Mat4 pvmMat = pMat * vMat * mMat;

	for (int triIdx=0; triIdx<data::numTris; triIdx++) {
		TriScreen triS = createTriScreen(triIdx, camera, pvmMat, Vec3(255, 0, 255));
		if (!triS.valid) {continue;}
		Vec2 minBB = min(triS.vertices[0], min(triS.vertices[1], triS.vertices[2]));
		Vec2 maxBB = max(triS.vertices[0], max(triS.vertices[1], triS.vertices[2]));

		int minX = max(0, (int)floor(minBB.x));
		int maxX = min(LCD_WIDTH_PX - 1, (int)ceil(maxBB.x));
		int minY = max(0, (int)floor(minBB.y));
		int maxY = min(LCD_HEIGHT_PX - 1, (int)ceil(maxBB.y));

		triS.colour = createColour(255, 0, 255);
		for (int vX=minX; vX<=maxX; vX++) {
			for (int vY=minY; vY<=maxY; vY++) {
				if (inTri(Vec2(vX, vY), triS)) {
					drawPixel(vX, vY, triS.colour);
				}
			}
		}
	}
}


}