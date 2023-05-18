
// this file has the drawing code for our custom widget

#include "widget_button.h"
#include <cairomm/context.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>


widget_button::widget_button()
{
	text = "";
	pressed = false;
  
	// make the click events call the on_button_press_event function
	set_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::KEY_PRESS_MASK | Gdk::SCROLL_MASK);

}

widget_button::~widget_button()
{
}


void widget_button::set_text(string _text)
{
	text = _text;
	queue_draw();
}




bool widget_button::on_button_press_event(GdkEventButton* event)
{
	pressed = true;
	queue_draw();	
	return true;
}

bool widget_button::on_button_release_event(GdkEventButton* event)
{
	pressed = false;
	queue_draw();	
	return true;
}




bool widget_button::on_expose_event(GdkEventExpose* event)
{
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = get_window();

	if(window)
	{

    		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

		Gtk::Allocation allocation = get_allocation();

    		const int width = allocation.get_width();
    		const int height = allocation.get_height();

		cr->select_font_face("Bitstream Vera Sans", Cairo::FONT_SLANT_NORMAL,
			Cairo::FONT_WEIGHT_NORMAL);

		cr->set_source_rgba(0.0,0.0,0.0,1.0);
		cr->rectangle(0,0,width,height);
		cr->fill();

		cr->set_source_rgba(1.0,1.0,1.0,0.5 + ((float)pressed/3) );

		cr->set_font_size(width/12);
		cairo_text_extents_t txt_extents;
		cr->get_text_extents(text,txt_extents);
		cr->move_to( width/2 - (txt_extents.width/2),(height/2) + ((width/12)/2) );
	       	cr->show_text(text);

	}

  return true;
}


