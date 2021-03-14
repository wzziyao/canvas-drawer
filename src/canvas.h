#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "ppm_image.h"

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES};
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

   private:
      ppm_image _canvas;
      PrimitiveType p_type;
      int *vertices;
      int next_vertex, first_color;
      ppm_pixel old_color, current_color;
   };
}

#endif


