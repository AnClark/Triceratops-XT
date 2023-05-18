
// This is the header for our custom filter_widget.
// We override the on_expose_event function so GTK calls into this class
// to draw the filter_widget

#ifndef filterWIDGET
#define filterWIDGET

#include <gtkmm/drawingarea.h>
#include <gtkmm.h>
#include <iostream>

using namespace std;

// LV2UI stuff
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

class filter_widget : public Gtk::DrawingArea
{
  public:
    
    // public Lv2 communication stuff
    LV2UI_Controller controller;
    LV2UI_Write_Function write_function;

	Gdk::Color top_colour;
	Gdk::Color bottom_colour;
    
    filter_widget();
    ~filter_widget();
    void set_value(float);
    int pos_mode;

    void set_label(string);

    int port_number;
    float val_cutoff;
    float val_res;
    int val;

  protected:


	string label;    

	virtual bool on_expose_event(GdkEventExpose*);
	virtual bool on_button_press_event(GdkEventButton*);
	virtual bool on_button_release_event(GdkEventButton*);
};

#endif



