"LUT.py"
import math as maths;

#Generate 90 sine values [0-90 deg]
lut:list[float] = [maths.sin(maths.radians(deg)) for deg in range(90)];

#Format as C array
print("static const float sin_LUT[90] = {");
for (i, val) in enumerate(lut):
	end:str = "," if (i < 89) else "";
	if ((i % 10) == 9): end += "\n"; #10 values per line.
	print(f"	{val:.6f}f", end=end);
print("};");