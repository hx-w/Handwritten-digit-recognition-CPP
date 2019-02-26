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
    __gard = 80
    __pad = 5
    __smpic = []
    
    def __init__(self, *args, **kwargs):
        self.__path = sys.path[0] + '\\canvas.png'
    
    def open_pic(self):
        self.__pic = Image.open(self.__path).convert('L')
        self.__pic = PIL.ImageOps.invert(self.__pic)
        
    def binary(self):
        table = [int(elem >= self.__threthod) for elem in range(256)]
        self.__pic.point(table, '1')

    def cut(self, height, width):
        self.__height, self.__width = height, width
        w, h = self.__pic.size
        flg = True
        while flg:
            flg = False
            cluster_y_x = []
            FX, FY, SX, SY = 0, 0, 0, 0
            for x in range(w):
                for y in range(h):
                    if self.__pic.getpixel((x, y)) > self.__gard:
                        flg = True
                        FX = x
                        cluster_y_x.append(y)
                        break
                if flg:
                    break
            if flg == False:
                break
            for y in range(h):
                for x in range(w):
                    if self.__pic.getpixel((x, y)) > self.__gard:
                        FY = y
                        flg = False
                        cluster_y_x.append(x)
                        break
                if flg == False:
                    break
            # for SX SY
            flg = True
            for x in range(cluster_y_x[1], w):
                for y in range(cluster_y_x[0] + 1, h):
                    flg = True
                    # current (x, y)
                    for dx in range(FX, x + 1):
                        if self.__pic.getpixel((dx, y)) > self.__gard:
                            flg = False
                            break
                    if flg == False:
                        continue
                    for dy in range(FY, y + 1):
                        if self.__pic.getpixel((x, dy)) > self.__gard:
                            flg = False
                            break
                    if flg == False:
                        break
                    SX, SY = x, y
                    break
                if flg:
                    break
            flg = True
            print FX, FY, SX, SY
            sm = self.__pic.crop((FX, FY, SX, SY))
            sm = sm.resize((height - 2 * self.__pad, width - 2 * self.__pad), Image.ANTIALIAS)
            newImg = Image.new('RGB', (SX - FX, SY - FY), 'black')
            self.__pic.paste(newImg, (FX, FY, SX, SY))
            # [TODO] digit '1' crop unsuit
            self.__smpic.append(sm)

    def save(self):
        # save pic
        os.system('del res.txt')
        cnt = 0
        for pic in self.__smpic:
            cnt += 1
            self.__base = Image.new('RGB', (self.__height, self.__width)).convert('L')
            self.__base.paste(pic, (self.__pad, self.__pad, self.__width - self.__pad, self.__height - self.__pad))
            self.__base.save(self.__path.replace('canvas.png', str(cnt) + '.jpg'))
            # save numbers
            with open(sys.path[0] + '\\' + self.__tmp, "w") as outFile:
                for h in range(0, self.__height):
                    for w in range(0, self.__width):
                        pixel = self.__base.getpixel((w, h))
                        outFile.write(str(pixel) + ' ')
            os.system('.\Rec_digit.exe src.tmp res.txt')
    
def main():
    image = Image_helper()
    image.open_pic()
    image.binary()
    image.cut(28, 28)
    image.save()

if __name__ == '__main__':
    main()
