
#include "echo_gui.h"

//-------------------------------------------------------------------

echo_gui::echo_gui(int port_number,string top_colour, string bottom_colour)
{

	tab = new Gtk::HBox();

	gui_active = new toggle();
	gui_active->top_colour.set(top_colour);
	gui_active->bottom_colour.set(bottom_colour);
	gui_active->set_label("ACTIVE");
	gui_active->pos_mode = 1;
	gui_active->port_number = port_number;
	++port_number;

	gui_speed = new knob();
	gui_speed->top_colour.set(top_colour);
	gui_speed->bottom_colour.set(bottom_colour);
	gui_speed->set_label("SPEED");
	gui_speed->set_min(512);
	gui_speed->set_max(65536);
	gui_speed->set_value(4096);
	gui_speed->pos_mode = 1;
	gui_speed->port_number = port_number;
	++port_number;

	gui_decay = new knob();
	gui_decay->top_colour.set(top_colour);
	gui_decay->bottom_colour.set(bottom_colour);
	gui_decay->set_label("DECAY");
	gui_decay->pos_mode=2;
	gui_decay->port_number = port_number;
	++port_number;

	gui_eq_low = new fader();
	gui_eq_low->top_colour.set(top_colour);
	gui_eq_low->bottom_colour.set(bottom_colour);
	gui_eq_low->set_label("EQ-LOW");
	gui_eq_low->set_min(0);
	gui_eq_low->set_max(2);
	gui_eq_low->set_value(1);
	gui_eq_low->port_number = port_number;
	++port_number;

	gui_eq_mid = new fader();
	gui_eq_mid->top_colour.set(top_colour);
	gui_eq_mid->bottom_colour.set(bottom_colour);
	gui_eq_mid->set_label("EQ-MID");
	gui_eq_mid->set_min(0);
	gui_eq_mid->set_max(2);
	gui_eq_mid->set_value(1);
	gui_eq_mid->port_number = port_number;
	++port_number;

	gui_eq_high = new fader();
	gui_eq_high->top_colour.set(top_colour);
	gui_eq_high->bottom_colour.set(bottom_colour);
	gui_eq_high->set_label("EQ-HI");
	gui_eq_high->set_min(0);
	gui_eq_high->set_max(2);
	gui_eq_high->set_value(1);
	gui_eq_high->port_number = port_number;
	++port_number;

	//---------

	spacer* spacer1 = new spacer();
	spacer1->top_colour.set(top_colour);
	spacer1->bottom_colour.set(bottom_colour);
	spacer1->pos_mode = 2;
	spacer1->set_size_request(40,70);

	Gtk::VBox* vbox1 = new Gtk::VBox();
	vbox1->add(*gui_active);
	vbox1->add(*spacer1);
	tab->add(*vbox1);

	Gtk::VBox* vbox2 = new Gtk::VBox();
	vbox2->add(*gui_speed);
	vbox2->add(*gui_decay);
	tab->add(*vbox2);

	spacer* spacer2 = new spacer();
	spacer2->top_colour.set(top_colour);
	spacer2->bottom_colour.set(bottom_colour);
	spacer2->set_size_request(40*6,70);

	tab->add(*spacer2);

	tab->add(*gui_eq_low);
	tab->add(*gui_eq_mid);
	tab->add(*gui_eq_high);
}

//-------------------------------------------------------------------

echo_gui::~echo_gui()
{

}

//-------------------------------------------------------------------

void echo_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
{

	gui_active->controller = controller;
	gui_active->write_function = write_function;

	gui_speed->controller = controller;
	gui_speed->write_function = write_function;

	gui_decay->controller = controller;
	gui_decay->write_function = write_function;

	gui_eq_low->controller = controller;
	gui_eq_low->write_function = write_function;

	gui_eq_mid->controller = controller;
	gui_eq_mid->write_function = write_function;

	gui_eq_high->controller = controller;
	gui_eq_high->write_function = write_function;

}

