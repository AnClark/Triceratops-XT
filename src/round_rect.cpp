
#include "round_rect.h"
#include <gtkmm/drawingarea.h>

//----------------------------------------------------------------------------

void rounded_rectangle(Cairo::RefPtr<Cairo::Context> cr,int type, double grad_top_colour, double grad_bottom_colour, double x, double y, double width, double height, int pos_mode, string topcolour, string bottomcolour)
{

/* a custom shape that could be wrapped in a function */
double aspect = height/175	,     /* aspect ratio */
corner_radius = height / 10.0;   /* and corner curvature radius */

double radius = corner_radius / aspect;
double degrees = M_PI / 180.0;

// background grad

	Cairo::RefPtr<Cairo::LinearGradient> back_grad =
		Cairo::LinearGradient::create( 0,0,0,height );

	Gdk::Color top_colour( topcolour);
	Gdk::Color bottom_colour( bottomcolour);

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

switch(type)
{
	case 0:
		cr->set_source(back_grad);
		cr->rectangle(width-radius,0,radius,radius);
		cr->rectangle(width-radius,height-radius,radius,radius);
		cr->rectangle(0,height-radius,radius,radius);
		cr->fill();
		break;
	case 1:
		cr->set_source(back_grad);
		cr->rectangle(0,0,radius,radius);
		cr->rectangle(width-radius,height-radius,radius,radius);
		cr->rectangle(0,height-radius,radius,radius);
		cr->fill();
		break;
	case 2:
		cr->set_source(back_grad);
		cr->rectangle(0,0,radius,radius);
		cr->rectangle(width-radius,0,radius,radius);
		cr->rectangle(width-radius,height-radius,radius,radius);
		cr->fill();
		break;
	case 3:
		cr->set_source(back_grad);
		cr->rectangle(0,0,radius,radius);
		cr->rectangle(width-radius,0,radius,radius);
		cr->rectangle(0,height-radius,radius,radius);
		cr->fill();
		break;
	case 4:
		break;
}

cr->set_source(back_grad);
cr->move_to(x,y);
cr->arc(x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
cr->arc(x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
cr->arc(x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
cr->arc(x + radius, y + radius, radius, 180 * degrees, 270 * degrees);

cr->fill_preserve();
cr->set_source_rgba(0.0, 0, 0, 0.0);
cr->set_line_width (10.0);
cr->stroke();

}
