
// This is the header for our custom knob.
// We override the on_expose_event function so GTK calls into this class
// to draw the knob

#ifndef KNOB
#define KNOB

#include <gtkmm/drawingarea.h>
#include <iostream>

using namespace std;

// LV2UI stuff
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

class knob : public Gtk::DrawingArea
{
  public:
    
    // public Lv2 communication stuff
    LV2UI_Controller controller;
    LV2UI_Write_Function write_function;

   Gdk::Color top_colour;
   Gdk::Color bottom_colour;
    
    knob();
    ~knob();
    void position_top(bool);
    int pos_mode;

    void set_label(string);
    void set_value(float);
    void set_snap(bool);
    void set_min(float);
    void set_max(float);

    bool invert;
    int port_number;
    float knob_value,value,min,max;
    float scroll_wheel_speed;

  protected:

	// stores frequency we're currently at.

	bool drag;
	bool snap;
	string label;    
    	float grad_top_colour;
    	float grad_bottom_colour;
	

	bool on_expose_event(GdkEventExpose* event);
	bool on_button_press_event(GdkEventButton* event);
	virtual bool on_scroll_event(GdkEventScroll*);
	virtual bool on_button_release_event(GdkEventButton*);
	virtual bool on_motion_notify_event(GdkEventMotion*);
	void draw_slider(int,int);
};

#endif


	
