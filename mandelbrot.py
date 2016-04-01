from PIL import Image

N_MAX = 100

IMAGE_HEIGHT = 200
IMAGE_WIDTH = int(IMAGE_HEIGHT * 3.5/2.0)

# Maps a pixel co-ordinates to a complex number
def pixel_to_complex(x, y):
    real = 3.5 * x / IMAGE_WIDTH - 2.5  # -2.5 to 1
    imaginary = 2.0 * y / IMAGE_HEIGHT - 1.0  # -1 to 1
    return complex(real, imaginary)


im = Image.new("RGB", (IMAGE_WIDTH, IMAGE_HEIGHT), "white")

for i in xrange(IMAGE_WIDTH):
    for j in xrange(IMAGE_HEIGHT):
        c = pixel_to_complex(i, j)
        z = c
        for _ in xrange(N_MAX):
            z = z**2 + c
            if abs(z) > 2:
                im.putpixel((i, j), (0, 0, 0))
                break


im.show()