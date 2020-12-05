#ifndef hexgrid_hpp
#define hexgrid_hpp

#include <list>
#include <ostream>
#include <sstream>



constexpr unsigned combat_unit = 5;

constexpr unsigned mile = 5280;

constexpr unsigned province_unit = mile;
constexpr unsigned kingdom_unit = province_unit * 6;
constexpr unsigned continent_unit = kingdom_unit * 10;


// Creating a base triangle grid, then drawing hexes on it where each hex has a center vertex and 6 perimeter vertices
//
// . . . . . . . . . .
//  . . . . . . . . . .
// . . . . . . . . . .
//  . . . . . . . . . .
// . . . . . . . . . .
//  . . . . . . . . . .
// . . . . . . . . . .
//  . . . . . . . . . .
// . . . . . . . . . .
//  . . . . . . . . . .
// . . . . . . . . . .
//  . . . . . . . . . .
//

constexpr double sin60 = 0.86602540378;
constexpr unsigned pixels_per_unit = 50;
constexpr int triangle_grid_dx = pixels_per_unit;
constexpr int triangle_grid_dy = triangle_grid_dx * sin60;


struct Point {
  Point(const int& inX, const int& inY): mX(inX), mY(inY) {}

  const int& x() const { return mX; }
  const int& y() const { return mY; }

  friend std::ostream& operator<<(std::ostream& os, const Point& inPoint) {
    return os << inPoint.x() << "," << inPoint.y();
  }

private:
  const int mX;
  const int mY;
};


// Vertices are described by their x/y coordinate.
// Rendering Notes:
//   dX is N
//   dY is N * sin(60)
//   Even Ys have no X offset
//   Odd Ys have an X offset of (dX / 2)
struct Vertex : public Point {
  Vertex(const int& inX, const int& inY): Point(inX, inY) {}

  const int x_translated() const {
    if (y() % 2 == 0) {
      return triangle_grid_dx * x();
    }
    else {
      return triangle_grid_dx / 2 + triangle_grid_dx * x();
    }
  }
  const int y_translated() const {
    return triangle_grid_dy * y();
  }

  friend std::ostream& operator<<(std::ostream& os, const Vertex& inVertex) {
    return os << inVertex.x_translated() << "," << inVertex.y_translated();
  }
};


template <typename VertexType>
struct Polygon {
  virtual const std::list<VertexType> vertices() const = 0;

  friend std::ostream& operator<<(std::ostream& os, const Polygon& inPolygon) {
    os << "<polygon points=\"";
    for (auto& vertex : inPolygon.vertices()) {
      os << vertex << " ";
    }
    return os << "\"/>";
  }

};


// Triangles are described by the three corner vertices
template <typename VertexType=Vertex>
struct Triangle : public Polygon<VertexType> {
  Triangle(
    const VertexType& inA,
    const VertexType& inB,
    const VertexType& inC
  ):
    mA(inA),
    mB(inB),
    mC(inC)
  {
  }

  const VertexType& a() const { return mA; }
  const VertexType& b() const { return mB; }
  const VertexType& c() const { return mC; }

  virtual const std::list<VertexType> vertices() const {
    return { a(), b(), c() };
  }

private:
  const VertexType mA;
  const VertexType mB;
  const VertexType mC;

};


// Hexes are described by their center vertex and radius
template <unsigned Radius, typename VertexType=Vertex>
struct Hex : public Polygon<VertexType> {
  Hex(const VertexType& inCenter): mCenter(inCenter) {}

  static constexpr unsigned radius() { return Radius; }
  const VertexType& center() const { return mCenter; }

  virtual const std::list<VertexType> vertices() const {
    if (center().y() & 1) {
      return {
        VertexType(center().x() - (radius()/2)           , center().y() + radius()),
        VertexType(center().x() - (radius()/2) + radius(), center().y() + radius()),
        VertexType(center().x()                + radius(), center().y()           ),
        VertexType(center().x() - (radius()/2) + radius(), center().y() - radius()),
        VertexType(center().x() - (radius()/2)           , center().y() - radius()),
        VertexType(center().x()                - radius(), center().y()           )
      };
    }
    else {
      return {
        VertexType(center().x() + (radius()/2) - radius(), center().y() + radius()),
        VertexType(center().x() + (radius()/2)           , center().y() + radius()),
        VertexType(center().x()                + radius(), center().y()           ),
        VertexType(center().x() + (radius()/2)           , center().y() - radius()),
        VertexType(center().x() + (radius()/2) - radius(), center().y() - radius()),
        VertexType(center().x()                - radius(), center().y()           )
      };
    }
  }

private:
  const VertexType mCenter;

};


template <typename TileFactoryType>
struct Grid {
  using tile_factory = TileFactoryType;
  using tile_type = typename tile_factory::tile_type;

  Grid(const Point& start, const Point& end, const double& stroke_width): mStart(start), mEnd(end), mStrokeWidth(stroke_width) {}

  const std::list<Point> points() const {
    std::list<Point> point_list;
    for (int x = mStart.x(); x < mEnd.x(); x += tile_factory::dx) {
      for (int y = mStart.y(); y < mEnd.y(); y += tile_factory::dy) {
        if (tile_factory::can_make_tile(x, y)) {
          point_list.push_back(Point(x, y));
        }
      }
    }
    return point_list;
  }

  const std::list<tile_type> tiles() const{
    std::list<tile_type> tile_list;
    for (int x = mStart.x(); x < mEnd.x(); x += tile_factory::dx) {
      for (int y = mStart.y(); y < mEnd.y(); y += tile_factory::dy) {
        tile_factory::add_tiles(tile_list, x, y);
      }
    }
    return tile_list;
  }

  friend std::ostream& operator<<(std::ostream& os, const Grid<TileFactoryType>& inGrid) {
    os << "  <g fill=\"none\" stroke=\"gray\" stroke-width=\"" << inGrid.mStrokeWidth << "\">" << std::endl;
    for (auto& tile : inGrid.tiles()) {
      os << "    " << tile << std::endl;
    }
    os << "  </g>" << std::endl;
    return os;
  }

private:
  Point mStart;
  Point mEnd;
  const double mStrokeWidth;

};


struct SVG {};

#endif
