#include "hexgrid.hpp"

#include <iostream>
#include <list>

using std::cout;
using std::cerr;
using std::endl;
using std::list;
constexpr int start = 0;
constexpr int end = 50;


inline Triangle top_even(const int& x, const int& y) {
  return Triangle(
    Vertex(x, y),
    Vertex(x, y+1),
    Vertex(x+1, y)
  );
}


inline Triangle top_odd(const int& x, const int& y) {
  return Triangle(
    Vertex(x+1, y+1),
    Vertex(x, y+1),
    Vertex(x+1, y)
  );
}


inline Triangle bottom_even(const int& x, const int& y) {
  return Triangle(
    Vertex(x, y),
    Vertex(x, y+1),
    Vertex(x+1, y+1)
  );
}


inline Triangle bottom_odd(const int& x, const int& y) {
  return Triangle(
    Vertex(x, y),
    Vertex(x+1, y),
    Vertex(x+1, y+1)
  );
}


template <unsigned R>
inline void even_hex(std::ostream& os, const int& x, const int& y) {
  // cerr << x << ", " << y << endl;
  if (x % (3 * R) == 0) {
    os << "  " << Hex<R>(Vertex(x, y)) << endl;
  }
}


template <unsigned R>
inline void odd_hex(std::ostream& os, const int& x, const int& y) {
  // cerr << x << ", " << y << endl;
  if (x % (3 * R) == 2) {
    os << "  " << Hex<R>(Vertex(x, y)) << endl;
  }
}


int main() {
  cout
    << "<svg viewBox=\"0 0 " << ((end+1) * pixels_per_unit) << " " << ((end+1) * pixels_per_unit)
    << "\" xmlns=\"http://www.w3.org/2000/svg\">"
    << endl;

  for (int x = start; x < end; ++x) {
    for (int y = start; y < end; ++y) {
      if (y & 0x1) {
        cout << "  " << bottom_even(x, y) << endl;
        cout << "  " << bottom_odd(x, y) << endl;
        even_hex<1>(cout, x, y);
        even_hex<2>(cout, x, y);

        // if (x % 3 == 0) {
        //   cerr << x << ", " << y << endl;
        //   cout << "  " << Hex<1>(Vertex(x, y)) << endl;
        // }
      }
      else {
        cout << "  " << top_even(x, y) << endl;
        cout << "  " << top_odd(x, y) << endl;
        odd_hex<1>(cout, x, y);

        // if (x % 3 == 2) {
        //   cerr << x << ", " << y << endl;
        //   cout << "  " << Hex<1>(Vertex(x, y)) << endl;
        // }
      }
    }
  }

  cout << Hex<6>(Vertex(6, 6)) << endl;
  cout << "</svg>" << endl;
  return 0;
}
