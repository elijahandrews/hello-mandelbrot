#include <iostream>
#include <vector>

#define __CL_ENABLE_EXCEPTIONS
#ifdef __APPLE__
  #include "../lib/cl.hpp"
#else
  #include <CL/cl.hpp>
#endif

#include <png++/png.hpp>

// TODO: crashes if I make height too big. There's an overflow somewhere
unsigned int height = 5000;
unsigned int width = int(height * 3.5/2.0);
unsigned int max_iters = 80;

int main(int argc, char* argv[]) {
  cl_float3* colors = (cl_float3*)malloc(height * width * sizeof(cl_float3));
  if (!colors) {
    std::cerr << "Can't allocate memory for colors" << std::endl;
    throw;
  }

  // Get available platforms
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  // Select the default platform and create a context using this platform
  // TODO: Make sure this is using my AMD graphics and not iris
  cl_context_properties cpus[3] = {
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)(platforms[0])(),
    0
  };
  cl::Context context(CL_DEVICE_TYPE_GPU, cpus);

  // Get a list of device on this platform
  std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

  // Create a command queue and use the first device
  cl::CommandQueue queue = cl::CommandQueue(context, devices[0]);

  // Read source file
  std::ifstream sourceFile("src/kernel.cl");
  if(!sourceFile.is_open()){
    std::cerr << "Cannot find kernel file" << std::endl;
    throw;
  }
  std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
  cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

  // Make program of the source code in the context
  cl::Program program = cl::Program(context, source);

  // Build program for these specific devices
  try {
    program.build(devices);
  } catch(cl::Error error) {
    std::cerr << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;
    throw;
  }

  // Make kernel
  cl::Kernel kernel(program, "mandelbrot");

  // Create buffer
  cl::Buffer colors_buf = cl::Buffer(
    context,
    CL_MEM_READ_ONLY,
    width * height * sizeof(cl_float3)
  );

  // Set arguments to kernel
  kernel.setArg(0, max_iters);
  kernel.setArg(1, height);
  kernel.setArg(2, width);
  kernel.setArg(3, colors_buf);

  // Run the kernel on specific ND range
  cl::NDRange global(width * height);
  queue.enqueueNDRangeKernel(
    kernel,
    cl::NullRange,
    global,
    cl::NullRange  // Don't specify a local size, let OpenCL figure it out
  );


  // Read colors buffer into local list
  queue.enqueueReadBuffer(
    colors_buf,
    CL_TRUE,
    0,
    width * height * sizeof(cl_float3),
    colors
  );

  // Output to PNG
  // TODO: Make this faster (is there a zero copy way to do it?)
  png::image<png::rgb_pixel> image(width, height);
  for (png::uint_32 y = 0;  y < image.get_height(); ++y) {
    for (png::uint_32 x = 0; x < image.get_width(); ++x) {
      unsigned int i = y * width + x;
      cl_float3 c = colors[i];
      image[y][x] = png::rgb_pixel(c.s[0], c.s[1], c.s[2]);
    }
  }
  image.write("output.png");

  free(colors);
}
