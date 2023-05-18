
// this file has the drawing code for our custom widget

#include "knob_widget.h"
#include <cairomm/context.h>

#include <iostream>



knob::knob()
{
	knob_value = 0;
	value = 0;
	snap = false;
	invert = false;
	min = 0;
	max = 1;
	drag = false;
	label = "KNOB";
	pos_mode = 0;
	scroll_wheel_speed = 0.1;

	top_colour.set( "#550000");
	bottom_colour.set( "#220000");
  
	// make the click events call the on_button_press_event function
	// add_events( Gdk::BUTTON_PRESS_MASK);
	set_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::SCROLL_MASK);
  
	// make the click event do that function
	signal_button_press_event().connect( sigc::mem_fun(*this, &knob::on_button_press_event) );
  
	set_size_request(40,70);
}

knob::~knob()
{
}

void knob::position_top(bool mode)
{
	if (mode)
	{
		grad_top_colour = 0.3;
		grad_bottom_colour = 0.1*2;
		pos_mode = 1;
	}
	else
	{
		grad_top_colour = 0.1*2;
		grad_bottom_colour = 0.1;
		pos_mode = 2;
	}

	return;
}


void knob::set_snap(bool _snap)
{
	snap = _snap;
}

void knob::set_label(string text)
{
	label = text;
	queue_draw();
}

void knob::set_value(float _value)
{

	if (invert)
	{
		value = max - _value;
	}
	else
	{
		value = _value;
	}	

	if (min > max && max >=0)
	{
		knob_value =  value / (min-max);
	}

	if (min > max && max <0)
	{
		knob_value = (value / (min - max)) + 0.5;
	}

	if (min <= max && min >=0)
	{
		knob_value = value / (max-min);
	}

	if (min <= max && min <0)
	{
		knob_value = (value / (max - min)) + 0.5;
	}
	
	queue_draw();
}

void knob::set_min(float _min)
{
	min = _min;
	queue_draw();
}

void knob::set_max(float _max)
{
	max = _max;
	queue_draw();
}

void knob::draw_slider(int x, int y)
{

	
	if (drag==false && max>1)
	{
		if (y > get_allocation().get_height()/2 && value > min) { value -= 1; }
		if (y < get_allocation().get_height()/2 && value < max) { value += 1; }
	}

	if (drag==false && max==1)
	{
		if (y > get_allocation().get_height()/2 && value > min) { value -= 0.001; }
		if (y < get_allocation().get_height()/2 && value < max) { value += 0.001; }
	}


	float width = get_allocation().get_width();
	float height = (get_allocation().get_height() / 1.5);

	if (drag==true)
	{
		// converty mouse y to knob value
		y-=(get_allocation().get_height()/6);
		knob_value = (height-y)/height;	

		if (knob_value < 0) { knob_value = 0; }
		if (knob_value > 1) { knob_value = 1; }

		if (max > min)
		{
			value =  min + (knob_value*(max-min));
	    	}
	
		if (max <= min)
		{
			value =  max + ((1-knob_value)*(min-max));
	    	}

	}	


	if (snap)
	{
		value = int(value);
		set_value(value);
	}



	// which port to write to, check your .ttl file for the index of
	// each port
   
	// here we use the LV2UI_Controller and LV2UI_write_function "things"
	// to write some data to a port
	if (invert)
	{
		float inverted_value = max - value;
		write_function( controller, port_number, sizeof(float), 0, (const void*)&inverted_value);
	}
	else
	{
		write_function( controller, port_number, sizeof(float), 0, (const void*)&value);
	}

	// ask GTK to redraw when it suits
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0,0,get_allocation().get_width(), get_allocation().get_height() );
		win->invalidate_rect(r, false);
	}
}

//----------------------------------------------------------------

bool knob::on_motion_notify_event (GdkEventMotion *event)
{
	if (drag==true) { draw_slider(event->x,event->y); }
        return TRUE;
}

//----------------------------------------------------------------
bool knob::on_scroll_event(GdkEventScroll* event)
{
	if (event->direction == GDK_SCROLL_UP) value += scroll_wheel_speed;
	if (event->direction == GDK_SCROLL_DOWN) value -= scroll_wheel_speed;

	value = ((float)roundf(value*100))/100;

	if (value < min) value = min;
	if (value > max) value = max;
	set_value(value);


	if (invert)
	{
		float inverted_value = max - value;
		write_function( controller, port_number, sizeof(float), 0, (const void*)&inverted_value);
	}
	else
	{
		write_function( controller, port_number, sizeof(float), 0, (const void*)&value);
	}

	// ask GTK to redraw when it suits
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0,0,get_allocation().get_width(), get_allocation().get_height() );
		win->invalidate_rect(r, false);
	}


	return true;
}


//-----------------------------------------------------------------

bool knob::on_button_release_event (GdkEventButton *event)
{

	drag = false;
	return TRUE;
}


bool knob::on_button_press_event(GdkEventButton* event)
{
	// here we handle mouse clicks
	if ( event->button == 1 )
	{
		drag = TRUE;
		draw_slider(event->x,event->y);
	}


	if ( event->button == 3 )
	{
		drag = false;
		draw_slider(event->x,event->y);
	}
  
  return true;
}

bool knob::on_expose_event(GdkEventExpose* event)
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


	cr->rectangle(event->area.x, event->area.y,
		event->area.width, event->area.height);


	cr->set_source(back_grad);
	cr->fill();
    
    // ------------------------------------------------------------------

	float cos_x =  (allocation.get_width()/5) * (cos(((((1-knob_value)*0.75)-0.3)*2) * M_PI));
	float sin_y =  (allocation.get_width()/5) * (sin(((((1-knob_value)*0.75)-0.3)*2) * M_PI));

     Cairo::RefPtr<Cairo::RadialGradient> grad1 =
Cairo::RadialGradient::create( (allocation.get_width()/2) + sin_y, (allocation.get_height()/2) + cos_x, 0, (allocation.get_width()/2) , (allocation.get_height()/2) ,(allocation.get_width()/2.5));
        grad1->add_color_stop_rgba(0,0.4,0.4,0.4,1);
        grad1->add_color_stop_rgba(0.5,0.2,0.2,0.2,1);
        grad1->add_color_stop_rgba(0.8,0.17,0.17,0.17,1);
        grad1->add_color_stop_rgba(1.0,0.0,0.0,0.0,1);

	cos_x =  (allocation.get_width()/5) * (cos((((knob_value*0.75)-0.61)*2) * M_PI));
	sin_y =  (allocation.get_width()/5) * (sin((((knob_value*0.75)-0.61)*2) * M_PI));

	cr->set_source(grad1);
	cr->arc(allocation.get_width()/2, allocation.get_height()/2, (allocation.get_width()/2.5), 0.0, 2 * M_PI);
	cr->fill();
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->arc((allocation.get_width()/2) +cos_x, (allocation.get_height()/2)+sin_y, (allocation.get_width()/16), 0.0, 2 * M_PI);
	cr->fill();

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

		cairo_text_extents_t txt_extents;
		stringstream txt;

		cr->set_font_size(width/5);
		txt.str("");
		txt << label;
		cr->get_text_extents(txt.str(),txt_extents);
		cr->move_to( width/2 - (txt_extents.width/2),height/3.5);
	       	cr->show_text(txt.str());


    ostringstream slider_value;
    slider_value.str("");

    if (invert)
    {
	slider_value << max - value;
    }
    else
    {
    	slider_value << value;
    }

    slider_value.str(slider_value.str().substr(0,5));

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


