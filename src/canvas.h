#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "ppm_image.h"

using namespace std;

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES, CIRCLES, RECTANGLES, POINTS, STARS};
   class canvas
   {
   public:
      canvas(int w, int h);
      virtual ~canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      // drawline functions
      void bresenhamLow(int ax, int ay, int bx, int by);
      void bresenhamHigh(int ax, int ay, int bx, int by);
      void horizontal(int ax, int ay, int bx, int by);
      void vertical(int ax, int ay, int bx, int by);
      void diagonal(int ax, int ay, int bx, int by);

      // triangles
      void fillTriangle(int ax, int ay, int bx, int by, int cx, int cy, int px, int py);
      int implicitFunction(int ax, int ay, int bx, int by, int x, int y);

      // circles
      void drawCircle(int xc, int yc, int x, int y, const ppm_pixel& color);
      void bresenhamCircle(int xc, int yc, int r, const ppm_pixel& color);
      void setRadius(int r);

      // rectangles
      void rectangle(int xc, int yc, int w, int h, const ppm_pixel& color);
      void setWidthHeight(int w, int h);

      // points
      void point(int x, int y, const ppm_pixel& color);

      // stars
      void star(int xc, int yc, int outer_r, int inner_r, const ppm_pixel& color);

      void clamp(int ax, int ay, int bx, int by);

   private:
      ppm_image _canvas;
      PrimitiveType p_type;
      int first_color;
      ppm_pixel old_color, current_color;
      vector<int> colors;
      vector<int> vertices;
      vector<int> radius;
      vector<int> widthHeight;
   };
}

#endif


