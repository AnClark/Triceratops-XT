
// this file has the drawing code for our custom widget

#include "fader_widget.h"
#include <cairomm/context.h>

#include <iostream>



fader::fader()
{
	fader_value = 0;
	route_number = 0;
	route_port_number = -1;
	route_max = 15;	
	value = 0;
	snap = false;
	min = 0;
	max = 1;
	rounded = 5;
	drag = false;
	invert = false;
	pos_mode = 0;
	label = "FADER";
	scroll_wheel_speed = 0.01;

	top_colour.set( "#550000");
	bottom_colour.set( "#220000");

	filter_widget_object = NULL;
	filter_widget_connect = false;
  
	// make the click events call the on_button_press_event function
	// add_events( Gdk::BUTTON_PRESS_MASK);
	set_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::SCROLL_MASK);
  
	// make the click event do that function
	// signal_button_press_event().connect( sigc::mem_fun(*this, &fader::on_button_press_event) );
  
	set_size_request(40,140);
}

fader::~fader()
{
}

void fader::set_label(string text)
{
	label = text;
	queue_draw();
}

void fader::set_rounded(int _rounded)
{
	rounded = _rounded;
}

void fader::set_snap(bool _snap)
{
	snap = _snap;
}

void fader::set_value(float _value)
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
		fader_value =  value / (min-max);
	}

	if (min > max && max <0)
	{
		fader_value = (value / (min - max)) + 0.5;
	}

	if (min <= max && min >=0)
	{
		fader_value = value / (max-min);
	}

	if (min <= max && min <0)
	{
		fader_value = (value / (max - min)) + 0.5;
	}

	queue_draw();
}

void fader::set_min(float _min)
{
	min = _min;
	queue_draw();
}

void fader::set_max(float _max)
{
	max = _max;
	queue_draw();
}

void fader::draw_slider(int x, int y)
{
	float width = get_allocation().get_width();
	float height = (get_allocation().get_height() / 1.5);
    
	// convert mouse y to fader value - if y is -1 no value change

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

	if (y > -1 && drag == true)
	{
		y-=(get_allocation().get_height()/6);
		fader_value = (height-y)/height;	

		if (fader_value < 0) { fader_value = 0; }
		if (fader_value > 1) { fader_value = 1; }

		if (max > min)
		{
			value =  min + (fader_value*(max-min));
    		}

		if (max <= min)
		{
			value =  max + ((1-fader_value)*(min-max));
    		}

		if (snap)
		{
			value = int(value);
			set_value(value);
		}
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

	if (filter_widget_connect == true)
	{		
		if (port_number == 7) // update wave widget's cutoff
		{
			filter_widget_object->val_cutoff = value;
			filter_widget_object->queue_draw();
		}
		if (port_number == 8) // update wave widget's resonance peak
		{
			filter_widget_object->val_res = value;
			filter_widget_object->queue_draw();
		}
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

bool fader::on_motion_notify_event (GdkEventMotion *event)
{
	if (drag==true) { draw_slider(event->x,event->y); }
        return TRUE;
}


//-----------------------------------------------------------------

bool fader::on_button_release_event (GdkEventButton *event)
{

		float y= event->y - (get_allocation().get_height()/6);
		float height = (get_allocation().get_height() / 1.5);
    
		// convert mouse y to fader value
		float pos_y = (height-y) / height;

		if (pos_y > 1 && route_port_number != -1 && event->button == 1)
		{
			route_number = (--route_number);
			if (route_number < 0) route_number = 14;
			show_route_dest();
			float rn = route_number;
			write_function( controller, route_port_number, sizeof(float), 0, (const void*)&rn);
			draw_slider(-1,-1);
		}

		if (pos_y > 1 && route_port_number != -1 && event->button == 3)
		{
			route_number = (++route_number % route_max);
			show_route_dest();
			float rn = route_number;
			write_function( controller, route_port_number, sizeof(float), 0, (const void*)&rn);
			draw_slider(-1,-1);
		}

	drag = false;
	return TRUE;
}


void fader::show_route_dest()
{	
	switch (route_number)
	{	
		case 0:	
		label = "AMP";
		break; 
				
		case 1:
		label = "CUTOFF";
		break;

		case 2:
		label = "RES";
		break;
	
		case 3:
		label = "DCO1";
		break;

		case 4:	
		label = "DCO2";
		break; 
				
		case 5:
		label = "DCO3";
		break;

		case 6:
		label = "PW1";
		break;

		case 7:
		label = "PW2";
		break;

		case 8:
		label = "PW3";
		break;
	
		case 9:
		label = "PAN1";
		break;

		case 10:
		label = "PAN2";
		break;

		case 11:
		label = "PAN3";
		break;

		case 12:
		label = "LFO1-SP";
		break;

		case 13:
		label = "LFO2-SP";
		break;

		case 14:
		label = "LFO3-SP";
		break;

	}

}
bool fader::on_scroll_event(GdkEventScroll* event)
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

	if (filter_widget_connect == true)
	{		
		if (port_number == 7) // update wave widget's cutoff
		{
			filter_widget_object->val_cutoff = value;
			filter_widget_object->queue_draw();
		}
		if (port_number == 8) // update wave widget's resonance peak
		{
			filter_widget_object->val_res = value;
			filter_widget_object->queue_draw();
		}
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

bool fader::on_button_press_event(GdkEventButton* event)
{

	// here we handle mouse clicks
	if ( event->button == 1 )
	{	

		float y= event->y - (get_allocation().get_height()/6);
		float height = (get_allocation().get_height() / 1.5);
    
		// convert mouse y to fader value
		float pos_y = (height-y) / height;

		if (pos_y > 1) return true;
		
		if (pos_y>=0 && pos_y<=1) 
		{
			drag = TRUE;
			draw_slider(event->x,event->y);
		}
	}

 
	// right click - reset fader to zero

	if ( event->button == 3 && min<0 && max>0)
	{
		set_value(0);
		drag = true;
		draw_slider(-1,-1);
		return true;
	}

	if (event->button == 3)
	{
		drag = false;
		draw_slider(event->x,event->y);
	}
 
  return true;
}

bool fader::on_expose_event(GdkEventExpose* event)
{

  if (route_port_number != -1) show_route_dest();

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


		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);


	cr->set_source(back_grad);
	cr->fill();
    
    // ------------------------------------------------------------------


    float freqPix = height - (height*(fader_value) ); // convert to pix

     Cairo::RefPtr<Cairo::LinearGradient> grad2 =
Cairo::LinearGradient::create(width/4, freqPix,width-(width/4), freqPix );
        grad2->add_color_stop_rgba(0.0,0.2,0.2,0.2,1);
        grad2->add_color_stop_rgba(0.4,0.8,0.8,0.8,1);
        grad2->add_color_stop_rgba(1.0,0.4,0.4,0.4,1);

    // draw vertical scale bars

    cr->set_line_width(0.4);
    cr->set_source_rgba(0.9,0.9,0.9,1.0);
    for (int y=0; y<height; y+=(height/10))
    {
    	cr->move_to(width/3, y + height_offset);
 	cr->line_to(width-(width/3), y + height_offset);
    	cr->stroke();
    }

    // draw vertical grey line down the middle
    cr->set_line_width(4);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(xc, 0 + height_offset);
    cr->line_to(xc, height + height_offset);
    cr->stroke();

    // draw horizontal black line at fader_value height

    cr->set_line_width(height/6);
    cr->set_source_rgba(0.0,0.0,0.0,1.0);
    cr->move_to(width/4, freqPix + height_offset);
    cr->line_to(width-(width/4), freqPix + height_offset);
    cr->stroke();

    // draw horizontal black line at fader_value height

    cr->set_line_width(height/4);
    cr->set_source_rgba(0.0,0.0,0.0,0.3);
    cr->move_to(width/4, freqPix +  (height_offset));
    cr->line_to(width-(width/4), freqPix + (height_offset*1.1));
    cr->stroke();
    
    // draw horizontal blue line at fader_value height
    cr->set_line_width(height/24);
    cr->set_source(grad2);
    cr->move_to(width/4, freqPix + height_offset);
    cr->line_to(width-(width/4), freqPix + height_offset);
    cr->stroke();

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
		cr->move_to( width/2 - (txt_extents.width/2),height/7);
	       	cr->show_text(txt.str());

    cr->move_to(x_font_centre,allocation.get_height()  - (height_offset/3) );

    stringstream slider_value;
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

    x_font_centre = (width/2) - ((width/5) * (slider_value.str().length()/3.5));
    cr->move_to(x_font_centre,allocation.get_height() - (height_offset/3) );

    cr->show_text(slider_value.str());

  }

  return true;
}


