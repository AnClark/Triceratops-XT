
#include "adsr_gui.h"

//-------------------------------------------------------------------

adsr_gui::adsr_gui(int port_number,string top_colour, string bottom_colour)
{

	tab = new Gtk::HBox();

	gui_attack = new fader();
	gui_attack->top_colour.set(top_colour);
	gui_attack->bottom_colour.set(bottom_colour);
	gui_attack->set_label("ATTACK");
	gui_attack->port_number = port_number;
	++port_number;

	gui_decay = new fader();
	gui_decay->top_colour.set(top_colour);
	gui_decay->bottom_colour.set(bottom_colour);
	gui_decay->set_label("DECAY");
	gui_decay->port_number = port_number;
	++port_number;

	gui_sustain = new fader();
	gui_sustain->top_colour.set(top_colour);
	gui_sustain->bottom_colour.set(bottom_colour);
	gui_sustain->set_label("SUSTAIN");
	gui_sustain->port_number = port_number;
	++port_number;

	gui_release = new fader();
	gui_release->top_colour.set(top_colour);
	gui_release->bottom_colour.set(bottom_colour);
	gui_release->set_label("RELEASE");
	gui_release->port_number = port_number;
	++port_number;

	gui_route1 = new fader();
	gui_route1->top_colour.set(top_colour);
	gui_route1->bottom_colour.set(bottom_colour);
	gui_route1->set_label("ROUTE 1");
	gui_route1->port_number = port_number;
	++port_number;
	gui_route1->route_port_number = port_number;
	++port_number;

	gui_route2 = new fader();
	gui_route2->top_colour.set(top_colour);
	gui_route2->bottom_colour.set(bottom_colour);
	gui_route2->set_label("ROUTE 2");
	gui_route2->port_number = port_number;
	++port_number;
	gui_route2->route_port_number = port_number;
	++port_number;

	tab->add(*gui_attack);
	tab->add(*gui_decay);
	tab->add(*gui_sustain);
	tab->add(*gui_release);

	spacer* spacer1 = new spacer();
	spacer1->top_colour.set(top_colour);
	spacer1->bottom_colour.set(bottom_colour);
	spacer1->set_size_request(40,70);
	spacer1->pos_mode = 1;

	spacer* spacer2 = new spacer();
	spacer2->top_colour.set(top_colour);
	spacer2->bottom_colour.set(bottom_colour);
	spacer2->set_size_request(40,70);
	spacer2->pos_mode = 2;

	Gtk::VBox* vbox1 = new Gtk::VBox();
	vbox1->add(*spacer1);
	vbox1->add(*spacer2);
	tab->add(*vbox1);

	spacer* spacer3 = new spacer();
	spacer3->top_colour.set(top_colour);
	spacer3->bottom_colour.set(bottom_colour);
	spacer3->set_size_request(40,70);
	spacer3->pos_mode = 1;

	spacer* spacer4 = new spacer();
	spacer4->top_colour.set(top_colour);
	spacer4->bottom_colour.set(bottom_colour);
	spacer4->set_size_request(40,70);
	spacer4->pos_mode = 2;

	Gtk::VBox* vbox2 = new Gtk::VBox();
	vbox2->add(*spacer3);
	vbox2->add(*spacer4);
	tab->add(*vbox2);

	tab->add(*gui_route1);
	tab->add(*gui_route2);
	
}

//-------------------------------------------------------------------

adsr_gui::~adsr_gui()
{

}

//-------------------------------------------------------------------

void adsr_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
{
	gui_attack->controller = controller;
	gui_attack->write_function = write_function;

	gui_decay->controller = controller;
	gui_decay->write_function = write_function;

	gui_sustain->controller = controller;
	gui_sustain->write_function = write_function;

	gui_release->controller = controller;
	gui_release->write_function = write_function;

	gui_route1->controller = controller;
	gui_route1->write_function = write_function;

	gui_route2->controller = controller;
	gui_route2->write_function = write_function;
	

}

