#!/usr/bin/env python

from PIL import Image, ImageOps

# Load picture and layout 
empty = Image.open('./layout/empty.png') # size 1748px * 1181px, 300dpi 
layout = Image.open('./layout/layout1.png')
pic_raw = Image.open('test.JPG')
pic = pic_raw.resize((924,693),resample=0)

# Paste together
empty.paste(pic, (35, 460), mask=None)
empty.paste(pic.rotate(90, resample=0, expand=0), (1027,129), mask=None)
empty.paste(layout, (0,0), layout) # mask uses alpha channel of .png image
empty.save('merge_out.png') # save file
