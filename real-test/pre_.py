#!usr/bin/env/ python
#-*- coding: utf-8 -*-

from PIL import Image
import PIL.ImageOps
import sys
import os

class Image_helper:
    __path = ''
    __tmp  = '/src.tmp'
    __height = 0
    __width = 0
    def __init__(self, *args, **kwargs):
        self.__path = sys.path[0] + '\\6.png'
    
    def open_pic(self):
        self.__pic = Image.open(self.__path).convert('L')
        self.__pic = PIL.ImageOps.invert(self.__pic)
        
    def resize(self, height, width):
        self.__height, self.__width = height, width
        self.__pic = self.__pic.resize((height, width), Image.ANTIALIAS)

    def save(self):
        # save pic
        self.__pic.save(self.__path.replace('png', 'jpg'))
        # save numbers
        with open(sys.path[0] + '\\' + self.__tmp, "w") as outFile:
            for h in range(0, self.__height):
                for w in range(0, self.__width):
                    pixel = self.__pic.getpixel((w, h))
                    outFile.write(str(pixel) + ' ')
    
def main():
    image = Image_helper()
    image.open_pic()
    image.resize(28, 28)
    image.save()
    os.system('.\Rec_digit.exe src.tmp res.txt')

if __name__ == '__main__':
    main()
