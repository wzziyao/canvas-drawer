#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   canvas drawer(640, 380);
   // circles
   drawer.background(0, 0, 0);
   drawer.begin(CIRCLES);
   drawer.color(255, 0, 255);
   drawer.vertex(320, 190);
   drawer.setRadius(150);
   drawer.color(0, 255, 255);
   drawer.vertex(320, 190);
   drawer.setRadius(120);
   drawer.end();
   drawer.save("circle-test.png");

   // rectangles
   drawer.background(0, 0, 0);
   drawer.begin(RECTANGLES);
   drawer.color(255, 0, 255);
   drawer.vertex(320, 190);
   drawer.setWidthHeight(400, 200);
   drawer.color(0, 255, 255);
   drawer.vertex(320, 190);
   drawer.setWidthHeight(350, 150);
   drawer.end();
   drawer.save("rectangle-test.png");

   // points
   drawer.background(0, 0, 0);
   drawer.begin(POINTS);
   drawer.color(255, 0, 255);
   drawer.vertex(320, 190);
   drawer.color(0, 255, 255);
   drawer.vertex(320, 200);
   drawer.end();
   drawer.save("point-test.png");

   return 0;
}

