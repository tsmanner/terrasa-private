#include "hexgrid.hpp"

#include <cstdlib>
#include <iostream>
#include <list>
#include <type_traits>

using std::cout;
using std::cerr;
using std::endl;
using std::list;


inline Triangle<Vertex> even_top(const int& x, const int& y) {
  return Triangle<Vertex>(
    Vertex(x, y),
    Vertex(x, y+1),
    Vertex(x+1, y)
  );
}


inline Triangle<Vertex> even_bottom(const int& x, const int& y) {
  return Triangle<Vertex>(
    Vertex(x+1, y+1),
    Vertex(x, y+1),
    Vertex(x+1, y)
  );
}


inline Triangle<Vertex> odd_bottom(const int& x, const int& y) {
  return Triangle<Vertex>(
    Vertex(x, y),
    Vertex(x, y+1),
    Vertex(x+1, y+1)
  );
}


inline Triangle<Vertex> odd_top(const int& x, const int& y) {
  return Triangle<Vertex>(
    Vertex(x, y),
    Vertex(x+1, y),
    Vertex(x+1, y+1)
  );
}


inline void triangles(std::ostream& os, const int& start, const int& end) {
  cout << "  <g fill=\"none\" stroke=\"gray\" stroke-width=\"" << (0.25) << "\">" << endl;
  for (int x = start; x < end; ++x) {
    for (int y = start; y < end; ++y) {
      if (y & 0x1) {
        cout << "    " << odd_bottom(x, y) << endl;
        cout << "    " << odd_top(x, y) << endl;
      }
      else {
        cout << "    " << even_bottom(x, y) << endl;
        cout << "    " << even_top(x, y) << endl;
      }
    }
  }
  cout << "  </g>" << endl;
}


struct TriangleFactory {
  using tile_type = Triangle<Vertex>;
  static constexpr int dx = 1;
  static constexpr int dy = 1;

  static bool can_make_tile(const int& x, const int& y) { return true; }

  static void add_tiles(std::list<tile_type>& tiles, const int& x, const int& y) {
    if (y & 0x1) {
      tiles.push_back(odd_bottom(x, y));
      tiles.push_back(odd_top(x, y));
    }
    else {
      tiles.push_back(even_bottom(x, y));
      tiles.push_back(even_top(x, y));
    }
  }
};


template <unsigned R, int DX, int DY>
struct HexFactoryImplementation {
  using tile_type = Hex<R, Vertex>;
  static constexpr unsigned radius = R;
  static constexpr int dx = DX;
  static constexpr int dy = DY;

  static void add_tiles(std::list<tile_type>& tiles, const int& x, const int& y) {
    if (can_make_tile(x, y)) {
      tiles.push_back(make_tile(x, y));
    }
  }

  static bool can_make_tile(const int& x, const int& y) {
    int x_translated = x + dx;
    return (( x_translated      % (3*R) ==    0 ) and (y % (2*R) == R)) or
           (((x_translated+R/2) % (3*R) == (2*R)) and (y % (2*R) == 0));
  }

  static tile_type make_tile(const int& x, const int& y) {
    return tile_type(Vertex(x, y));
  }
};


template <unsigned R, typename Enabled=void> struct HexFactory : public HexFactoryImplementation<R, R/2, R> {};

template <unsigned R> struct HexFactory<R, typename std::enable_if<R == 1>::type> : public HexFactoryImplementation<R, 1, 1> {};


int main() {
  auto start = Point(0, 0);
  auto end = Point(240, 180);

  auto triangle_grid = Grid<TriangleFactory>(start, end, 0.25);
  auto province_grid = Grid<HexFactory< 1>>(start, end, 0.75);
  auto kingdom_grid = Grid<HexFactory< 6>>(start, end, 1.5);
  auto continent_grid = Grid<HexFactory<60>>(start, end, 6.0);

  cout
    << "<svg viewBox=\"" << (start.x()) << " " << (start.y()) << " " << (end.x() * pixels_per_unit) << " " << (end.y() * pixels_per_unit)
    << "\" xmlns=\"http://www.w3.org/2000/svg\">"
    << endl;

  cout << triangle_grid;
  cout << province_grid;
  cout << kingdom_grid;
  cout << continent_grid;

  for (auto& p : kingdom_grid.points()) {
    cerr << p << endl;
  }

  cout << "</svg>" << endl;

  return 0;
}
