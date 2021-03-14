#include "canvas.h"
#include <cassert>
#include <typeinfo>
#include <iostream>

using namespace std;
using namespace agl;

canvas::canvas(int w, int h) : _canvas(w, h)
{
   current_color = {0, 0, 0};
   first_color = 1;
}

canvas::~canvas()
{
   delete[] vertices;
}

void canvas::save(const std::string& filename)
{
   cout << "SAVE" << endl;
   _canvas.save(filename);
}

void canvas::begin(PrimitiveType type)
{
   cout << "BEGIN" << endl;
   p_type = type;
   next_vertex = 0;
   if (p_type == LINES) {
      vertices = new int [2 * 2];
   } else if (p_type == TRIANGLES) {
      vertices = new int [3 * 2];
   }
   cout << "size of vertices: " << sizeof(vertices) << endl;
}

void canvas::end()
{
   cout << "END" << endl;
   // cout << "size of vertices: " << sizeof(vertices) << endl;
   for (int i = 0; i < sizeof(vertices) / 2 - 1; i+=3) {
      int ax = vertices[i];
      int ay = vertices[i+1];
      int bx = vertices[i+2];
      int by = vertices[i+3];
      cout << "a: " << ax << ", " << ay << endl;
      cout << "b: " << bx << ", " << by << endl;
      int width = bx - ax;
      int height = by - ay;
      // cout << width << endl;
      // cout << height << endl;
      if (abs(width) > abs(height) && abs(width) != 0 && abs(height) != 0) {
         cout << "width > height" << endl;
         if (ax < bx) {
            bresenhamLow(ax, ay, bx, by);
         } else {
            bresenhamLow(bx, by, ax, ay);
         }
      } else if (abs(width) < abs(height) && abs(width) != 0 && abs(height) != 0) {
         cout << "width < height" << endl;
         if (ay < by) {
            bresenhamHigh(ax, ay, bx, by);
         } else {
            bresenhamHigh(bx, by, ax, ay);
         }
      } else if (abs(width) == abs(height)) {
         cout << "diagonal" << endl;
         if (ax < bx) {
            diagonal(ax, ay, bx, by);
         } else {
            diagonal(bx, by, ax, ay);
         }
      } else if (abs(width) == 0) {
         cout << "width == 0" << endl;
         vertical(ax, ay, bx, by);
      } else if (abs(height) == 0) {
         cout << "height == 0" << endl;
         horizontal(ax, ay, bx, by);
      }
   }

}

void canvas::vertex(int x, int y)
{
   cout << "VERTEX" << endl;
   vertices[next_vertex] = x;
   vertices[next_vertex+1] = y;
   next_vertex+=2;
}

void canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   cout << "COLOR" << endl;
   if (first_color == 1) {
      old_color.r = r;
      old_color.g = g;
      old_color.b = b;

      current_color.r = r;
      current_color.g = g;
      current_color.b = b;

      first_color = 0;
   } else {
      ppm_pixel temp = current_color;
      current_color.r = r;
      current_color.g = g;
      current_color.b = b;
      old_color.r = temp.r;
      old_color.g = temp.g;
      old_color.b = temp.b;
   }
}

void canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   cout << "BACKGROUND" << endl;
   int h = _canvas.height();
   int w = _canvas.width();
   ppm_pixel bg;
   bg.r = r;
   bg.g = g;
   bg.b = b;
   for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
         _canvas.set(i, j, bg);
      }
   }
}

void canvas::bresenhamLow(int ax, int ay, int bx, int by)
{
   int y = ay;
   int W = bx - ax;
   int H = abs(by - ay);
   int F = 2 * H - W;
   float t = 0.0;
   float inc = 1.0 / (bx - ax);
   for (int x = ax; x <= bx; x++) {
      cout << y << endl;
      ppm_pixel current;
      current.r = (unsigned char) old_color.r * (1 - t) + current_color.r * t;
      current.g = (unsigned char) old_color.g * (1 - t) + current_color.g * t;
      current.b = (unsigned char) old_color.b * (1 - t) + current_color.b * t;
      _canvas.set(y, x, current);
      cout << F << endl;
      if (F > 0) {
         if (by > ay) y++;
         else y--;
         F += 2 * (H - W);
      } else {
         F += 2 * H;
      }
      t += inc;
   }
}

void canvas::bresenhamHigh(int ax, int ay, int bx, int by)
{
   int x = ax;
   int W = abs(bx - ax);
   int H = by - ay;
   int F = 2 * W - H;
   float t = 0.0;
   float inc = 1.0 / (bx - ax);
   for (int y = ay; y <= by; y++) {
      cout << x << endl;
      ppm_pixel current;
      current.r = (unsigned char) old_color.r * (1 - t) + current_color.r * t;
      current.g = (unsigned char) old_color.g * (1 - t) + current_color.g * t;
      current.b = (unsigned char) old_color.b * (1 - t) + current_color.b * t;
      _canvas.set(y, x, current);
      // cout << F << endl;
      if (F > 0) {
         if (bx > ax) x++;
         else x--;
         F += 2 * (W - H);
      } else {
         F += 2 * W;
      }
      t += inc;
   }
}

void canvas::horizontal(int ax, int ay, int bx, int by)
{
   float t = 0.0;
   float inc = 1.0 / (bx - ax);
   for (int x = ax; x <= bx; x++) {
      ppm_pixel current;
      current.r = (unsigned char) old_color.r * (1 - t) + current_color.r * t;
      current.g = (unsigned char) old_color.g * (1 - t) + current_color.g * t;
      current.b = (unsigned char) old_color.b * (1 - t) + current_color.b * t;
      // cout << "row = " << x << endl;
      // cout << "col = " << ay << endl;
      // cout << "myHeight = " << _canvas.height() << endl;
      // cout << "myWidth = " << _canvas.width() << endl;
      _canvas.set(ay, x, current);
      t += inc;
   }
}

void canvas::vertical(int ax, int ay, int bx, int by)
{
   float t = 0.0;
   float inc = 1.0 / (bx - ax);
   for (int y = ay; y <= by; y++) {
      ppm_pixel current;
      current.r = (unsigned char) old_color.r * (1 - t) + current_color.r * t;
      current.g = (unsigned char) old_color.g * (1 - t) + current_color.g * t;
      current.b = (unsigned char) old_color.b * (1 - t) + current_color.b * t;
      _canvas.set(y, ax, current);
      t += inc;
   }
}

void canvas::diagonal(int ax, int ay, int bx, int by) {
   float t = 0.0;
   float inc = 1.0 / (bx - ax);
   int x = ax;
   int y = ay;
   cout << "ax: " << ax << endl;
   cout << "ay: " << ay << endl;
   while (x <= bx) {
      cout << x << endl;
      cout << y << endl;
      ppm_pixel current;
      current.r = (unsigned char) old_color.r * (1 - t) + current_color.r * t;
      current.g = (unsigned char) old_color.g * (1 - t) + current_color.g * t;
      current.b = (unsigned char) old_color.b * (1 - t) + current_color.b * t;
      _canvas.set(y, x, current);
      t += inc;
      x++;
      if (by > ay) y++;
      else y--;
   }
}
