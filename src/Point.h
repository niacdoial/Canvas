
#ifndef POINT_GUARD
#define POINT_GUARD

//typedef void (*Callback)(void*,void*);

namespace Graph_lib {

template <typename C_type>
struct Coord {
	C_type x, y;
		Coord(C_type xx, C_type yy) : x(xx), y(yy) { }
		Coord() :x(0), y(0) { }
		//	Point& operator+=(Point d) { x+=d.x; y+=d.y; return *this; }
};

template<typename C_type> inline bool operator==(Coord<C_type> a, Coord<C_type> b) { return a.x == b.x && a.y == b.y; }
	template<typename C_type> inline bool operator!=(Coord<C_type> a, Coord<C_type> b){ return !(a == b); }

	typedef Coord<int> Point;

}
#endif