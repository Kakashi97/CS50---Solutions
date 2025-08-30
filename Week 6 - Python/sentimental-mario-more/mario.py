from cs50 import get_int

height = get_int("Height : ")
while height < 1 or height > 8:
    height = get_int("Height : ")

for h in range(height):
    for i in range(height - h - 1):
        print(" ", end="")
    for j in range(h + 1):
        print("#", end="")
    print("  ", end="")
    for j in range(h + 1):
        print("#", end="")
    print("")
