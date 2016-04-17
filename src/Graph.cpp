#include "ststpstdlib.h"
#include "Graph.h"
#include<map>
#include "pch.h"

using std::pair;
using std::string;

namespace Graph_lib {

	template <typename External_coord_t,
		typename Tracer_coord_t,
		//typename color_t, typename Style_t,
	class Tt = BASE_Graph::Tracer < Tracer_coord_t, class Color_t, class Style_t >>
		void Canvas<External_coord_t, Tracer_coord_t, Tt>::detach(Shape& s)
	{
		for (unsigned int i = shapes.size(); 0 < i; --i)	// guess last attached will be first released
			if (shapes[i - 1] == &s)
				shapes.erase(shapes.begin() + (i - 1));//&shapes[i-1]);
	}

	template <typename External_coord_t,
		typename Tracer_coord_t,
		//typename color_t, typename Style_t,
	class Tt>
		void Canvas<External_coord_t, Tracer_coord_t, Tt>::draw()
	{
		for (unsigned int i = 0; i < shapes.size(); i++) vct[i]->draw();
	}

	template <class Ct>
	void Shape<Ct>::draw_lines() const
	{
		if (color().visibility() && 1 < points.size())	// draw sole pixel?
			for (unsigned int i = 1; i < points.size(); ++i)
				C->line_draw(points[i - 1], points[i], color(), style());
		else if (color().visibility() && points.size() == 1)
			C->pixel_draw(points[0], color(), style());
	}

	template <class Ct>
	void Shape<Ct>::draw() const
	{
		C->T->set_color(lcolor);  // set,
		C->T->set_style(ls);
		draw_lines();  // draw,
		C->T->unset_color();
		C->T->unset_style();  // and unset.
	}


	// does two lines (p1,p2) and (p3,p4) intersect?
	// if se return the distance of the intersect point as distances from p1
	template<typename Coord_t=Coord<typename C_type>> inline pair<double, double> line_intersect(Coord_t p1, Coord_t p2, Coord_t p3, Coord_t p4, bool& parallel)
	{
		double x1 = p1.x;
		double x2 = p2.x;
		double x3 = p3.x;
		double x4 = p4.x;
		double y1 = p1.y;
		double y2 = p2.y;
		double y3 = p3.y;
		double y4 = p4.y;

		double denom = ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
		if (denom == 0){
			parallel = true;
			return pair<double, double>(0, 0);
		}
		parallel = false;
		return pair<double, double>(((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / denom,
			((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / denom);
	}

	//intersection between two line segments
	//Returns true if the two segments intersect,
	//in which case intersection is set to the point of intersection
	template <typename Coord_t=Coord<typename C_type>> bool line_segment_intersect(Coord_t p1, Coord_t p2, Coord_t p3, Coord_t p4, Coord_t& intersection){
		bool parallel;
		pair<double, double> u = line_intersect(p1, p2, p3, p4, parallel);
		if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
		intersection.x = p1.x + u.first*(p2.x - p1.x);
		intersection.y = p1.y + u.first*(p2.y - p1.y);
		return true;
	}

	template <class Ct>
	void Polygon<Ct>::add(lCoord p)
	{
		int np = number_of_points();

		if (1 < np) {	// check that thenew line isn't parallel to the previous one
			if (p == point(np - 1)) error("polygon point equal to previous point");
			bool parallel;
			line_intersect(point(np - 1), p, point(np - 2), point(np - 1), parallel);
			if (parallel)
				error("three polygon points lie in a straight line");
		}

		for (int i = 1; i < np - 1; ++i) {	// check that new segment doesn't interset and old point
			lCoord ignore(0, 0);
			if (line_segment_intersect(point(np - 1), p, point(i - 1), point(i), ignore))
				error("intersect in polygon");
		}


		Closed_polyline<Ct>::add(p);
	}

	template <class Ct>
	void Polygon<Ct>::draw_lines() const
	{
		if (number_of_points() < 3) error("less than 3 points in a Polygon");
		Closed_polyline<Ct>::draw_lines();
	}

	template <class Ct>
	// TODO: what the heck?!
	void Open_polyline<Ct>::draw_lines() const
	{
		if (fill_color().visibility()) {
			fl_color(fill_color().as_int());
			fl_begin_complex_polygon();
			for (int i = 0; i < number_of_points(); ++i){
				fl_vertex(point(i).x, point(i).y);
			}
			fl_end_complex_polygon();
			fl_color(color().as_int());	// reset color
		}

		if (color().visibility())
			Shape<Ct>::draw_lines();
	}

	template <class Ct>
	void Closed_polyline<Ct>::draw_lines() const
	{
		Open_polyline<Ct>::draw_lines();

		if (color().visibility())	// draw closing line:
			C->line_draw(point(number_of_points() - 1), point(0), color(), style());
	}

	template <class Ct>
	void Shape<Ct>::move(Ct::lCoord:C_type dx, Ct::lCoord::C_type dy)
	{
		for (unsigned int i = 0; i < points.size(); ++i) {
			points[i].x += dx;
			points[i].y += dy;
		}
	}

	template <class Ct>
	void Lines<Ct>::draw_lines() const
	{
		//	if (number_of_points()%2==1) error("odd number of points in set of lines");
		if (color().visibility())
			for (int i = 1; i < number_of_points(); i += 2)
				C->line_draw(point(i - 1), point(i), color(), style());
	}

	template <class Ct>
	// TODO :heck again
	void Text<Ct>::draw_lines() const
	{
		C->T->set_font(fnt, fnt_sz);
		C->font_draw(point(0), lab, );
		C->T->unset_font();
	}

	template <class Ct>
	SimpleFunction<Ct>::SimpleFunction(Fct f, double r1, double r2, lCoord xy, int count, double xscale, double yscale)
		// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
		// x coordinates are scaled by xscale and y coordinates scaled by yscale
	{
		if (r2 - r1 <= 0) error("bad graphing range");
		if (count <= 0) error("non-positive graphing count");
		double dist = (r2 - r1) / count;
		double r = r1;
		for (int i = 0; i < count; ++i) {
			add(lCoord(xy.x + int(r*xscale), xy.y - int(f(r)*yscale)));
			r += dist;
		}
	}

	template <class Ct>
	void Rectangle<Ct>::draw_lines() const
	{
		if (fill_color().visibility()) {	// fill
			C->T->set_color(fill_color());
			fl_rectf(point(0).x, point(0).y, w, h);
			C->T->unset_color();
			C->T->set_color(color());	// reset color
		}    // TODO: extract more!

		if (color().visibility()) {	// edge on top of fill
			C->T->set_color(color());
			fl_rect(point(0).x, point(0).y, w, h);
		}
	}

	template <class Ct>
	Axis<Ct>::Axis(Orientation d, lCoord xy, int length, int n, string lab)
		:label(Point(0, 0), lab)
	{
		if (length < 0) error("bad axis length");
		switch (d){
		case Axis<Ct>::x:
		{	Shape::add(xy);	// axis line
		Shape::add(lCoord(xy.x + length, xy.y));	// axis line
		if (1 < n) {
			int dist = length / n;
			int x = xy.x + dist;
			for (int i = 0; i < n; ++i) {
				notches.add(Point(x, xy.y), Point(x, xy.y - 5));
				x += dist;
			}
		}
		// label under the line
		label.move(length / 3, xy.y + 20);
		break;
		}
		case Axis<Ct>::y:
		{	Shape::add(xy);	// a y-axis goes up
		Shape::add(lCoord(xy.x, xy.y - length));
		if (1 < n) {
			int dist = length / n;
			int y = xy.y - dist;
			for (int i = 0; i < n; ++i) {
				notches.add(Point(xy.x, y), Point(xy.x + 5, y));
				y -= dist;
			}
		}
		// label at top
		label.move(xy.x - 10, xy.y - length - 10);
		break;
		}
		case Axis<Ct>::z:
			error("z axis not implemented");
		}
	}

	template <class Ct>
	void Axis<Ct>::draw_lines() const
	{
		Shape<Ct>::draw_lines();	// the line
		notches.draw();	// the notches may have a different color from the line
		label.draw();	// the label may have a different color from the line
	}

	template <class Ct>
	void Axis<Ct>::set_color(Color c)
	{
		Shape<Ct>::set_color(c);
		notches.set_color(c);
		label.set_color(c);
	}

	template <class Ct>
	void Axis<Ct>::move(lCoord::C_type dx, lCoord::C_type dy)
	{
		Shape<Ct>::move(dx, dy);
		notches.move(dx, dy);
		label.move(dx, dy);
	}

	template <class Ct>
	void Circle<Ct>::draw_lines() const
	{
		if (fill_color().visibility()) {	// fill
			C->T->set_color(fill_color());
			fl_pie(point(0).x, point(0).y, r + r - 1, r + r - 1, 0, 360);
			C->T->set_color(color());	// reset color
		}

		if (color().visibility()) {
			C->T->set_color(color());
			fl_arc(point(0).x, point(0).y, r + r, r + r, 0, 360);
		}
	}

	template <class Ct>
	void Ellipse<Ct>::draw_lines() const
	{
		if (fill_color().visibility()) {	// fill
			C->T->set_color(fill_color());
			fl_pie(point(0).x, point(0).y, w + w - 1, h + h - 1, 0, 360);
			C->T->set_color(color());	// reset color
		}

		if (color().visibility()) {
			C->T->set_color(color());
			fl_arc(point(0).x, point(0).y, w + w, h + h, 0, 360);
		}
	}

	template <class Ct>
	void draw_mark(Ct::External_coord_t xy, char c)
	{
		static const int dx = 4;
		static const int dy = 4;
		string m(1, c);
		C->font_draw(Ct::External_coord_t(xy.x - dx, xy.y + dy), m);
	}

	template <class Ct>
	void Marked_polyline<Ct>::draw_lines() const
	{
		Open_polyline<Ct>::draw_lines();
		for (int i = 0; i < number_of_points(); ++i)
			draw_mark<Ct>(point(i), mark[i % mark.size()]);
	}
	/*
	void Marks::draw_lines() const
	{
	for (int i=0; i<number_of_points(); ++i)
	fl_draw(mark.c_str(),point(i).x-4,point(i).y+4);
	}
	*/




	// somewhat overelaborate constructor
	// because errors related to image files can be such a pain to debug
	template <class Ct>
	void Image<Ct>::draw_lines() const
	{
		if (fn.label() != "") fn.draw_lines();

		if (w&&h)
			p->draw(point(0), w, h, cx, cy);
		else
			p->draw(point(0));
	}

} // Graph