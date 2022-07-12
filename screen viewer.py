import PIL
from PIL import Image
import os
imfile = PIL.Image.new("RGB", (400,600), color=(255, 0, 0))
im = imfile.load()
os.chdir(input("DIR: "))

f = open("TOST.TXT", "r")
read = f.read()
f.close()

read = read.replace("4", "")
split = read.split("3")

for x, line in enumerate(split):
    for y in range(len(line)):
        char = line[y]
        if char=="0":
            color = (0, 0, 0)
        elif char=="1":
            color = (255, 255, 255)
        try:
            im[y, x]= color
        except:
            print("Y", y, "X", x, "CHAR", char)

imfile.save("test.png")
