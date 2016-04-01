import math

from lib.colorsys import hsv_to_rgb

from PIL import Image
from progress.bar import Bar


N_MAX = 300

IMAGE_HEIGHT = 30000
IMAGE_WIDTH = int(IMAGE_HEIGHT * 3.5/2.0)

# Maps a pixel co-ordinates to a complex number
def pixel_to_complex(x, y):
    real = 3.5 * x / IMAGE_WIDTH - 2.5  # -2.5 to 1
    imaginary = 2.0 * y / IMAGE_HEIGHT - 1.0  # -1 to 1
    return complex(real, imaginary)

im = Image.new("RGB", (IMAGE_WIDTH, IMAGE_HEIGHT), "black")

bar = Bar('Processing', max=IMAGE_HEIGHT)

for j in xrange(IMAGE_HEIGHT):
    for i in xrange(IMAGE_WIDTH):
        c = pixel_to_complex(i, j)
        z = c
        for n in xrange(N_MAX):
            z = z**2 + c
            if abs(z) > 2:
                # smooth from http://stackoverflow.com/a/1243788
                smooth_color = (n + 1 - math.log(math.log(abs(z)))/math.log(2)) / N_MAX

                r, g, b = hsv_to_rgb(smooth_color, 0.5, 0.5)

                im.putpixel((i, j), (int(r * 255), int(g * 255), int(b * 255)))
                break
    bar.next()

bar.finish()

im.save("output.png")
im.show()
