yet another simple Matrix Library - (yasML)
===========================================
![C CI](https://github.com/nhomble/yasML/workflows/C/C++%20CI/badge.svg?branch=master)

Purpose:
Reading online, I had a hard time finding a matrix library in C
for small data sets. Many of the suggested libraries (BLAS for
example from stackoverflow.com) that expect data much larger
than what I plan to use. In my linear algebra courses, matrices
rarely ever exceeded the 5x5. Still indev, I hope to provide a
simple and fast C library for matrix calculations on small sets.
Ideal for students in an introductory class in linear algebra
who want to do their scratchwork through a simple .c program or
simply want to see matrix relationships at work. The main goal
in mind was to make a quick and dirty matrix library that would
be more intuitive to use than matlab.

Documentation:
Using the library is quite simply after understanding the matrix
structure that drives the entire library. A matrix is simple, just
an array of pointers to double arrays. Each pointer corresponds
to a vector in our matrix as matrices are sets of vectors. We also
store the lengths of each of these arrays in the rows and columns 
entries accordingly.

For the most part, calling functions in this library follows the
general pattern of passing the matrix pointer and then catching
what you desire from the output of the function. Destruction
of created pointer in main are up to you. I assumed you may want
to continue using your passed matrix after you did something on it.
On a few exceptions does this thinking become inconsistent.

To Do:
------>	Different factorizations
------>	Optimize accordingly
------> Multi-threading

Recognition:
Linear Algebra and its Applications 4th edit.
	David C. Lay
