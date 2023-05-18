
// This is the header for our custom wave_widget.
// We override the on_expose_event function so GTK calls into this class
// to draw the wave_widget

#ifndef WAVEWIDGET
#define WAVEWIDGET

#include <gtkmm/drawingarea.h>
#include <iostream>

using namespace std;

// LV2UI stuff
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

class wave_widget : public Gtk::DrawingArea
{
	public:
    
	// public Lv2 communication stuff
	LV2UI_Controller controller;
	LV2UI_Write_Function write_function;

	Gdk::Color top_colour;
	Gdk::Color bottom_colour;
    
	wave_widget();
	~wave_widget();

	void set_label(string);
	void set_value(int);
	void set_lfo_mode(bool);
	int pos_mode;

	int port_number;
	int last_waveform;
	bool lfo_mode;
	int val;

  protected:

	string label;    

	virtual bool on_expose_event(GdkEventExpose*);
	virtual bool on_button_press_event(GdkEventButton*);
	virtual bool on_button_release_event(GdkEventButton*);
};

#endif



