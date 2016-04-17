#ifndef FLGR_Guard
#define FLGR_Guard

#include "STD_HEAD.h"
#include "BASE.h"
#include "fltk.h"
#include "Point.h"

namespace FL_Graph
{
	enum FL_Color_type {
		red = FL_RED, blue = FL_BLUE, green = FL_GREEN,
		yellow = FL_YELLOW, white = FL_WHITE, black = FL_BLACK,
		magenta = FL_MAGENTA, cyan = FL_CYAN, dark_red = FL_DARK_RED,
		dark_green = FL_DARK_GREEN, dark_yellow = FL_DARK_YELLOW, dark_blue = FL_DARK_BLUE,
		dark_magenta = FL_DARK_MAGENTA, dark_cyan = FL_DARK_CYAN
	};
	enum FL_Transparency { invisible = 0, visible = 255 };
	class Color : public BASE_Graph::Color<FL_Color_type, Fl_Color, FL_Transparency, int> {};

	enum Line_style_type {
		solid = FL_SOLID,				// -------
		dash = FL_DASH,				// - - - -
		dot = FL_DOT,					// ....... 
		dashdot = FL_DASHDOT,			// - . - . 
		dashdotdot = FL_DASHDOTDOT,	// -..-..
	};
	class Line_style : public BASE_Graph::Line_style<Line_style_type, int>{};

	enum Font_type {
		helvetica = FL_HELVETICA,
		helvetica_bold = FL_HELVETICA_BOLD,
		helvetica_italic = FL_HELVETICA_ITALIC,
		helvetica_bold_italic = FL_HELVETICA_BOLD_ITALIC,
		courier = FL_COURIER,
		courier_bold = FL_COURIER_BOLD,
		courier_italic = FL_COURIER_ITALIC,
		courier_bold_italic = FL_COURIER_BOLD_ITALIC,
		times = FL_TIMES,
		times_bold = FL_TIMES_BOLD,
		times_italic = FL_TIMES_ITALIC,
		times_bold_italic = FL_TIMES_BOLD_ITALIC,
		symbol = FL_SYMBOL,
		screen = FL_SCREEN,
		screen_bold = FL_SCREEN_BOLD,
		zapf_dingbats = FL_ZAPF_DINGBATS
	};
	class Font :public BASE_Graph::Font</*FLTK's*/Font_type, int>{};


	class Raw_fl_img :BASE_Graph::Raw_image<Graph_lib::Point>
	{
	public:
		Raw_fl_img(std::string s)
			:Raw_fl_img(s, BASE_Graph::Suffix::none){}
		Raw_fl_img(std::string s, BASE_Graph::Suffix::Encoding force_encoding = BASE_Graph::Suffix::none);
		~Raw_fl_img(){ delete p; }
		void draw(Graph_lib::Point upleft)
			{ p->draw(upleft.x, upleft.y); }
		void draw(Graph_lib::Point upleft, int w, int h, int cx, int cy)
			{ p->draw(upleft.x,upleft.y,w,h,cx,cy); }
		bool is_valid(){ return valid; }
	private:
		bool valid;
		Fl_Image* p;
	};


	class Tracer : public BASE_Graph::Tracer<Graph_lib::Point, Color, Line_style, Font, Raw_fl_img>
	{
		void line_draw(Graph_lib::Point p1, Graph_lib::Point p2, Color col, Line_style stl)
		{
			fl_line(p1.x, p1.y, p2.x, p2.y);
		}
		void pixel_draw(Graph_lib::Point p, Color col, Line_style stl)
		{
			if (col.visibility())
				fl_line(p.x, p.y, p.x, p.y);
		}
		void font_draw(Graph_lib::Point p, std::string str, Font font, int size)
		{
			fl_draw(str.c_str(), p.x, p.y);
		}
		void set_color(Color col)
		{
			ocol = fl_color();
			fl_color(col.as_int());
		}
		void unset_color()
		{
			fl_color(ocol);
		}
		void set_style(Line_style stl)
		{
			// there is no good portable way of retrieving the current style
			fl_line_style(stl.style(), stl.width());
		}
		void unset_style()
		{
			fl_line_style(0);
		}
		void set_font(Font font, int size)
		{
			ofnt = fl_font();
			osz = fl_size();
			fl_font(font.as_int(), size);
		}
		void unset_font()
		{
			fl_font(ofnt, osz);
		}
	private:
		Fl_Color ocol; //color
		int ostyl;  // style
		int ofnt;  //font
		int osz;  // font size
	};
	Tracer tracer();
}

#endif //file guard