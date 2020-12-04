#include "hexgrid.hpp"

#include <cstdlib>
#include <iostream>
#include <list>
#include <type_traits>

using std::cout;
using std::cerr;
using std::endl;
using std::list;


inline Triangle even_top(const int& x, const int& y) {
  return Triangle(
    Vertex(x, y),
    Vertex(x, y+1),
    Vertex(x+1, y)
  );
}


inline Triangle even_bottom(const int& x, const int& y) {
  return Triangle(
    Vertex(x+1, y+1),
    Vertex(x, y+1),
    Vertex(x+1, y)
  );
}


inline Triangle odd_bottom(const int& x, const int& y) {
  return Triangle(
    Vertex(x, y),
    Vertex(x, y+1),
    Vertex(x+1, y+1)
  );
}


inline Triangle odd_top(const int& x, const int& y) {
  return Triangle(
    Vertex(x, y),
    Vertex(x+1, y),
    Vertex(x+1, y+1)
  );
}


inline void triangles(std::ostream& os, const int& x, const int& y) {
  cout << "  <g fill=\"none\" stroke=\"gray\" stroke-width=\"" << (0.25) << "\">" << endl;
  if (y & 0x1) {
    cout << "    " << odd_bottom(x, y) << endl;
    cout << "    " << odd_top(x, y) << endl;
  }
  else {
    cout << "    " << even_bottom(x, y) << endl;
    cout << "    " << even_top(x, y) << endl;
  }
  cout << "  <g/>" << endl;
}


template <unsigned R, int X, int Y=0>
inline void hex(std::ostream& os, const int& x, const int& y) {
  int x_translated = x + X;
  int y_translated = y + Y;
  if (
    (( x_translated      % (3*R) ==    0 ) and (y_translated % (2*R) == R))
    or
    (((x_translated+R/2) % (3*R) == (2*R)) and (y_translated % (2*R) == 0))
  ) {
    os << "    " << Hex<R>(Vertex(x, y)) << endl;
  }
}


template <unsigned R>
inline typename std::enable_if<(R == 1)>::type
hex(std::ostream& os, const int& x, const int& y) {
  hex<R, 1, 0>(os, x, y);
}


template <unsigned R>
inline typename std::enable_if<(R > 1)>::type
hex(std::ostream& os, const int& x, const int& y) {
  hex<R, R/2, 0>(os, x, y);
}


template <unsigned R, int dx, int dy>
inline void hexes(std::ostream& os, const int& start, const int& end) {
  cout << "  <g fill=\"none\" stroke=\"gray\" stroke-width=\"" << (0.75 * R) << "\">" << endl;
  for (int x = start; x < end; x += dx) {
    for (int y = start; y < end; y += dy) {
      hex<R>(os, x, y);
    }
  }
  cout << "  <g/>" << endl;
}


template <unsigned R>
inline typename std::enable_if<(R == 1)>::type
hexes(std::ostream& os, const int& start, const int& end) {
  hexes<R, 1, 1>(os, start, end);
}


template <unsigned R>
inline typename std::enable_if<(R > 1)>::type
hexes(std::ostream& os, const int& start, const int& end) {
  hexes<R, R/2, R>(os, start, end);
}


constexpr int start = 0;
constexpr int end = 360;
constexpr int width = end - start + 1;
constexpr int height = end - start + 1;

int main() {
  cout
    << "<svg viewBox=\"0 0 " << (width * pixels_per_unit) << " " << (height * pixels_per_unit)
    << "\" xmlns=\"http://www.w3.org/2000/svg\">"
    << endl;

  hexes< 1>(cout, start, end);
  hexes< 6>(cout, start, end);
  hexes<60>(cout, start, end);

  cout << "</svg>" << endl;

  return 0;
}
