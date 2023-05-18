
// this file has the drawing code for our custom widget

#include "spacer_widget.h"
#include <cairomm/context.h>

#include <iostream>



spacer::spacer()
{
	label = "";
	pos_mode = 0;

	top_colour.set( "#550000");
	bottom_colour.set( "#220000");
  
	// make the click events call the on_button_press_event function
	set_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);

	// make the click event do that function
	// signal_button_press_event().connect( sigc::mem_fun(*this, &spacer::on_button_press_event) );
  
	set_size_request(40,70);
}

spacer::~spacer()
{
}

void spacer::position_top(bool mode)
{
	if (mode)
	{
		grad_top_colour = 0.3;
		grad_bottom_colour = 0.1*2;
	}
	else
	{
		grad_top_colour = 0.1*2;
		grad_bottom_colour = 0.1;
	}
}

void spacer::set_label(string text)
{
	label = text;
	queue_draw();
}

void spacer::set_value(int _value)
{
	val = _value;	
	queue_draw();
}

bool spacer::on_button_press_event(GdkEventButton* event)
{
	return true;
}

bool spacer::on_button_release_event(GdkEventButton* event)
{



	// here we use the LV2UI_Controller and LV2UI_write_function "things"
	// to write some data to a port
	// write_function( controller, port_number, sizeof(float), 0, (const void*)&new_value);
	
	queue_draw();
  
  return true;
}

bool spacer::on_expose_event(GdkEventExpose* event)
{
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = get_window();

	if(window)
	{
		Gtk::Allocation allocation = get_allocation();
    		const int width = allocation.get_width();
    		const int height = allocation.get_height() - (allocation.get_height()/3);
    		const int height_offset = allocation.get_height()/6;

    		// coordinates for the center of the window
    		int xc, yc;
    		xc = width / 2;
    		yc = height / 2;

    		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

    		// clip to the area indicated by the expose event so that we only redraw
    		// the portion of the window that needs to be redrawn
    		cr->rectangle(event->area.x, event->area.y,
            		event->area.width, event->area.height);
    		cr->clip();
    
   		 // background grad

     		Cairo::RefPtr<Cairo::LinearGradient> back_grad =
			Cairo::LinearGradient::create( 0,0,0,allocation.get_height() );

	switch (pos_mode)
	{
		case 0:
	        back_grad->add_color_stop_rgba(0,top_colour.get_red_p(),top_colour.get_green_p(),top_colour.get_blue_p(),1);
	        back_grad->add_color_stop_rgba(1,bottom_colour.get_red_p(),bottom_colour.get_green_p(),bottom_colour.get_blue_p(),1);
		break;

		case 1:
	        back_grad->add_color_stop_rgba(0,top_colour.get_red_p(),top_colour.get_green_p(),top_colour.get_blue_p(),1);
	        back_grad->add_color_stop_rgba(1,
			(bottom_colour.get_red_p() + top_colour.get_red_p())/2,
			(bottom_colour.get_green_p() + top_colour.get_green_p())/2,
			(bottom_colour.get_blue_p() + top_colour.get_blue_p())/2,
			1);
		break;

		case 2:
	        back_grad->add_color_stop_rgba(0,
			(bottom_colour.get_red_p() + top_colour.get_red_p())/2,
			(bottom_colour.get_green_p() + top_colour.get_green_p())/2,
			(bottom_colour.get_blue_p() + top_colour.get_blue_p())/2,
			1);
	        back_grad->add_color_stop_rgba(1,bottom_colour.get_red_p(),bottom_colour.get_green_p(),bottom_colour.get_blue_p(),1);
		break;
	}

		// fill background
		cr->set_source(back_grad);
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);

		cr->fill();

    		Cairo::FontOptions font_options;
    		font_options.set_hint_style(Cairo::HINT_STYLE_NONE);
    		font_options.set_hint_metrics(Cairo::HINT_METRICS_OFF);
    		font_options.set_antialias(Cairo::ANTIALIAS_GRAY);
	
    		int x_font_centre = (width/2) - ((width/5) * (label.length()/3.5));

    		cr->set_font_options(font_options);
    		cr->move_to(x_font_centre,height/3);
    		cr->show_text(label);

	}
  return true;
}


