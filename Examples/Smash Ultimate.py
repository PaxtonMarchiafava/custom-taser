
from PIL import Image
from PIL import ImageGrab
from PIL import ImageShow

import serial
import time


yapper = serial.Serial()  # open serial port
yapper.port = 'COM3'
yapper.baudrate = 115200
yapper.timeout = 0.1 # read timeout value. we dont read here
yapper.open()
time.sleep(4) # wait for serial port to actually open

image = ImageGrab.grab()

box = (image.size[0] / 2, 0, image.size[0], image.size[1]) # Right half of image
box2 = (0, 0, image.size[0] / 2, image.size[1]) # Left half of image


# full %
bbox = (500, 980, 650, 1052) # player 1 box to build values in next line
playerbbox = ((bbox[0], bbox[1], bbox[2], bbox[3]), (bbox[0] + 739, bbox[1], bbox[2] + 739, bbox[3])) # score boxes for all players

playerScore = (255, 255)


def avgColor(ColorImage): # returns rgb 3 tuple
    px = ColorImage.load() # decodes image so we can process
    RGB = (0, 0, 0)
    for i in range(ColorImage.size[0]):
        for j in range(ColorImage.size[1]):
            RGB = (RGB[0] + px[i,j][0], RGB[1] + px[i,j][1], RGB[2] + px[i,j][2])

            # print(i, ', ', j, ', ', RGB)

    RGB = (RGB[0] / (ColorImage.size[0] * ColorImage.size[1]), RGB[1] / (ColorImage.size[0] * ColorImage.size[1]), RGB[2] / (ColorImage.size[0] * ColorImage.size[1]))
    
    return RGB

def testView ():
    a = ImageGrab.grab(playerbbox[0])
    b = ImageGrab.grab(playerbbox[1])
    c = Image.new('RGB', (a.size[0] + b.size[0], a.size[1]))
    c.paste(a)
    c.paste(b, (a.size[0], 0))
    c.save('Prostitute/balls.png')
    c.show()

def run(scores, shock):
    while True:
        p1 = ImageGrab.grab(playerbbox[0]) # get images
        p2 = ImageGrab.grab(playerbbox[1])
        
        x = avgColor(p1)
        print(x, '  ', scores[0])
        if ((x[1] + x[2]) / 2) + 2 < scores[0]: # the red value will not change. check how green / blue the % is. it goes down to turn the score more red
            scores = (((x[1] + x[2]) / 2), scores[1])
            print()
            print('AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaaaaaaaaaaaaaaaaaAAAAAAAAA')

            if shock:
                print('shocking')
                print()
                yapper.write(b'7')

        time.sleep(0.1)

# testView()

run(playerScore, 0)

