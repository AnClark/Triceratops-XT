
#include "unison_gui.h"

//-------------------------------------------------------------------

unison_gui::unison_gui(int port_number,string top_colour, string bottom_colour)
{

	tab = new Gtk::HBox();

	gui_active = new toggle();
	gui_active->top_colour.set(top_colour);
	gui_active->bottom_colour.set(bottom_colour);
	gui_active->set_label("ACTIVE");
	gui_active->port_number = port_number;
	gui_active->pos_mode = 1;
	++port_number;

	gui_dco1_unison = new fader();
	gui_dco1_unison->top_colour.set(top_colour);
	gui_dco1_unison->bottom_colour.set(bottom_colour);
	gui_dco1_unison->set_label("DCO1");
	gui_dco1_unison->set_min(0);
	gui_dco1_unison->set_max(7);
	gui_dco1_unison->set_value(0);
	gui_dco1_unison->port_number = port_number;
	++port_number;

	gui_dco2_unison = new fader();
	gui_dco2_unison->top_colour.set(top_colour);
	gui_dco2_unison->bottom_colour.set(bottom_colour);
	gui_dco2_unison->set_label("DCO2");
	gui_dco2_unison->set_min(0);
	gui_dco2_unison->set_max(7);
	gui_dco2_unison->set_value(0);
	gui_dco2_unison->port_number = port_number;
	++port_number;

	gui_dco3_unison = new fader();
	gui_dco3_unison->top_colour.set(top_colour);
	gui_dco3_unison->bottom_colour.set(bottom_colour);
	gui_dco3_unison->set_label("DCO3");
	gui_dco3_unison->set_min(0);
	gui_dco3_unison->set_max(7);
	gui_dco3_unison->set_value(0);
	gui_dco3_unison->port_number = port_number;
	++port_number;


	//---------

	spacer* spacer1 = new spacer();
	spacer1->top_colour.set(top_colour);
	spacer1->bottom_colour.set(bottom_colour);
	spacer1->set_size_request(40,70);

	Gtk::VBox* vbox1 = new Gtk::VBox();
	vbox1->add(*gui_active);
	vbox1->add(*spacer1);
	spacer1->pos_mode = 2;
	tab->add(*vbox1);

	spacer* spacer2 = new spacer();
	spacer2->top_colour.set(top_colour);
	spacer2->bottom_colour.set(bottom_colour);
	spacer2->set_size_request(80,70);

	tab->add(*spacer2);

	tab->add(*gui_dco1_unison);
	tab->add(*gui_dco2_unison);
	tab->add(*gui_dco3_unison);
}

//-------------------------------------------------------------------

unison_gui::~unison_gui()
{

}

//-------------------------------------------------------------------

void unison_gui::set_controller(LV2UI_Controller controller, LV2UI_Write_Function write_function)
{

	gui_active->controller = controller;
	gui_active->write_function = write_function;

	gui_dco1_unison->controller = controller;
	gui_dco1_unison->write_function = write_function;

	gui_dco2_unison->controller = controller;
	gui_dco2_unison->write_function = write_function;

	gui_dco3_unison->controller = controller;
	gui_dco3_unison->write_function = write_function;


}

