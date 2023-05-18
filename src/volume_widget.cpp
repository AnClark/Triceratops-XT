
// this file has the drawing code for our custom widget

#include "volume_widget.h"
#include <cairomm/context.h>

#include <iostream>



volume::volume()
{
	volume_value = 0;
	value = 0;
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
	set_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
		Gdk::POINTER_MOTION_MASK | Gdk::SCROLL_MASK);
 
	set_size_request(60,160);
}

volume::~volume()
{
}

void volume::position_top(bool mode)
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


void volume::set_label(string text)
{
	label = text;
	queue_draw();
}

void volume::set_value(float _value)
{

	value = _value;
	
	if (min > max)
	{
		volume_value =  value / (min-max);
	}

	if (min <= max)
	{
		volume_value = value / (max-min);
	}
	
	queue_draw();
}

void volume::set_min(float _min)
{
	min = _min;
	queue_draw();
}

void volume::set_max(float _max)
{
	max = _max;
	queue_draw();
}

void volume::draw_slider(int x, int y)
{

	// normal click, so write Y co-ordinate to class variable frequceny
	y-=(get_allocation().get_height()/6);
	float width = get_allocation().get_width();
	float height = (get_allocation().get_height() / 1.5);
    
	// see sinsynth.ttl for details, 880 - 40 is the port's value range
	volume_value = (height-y)/height;	

	if (volume_value < 0) { volume_value = 0; }
	if (volume_value > 1) { volume_value = 1; }

	if (max > min)
	{
		value =  min + (volume_value*(max-min));
    	}

	if (max <= min)
	{
		value =  max + ((1-volume_value)*(min-max));
    	}


	// which port to write to, check your .ttl file for the index of
	// each port
   
	// here we use the LV2UI_Controller and LV2UI_write_function "things"
	// to write some data to a port
	write_function( controller, port_number, sizeof(float), 0, (const void*)&value);

	// ask GTK to redraw when it suits
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0,0,get_allocation().get_width(), get_allocation().get_height() );
		win->invalidate_rect(r, false);
	}
}

//----------------------------------------------------------------

bool volume::on_motion_notify_event (GdkEventMotion *event)
{
	if (drag==true) { draw_slider(event->x,event->y); }
        return TRUE;
}


//-----------------------------------------------------------------

bool volume::on_button_release_event (GdkEventButton *event)
{
	drag = false;
	return TRUE;
}


bool volume::on_button_press_event(GdkEventButton* event)
{
	// here we handle mouse clicks
	if ( event->button == 1 )
	{
		drag = TRUE;
		draw_slider(event->x,event->y);
	}
  
  return true;
}

bool volume::on_expose_event(GdkEventExpose* event)
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
    // draw outer level scale lines

	cr->set_source_rgba(1.0,1.0,1.0,0.2);
	cr->set_line_width(width/24);

	for (float scale=-.05; scale<1; scale+=0.05 )
	{
 		float cos_x =  (allocation.get_width()/2.6) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		float sin_y =  (allocation.get_width()/2.6) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));
 		float cos_x2 =  (allocation.get_width()/2.2) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		float sin_y2 =  (allocation.get_width()/2.2) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));

		if (scale > 0.9-volume_value && volume_value != 0) { cr->set_source_rgba(1.0,1.0,1.0,0.7); }

		cr->move_to( (allocation.get_width()/2) + cos_x, (allocation.get_height()/2) + sin_y);
		cr->line_to( (allocation.get_width()/2) + cos_x2, (allocation.get_height()/2) + sin_y2);
		cr->stroke();
	}    


    // ------------------------------------------------------------------

	float cos_x =  (allocation.get_width()/5) * (cos(((((1-volume_value)*0.75)-0.3)*2) * M_PI));
	float sin_y =  (allocation.get_width()/5) * (sin(((((1-volume_value)*0.75)-0.3)*2) * M_PI));

     Cairo::RefPtr<Cairo::RadialGradient> grad1 =
Cairo::RadialGradient::create( (allocation.get_width()/2) + sin_y, (allocation.get_height()/2) + cos_x, 0, (allocation.get_width()/2) , (allocation.get_height()/2) ,(allocation.get_width()/2.5));
        grad1->add_color_stop_rgba(0,0.4,0.4,0.4,1);
        grad1->add_color_stop_rgba(0.5,0.2,0.2,0.2,1);
        grad1->add_color_stop_rgba(0.8,0.17,0.17,0.17,1);
        grad1->add_color_stop_rgba(1.0,0.0,0.0,0.0,1);

	cos_x =  (allocation.get_width()/5) * (cos((((volume_value*0.75)-0.61)*2) * M_PI));
	sin_y =  (allocation.get_width()/5) * (sin((((volume_value*0.75)-0.61)*2) * M_PI));

	cr->set_source_rgb(0.2,0.2,0.2);
	cr->arc(allocation.get_width()/2, allocation.get_height()/2, (allocation.get_width()/3), 0.0, 2 * M_PI);
	cr->fill();
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->arc((allocation.get_width()/2) +cos_x, (allocation.get_height()/2)+sin_y, (allocation.get_width()/16), 0.0, 2 * M_PI);
	cr->fill();

    // draw text label
    cr->select_font_face("Bitstream Vera Sans", Cairo::FONT_SLANT_NORMAL,
     Cairo::FONT_WEIGHT_NORMAL);
    cr->set_font_size(width/8);
    cr->set_source_rgba(0.9,0.9,0.9,0.8);
    Cairo::FontOptions font_options;
    font_options.set_hint_style(Cairo::HINT_STYLE_NONE);
    font_options.set_hint_metrics(Cairo::HINT_METRICS_OFF);
    font_options.set_antialias(Cairo::ANTIALIAS_GRAY);
	
    int x_font_centre = (width/2) - ((width/8) * (label.length()/3.5));

    cr->set_font_options(font_options);

		cairo_text_extents_t txt_extents;
		stringstream txt;

		cr->set_font_size(width/12);
		txt.str("");
		txt << "VOLUME";
		cr->get_text_extents(txt.str(),txt_extents);
		cr->move_to( width/2 - (txt_extents.width/2),height/7);
	       	cr->show_text(txt.str());

    cr->move_to(x_font_centre,allocation.get_height()  - (height_offset/3) );
    ostringstream slider_value;
    slider_value.str("");
    slider_value << value;

    slider_value.str(slider_value.str().substr(0,10));

		txt.str("");
		txt << value;
		cr->get_text_extents(txt.str(),txt_extents);
		cr->move_to( width/2 - (txt_extents.width/2),
			allocation.get_height() - (height_offset/3));
	       	cr->show_text(txt.str());
  }

  return true;
}


//----------------------------------------------------------------
bool volume::on_scroll_event(GdkEventScroll* event)
{
	if (event->direction == GDK_SCROLL_UP) value += scroll_wheel_speed;
	if (event->direction == GDK_SCROLL_DOWN) value -= scroll_wheel_speed;

	value = ((float)roundf(value*100))/100;

	if (value < min) value = min;
	if (value > max) value = max;
	set_value(value);


	write_function( controller, port_number, sizeof(float), 0, (const void*)&value);

	// ask GTK to redraw when it suits
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0,0,get_allocation().get_width(), get_allocation().get_height() );
		win->invalidate_rect(r, false);
	}
	
	return true;
}

