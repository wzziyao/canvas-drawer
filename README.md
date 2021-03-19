# canvas-drawer

Implements a simple drawing api

<img src="images/art.png" alt="unique" width="500"/>

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
canvas-drawer/build $ ../bin/Debug/draw_test
canvas-drawer/build $ ../bin/Debug/draw_art
```

*macOS*

Open terminal to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake ..
canvas-drawer/build $ make
```

To run each program from build, you would type

```
canvas-drawer/build $ ../bin/draw_test
canvas-drawer/build $ ../bin/draw_art
```

## Supported primitives

**Circles**
```
drawer.background(int r, int g, int, b);
drawer.begin(CIRCLES);
drawer.color(int r, int g, int, b);
drawer.vertex(int y, int x);
drawer.setRadius(int r);
drawer.end();
drawer.save(string filename);
```

**Rectangles**
```
drawer.background(int r, int g, int, b);
drawer.begin(RECTANGLES);
drawer.color(int r, int g, int, b);
drawer.vertex(int y, int x);
drawer.setWidthHeight(int width, int height);
drawer.end();
drawer.save(string filename);
```

**Points**
```
drawer.background(int r, int g, int, b);
drawer.begin(POINTS);
drawer.color(int r, int g, int, b);
drawer.vertex(int y, int x);
drawer.end();
drawer.save(string filename);
```

**Stars**
```
drawer.background(int r, int g, int, b);
drawer.begin(PrimitiveType p);
drawer.color(int r, int g, int, b);
drawer.vertex(int y, int x);
drawer.setRadius(int outer_radius);
drawer.setRadius(int inner_radius);
drawer.end();
drawer.save(string filename);
```

## Results

<img src="images/art.png" alt="unique" width="500"/>
<img src="images/circle-test.png" alt="unique" width="500"/>
<img src="images/rectangle-test.png" alt="unique" width="500"/>
<img src="images/star-test.png" alt="unique" width="500"/>
