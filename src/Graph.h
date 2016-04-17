#ifndef GRAPH_GUARD
#define GRAPH_GUARD 1

#include "ststpstdlib.h"
#include "Point.h"

using std::string;

//#include<string>
//#include<cmath>
#include "BASE.h"
//#include "std_lib_facilities.h"

namespace Graph_lib {
	// defense against ill-behaved Linux macros:
#undef major
#undef minor
	///////////////////////////////////////////////////////////// pre-defs
	template <typename External_coord_t,
		typename Tracer_coord_t,
	class Tt = BASE_Graph::Tracer<Tracer_coord_t, typename Color_t, typename Style_t, class Font_t, class Raw_Img >>
	class Canvas;

	template <class Ct = Canvas<class lCoord, class in_coord, class Tt>>
	class Shape;
	///////////////////////////////////////////////////////////// canvas
	template <typename External_coord_t,
		typename Tracer_coord_t,
	class Tt>
	class Canvas
	{
		typedef typename Tt::Color_T Color_t;
		typedef typename Tt::Style_T Style_t;
		typedef typename Shape < Canvas < External_coord_t, Tracer_coord_t, Tt>> Shape_t;
	public:
		Canvas(Tt& t) :T(&t), vct(), cl(&(this->draw)) { t.attach(cl); }
		virtual void update();
		virtual void draw() const;
		virtual Tracer_coord_t Coords_converter(External_coord_t) const;
		virtual ~Canvas(){ t.detach(cl) };
		virtual void line_draw(External_coord_t p1, External_coord_t p2, Color_t cl, Style_t st)
		{
			T->line_draw(Coords_converter(p1), Coords_converter(p2), cl, st);
		}
		virtual void pixel_draw(External_coord_t p, Color_t cl, Style_t st)
		{
			T->pixel_draw(Coords_converter(p), cl, st);
		}
		virtual void font_draw(External_coord_t c, std::string str)
		{
			T->font_draw(Tracer_coord_t(c), str);
		}
		void attach(Shape_t& s) { vct.push_back(s); }
		void attach(Shape_t* s) { vct.push_back(s); }

		void detach(Shape_t& s);	// remove s from shapes 
		Tt* T;
	private:
		BASE_Graph::Vector_ref <Shape_t> vct;
		BASE_Graph::caller cl;
	};

	///////////////////////////////////////////////////////////////////////// shapes

	typedef double Fct(double);

	template <class Ct>
	class Shape  {	// deals with color and style, and holds sequence of lines
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_ty Color;
		typedef typename Ct::Tt::Style_ty Style;
	protected:
		Shape(Ct t) : C(&c) { }
		Shape(std::initializer_list<Coord> lst);  // add() the Points to this Shape

		//	Shape() : lcolor(fl_color()),
		//		ls(0),
		//		fcolor(Color::invisible) { }

		void add(Coord p){ points.push_back(p); }
		void set_point(int i, Coord p) { points[i] = p; }
	public:
		void draw() const;					// deal with color and draw_lines
	protected:
		virtual void draw_lines() const;	// simply draw the appropriate lines
	public:
		virtual void move(SingleCoord dx, SingleCoord dy);	// move the shape +=dx and +=dy

		void set_color(Color col) { lcolor = col; }
		Color color() const { return lcolor; }

		void set_style(Style sty) { ls = sty; }
		Style style() const { return ls; }

		void set_fill_color(Color col) { fcolor = col; }
		Color fill_color() const { return fcolor; }

		Coord point(int i) const { return points[i]; }
		int number_of_points() const { return int(points.size()); }

		virtual ~Shape() { }
		/*
		struct Window* attached;
		Shape(const Shape& a)
		:attached(a.attached), points(a.points), line_color(a.line_color), ls(a.ls)
		{
		if (a.attached)error("attempt to copy attached shape");
		}
		*/
		Shape(const Shape<Ct>& s) = delete;
		Shape<Ct>& operator=(const Shape<Ct>&) = delete;
	private:
		vector<Coord> points;	// not used by all shapes
		Color lcolor{ fl_color() };
		Style ls{ 0 };
		Color fcolor{ Color::Transparency(0) };
		Ct* C;

			//	Shape(const Shape&);
			//	Shape& operator=(const Shape&);
	};

	template <class Ct>
	// TODO: useless args and restructure needed
	struct SimpleFunction : Shape<Ct> {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		// the function parameters are not stored
		SimpleFunction(Fct f, double r1, double r2, Coord orig, int count = int(r2 - r1), double xscale = 1, double yscale = 1);
		//Function(Point orig, Fct f, double r1, double r2, int count, double xscale = 1, double yscale = 1);	
	};

	template <class Ct>
	struct Fill {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		Fill() :no_fill(true), fcolor(0) { }
		Fill(Color c) :no_fill(false), fcolor(c) { }

		void set_fill_color(Color col) { fcolor = col; }
		Color fill_color() { return fcolor; }
	protected:
		bool no_fill;
		Color fcolor;
	};

	template <class Ct>
	struct Line : Shape<Ct> {	// deals with color and style, and holds sequence of lines
		typedef typename Ct::lCoord Coord;
		Line(Coord p1, Coord p2) { add(p1); add(p2); }
	};

	template <class Ct>
	struct Rectangle : Shape<Ct> {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		Rectangle(Coord xy, SingleCoord ww, SingleCoord hh) :w{ ww }, h{ hh }
		{
			if (h <= 0 || w <= 0) error("Bad rectangle: non-positive side");
			add(xy);
		}
		Rectangle(Coord x, Coord y) :w{ y.x - x.x }, h{ y.y - x.y }
		{
			if (h <= 0 || w <= 0) error("Bad rectangle: first point is not top left");
			add(x);
		}
		void draw_lines() const;

		//	void set_fill_color(Color col) { fcolor = col; }
		//	Color fill_color() { return fcolor; }

		SingleCoord height() const { return h; }
		SingleCoord width() const { return w; }
	private:
		SingleCoord h;			// height
		SingleCoord w;			// width
		//	Color fcolor;	// fill color; 0 means "no fill"
	};

	template<typename Coord=Coord<typename C_type>>
	bool intersect(Coord p1, Coord p2, Coord p3, Coord p4);

	template <class Ct>
	struct Open_polyline : Shape<Ct> {	// open sequence of lines
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;

		using Shape<Ct>::Shape;
		void add(Coord p) { Shape<Ct>::add(p); }
		void draw_lines() const;
	};

	template <class Ct>
	struct Closed_polyline : Open_polyline<Ct> {	// closed sequence of lines
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		using Open_polyline<Ct>::Open_polyline;
		void draw_lines() const;

		//	void add(Point p) { Shape::add(p); }
	};

	template <class Ct>
	struct Polygon : Closed_polyline<Ct> {	// closed sequence of non-intersecting lines
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		using Closed_polyline<Ct>::Closed_polyline;
		void add(lCoord p);
		void draw_lines() const;
	};

	template <class Ct>
	struct Lines : Shape<Ct> {	// indepentdent lines
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		Lines() :Shape<Ct>() {}
		Lines(std::initializer_list<Coord> lst) : Shape<Ct>(lst) { if (lst.size() % 2) error("odd number of points for Lines"); }
		void draw_lines() const;
		void add(Coord p1, Coord p2) { Shape<Ct>::add(p1); Shape<Ct>::add(p2); }
	};

	template <class Ct>
	struct Text : Shape<Ct> {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		// the point is the bottom left of the first letter
		Text(Coord x, const string& s) : lab(s) { add(x); }

		void draw_lines() const;

		void set_label(const string& s) { lab = s; }
		string label() const { return lab; }

		void set_font(Font f) { fnt = f; }
		Font font() const { return Font(fnt); }

		void set_font_size(int s) { fnt_sz = s; }
		int font_size() const { return fnt_sz; }
	private:
		string lab;	// label
		Font fnt{ fl_font() };
		int fnt_sz{ (14 < fl_size()) ? fl_size() : 14 };	// at least 14 point
	};

	template <class Ct>
	// TODO :needs other args
	struct Axis : Shape<Ct> {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		// representation left public
		enum Orientation { x, y, z };
		Axis(Orientation d, Coord xy, SingleCoord length, int nummber_of_notches = 0, string label = "");

		void draw_lines() const;
		void move(int dx, int dy);

		void set_color(Color c);

		Text label;
		Lines notches;
		//	Orientation orin;
		//	int notches;
	};

	template <class Ct>
	struct Circle : Shape<Ct> {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;

		Circle(Coord p, SingleCoord rr)	// center and radius
			:r{ rr } {
			add(Coord{ p.x - r, p.y - r });
		}

		void draw_lines() const;

		Coord center() const { return{ point(0).x + r, point(0).y + r }; }

		void set_radius(SignleCoord rr) { r = rr; }
		SingleCoord radius() const { return r; }
	private:
		SingleCoord r;
	};

	template <class Ct>
	struct Ellipse : Shape<Ct> {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;

		Ellipse(Coord p, SingleCoord ww, SingleCoord hh)	// center, min, and max distance from center
			:w{ ww }, h{ hh } {
			add(Coord{ p.x - ww, p.y - hh });
		}

		void draw_lines() const;

		Coord center() const { return{ point(0).x + w, point(0).y + h }; }
		Coord focus1() const { return{ center().x + SingleCoord(sqrt(double(w*w - h*h))), center().y }; }
		Coord focus2() const { return{ center().x - SingleCoord(sqrt(double(w*w - h*h))), center().y }; }

		void set_major(SingleCoord ww) { w = ww; }
		SingleCoord major() const { return w; }
		void set_minor(SingleCoord hh) { h = hh; }
		SingleCoord minor() const { return h; }
	private:
		SingleCoord w;
		SingleCoord h;
	};
	/*
	struct Mark : Text {
	static const int dw = 4;
	static const int dh = 4;
	Mark(Point xy, char c) : Text(Point(xy.x-dw, xy.y+dh),string(1,c)) {}
	};
	*/
	template <class Ct>
	struct Marked_polyline : Open_polyline<Ct> {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		Marked_polyline(const string& m) :mark(m) { }
		void draw_lines() const;
	private:
		string mark;
	};

	template <class Ct>
	struct Marks : Marked_polyline {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;

		Marks(const string& m) :Marked_polyline(m)
		{
			set_color(Color(Color::Transparency(0)));
		}
	};

	template <class Ct>
	struct Mark : Marks {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;
		Mark(Coord xy, char c) : Marks(string(1, c)) { add(xy); }
	};

	/*

	struct Marks : Shape {
	Marks(char m) : mark(string(1,m)) { }
	void add(Point p) { Shape::add(p); }
	void draw_lines() const;
	private:
	string mark;
	};
	*/

	template <class Ct>
	struct Image : Shape<Ct> {
		typedef typename Ct::lCoord Coord;
		typedef typename Coord::C_type SingleCoord;
		typedef typename Ct::Tt::Color_t Color;
		typedef typename Ct::Tt::Style_t Style;

		Image(Coord xy, string s, Suffix::Encoding e = Suffix::none)
			:w(0), h(0), fn(xy, ""), p(new Raw_image(s, e)) {
			add(xy);
		}
		~Image() { delete p; }
		void draw_lines() const;
		void set_mask(Coord xy, int ww, int hh) { w = ww; h = hh; cx = xy.x; cy = xy.y; }
		void move(int dx, int dy) { Shape::move(dx, dy); p->draw(point(0).x, point(0).y); }
	private:
		int w, h, cx, cy; // define "masking box" within image relative to position (cx,cy)
		Ct::Tt::Raw_image* p;
		Text fn;
	};

}
#endif

