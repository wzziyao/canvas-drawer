#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   canvas drawer(640, 380);
   // // circles
   // drawer.background(0, 0, 0);
   // drawer.begin(CIRCLES);
   // drawer.color(255, 0, 255);
   // drawer.vertex(320, 190);
   // drawer.setRadius(150);
   // drawer.color(0, 255, 255);
   // drawer.vertex(320, 190);
   // drawer.setRadius(120);
   // drawer.end();
   // drawer.save("circle-test.png");

   // // rectangles
   // drawer.background(0, 0, 0);
   // drawer.begin(RECTANGLES);
   // drawer.color(255, 0, 255);
   // drawer.vertex(320, 190);
   // drawer.setWidthHeight(400, 200);
   // drawer.color(0, 255, 255);
   // drawer.vertex(320, 190);
   // drawer.setWidthHeight(350, 150);
   // drawer.end();
   // drawer.save("rectangle-test.png");

   // // points
   // drawer.background(0, 0, 0);
   // drawer.begin(POINTS);
   // drawer.color(255, 0, 255);
   // drawer.vertex(320, 190);
   // drawer.color(0, 255, 255);
   // drawer.vertex(320, 200);
   // drawer.end();
   // drawer.save("point-test.png");

   // star
   drawer.background(0, 0, 0);
   drawer.begin(STARS);
   drawer.color(255, 0, 255);
   drawer.vertex(320, 190);
   drawer.setRadius(150);
   drawer.setRadius(100);
   drawer.arms(15);
   drawer.color(0, 255, 255);
   drawer.vertex(320, 190);
   drawer.setRadius(120);
   drawer.setRadius(80);
   drawer.arms(15);
   drawer.color(255, 255, 0);
   drawer.vertex(320, 190);
   drawer.setRadius(90);
   drawer.setRadius(60);
   drawer.arms(15);
   drawer.end();
   drawer.save("star-test.png");

   // // art
   // for (int i = 0; i < 20; i++) {
   //    drawer.begin(CIRCLES);
   //    drawer.color(0, 255, 255);
   //    drawer.vertex(50 + (rand() % (640 - 50 + 1)), 50 + (rand() % (340 - 50 + 1)));
   //    drawer.setRadius(20);
   //    drawer.color(255, 0, 255);
   //    drawer.vertex(50 + (rand() % (640 - 50 + 1)), 50 + (rand() % (340 - 50 + 1)));
   //    drawer.setRadius(10);
   //    drawer.color(255, 255, 0);
   //    drawer.vertex(50 + (rand() % (640 - 50 + 1)), 50 + (rand() % (340 - 50 + 1)));
   //    drawer.setRadius(5);
   //    drawer.end();
   // }
   // for (int i = 0; i < 10; i++){
   //    drawer.begin(STARS);
   //    drawer.color(255, 255, 255);
   //    drawer.arms(10);
   //    drawer.vertex(50 + (rand() % (640 - 50 + 1)), 50 + (rand() % (340 - 50 + 1)));
   //    drawer.end();
   // }
   // drawer.save("art.png");

   return 0;
}

