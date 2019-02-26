#!usr/bin/env/ python
#-*- coding: utf-8 -*-

from PIL import Image
import PIL.ImageOps
import sys
import os
import math

class Image_helper:
    __path = ''
    __tmp  = '/src.tmp'
    __height = 0
    __width = 0
    __threthod = 125
    __gard = 111
    __pad = 6
    __smpic = []
    
    def __init__(self, *args, **kwargs):
        self.__path = sys.path[0] + '\\canvas.png'
    
    def open_pic(self):
        self.__pic = Image.open(self.__path).convert('L')
        self.__pic = PIL.ImageOps.invert(self.__pic)
        
    def binary(self):
        table = [int(elem >= self.__threthod) for elem in range(256)]
        self.__pic.point(table, '1')
        w, h = self.__pic.size
        for x in range(w):
            for y in range(h):
                pix = self.__pic.getpixel((x, y))
                if pix < self.__gard:
                    self.__pic.putpixel((x, y), 0)
                elif pix < 200:
                    self.__pic.putpixel((x, y), pix + 50)
        self.__pic.save(self.__path.replace('png', 'jpg'))

    def __col_cut(self):
        w, h = self.__pic.size
        front, rec = 0, False
        for col in range(w):
            __is_edge = False
            for row in range(h):
                if self.__pic.getpixel((col, row)) > self.__gard:
                    __is_edge = True
                    break
            if __is_edge == False:
                if rec == True:
                    self.__smpic.append(self.__pic.crop((front, 0, col, h)))
                    rec = False
            else:                
                if rec == False:
                    front = col
                    rec = True
                elif rec == True and col == w - 1:
                    self.__smpic.append(self.__pic.crop((front, 0, col, h)))

    def __mid_func(self, pic):
        w, h = pic.size
        top, rec = 0, False
        for y in range(h):
            __is_edge = False
            for x in range(w):
                if pic.getpixel((x, y)) > self.__gard:
                    __is_edge = True
                    break
            if __is_edge == True and rec == False:
                top = y
                rec = True
            elif __is_edge == False and rec == True:
                return pic.crop((0, top, w, y))
                # return pic.resize((self.__height - 2 * self.__pad, self.__width - 2 * self.__pad), Image.ANTIALIAS)
            elif rec == True and __is_edge == True and y == h - 1:
                return pic.crop((0, top, w, y))
   
    def __row_cut(self):
        self.__smpic = list(map(self.__mid_func, self.__smpic))
    
    def cuts(self, height, width):
        self.__height, self.__width = height, width
        self.__col_cut()
        self.__row_cut()

    def save(self):
        # save pic
        os.system('del res.txt')
        cnt = 0
        for pic in self.__smpic:
            cnt += 1
            print 'Image No.', cnt
            sw, sh = pic.size
            rate = 1.0 * (self.__height - 2 * self.__pad) / max(sw, sh)
            pic = pic.resize((int(math.ceil(sw * rate)), int(math.ceil(sh * rate))))
            
            sw, sh = pic.size
            self.__base = Image.new('RGB', (self.__height, self.__width)).convert('L')
            FX, FY = self.__width / 2 - sw / 2, self.__height / 2 - sh / 2
            self.__base.paste(pic, (FX, FY, FX + sw, FY + sh))
            # self.__base.paste(pic, (self.__pad, self.__pad, self.__width - self.__pad, self.__height - self.__pad))
            self.__base.save(self.__path.replace('canvas.png', 'split/' + str(cnt) + '.jpg'))
            # save numbers
            with open(sys.path[0] + '\\' + self.__tmp, "w") as outFile:
                for h in range(0, self.__height):
                    for w in range(0, self.__width):
                        pixel = self.__base.getpixel((w, h))
                        outFile.write(str(pixel) + ' ')
            os.system('.\Rec_digit.exe src.tmp res.txt')
            os.system('del src.tmp')
    
def main():
    image = Image_helper()
    image.open_pic()
    image.binary()
    image.cuts(28, 28)
    image.save()

if __name__ == '__main__':
    main()
