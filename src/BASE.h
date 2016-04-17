/*
this is the base stuff, containing the parent classes for platform (FLTK, STD, GTK,...) dependent structure.
*/
#ifndef BASEGR_Guard
#define BASEGR_Guard

#include "STD_HEAD.h"

namespace BASE_Graph
{

	template<class T> class Vector_ref {
		vector<T*> v;
		vector<T*> owned;
	public:
		Vector_ref() {}

		Vector_ref(T* a, T* b = 0, T* c = 0, T* d = 0)
		{
			if (a) push_back(a);
			if (b) push_back(b);
			if (c) push_back(c);
			if (d) push_back(d);
		}

		~Vector_ref() { for (unsigned int i = 0; i < owned.size(); ++i) delete owned[i]; }

		void push_back(T& s) { v.push_back(&s); }
		void push_back(T* p) { v.push_back(p); owned.push_back(p); }

		// ???void erase(???)

		T& operator[](int i) { return *v[i]; }
		const T& operator[](int i) const { return *v[i]; }
		unsigned int size() const { return v.size(); }
	};

	class caller  // MUST be revamped (operator() must be static func)
	{
	private:
		void(*fnc)(void);
	public:
		caller(void(*fc)(void)) :fnc(fc){}
		void operator() (){ (*fnc)(); }
		bool operator== (caller other){ return fnc == other.fnc; }
	};

	template <typename Color_type, // user_accessible
		typename Internal_color_type, // defined by graphical platform
		typename Transparency,  //user accessible
		typename Internal_transparency_type> // defined by graphical platform>
	struct Color {

		Color(Color_type cc) :c(Internal_color_type(cc)), v(Internal_transparency_type(Transparency())) {}
		Color(Color_type cc, Transparency vv) :c(Internal_color_type(cc)), v(Internal_transparency_type(vv)) {}
		Color(int cc) :c(Internal_color_type(cc)), v(Internal_transparency_type(Transparency())) { }
		Color(Transparency vv) :c(Internal_color_type(Color_type())), v(Internal_transparency_type(vv)) { }

		Color_type get_color() const { return Color_type(c); }
		void set_color(Color_type cc) { c = Internal_color_type(cc); }
		int as_int() const { return int(Color_type(c)); }
		int visibility() const { return int(Transparency(v)); }
		void set_visibility(Transparency vv) { v = Internal_transparency_type(vv); }
		virtual ~Color();
	private:
		Internal_transparency_type v;
		Internal_color_type c;
	};


	template<typename External_type, typename Internal_type>
	struct Line_style {
		Line_style(External_type ss, int ww = 0) :s(Internal_type(ss)), w(ww) { }
		Line_style(int ss) :s(int(External_type(ss))), w(0) { }

		int width() const { return w; }
		Internal_type style() const { return s; }
		virtual ~Line_style();
	private:
		Internal_type s;
		int w;
	};


	template <typename Font_type, typename Internal_font_type>
	class Font {
	public:
		Font(Font_type ff) :f(Internal_font_type(ff)) { }
		Font(int ff) :f(Internal_font_type(Font_type(ff))) { }
		int as_int() const { return int(Font_type(f)); }
	private:
		Internal_font_type f;
	};

	bool can_open(const string& s)
		// check if a file named s exists and can be opened for reading
	{
		std::ifstream ff(s.c_str());
		return bool(ff);
	}

	struct Suffix {
		enum Encoding { none, jpg, gif, bmp };
	};

	std::map<string, Suffix::Encoding> suffix_map;

	int init_suffix_map()
	{
		suffix_map["jpg"] = Suffix::jpg;
		suffix_map["JPG"] = Suffix::jpg;
		suffix_map["jpeg"] = Suffix::jpg;
		suffix_map["JPEG"] = Suffix::jpg;
		suffix_map["gif"] = Suffix::gif;
		suffix_map["GIF"] = Suffix::gif;
		suffix_map["bmp"] = Suffix::bmp;
		suffix_map["BMP"] = Suffix::bmp;
		return 0;
	}

	Suffix::Encoding get_encoding(const string& s)
		// try to deduce type from file name using a lookup table
	{
		static int x = init_suffix_map();

		string::const_iterator p = find(s.begin(), s.end(), '.');
		if (p == s.end()) return Suffix::none;	// no suffix

		string suf(p + 1, s.end());
		return suffix_map[suf];
	}

	template<typename coord>class Raw_image
	{
	public:
		Raw_image(){}
		Raw_image(std::string path){}
		virtual void draw(coord upleft) const = 0;
		virtual void draw(coord upleft, int, int, int, int)const = 0;
		virtual ~Raw_image(){}
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	template<class Coord_t, class Color_t, class Style_t, class Font_t, class Raw_image>
	struct Tracer //contains the GUI funcs to draw.
		//Can be internal or external types. just remember which one you use when implementing it.
		// you can leave "set_color" and "set_style" empty, or ignore the "col" and "stl" args, to fit better the specs of the library.
	{

		virtual void pixel_draw(Coord_t c,
			Color_t col = Color_t(), Style_t stl = Style_t()) const = 0;
		virtual void line_draw(Coord_t c1, Coord_t c2,
			Color_t col = Color_t(), Style_t stl = Style_t()) const = 0;
		virtual void font_draw(Coord_t p, std::string str,
			Font_t font, int size) const = 0;
		virtual void set_color(Color_t col = Color_t());
		virtual void unset_color();
		virtual void set_style(Style_t col = Style_t());
		virtual void unset_style();
		virtual void set_font(Font_t font);
		virtual void unset_font();
		virtual ~Tracer();
		void attach(caller cl){vct.push_back(cl)}
		void detach(caller cl)
		{
			for (unsigned int i = vct.size(); 0 < i; --i)	// guess last attached will be first released
				if (vct[i - 1] == s)
					shapes.erase(shapes.begin() + (i - 1));//&shapes[i-1]);
			// TODO: ther is a better way.
		}
	private:
		vector<caller> vct;
	};

}

#endif //file guard