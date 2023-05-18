
#include "adsr_lfo_gui.h"

//-------------------------------------------------------------------

adsr_lfo_gui::adsr_lfo_gui(int port_number,string top_colour, string bottom_colour)
{

	tab = new Gtk::HBox();

	gui_attack = new fader();
	gui_attack->top_colour.set(top_colour);
	gui_attack->bottom_colour.set(bottom_colour);
	gui_attack->set_label("ATTACK");
	gui_attack->port_number = port_number;
	++port_number;

	gui_decay = new fader();
	gui_decay ->top_colour.set(top_colour);
	gui_decay ->bottom_colour.set(bottom_colour);
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
	gui_release ->top_colour.set(top_colour);
	gui_release ->bottom_colour.set(bottom_colour);
	gui_release->set_label("RELEASE");
	gui_release->port_number = port_number;
	++port_number;

	gui_lfo1 = new toggle();
	gui_lfo1->top_colour.set(top_colour);
	gui_lfo1->bottom_colour.set(bottom_colour);
	gui_lfo1->set_label("LFO1");
	gui_lfo1->port_number = port_number;
	gui_lfo1->pos_mode = 1;
	++port_number;

	gui_lfo2 = new toggle();
	gui_lfo2->top_colour.set(top_colour);
	gui_lfo2->bottom_colour.set(bottom_colour);
	gui_lfo2->set_label("LFO2");
	gui_lfo2->port_number = port_number;
	gui_lfo2->pos_mode = 2;
	++port_number;

	gui_lfo3 = new toggle();
	gui_lfo3->top_colour.set(top_colour);
	gui_lfo3->bottom_colour.set(bottom_colour);
	gui_lfo3->set_label("LFO3");
	gui_lfo3->port_number = port_number;
	gui_lfo3->pos_mode = 1;
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

	//--------------------------------------

	tab->add(*gui_attack);
	tab->add(*gui_decay);
	tab->add(*gui_sustain);
	tab->add(*gui_release);

	spacer* spacer3 = new spacer();
	spacer3->top_colour.set(top_colour);
	spacer3->bottom_colour.set(bottom_colour);
	spacer3->set_size_request(100,70);
	spacer3->pos_mode = 0;

	tab->add(*spacer3);

	Gtk::VBox* vbox1 = new Gtk::VBox();
	vbox1->add(*gui_lfo1);
	vbox1->add(*gui_lfo2);
	tab->add(*vbox1);

	spacer* spacer2 = new spacer();
	spacer2->top_colour.set(top_colour);
	spacer2->bottom_colour.set(bottom_colour);
	spacer2->set_size_request(40,70);
	spacer2->pos_mode = 2;

	Gtk::VBox* vbox2 = new Gtk::VBox();
	vbox2->add(*gui_lfo3);
	vbox2->add(*spacer2);
	tab->add(*vbox2);




	// tab->add(*gui_route1); // WILL IMPLEMENT THESE LATER
	// ab->add(*gui_route2);

	
}

//-------------------------------------------------------------------

adsr_lfo_gui::~adsr_lfo_gui()
{

}

//-------------------------------------------------------------------

void adsr_lfo_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
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

	gui_lfo1->controller = controller;
	gui_lfo1->write_function = write_function;

	gui_lfo2->controller = controller;
	gui_lfo2->write_function = write_function;

	gui_lfo3->controller = controller;
	gui_lfo3->write_function = write_function;
	
}

