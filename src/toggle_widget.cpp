
// this file has the drawing code for our custom widget

#include "toggle_widget.h"


toggle::toggle()
{
	val = 0;
	pos_mode=0;
	label = "TOGGLE";

	top_colour.set( "#550000");
	bottom_colour.set( "#220000");

	// make the click events call the on_button_press_event function
	set_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);

	// make the click event do that function
	// signal_button_press_event().connect( sigc::mem_fun(*this, &toggle::on_button_press_event) );
  
	set_size_request(40,70);
}

toggle::~toggle()
{
}

void toggle::position_top(bool mode)
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

void toggle::set_label(string text)
{
	label = text;
	queue_draw();
}

void toggle::set_value(int _value)
{
	val = _value;	
	queue_draw();
}

bool toggle::on_button_press_event(GdkEventButton* event)
{
	return true;
}

bool toggle::on_button_release_event(GdkEventButton* event)
{

		val = 1 - val;

	float new_value = (float)val;

	// here we use the LV2UI_Controller and LV2UI_write_function "things"
	// to write some data to a port
	write_function( controller, port_number, sizeof(float), 0, (const void*)&new_value);
	
	queue_draw();
  
  return true;
}

bool toggle::on_expose_event(GdkEventExpose* event)
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

	cr->rectangle(event->area.x, event->area.y,
		event->area.width, event->area.height);


	cr->set_source(back_grad);
	cr->fill();



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


		cairo_text_extents_t txt_extents;
		stringstream txt;

		cr->set_font_size(width/5);
		txt.str("");
		txt << label;
		cr->get_text_extents(txt.str(),txt_extents);
		cr->move_to( width/2 - (txt_extents.width/2),height/3.5);
	       	cr->show_text(txt.str());



    cr->set_line_width(width/14);
    cr->set_source_rgba(0.8,0.8,0.8,1.0);

     Cairo::RefPtr<Cairo::LinearGradient> grad2 =
Cairo::LinearGradient::create(width/4, 0,width-(width/4), 0 );
        grad2->add_color_stop_rgba(0.0,0.2,0.2,0.2,1);
        grad2->add_color_stop_rgba(0.4,0.8,0.8,0.8,1);
        grad2->add_color_stop_rgba(1.0,0.4,0.4,0.4,1);

    cr->set_source(grad2);

	Cairo::RefPtr<Cairo::LinearGradient> toggle_grad1;
	Cairo::RefPtr<Cairo::RadialGradient> grad_led;

    stringstream slider_value;
    switch (val)
    {
    	case 0:
		slider_value.str("OFF");

		cr->arc((allocation.get_width()/1.9), (allocation.get_height()/1.9), (allocation.get_width()/2.8), 0.0, 2 * M_PI);

		cr->set_source_rgba(0,0,0,0.5);
		cr->fill();
	
		toggle_grad1 = Cairo::LinearGradient::create( (allocation.get_width()/1.9), (allocation.get_height()/1.9),allocation.get_width()/3,allocation.get_height()/3 );
		toggle_grad1->add_color_stop_rgba(0,0.2,0.2,0.2,1);
		toggle_grad1->add_color_stop_rgba(0.5,0.4,0.4,0.4,1);
		toggle_grad1->add_color_stop_rgba(1,0.0,0.0,0.0,1);

		cr->arc((allocation.get_width()/1.9), (allocation.get_height()/1.9), (allocation.get_width()/3), 0.0, 2 * M_PI);

		cr->set_source(toggle_grad1);
		cr->fill();

		cr->arc((allocation.get_width()/1.9), (allocation.get_height()/1.9), (allocation.get_width()/5), 0.0, 2 * M_PI);

		cr->set_source_rgba(0,0,0,0.5);
		cr->fill();

		grad_led = Cairo::RadialGradient::create( (allocation.get_width()/2),
			(allocation.get_height()/2), 0, (allocation.get_width()/2),
			(allocation.get_height()/2) ,(allocation.get_width()/2.5));
	
	        grad_led->add_color_stop_rgba(0,0.0,0.2,0.0,1);
	        grad_led->add_color_stop_rgba(0.3,0.0,0.01,0.0,1);
	        grad_led->add_color_stop_rgba(0.8,0.0,0.01,0.0,1);
	        grad_led->add_color_stop_rgba(1.0,0.0,0.0,0.0,1);

		cr->arc((allocation.get_width()/1.9), (allocation.get_height()/1.9), (allocation.get_width()/6), 0.0, 2 * M_PI);
		cr->set_source(grad_led);
		cr->fill();

		break;
    	case 1:
		slider_value.str("ON");

		cr->arc((allocation.get_width()/1.9), (allocation.get_height()/1.9), (allocation.get_width()/2.8), 0.0, 2 * M_PI);

		cr->set_source_rgba(0,0,0,0.5);
		cr->fill();

		toggle_grad1 = Cairo::LinearGradient::create( (allocation.get_width()/1.9), (allocation.get_height()/1.9),allocation.get_width()/3,allocation.get_height()/3 );
		toggle_grad1->add_color_stop_rgba(0,0.2,0.2,0.2,1);
		toggle_grad1->add_color_stop_rgba(0.3,0.4,0.4,0.4,1);
		toggle_grad1->add_color_stop_rgba(1,0.0,0.0,0.0,1);

		cr->arc((allocation.get_width()/1.9), (allocation.get_height()/1.9), (allocation.get_width()/3), 0.0, 2 * M_PI);

		cr->set_source(toggle_grad1);
		cr->fill();

		cr->arc((allocation.get_width()/1.9), (allocation.get_height()/1.9), (allocation.get_width()/5), 0.0, 2 * M_PI);

		cr->set_source_rgba(0,0,0,0.5);
		cr->fill();

		grad_led = Cairo::RadialGradient::create( (allocation.get_width()/1.9),
			(allocation.get_height()/1.9), 0, (allocation.get_width()/2),
			(allocation.get_height()/2) ,(allocation.get_width()/2.5));
	
	        grad_led->add_color_stop_rgba(0,0.0,0.9,0.0,1);
	        grad_led->add_color_stop_rgba(0.3,0.0,0.1,0.0,1);
	        grad_led->add_color_stop_rgba(0.9,0.0,0.10,0.0,1);
	        grad_led->add_color_stop_rgba(1.0,0.0,0.0,0.0,1);

		cr->arc((allocation.get_width()/1.9), (allocation.get_height()/1.9), (allocation.get_width()/6), 0.0, 2 * M_PI);
		cr->set_source(grad_led);
		cr->fill();
		break;
    }

	cr->set_source_rgba(0.9,0.9,0.9,0.8);
	cr->set_font_size(width/5);
	txt.str("");
	txt << slider_value.str();
	cr->get_text_extents(txt.str(),txt_extents);
	cr->move_to( width/2 - (txt_extents.width/2),
		allocation.get_height() - (height_offset/1.5));
       	cr->show_text(txt.str());

  }

  return true;
}


