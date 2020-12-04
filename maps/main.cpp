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
  if (y & 0x1) {
    cout << "  " << odd_bottom(x, y) << endl;
    cout << "  " << odd_top(x, y) << endl;
  }
  else {
    cout << "  " << even_bottom(x, y) << endl;
    cout << "  " << even_top(x, y) << endl;
  }
}


template <unsigned R, int X, int Y=0>
inline void hexes(std::ostream& os, const int& x, const int& y) {
  int x_translated = x + X;
  int y_translated = y + Y;
  if (
    (( x_translated      % (3*R) ==    0 ) and (y_translated % (2*R) == R))
    or
    (((x_translated+R/2) % (3*R) == (2*R)) and (y_translated % (2*R) == 0))
  ) {
    os << "  " << Hex<R>(Vertex(x, y)) << endl;
  }
}


template <unsigned R>
inline typename std::enable_if<(R == 1)>::type
hexes(std::ostream& os, const int& x, const int& y) {
  hexes<R, 1, 0>(os, x, y);
}


template <unsigned R>
inline typename std::enable_if<(R > 1)>::type
hexes(std::ostream& os, const int& x, const int& y) {
  hexes<R, R/2, 0>(os, x, y);
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

  for (int x = start; x < end; ++x) {
    for (int y = start; y < end; ++y) {
      // triangles(cout, x, y);
      hexes<1>(cout, x, y);
      hexes<6>(cout, x, y);
      hexes<60>(cout, x, y);
    }
  }

  cout << "</svg>" << endl;

  return 0;
}
