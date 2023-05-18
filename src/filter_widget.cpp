
// this file has the drawing code for our custom widget

#include "filter_widget.h"
#include <cmath>
#include <cairomm/context.h>
#include "triceratops.hpp"

#include <iostream>



filter_widget::filter_widget()
{
  val = 0;
  label = "filter";
  val_cutoff = 0;
  val_res = 0;
  pos_mode = 0;

	top_colour.set( "#550000");
	bottom_colour.set( "#220000");
  
  // make the click events call the on_button_press_event function
  set_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
  
  set_size_request(40,140);
}

filter_widget::~filter_widget()
{
}

void filter_widget::set_value(float _value)
{
	val = _value;
	queue_draw();
}

void filter_widget::set_label(string text)
{
	label = text;
	queue_draw();
}

bool filter_widget::on_button_press_event(GdkEventButton* event)
{
	return true;
}

bool filter_widget::on_button_release_event(GdkEventButton* event)
{



		if (event->button == 1)
		{
			--val;
			if (val < 0) { val = 4; }
		}

		if (event->button == 3)
		{
			++val;
			if (val > 4) { val = 0; }
		}

	float new_value = val;

	// here we use the LV2UI_Controller and LV2UI_write_function "things"
	// to write some data to a port
	write_function( controller, port_number, sizeof(float), 0, (const void*)&new_value);
	
	queue_draw();
  
  return true;
}

bool filter_widget::on_expose_event(GdkEventExpose* event)
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
    cr->set_line_width(6.0);

    // clip to the area indicated by the expose event so that we only redraw
    // the portion of the window that needs to be redrawn
    cr->rectangle(event->area.x, event->area.y,
            event->area.width, event->area.height);
    cr->clip();
    
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
	cr->rectangle(event->area.x, event->area.y,
		event->area.width, event->area.height);
	cr->set_source(back_grad);
	cr->fill();

    cr->set_source_rgb(0.1, 0.0, 0.0);
    cr->set_line_width(width/12);
    cr->move_to(0,0);
    cr->line_to(0,allocation.get_height());
    cr->move_to(width,0);
    cr->line_to(width,allocation.get_height());
    cr->stroke();     

    // ------------------------------------------------------------------



    // draw text label
    cr->select_font_face("Bitstream Vera Sans", Cairo::FONT_SLANT_NORMAL,
    Cairo::FONT_WEIGHT_NORMAL);
    cr->set_font_size(width/4.5);
    cr->set_source_rgba(0.9,0.9,0.9,0.8);
    Cairo::FontOptions font_options;
    font_options.set_hint_style(Cairo::HINT_STYLE_NONE);
    font_options.set_hint_metrics(Cairo::HINT_METRICS_OFF);
    font_options.set_antialias(Cairo::ANTIALIAS_GRAY);
	
    int x_font_centre = (width/2) - ((width/5) * (label.length()/3.5));

    cr->set_font_options(font_options);
    cr->move_to(x_font_centre,height/7);
    cr->show_text(label);
    cr->move_to(x_font_centre,allocation.get_height()  - height_offset );


    cr->set_line_width(width/14);
    cr->set_source_rgba(0.8,0.8,0.8,1.0);

     Cairo::RefPtr<Cairo::LinearGradient> grad2 =
Cairo::LinearGradient::create(width/4, 0,width-(width/4), 0 );
        grad2->add_color_stop_rgba(0.0,0.2,0.2,0.2,1);
        grad2->add_color_stop_rgba(0.4,0.8,0.8,0.8,1);
        grad2->add_color_stop_rgba(1.0,0.4,0.4,0.4,1);

    cr->set_source(grad2);

    float wave_freq = 1 + (val_cutoff*4);
    float wave_res = 2 + (val_res*2);

    stringstream slider_value;
    switch (val)
    {

    	case 0:
		slider_value.str("Off");
		break;
    	case 1:
		slider_value.str("LowPass");
		cr->move_to(width/8, allocation.get_height()/2.5);

  		cr->curve_to(width/6, allocation.get_height()/3,
			width-(width/(wave_freq)),allocation.get_height()/2.5,
			width-(width/wave_freq),allocation.get_height()/wave_res);

  		cr->curve_to(width-(width/wave_freq), allocation.get_height()-(allocation.get_height()/3.5),
			width-(width/8),allocation.get_height()-(allocation.get_height()/3.5),
			width-(width/8),allocation.get_height()-(allocation.get_height()/3.5));

		cr->stroke();
		break;

	case 2:
		slider_value.str("HiPass");
		cr->move_to(width/4, allocation.get_height()/1.5);

  		cr->curve_to(width/6, allocation.get_height()/2,
			width-(width/(wave_freq)),allocation.get_height()/2.5,
			width-(width/wave_freq),allocation.get_height()/(wave_res+0.6));

  		cr->curve_to(width-(width/wave_freq), (allocation.get_height()/2.6),
			width-(width/8),(allocation.get_height()/2.6),
			width-(width/8),(allocation.get_height()/2.6));

		cr->stroke();

		break;

    	case 3:
		slider_value.str("Band");
		wave_freq *= 2;
		cr->move_to(0, allocation.get_height()/1.5);
		cr->line_to((width/2) - (width/(wave_freq/1.2)), allocation.get_height()/1.5);
		
		//cr->line_to(width - (width/wave_freq), allocation.get_height()/(wave_res+0.6));

  		cr->curve_to(width - (width/(wave_freq/2)), allocation.get_height()/(wave_res+0.6),
			width - (width/(wave_freq/1.2)), allocation.get_height()/(wave_res+1),
			width - (width/(wave_freq/1.2)), allocation.get_height()/(wave_res+0.6));

  		cr->curve_to(width - (width/(wave_freq/1.2)), allocation.get_height()/1.5,
			width - (width/(wave_freq/0.8)),  allocation.get_height()/1.5,
			width - (width/(wave_freq/0.8)),  allocation.get_height()/1.5);

		//cr->line_to(width - (width/(wave_freq/1.2)), allocation.get_height()/1.5);
		cr->line_to(width, allocation.get_height()/1.5);

		cr->stroke();

		break;

    	case 4:
		slider_value.str("Formant");
		cr->move_to(width/8, allocation.get_height()/1.5);

  		cr->curve_to(width/4, allocation.get_height()/3.5,
		width/4, allocation.get_height()/3.5,
		width/4, allocation.get_height()/3.5);

  		cr->curve_to(width/3, allocation.get_height()/1.5,
		width/3, allocation.get_height()/1.5,
		width/3, allocation.get_height()/1.5);

  		cr->curve_to(width/2, allocation.get_height()/3.5,
		width/2, allocation.get_height()/3.5,
		width/2, allocation.get_height()/3.5);

  		cr->curve_to(width/1.5, allocation.get_height()/1.5,
		width/1.5, allocation.get_height()/1.5,
		width/1.5, allocation.get_height()/1.5);

  		cr->curve_to(width/1.25, allocation.get_height()/3.5,
		width/1.25, allocation.get_height()/3.5,
		width/1.25, allocation.get_height()/3.5);

  		cr->curve_to(width, allocation.get_height()/1.5,
		width, allocation.get_height()/1.5,
		width, allocation.get_height()/1.5);

		cr->stroke();
		break;
    }

    x_font_centre = (width/2) - ((width/5) * (slider_value.str().length()/3.5));
    cr->move_to(x_font_centre,allocation.get_height() - (height_offset/3) );

    cr->set_source_rgba(0.9,0.9,0.9,0.8);
    cr->set_font_size(width/5.5);
    cr->show_text(slider_value.str());

  }

  return true;
}


