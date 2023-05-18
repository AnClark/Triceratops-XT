

#include <string>
#include <iostream>
#include <gtkmm.h>

// these are our custom widget includes

using namespace std;

//--------------------------------------------------------

class logo_gui : public Gtk::DrawingArea
{
	public:

	logo_gui();
	virtual ~logo_gui();
	void load_logo(string);
	int pos_mode;

	Gdk::Color top_colour;
	Gdk::Color bottom_colour;

	private:

	bool on_expose_event(GdkEventExpose* event);
	bool logo_loaded;

	Glib::RefPtr< Gdk::Pixbuf > logo_image;

	Cairo::RefPtr< Cairo::Context > image_context_ptr_;
	Cairo::RefPtr< Cairo::ImageSurface > image_surface_ptr_;
	Glib::RefPtr< Gdk::Pixbuf > image_ptr_;
	Cairo::Format format;

};



