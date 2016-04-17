//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//! debugé par liam 
//

#include "ststpstdlib.h"
#include "GUI.h"	// for Simple_window only (doesn't really belong in Window.h)

using namespace Graph_lib;
using std::string;

// Simple_window is basic scaffolding for ultra-simple interaction with graphics
// it provides one window with one "next" button for ultra-simple animation

struct Simple_window : Window {
	Simple_window(Point xy, int w, int h, const string& title )
	: Window(xy,w,h,title),
	  button_pushed(false),
	  next_button(Point(x_max()-70,0), 70, 20, "Next", cb_next)
	{ attach(next_button); }
	
	void wait_for_button()
		// modified event loop:
		// handle all events (as per default), quit when button_pushed becomes true
		// this allows graphics without control inversion
	{
		show();
		button_pushed = false;
#if 1
		// Simpler handler
		while (!button_pushed) Fl::wait();
		Fl::redraw();
#else
		// To handle the case where the user presses the X button in the window frame
		// to kill the application, change the condition to 0 to enable this branch.
		Fl::run();
#endif

	}

	Button next_button;
private:
	bool button_pushed;
	
	static void cb_next(Address, Address addr) // callback for next_button
	//	{ reference_to<Simple_window>(addr).next(); }
	{
		static_cast<Simple_window*>(addr)->next();
	}

	void next() { button_pushed = true; }
};

