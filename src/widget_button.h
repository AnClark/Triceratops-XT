

#ifndef _WIDGET_BUTTON
#define _WIDGET_BUTTON

#include <gtkmm/drawingarea.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// LV2UI stuff
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

class widget_button : public Gtk::DrawingArea
{
	public:

	widget_button();
	~widget_button();

	void set_text(string);
	
	protected:

	string text;    
	bool pressed;

	virtual bool on_expose_event(GdkEventExpose*);
	virtual bool on_button_press_event(GdkEventButton*);
	virtual bool on_button_release_event(GdkEventButton*);
};

#endif



