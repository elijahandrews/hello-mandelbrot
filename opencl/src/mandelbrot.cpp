#include <iostream>
#include <vector>

#include <png++/png.hpp>


int main(int argc, char* argv[]) {
  png::image<png::rgb_pixel> image(500, 500);
  for (png::uint_32 y = 0;  y < image.get_height(); ++y) {
    for (png::uint_32 x = 0; x < image.get_width(); ++x) {
      image[y][x] = png::rgb_pixel(x, y, x + y);
    }
  }
  image.write("rgb.png");
}
