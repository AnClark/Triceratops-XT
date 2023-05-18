
#include "reverb_gui.h"

//-------------------------------------------------------------------

reverb_gui::reverb_gui(int port_number,string top_colour, string bottom_colour)
{

	tab = new Gtk::HBox();

	gui_active = new toggle();
	gui_active->top_colour.set(top_colour);
	gui_active->bottom_colour.set(bottom_colour);
	gui_active->set_label("ACTIVE");
	gui_active->pos_mode=1;
	gui_active->port_number = port_number;
	++port_number;

	gui_decay = new fader();
	gui_decay->top_colour.set(top_colour);
	gui_decay->bottom_colour.set(bottom_colour);
	gui_decay->set_label("DECAY");
	gui_decay->set_min(0);
	gui_decay->set_max(15);
	gui_decay->set_value(4);
	gui_decay->set_snap(true);
	gui_decay->port_number = port_number;
	++port_number;

	gui_mix = new fader();
	gui_mix->top_colour.set(top_colour);
	gui_mix->bottom_colour.set(bottom_colour);
	gui_mix->set_label("MIX");
	gui_mix->set_min(0);
	gui_mix->set_max(0.8);
	gui_mix->set_value(0.5);
	gui_mix->port_number = port_number;
	++port_number;

	Gtk::VBox* vbox1 = new Gtk::VBox();

	spacer* spacer1 = new spacer();
	spacer1->top_colour.set(top_colour);
	spacer1->bottom_colour.set(bottom_colour);
	spacer1->pos_mode= 2;
	spacer1->set_size_request(40,70);
	vbox1->add(*gui_active);
	vbox1->add(*spacer1);
	
	tab->add(*vbox1);
	tab->add(*gui_decay);
	tab->add(*gui_mix);

	spacer* spacer2 = new spacer();
	spacer2->top_colour.set(top_colour);
	spacer2->bottom_colour.set(bottom_colour);
	spacer2->set_size_request(320,70);
	tab->add(*spacer2);
}

//-------------------------------------------------------------------

reverb_gui::~reverb_gui()
{

}

//-------------------------------------------------------------------

void reverb_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
{

	gui_active->controller = controller;
	gui_active->write_function = write_function;

	gui_decay->controller = controller;
	gui_decay->write_function = write_function;

	gui_mix->controller = controller;
	gui_mix->write_function = write_function;


}

