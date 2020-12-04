#ifndef hexgrid_hpp
#define hexgrid_hpp

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
constexpr double dx = 1.0 * pixels_per_unit;
constexpr double dy = dx * sin60;

// Vertices are described by their x/y coordinate.
// Rendering Notes:
//   dX is N
//   dY is N * sin(60)
//   Even Ys have no X offset
//   Odd Ys have an X offset of (dX / 2)
struct Vertex {
  Vertex(const int& inX, const int& inY): mX(inX), mY(inY) {}

  const int& x() const { return mX; }
  const int& y() const { return mY; }

  const int x_translated() const {
    if (y() % 2 == 0) {
      return dx * x();
    }
    else {
      return dx / 2 + dx * x();
    }
  }
  const int y_translated() const {
    return dy * y();
  }

  friend std::ostream& operator<<(std::ostream& os, const Vertex& inVertex) {
    return os << inVertex.x_translated() << "," << inVertex.y_translated();
  }

private:
  const int mX;
  const int mY;
};


// Triangles are described by the three corner vertices
struct Triangle {
  Triangle(
    const Vertex& inA,
    const Vertex& inB,
    const Vertex& inC
  ):
    mA(inA),
    mB(inB),
    mC(inC)
  {
  }

  const Vertex& a() const { return mA; }
  const Vertex& b() const { return mB; }
  const Vertex& c() const { return mC; }

  friend std::ostream& operator<<(std::ostream& os, const Triangle& inTriangle) {
    return os
      << "<polygon points=\""
      << inTriangle.a()
      << " " << inTriangle.b()
      << " " << inTriangle.c()
      << "\" fill=\"none\" stroke=\"gray\" stroke-width=\"0.25\"/>";
  }

private:
  const Vertex mA;
  const Vertex mB;
  const Vertex mC;

};


// Hexes are described by their center vertex and radius
template <unsigned R>
struct Hex {
  Hex(const Vertex& inCenter): mCenter(inCenter) {}

  const Vertex& center() const { return mCenter; }

  friend std::ostream& operator<<(std::ostream& os, const Hex& inHex) {
    // os << "<circle cx=\"" << inHex.center().x_translated() << "\" cy=\"" << inHex.center().y_translated() << "\" r=2 fill=\"gray\"/>";
    os << "<polygon points=\"";
    if (inHex.center().y() & 1) {
      os
        << Vertex(inHex.center().x()-(R/2)           , inHex.center().y()+R) << " "
        << Vertex(inHex.center().x()+R-(R/2), inHex.center().y()+R) << " "
        << Vertex(inHex.center().x()+R, inHex.center().y()           ) << " "
        << Vertex(inHex.center().x()+R-(R/2), inHex.center().y()-R) << " "
        << Vertex(inHex.center().x()-(R/2)           , inHex.center().y()-R) << " "
        << Vertex(inHex.center().x()-R, inHex.center().y()           )
        ;
    }
    else {
      os
        << Vertex(inHex.center().x()-R+(R/2), inHex.center().y()+R) << " "
        << Vertex(inHex.center().x()+(R/2)           , inHex.center().y()+R) << " "
        << Vertex(inHex.center().x()+R, inHex.center().y()           ) << " "
        << Vertex(inHex.center().x()+(R/2)           , inHex.center().y()-R) << " "
        << Vertex(inHex.center().x()-R+(R/2), inHex.center().y()-R) << " "
        << Vertex(inHex.center().x()-R, inHex.center().y()           )
        ;
    }
    return os << "\" fill=\"none\" stroke=\"gray\" stroke-width=\"" << (0.75 * R) << "\"/>";
  }


private:
  const Vertex mCenter;

};


struct Grid {};


struct SVG {};

#endif
