__kernel void mandelbrot(unsigned int max_iters, unsigned int height, unsigned int width, __global float3* C)
{
  // TODO: Look at warnings when compiling the kernel, there are a few
  int i = get_global_id(0);
  unsigned int x = i % width;
  unsigned int y = i / width;

  float re0 = 3.5 * float(x) / float(width) - 2.5;  // -2.5 to 1
  float im0 = 2.0 * float(y) / float(height) - 1.0;  // -1 to 1

  float re = 0;
  float im = 0;

  unsigned int iter = 0;

  // see https://en.wikipedia.org/wiki/Mandelbrot_set
  // (escape time algorithm) for details about the math
  while (re*re + im*im < 4 && iter < max_iters) {
      float re_new = re*re - im*im + re0;
      im = 2*re*im + im0;
      re = re_new;
      ++iter;
  }

  float intensity = 255.0 * float(iter) / max_iters;

  C[i] = float3(intensity, intensity, intensity);
}
