
// This is the header for our custom toggle.
// We override the on_expose_event function so GTK calls into this class
// to draw the toggle

#ifndef TOGGLEWIDGET
#define TOGGLEWIDGET

#include <gtkmm/drawingarea.h>
#include <cmath>
#include <cairomm/context.h>
#include <iostream>

using namespace std;

// LV2UI stuff
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

class toggle : public Gtk::DrawingArea
{
  public:
    
    // public Lv2 communication stuff
    LV2UI_Controller controller;
    LV2UI_Write_Function write_function;

	Gdk::Color top_colour;
	Gdk::Color bottom_colour;
    
    toggle();
    ~toggle();
    void position_top(bool);
    void set_label(string);
    void set_value(int);

    int port_number;
    int pos_mode;
    int val;

  protected:

	int rounded;
	string label;    
    	float grad_top_colour;
    	float grad_bottom_colour;

	virtual bool on_expose_event(GdkEventExpose*);
	virtual bool on_button_press_event(GdkEventButton*);
	virtual bool on_button_release_event(GdkEventButton*);
};

#endif



