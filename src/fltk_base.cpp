#include "fltk_base.h"
#include "pch.h"
#include "STD_HEAD.h"
namespace FL_Graph{
	// first generic stuff (help funcs and co)
	struct Bad_image : Fl_Image {
		Bad_image(int h, int w) : Fl_Image(h, w, 0) { }
		void draw(int x, int y, int, int, int, int) { draw_empty(x, y); }
	};

	// then 'real' stuff
	Raw_fl_img::Raw_fl_img(std::string s, BASE_Graph::Suffix::Encoding force_encoding)
	{
		if (!BASE_Graph::can_open(s)) {
		//	fn.set_label("cannot open \"" + s + '\"');
			p = new Bad_image(30, 20);	// the "error image"
			return;
		}

		if (force_encoding == BASE_Graph::Suffix::none) force_encoding = BASE_Graph::get_encoding(s);

		switch (force_encoding) {
		case BASE_Graph::Suffix::jpg:
			p = new Fl_JPEG_Image(s.c_str());
			break;
		case BASE_Graph::Suffix::gif:
			p = new Fl_GIF_Image(s.c_str());
			break;
			//	case Suffix::bmp:
			//		p = new Fl_BMP_Image(s.c_str());
			//		break;
		default:	// Unsupported image encoding
			//fn.set_label("unsupported file type \"" + s + '\"');
			p = new Bad_image(30, 20);	// the "error image"
		}

	}
}