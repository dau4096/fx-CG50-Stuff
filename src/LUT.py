"LUT.py"
import math as maths;

#Generate 90 sine values [0-90 deg]
sinLUT:list[float] = [maths.sin(maths.radians(deg)) for deg in range(90)];
atanLUT:list[float] = [maths.atan(maths.radians(deg*(45.0/64.0))) for deg in range(64)];

#Format as C arrays
print("static const float sin_LUT[90] = {");
for (i, val) in enumerate(sinLUT):
	end:str = "," if (i < 89) else "";
	if ((i % 10) == 9): end += "\n"; #10 values per line.
	print(f"	{val:.6f}f", end=end);
print("};");

print("static const float atan_LUT[64] = {");
for (i, val) in enumerate(atanLUT):
	end:str = "," if (i < 63) else "";
	if ((i % 8) == 7): end += "\n"; #10 values per line.
	print(f"	{val:.6f}f", end=end);
print("};");