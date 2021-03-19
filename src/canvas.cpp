#include "canvas.h"
#include <cassert>
#include <typeinfo>
#include <iostream>

using namespace std;
using namespace agl;

canvas::canvas(int w, int h) : _canvas(w, h)
{
   first_color = 1;
}

canvas::~canvas()
{
   vertices.clear();
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
}

void canvas::end()
{
   cout << "END" << endl;
   if (p_type == LINES) {
      for (int i = 0; i < vertices.size(); i+=4) {
         int ax = vertices[i];
         int ay = vertices[i+1];
         int bx = vertices[i+2];
         int by = vertices[i+3];

         ppm_pixel color_a, color_b;
         cout << "COLORS[I]: " << colors[i] << endl;
         color_a.r = (unsigned char) colors[i];
         color_a.g = (unsigned char) colors[i+1];
         color_a.b = (unsigned char) colors[i+2];
         color_b.r = (unsigned char) colors[i+3];
         color_b.g = (unsigned char) colors[i+4];
         color_b.b = (unsigned char) colors[i+5];
         _canvas.set(vertices[i+1], vertices[i], color_a);
         _canvas.set(vertices[i+3], vertices[i+2], color_b);

         cout << "a: " << ax << ", " << ay << endl;
         cout << "b: " << bx << ", " << by << endl;
         int width = bx - ax;
         int height = by - ay;
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
            if (ay < by) {
               vertical(ax, ay, bx, by);
            }
            else {
               vertical(bx, by, ax, ay);
            }
         } else if (abs(height) == 0) {
            cout << "height == 0" << endl;
            if (ax < bx) {
               horizontal(ax, ay, bx, by);
            }
            else {
               horizontal(bx, by, ax, ay);
            }
         }
      }
   } else if (p_type == TRIANGLES) {
      cout << "END TRIANGLE" << endl;
      int color_index = 0;
      for (int i = 0; i < vertices.size(); i+=6) {
         int ax = vertices[i];
         int ay = vertices[i+1];
         int bx = vertices[i+2];
         int by = vertices[i+3];
         int cx = vertices[i+4];
         int cy = vertices[i+5];

         ppm_pixel color_a, color_b, color_c;
         color_a.r = (unsigned char) colors[color_index];
         color_a.g = (unsigned char) colors[color_index+1];
         color_a.b = (unsigned char) colors[color_index+2];
         color_b.r = (unsigned char) colors[color_index+3];
         color_b.g = (unsigned char) colors[color_index+4];
         color_b.b = (unsigned char) colors[color_index+5];
         color_c.r = (unsigned char) colors[color_index+6];
         color_c.g = (unsigned char) colors[color_index+7];
         color_c.b = (unsigned char) colors[color_index+8];
         color_index += 9;

         _canvas.set(vertices[i+1], vertices[i], color_a);
         _canvas.set(vertices[i+3], vertices[i+2], color_b);
         _canvas.set(vertices[i+5], vertices[i+4], color_c);

         int f_alpha = implicitFunction(bx, by, cx, cy, ax, ay);
         int f_beta = implicitFunction(ax, ay, cx, cy, bx, by);
         int f_gamma = implicitFunction(ax, ay, bx, by, cx, cy);

         int xmin = min(ax, min(bx, cx));
         int ymin = min(ay, min(by, cy));
         int xmax = max(ax, max(bx, cx));
         int ymax = max(ay, max(by, cy));

         ppm_pixel current;

         for (int row = ymin; row <= ymax; row++) {
            for (int col = xmin; col <= xmax; col++) {
               float alpha = (float) implicitFunction(bx, by, cx, cy, col, row) / f_alpha;
               float beta = (float) implicitFunction(ax, ay, cx, cy, col, row) / f_beta;
               float gamma = (float) implicitFunction(ax, ay, bx, by, col, row) / f_gamma;

               current.r = (unsigned char) (int) (alpha * (float) (_canvas.get(ay, ax).r) + beta * (float) (_canvas.get(by, bx).r) + gamma * (float) (_canvas.get(cy, cx).r));
               current.g = (unsigned char) (int) (alpha * (float) (_canvas.get(ay, ax).g) + beta * (float) (_canvas.get(by, bx).g) + gamma * (float) (_canvas.get(cy, cx).g));
               current.b = (unsigned char) (int) (alpha * (float) (_canvas.get(ay, ax).b) + beta * (float) (_canvas.get(by, bx).b) + gamma * (float) (_canvas.get(cy, cx).b));

               if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                  bool drawAlpha = (alpha > 0 || f_alpha * implicitFunction(bx, by, cx, cy, -1, -1) >= 0);
                  bool drawBeta = (beta > 0 || f_beta * implicitFunction(ax, ay, cx, cy, -1, -1) >= 0);
                  bool drawGamma = (gamma > 0 || f_gamma * implicitFunction(ax, ay, bx, by, -1, -1) >= 0);
                  if (drawAlpha && drawBeta && drawGamma) _canvas.set(row, col, current);
               }
            }
         }
      }
   } else if (p_type == CIRCLES) {
      int color_index = 0;
      int radius_index = 0;
      for (int i = 0; i < vertices.size(); i+=2) {
         int xc = vertices[i];
         int yc = vertices[i+1];
         int r = radius[radius_index];

         ppm_pixel color;
         color.r = (unsigned char) colors[color_index];
         color.g = (unsigned char) colors[color_index+1];
         color.b = (unsigned char) colors[color_index+2];
         color_index += 3;
         radius_index++;

         bresenhamCircle(xc, yc, r, color);
      }
   } else if (p_type == RECTANGLES) {
      int color_index = 0;
      int widthHeight_index = 0;
      for (int i = 0; i < vertices.size(); i+=2) {
         int xc = vertices[i];
         int yc = vertices[i+1];
         int width = widthHeight[widthHeight_index];
         int height = widthHeight[widthHeight_index+1];

         ppm_pixel color;
         color.r = (unsigned char) colors[color_index];
         color.g = (unsigned char) colors[color_index+1];
         color.b = (unsigned char) colors[color_index+2];
         color_index += 3;
         widthHeight_index += 2;

         rectangle(xc, yc, width, height, color);
      }
   } else if (p_type == POINTS) {
      int color_index = 0;
      for (int i = 0; i < vertices.size(); i+=2) {
         int xc = vertices[i];
         int yc = vertices[i+1];

         ppm_pixel color;
         color.r = (unsigned char) colors[color_index];
         color.g = (unsigned char) colors[color_index+1];
         color.b = (unsigned char) colors[color_index+2];
         color_index += 3;

         point(xc, yc, color);
      }
   }
   vertices.clear();
   colors.clear();
}

int canvas::implicitFunction(int ax, int ay, int bx, int by, int x, int y)
{
   return (by - ay) * (x - ax) - (bx - ax) * (y - ay);
}

void canvas::vertex(int x, int y)
{
   cout << "VERTEX" << endl;
   vertices.emplace_back(x);
   vertices.emplace_back(y);
   colors.emplace_back((int)current_color.r);
   colors.emplace_back((int)current_color.g);
   colors.emplace_back((int)current_color.b);
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
      current.r = (unsigned char) _canvas.get(ay, ax).r * (1 - t) + _canvas.get(by, bx).r * t;
      current.g = (unsigned char) _canvas.get(ay, ax).g * (1 - t) + _canvas.get(by, bx).g * t;
      current.b = (unsigned char) _canvas.get(ay, ax).b * (1 - t) + _canvas.get(by, bx).b * t;
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
      current.r = (unsigned char) _canvas.get(ay, ax).r * (1 - t) + _canvas.get(by, bx).r * t;
      current.g = (unsigned char) _canvas.get(ay, ax).g * (1 - t) + _canvas.get(by, bx).g * t;
      current.b = (unsigned char) _canvas.get(ay, ax).b * (1 - t) + _canvas.get(by, bx).b * t;
      _canvas.set(y, x, current);
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
      current.r = (unsigned char) _canvas.get(ay, ax).r * (1 - t) + _canvas.get(by, bx).r * t;
      current.g = (unsigned char) _canvas.get(ay, ax).g * (1 - t) + _canvas.get(by, bx).g * t;
      current.b = (unsigned char) _canvas.get(ay, ax).b * (1 - t) + _canvas.get(by, bx).b * t;
      _canvas.set(ay, x, current);
      t += inc;
   }
}

void canvas::vertical(int ax, int ay, int bx, int by)
{
   float t = 0.0;
   float inc = 1.0 / (by - ay);
   for (int y = ay; y <= by; y++) {
      ppm_pixel current;
      current.r = (unsigned char) _canvas.get(ay, ax).r * (1 - t) + _canvas.get(by, bx).r * t;
      current.g = (unsigned char) _canvas.get(ay, ax).g * (1 - t) + _canvas.get(by, bx).g * t;
      current.b = (unsigned char) _canvas.get(ay, ax).b * (1 - t) + _canvas.get(by, bx).b * t;
      _canvas.set(y, ax, current);
      t += inc;
   }
}

void canvas::diagonal(int ax, int ay, int bx, int by) 
{
   float t = 0.0;
   float inc = 1.0 / (bx - ax);
   int x = ax;
   int y = ay;
   while (x <= bx) {
      ppm_pixel current;
      current.r = (unsigned char) _canvas.get(ay, ax).r * (1 - t) + _canvas.get(by, bx).r * t;
      current.g = (unsigned char) _canvas.get(ay, ax).g * (1 - t) + _canvas.get(by, bx).g * t;
      current.b = (unsigned char) _canvas.get(ay, ax).b * (1 - t) + _canvas.get(by, bx).b * t;
      _canvas.set(y, x, current);
      t += inc;
      x++;
      if (by > ay) y++;
      else y--;
   }
}

void canvas::drawCircle(int xc, int yc, int x, int y, const ppm_pixel& color)
{
   _canvas.set(yc+y, xc+x, color);
   _canvas.set(yc+y, xc-x, color);
   _canvas.set(yc-y, xc+x, color);
   _canvas.set(yc-y, xc-x, color);
   _canvas.set(yc+x, xc+y, color);
   _canvas.set(yc+x, xc-y, color);
   _canvas.set(yc-x, xc+y, color);
   _canvas.set(yc-x, xc-y, color);
}

void canvas::bresenhamCircle(int xc, int yc, int r, const ppm_pixel& color)
{
   int x = 0;
   int y = r;
   int d = 3 - 2 * r;
   drawCircle(xc, yc, x, y, color);
   while (y >= x) {
      x++;
      if (d > 0) {
         y --;
         d = d + 4 * (x - y) + 10;
      } else {
         d = d + 4 * x + 6;
      }
      drawCircle(xc, yc, x, y, color);
   }
}

void canvas::setRadius(int r) 
{
   radius.emplace_back(r);
}

void canvas::rectangle(int xc, int yc, int w, int h, const ppm_pixel& color)
{
   int ax = xc - w/2;
   int ay = yc - h/2;
   int bx = xc + w/2;
   int by = yc  - h/2;
   int cx = xc - w/2;
   int cy = yc + h/2;
   int dx = xc + w/2;
   int dy = yc + h/2;

   _canvas.set(ay, ax, color);
   _canvas.set(by, bx, color);
   _canvas.set(cy, cx, color);
   _canvas.set(dy, dx, color);

   horizontal(ax, ay, bx, by);
   horizontal(cx, cy, dx, dy);
   vertical(ax, ay, cx, cy);
   vertical(bx, by, dx, dy);
}

void canvas::setWidthHeight(int w, int h)
{
   widthHeight.emplace_back(w);
   widthHeight.emplace_back(h);
}

void canvas::point(int x, int y, const ppm_pixel& color)
{
   _canvas.set(y, x, color);
}