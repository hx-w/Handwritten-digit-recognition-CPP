#!usr/bin/env/ python
#-*- coding: utf-8 -*-

from PIL import Image
import PIL.ImageOps

class Image_helper:
    __path = ''
    def __init__(self, *args, **kwargs):
        self.__path = 'D:/Study/procedure/Handwritten-digit-recognition-CPP/real-test/6.png'
    
    def open_pic(self):
        self.__pic = Image.open(self.__path).convert('L')
        self.__pic = PIL.ImageOps.invert(self.__pic)
        
    def resize(self, height, width):
        self.__pic = self.__pic.resize((height, width), Image.ANTIALIAS)
        for h in range(0, height):
            for w in range(0, width):
                pixel = self.__pic.getpixel((w, h))
                print pixel

    def save(self):
        self.__pic.save(self.__path.replace('png', 'jpg'))
    
def main():
    image = Image_helper()
    image.open_pic()
    image.resize(28, 28)
    image.save()

if __name__ == '__main__':
    main()
