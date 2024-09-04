# Image Cluster Compressor

A basic image compressor that employs k-means clustering to reduce the colour depth of images. 

This project employs the k-means model of my [C Machine Learning Toolkit](https://github.com/tommorris512/cml-toolkit) project to perform colour depth reduction on parsed images. It's more of a fun proof-of-concept than a useful utility, but it does produce some interesting results nonetheless.

## Prerequisites

- Any C compiler, such as `clang` or `gcc`, that supports the C99 standard. (To use compilers other than `clang`, please change the compiler command at the top of each Makefile used).
- The `make` command to execute the Makefile rules.
- An image you'd like to compress!


## Installation

1. Clone the repositories for both this project and my [C Machine Learning Toolkit](https://github.com/tommorris512/cml-toolkit) project:
```bash
git clone https://github.com/tommorris/image-compressor.git
git clone https://github.com/tommorris/cml-toolkit.git
```

2. Build the shared library file `libcml.so` for my [C Machine Learning Toolkit](https://github.com/tommorris512/cml-toolkit) using the included Makefile:
```bash
cd ./cml-toolkit
make
```

3. Copy the shared library file `libcml.so` to the `lib` directory of the image compressor project and set the `LD_LIBRARY_PATH` environment variable:
```bash
cd ..
mkdir -p ./image-compressor/lib
cp ./cml-toolkit/build/libcml.so ./image-compressor/lib
export LD_LIBRARY_PATH=./image-compressor/lib:$LD_LIBRARY_PATH
```

4. Switch to this project's root directory and build the image compressor using the included Makefile:
```bash
cd ./image-compressor
make
```


## Usage

To execute image compression, run the following in the project's root directory:
```bash
./build/image_compressor <input_image> <output_image> <num_colours> <num_iterations>
```

### Arguments:
- `<input_image>` - A path to the image to be compressed.

- `<output_image>` - A path to write the compressed image to.

- `<num_colours>` - The number of distinct colours the compressed image should use. This corresponds to the number of centroids the k-means model will employ.

- `<num_iterations>` - The number of iterations the k-means model should be trained on before writing the compressed image. Increasing the iteration count will improve accuracy but will also increase execution time (a good trade-off is around 10 iterations).

### Example usage:
The following example usage will compress `input.jpg` to `output.jpg` using 16 colours using 10 iterations of training:
```bash
./build/image_compressor input.jpg output.jpg 16 10
```