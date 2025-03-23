menuHeight = 24
LCDWidth = 384
LCDHeight = 216

tileGridWidth = 12
tileGridHeight = int(tileGridWidth * ((LCDHeight - menuHeight) / LCDWidth))
tileGridScale = LCDWidth // tileGridWidth


print(f"LCD Dimensions: {LCDWidth}x{LCDHeight}px")
print(f"tileGrid Dimensions: {tileGridWidth}x{tileGridHeight} tiles")
print(f"tileGrid Scale: {tileGridScale}px/tile")
print()
print()


for x in range(int(tileGridScale//2)):
	height = tileGridScale // 4 - x // 2
	print(f"({tileGridScale//2 - x}, {height})")
	print(f"({tileGridScale//2 + x}, {height})")
	print()