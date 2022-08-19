# 2D Raycaster 

A basic *raycaster* using SFML

![til](gifs/raycaster.gif)

Walls are created at random at launch with the exception of 4 walls that surround the screen.

Rays are casted with a field of view of $\pi$/2 starting from the green triangle you can control with the arrow keys.

Their length are computed by intersecting rays with each walls by solving the equation 

[Alt text](gifs/intersect_equation.png)

Which has as a solution 

[Alt text](gifs/intersect_solution.png)

We have an intersection if t > 0 and u $\in$ [0,1] since we consider the ray as half-open line segment and the wall as a segment delimited by its two ends.

\\

To draw the wall, we have the following equalities 

[Alt text](gifs/eq1.png)

with 

[Alt text](gifs/zmax.png)

We can now draw the bottom of the wall at

[Alt text](gifs/bottom.png)

We only have to shift this value by adding it height/2

We can compute the value for the top of the wall the same way by replacing *bottom* by *top* and *-zeye* by the *height of the wall*


### Installing 

You need to install SFML

```sudo apt-get install libsfml-dev```

If you already installed it but you didn't use the standard package manager, you will have to modify the Makefile and recompile it after specifying you path to SFML.

You'll also need to install the Eigen library.

```sudo apt-get install libeigen3-dev```

### Running 

You only have to use the Makefile.

```make```

This will compile and run the program.
