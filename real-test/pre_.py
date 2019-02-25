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
    __threthod = 165
    __gard = 60
    __pad = 5
    
    def __init__(self, *args, **kwargs):
        self.__path = sys.path[0] + '\\6.png'
    
    def open_pic(self):
        self.__pic = Image.open(self.__path).convert('L')
        self.__pic = PIL.ImageOps.invert(self.__pic)
        
    def binary(self):
        table = [int(elem >= self.__threthod) for elem in range(256)]
        self.__pic.point(table, '1')

    def cut(self, height, width):
        self.__height, self.__width = height, width
        w, h = self.__pic.size
        FX, FY, SX, XY = 0, 0, 0, 0
        for x in range(w):
            flg = False
            for y in range(h):
                if self.__pic.getpixel((x, y)) > self.__gard:
                    flg = True
                    break
            if flg == True:
                FX = x
                break
        for x in range(w - 1, -1, -1):
            flg = False
            for y in range(h):
                if self.__pic.getpixel((x, y)) > self.__gard:
                    flg = True
                    break
            if flg == True:
                SX = x
                break
        for y in range(h - 1, -1, -1):
            flg = False
            for x in range(w):
                if self.__pic.getpixel((x, y)) > self.__gard:
                    flg = True
                    break
            if flg == True:
                SY = y
                break
        for y in range(h):
            flg = False
            for x in range(w):
                if self.__pic.getpixel((x, y)) > self.__gard:
                    flg = True
                    break
            if flg == True:
                FY = y
                break
        self.__pic = self.__pic.crop((FX, FY, SX, SY))
        self.__pic.save(self.__path.replace('6', '2'))
        self.__pic = self.__pic.resize((height - 2 * self.__pad, width - 2 * self.__pad), Image.ANTIALIAS)

    def save(self):
        # save pic
        self.__base = Image.new('RGB', (self.__height, self.__width)).convert('L')
        self.__base.paste(self.__pic, (self.__pad, self.__pad, self.__width - self.__pad, self.__height - self.__pad))
        self.__base.save(self.__path.replace('png', 'jpg'))
        # save numbers
        with open(sys.path[0] + '\\' + self.__tmp, "w") as outFile:
            for h in range(0, self.__height):
                for w in range(0, self.__width):
                    pixel = self.__base.getpixel((w, h))
                    outFile.write(str(pixel) + ' ')
    
def main():
    image = Image_helper()
    image.open_pic()
    image.binary()
    image.cut(28, 28)
    image.save()
    os.system('.\Rec_digit.exe src.tmp res.txt')

if __name__ == '__main__':
    main()
