
#include "logo_gui.h"
#include <fstream>

//-------------------------------------------------------------------

logo_gui::logo_gui()
{
	pos_mode = 0;

	top_colour.set( "#550000");
	bottom_colour.set( "#220000");
}

//-------------------------------------------------------------------

logo_gui::~logo_gui()
{

}

//-------------------------------------------------------------------

void logo_gui::load_logo(string bundle_path)
{

	stringstream file_name;
	logo_loaded = false;

	if (!logo_loaded)
	{
		file_name.str("");
		file_name  << bundle_path << "logo.png";
		ifstream check_file(file_name.str() );
		if (check_file)
		{
			// Load pixbuf
			image_ptr_ = Gdk::Pixbuf::create_from_file (file_name.str().c_str()); 
			logo_loaded = true;
		}
		check_file.close();
	}





	if (logo_loaded)
	{
		// Detect transparent colors for loaded image
		format = Cairo::FORMAT_RGB24;
		if (image_ptr_->get_has_alpha())
		{
			format = Cairo::FORMAT_ARGB32;
		}
	}

	set_size_request(100,140);

}

//-------------------------------------------------------------------

bool logo_gui::on_expose_event(GdkEventExpose* event)
{

	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window)
	{
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

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

		if (logo_loaded)
		{
			// Create a new ImageSurface
			image_surface_ptr_ = Cairo::ImageSurface::create  (format, image_ptr_->get_width(), image_ptr_->get_height());
	
			float image_width = image_ptr_->get_width();
			float image_height = image_ptr_->get_height();
			    
			// Create the new Context for the ImageSurface
			image_context_ptr_ = Cairo::Context::create (image_surface_ptr_);
			    
			// Draw the image on the new Context
			Gdk::Cairo::set_source_pixbuf (image_context_ptr_, image_ptr_, 0.0, 0.0);
			image_context_ptr_->paint();
			
			// Draw the source image on the widget context
			cr->scale( width / image_width, height /  image_height );
			cr->set_source (image_surface_ptr_, 6.0, 0.0);
			cr->paint();	
		}

	}
  return true;
}












