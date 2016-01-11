![](https://cloud.githubusercontent.com/assets/2152766/12165616/4272f0b0-b517-11e5-96bf-dcceab4560ce.gif)

This is my finals project for the [CS50x course "Introduction to Computer Science" on the Edx platform](https://courses.edx.org/courses/HarvardX/CS50x3/2015/info), started on December 10th 2015.

<br>
<br>
<br>

### Usage

To compile this project, you will need.

- clang
- cmake
- make

For the graphical user interface, you will also need.

- Python 3
- PyQt5

**Compilation**

```bash
$ git clone http://github.com/mottosso/blur
$ cd blur
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./blur -o outputimage.png inputimage.png
```

- [See detailed installation instructions](../wiki/Building-on-Ubuntu-14.04)

<br>
<br>
<br>

### Goal

To learn more about..

- Image processing
- Bottlenecks and optimisation, through real-time performance constraints
- Compiling multiple files along with external libraries
- Header-only libraries
- Project structure
- Pointers
- C, and how to integrate it with other languages.
- Multi-dimensional data structures, such as images (and later, 3d geometry)
- (Theading and TBB)

By..

Implementing - and illustrating the behaviour of - gaussian blur.

<br>
<br>
<br>

### Strategy

1. Find out Gaussian and what it means
2. Implement your own naive algorithm in C
3. Tailor it to better visualise behaviour
4. Provide for interactivity via a graphical user interface.

Along the way, it turns out that gaussian is achieved via an algorithm known as "convolution" which applies to fields beyond image processing, such as audio and physics. Many seemingly different image effects can be achieved with convolution alone by use of different "kernels". A kernel is a 2-dimensional array values between 0-1 and a sum of 1, used to derive an average value for each pixel in an image.

<br>
<br>
<br>

### References

The following are some of the references I studied during this project.

- [Convolution theory 1](http://setosa.io/ev/image-kernels)
- [Convolution theory 1](https://docs.gimp.org/en/plug-in-convmatrix.html)
- [Project layout 1](https://github.com/wjakob/instant-meshes)
- [Project layout 2](http://stackoverflow.com/questions/446017/popular-folder-structure-for-build)
- [Computing the Guassian Kernel](http://stackoverflow.com/questions/8204645/implementing-gaussian-blur-how-to-calculate-convolution-matrix-kernel)
