
/*
  LV2 triceratops - Analogue style subtractive synth plugin
  Nick S Bailey <tb303@gmx.com>
*/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <gtkmm.h>

// include the URI and global data of this plugin
#include "triceratops.hpp"

// these are our custom widget includes
#include "logo_gui.h"
#include "presets.h"

// core spec include
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

// GUI
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

using namespace std;

typedef struct {
	char *bundle_path;  
	
	presets* widget_presets;

	Gtk::HBox* container;
	Gtk::VBox* vbox1;
	Gtk::VBox* vbox2;
	Gtk::VBox* vbox3;
	Gtk::Notebook* dco_tab;

	dco_gui* dco1;
	dco_gui* dco2;
	dco_gui* dco3;
	unison_gui* unison;

	Gtk::Notebook* lfo_tab;

	lfo_gui* lfo1;
	lfo_gui* lfo2;
	lfo_gui* lfo3;

	logo_gui* logo1;
	logo_gui* logo2;
	logo_gui* logo3;
	logo_gui* logo4;

	Gtk::Notebook* adsr_tab;

	adsr_gui* adsr_amp;
	adsr_gui* adsr_filter;
	adsr_lfo_gui* adsr_lfo;

	Gtk::Notebook* amp_tab;

	amp_gui* amp_and_filter;
	echo_gui* echo;
	reverb_gui* reverb;
	modifier_gui* modifier;

} triceratopsGUI;

static GtkWidget* make_gui(triceratopsGUI *self) {

	// load configuration file
	// fall back values if no configure file is found

	Gdk::Color top_colour( "#440000");
	Gdk::Color bottom_colour( "#220000");

	// check for default config file

	string config_file_name = "";

	string bundle_config_name = string(self->bundle_path) + string("triceratops.conf");
	ifstream check_file(bundle_config_name);
	if (check_file) config_file_name = bundle_config_name;

	if (config_file_name !="");
	{
		string temp_str;
		string line;
		while (getline(check_file,line))  
		{

			if (line.find("topcolour", 0)==0)
			{
				string::size_type i = line.find("=", 0);
				top_colour.set( line.substr(i + 1));
			}

			if (line.find("bottomcolour", 0)==0)
			{
				string::size_type i = line.find("=", 0);
				bottom_colour.set( line.substr(i + 1));
			}
		}
	check_file.close();
	}

	stringstream local_config_filename;
	local_config_filename.str("");
	local_config_filename << getenv("HOME") << "/.triceratops.conf";

	check_file.open( local_config_filename .str());

	if (check_file) // local config file exists load and use that instead
	{
		string temp_str;
		string line;
		cout << "Loading local config file.." << endl;
		while (getline(check_file,line))  
		{

			if (line.find("topcolour", 0)==0)
			{
				string::size_type i = line.find("=", 0);
				top_colour.set( line.substr(i + 1));
			}

			if (line.find("bottomcolour", 0)==0)
			{
				string::size_type i = line.find("=", 0);
				bottom_colour.set( line.substr(i + 1));
			}
		}
	}
	else // it doesn't exist make a fresh copy with defaults
	{
		fstream local_config_file;
		local_config_file.open(local_config_filename.str()  , ios::out );

		if (local_config_file) 
		{

			local_config_file << "topcolour=" << top_colour.to_string() << endl;
			local_config_file << "bottomcolour=" << bottom_colour.to_string() << endl;
			local_config_file << flush;
			local_config_file.close();
		}
	}

	// set up main GUI

	self->container = new Gtk::HBox();

	self->vbox1 = new Gtk::VBox();
	self->vbox2 = new Gtk::VBox();
	self->vbox3 = new Gtk::VBox();

	self->widget_presets = new presets(self->bundle_path);

	self->widget_presets->top_colour = top_colour;
	self->widget_presets->bottom_colour = bottom_colour;

	self->widget_presets->set_size_request(160,260);

	// self->vbox3->add(*self->widget_presets->button_go_back);	
	self->vbox3->add(*self->widget_presets);	
	// self->vbox3->add(*self->widget_presets->button_save_preset);	

	self->container->add(*self->vbox3);

	Gtk::Combo test;

	//--------------

	self->dco_tab = new Gtk::Notebook();

	self->dco1 = new dco_gui(TRICERATOPS_ACTIVE_ONE, top_colour.to_string(), bottom_colour.to_string());
	self->dco2 = new dco_gui(TRICERATOPS_ACTIVE_TWO, top_colour.to_string(), bottom_colour.to_string());
	self->dco3 = new dco_gui(TRICERATOPS_ACTIVE_THREE, top_colour.to_string(), bottom_colour.to_string());
	self->unison = new unison_gui(TRICERATOPS_UNISON_ACTIVATE,top_colour.to_string(), bottom_colour.to_string());

	self->dco1->gui_pan->port_number = TRICERATOPS_DCO1_PAN;
	self->dco2->gui_pan->port_number = TRICERATOPS_DCO2_PAN;
	self->dco3->gui_pan->port_number = TRICERATOPS_DCO3_PAN;

	self->logo1 = new logo_gui();
	self->logo2 = new logo_gui();
	self->logo3 = new logo_gui();
	self->logo4 = new logo_gui();

	self->logo1->load_logo(self->bundle_path);
	self->logo2->load_logo(self->bundle_path);
	self->logo3->load_logo(self->bundle_path);
	self->logo4->load_logo(self->bundle_path);

	self->logo1->top_colour.set(top_colour.to_string());
	self->logo1->bottom_colour.set(bottom_colour.to_string());
	self->logo2->top_colour.set(top_colour.to_string());
	self->logo2->bottom_colour.set(bottom_colour.to_string());
	self->logo3->top_colour.set(top_colour.to_string());
	self->logo3->bottom_colour.set(bottom_colour.to_string());
	self->logo4->top_colour.set(top_colour.to_string());
	self->logo4->bottom_colour.set(bottom_colour.to_string());

	self->dco1->tab->add(*self->logo1);
	self->dco2->tab->add(*self->logo2);
	self->dco3->tab->add(*self->logo3);
	self->unison->tab->add(*self->logo4);

	Gtk::Label* dco1_label = new Gtk::Label("DCO1");
	Gtk::Label* dco2_label = new Gtk::Label("DCO2");
	Gtk::Label* dco3_label = new Gtk::Label("DCO3");
	Gtk::Label* unison_label = new Gtk::Label("UNISON");

	self->dco_tab->append_page(*self->dco1->tab,*dco1_label);
	self->dco_tab->append_page(*self->dco2->tab,*dco2_label);
	self->dco_tab->append_page(*self->dco3->tab,*dco3_label);
	self->dco_tab->append_page(*self->unison->tab,*unison_label);

	self->dco2->gui_active->set_label("DCO 2");
	self->dco3->gui_active->set_label("DCO 3");

	self->vbox1->add(*self->dco_tab);

	//--------------

	self->lfo_tab = new Gtk::Notebook();

	self->lfo1 = new lfo_gui(TRICERATOPS_LFO1_RETRIG,top_colour.to_string(), bottom_colour.to_string());
	self->lfo2 = new lfo_gui(TRICERATOPS_LFO2_RETRIG,top_colour.to_string(), bottom_colour.to_string());
	self->lfo3 = new lfo_gui(TRICERATOPS_LFO3_RETRIG,top_colour.to_string(), bottom_colour.to_string());

	self->lfo1->gui_route1->route_number = 0;
	self->lfo1->gui_route2->route_number = 1;

	Gtk::Label* lfo1_label = new Gtk::Label("LFO1");
	Gtk::Label* lfo2_label = new Gtk::Label("LFO2");
	Gtk::Label* lfo3_label = new Gtk::Label("LFO3");

	self->lfo_tab->append_page(*self->lfo1->tab,*lfo1_label);
	self->lfo_tab->append_page(*self->lfo2->tab,*lfo2_label);
	self->lfo_tab->append_page(*self->lfo3->tab,*lfo3_label);

	self->vbox2->add(*self->lfo_tab);

	//--------------

	self->adsr_tab = new Gtk::Notebook();
	self->adsr_tab->set_tab_pos(Gtk::POS_BOTTOM);

	self->adsr_amp = new adsr_gui(TRICERATOPS_ATTACK_ONE,top_colour.to_string(), bottom_colour.to_string());
	self->adsr_filter = new adsr_gui(TRICERATOPS_ATTACK_TWO,top_colour.to_string(), bottom_colour.to_string());
	self->adsr_lfo = new adsr_lfo_gui(TRICERATOPS_ATTACK_THREE,top_colour.to_string(), bottom_colour.to_string());

	Gtk::Label* adsr1_label = new Gtk::Label("AMP ADSR");
	Gtk::Label* adsr2_label = new Gtk::Label("FILTER ADSR");
	Gtk::Label* adsr3_label = new Gtk::Label("LFO ADSR");

	self->adsr_tab->append_page(*self->adsr_amp->tab,*adsr1_label);
	self->adsr_tab->append_page(*self->adsr_filter->tab,*adsr2_label);
	self->adsr_tab->append_page(*self->adsr_lfo->tab,*adsr3_label);

	self->vbox1->add(*self->adsr_tab);

	//--------------

	self->amp_tab = new Gtk::Notebook();
	self->amp_tab->set_tab_pos(Gtk::POS_BOTTOM);

	self->amp_and_filter = new amp_gui(TRICERATOPS_MASTER_VOLUME,top_colour.to_string(), bottom_colour.to_string());

	Gtk::Label* amp1_label = new Gtk::Label("AMP/FILTER");

	self->amp_tab->append_page(*self->amp_and_filter->tab,*amp1_label);

	self->vbox2->add(*self->amp_tab);


	//------

	self->echo = new echo_gui(TRICERATOPS_FX_ECHO_ACTIVE,top_colour.to_string(), bottom_colour.to_string());

	Gtk::Label* amp2_label = new Gtk::Label("ECHO");
	self->amp_tab->append_page(*self->echo->tab,*amp2_label);

	//------

	self->reverb = new reverb_gui(TRICERATOPS_FX_REVERB_ACTIVE,top_colour.to_string(), bottom_colour.to_string());

	Gtk::Label* reverb_label = new Gtk::Label("REVERB");
	self->amp_tab->append_page(*self->reverb->tab,*reverb_label);

	//------

	self->modifier = new modifier_gui(TRICERATOPS_MODIFIER_DIRT,top_colour.to_string(), bottom_colour.to_string());

	Gtk::Label* mod_label = new Gtk::Label("MOD");
	self->amp_tab->append_page(*self->modifier->tab,*mod_label);
	self->modifier->gui_modifier_stereo_mode->port_number = TRICERATOPS_MODIFIER_STEREO_MODE;
	self->modifier->gui_modifier_ring->port_number = TRICERATOPS_MODIFIER_RING;
	self->modifier->gui_pitch_bend_range->port_number = TRICERATOPS_PITCH_BEND_RANGE;
	self->modifier->gui_midi_channel->port_number = TRICERATOPS_MIDI_CHANNEL;

	//--------------



	self->container->add(*self->vbox1);
	self->container->add(*self->vbox2);

    return (GtkWidget*)self->container->gobj();
}


static LV2UI_Handle instantiate(const struct _LV2UI_Descriptor * descriptor,
                const char * plugin_uri,
                const char * bundle_path,
                LV2UI_Write_Function write_function,
                LV2UI_Controller controller,
                LV2UI_Widget * widget,
                const LV2_Feature * const * features) {

    if (strcmp(plugin_uri, triceratops_URI) != 0) {
        fprintf(stderr, "SORCER_URI error: this GUI does not support plugin with URI %s\n", plugin_uri);
        return NULL;
    }
    
    triceratopsGUI* self = (triceratopsGUI*)malloc(sizeof(triceratopsGUI));
    
    if (self == NULL) return NULL;

	self->bundle_path = (char*)malloc(strlen(bundle_path)+1);
	memcpy (self->bundle_path,bundle_path,strlen(bundle_path)+1);

	Gtk::Main::init_gtkmm_internals();
	
	*widget = (LV2UI_Widget)make_gui(self);
	
	self->dco1->set_controller(controller,write_function);
	self->dco2->set_controller(controller,write_function);
	self->dco3->set_controller(controller,write_function);
	self->unison->set_controller(controller,write_function);

	self->lfo1->set_controller(controller,write_function);
	self->lfo2->set_controller(controller,write_function);
	self->lfo3->set_controller(controller,write_function);

	self->adsr_amp->set_controller(controller,write_function);
	self->adsr_filter->set_controller(controller,write_function);
	self->adsr_lfo->set_controller(controller,write_function);

	self->amp_and_filter->set_controller(controller,write_function);
	self->echo->set_controller(controller,write_function);
	self->modifier->set_controller(controller,write_function); 
	self->reverb->set_controller(controller,write_function);

	self->widget_presets->dco1 = self->dco1;
	self->widget_presets->dco2 = self->dco2;
	self->widget_presets->dco3 = self->dco3;
	self->widget_presets->unison = self->unison;

	self->widget_presets->lfo1 = self->lfo1;
	self->widget_presets->lfo2 = self->lfo2;
	self->widget_presets->lfo3 = self->lfo3;

	self->widget_presets->adsr_amp = self->adsr_amp;
	self->widget_presets->adsr_filter = self->adsr_filter;
	self->widget_presets->adsr_lfo = self->adsr_lfo;

	self->widget_presets->amp_and_filter = self->amp_and_filter;
	self->widget_presets->echo = self->echo;
	self->widget_presets->reverb = self->reverb;
	self->widget_presets->modifier = self->modifier;

	self->widget_presets->controller =  controller;
	self->widget_presets->write_function = write_function;

	self->widget_presets->bundle_path = bundle_path;

    return (LV2UI_Handle)self;
}

static void cleanup(LV2UI_Handle ui) {

	triceratopsGUI *pluginGui = (triceratopsGUI *) ui;
	pluginGui->container->remove(*pluginGui->vbox1);
	pluginGui->container->remove(*pluginGui->vbox2);
	free(pluginGui->bundle_path);
	free(pluginGui);
}

//------------------------------------------------------------------------------------------------


static void port_event(LV2UI_Handle ui,
               uint32_t port_index,
               uint32_t buffer_size,
               uint32_t format,
               const void * buffer)
{
	triceratopsGUI *self = (triceratopsGUI *) ui;
	float val = * static_cast<const float*>(buffer);
    
	switch (port_index)
	{

		case TRICERATOPS_MASTER_VOLUME:
			self->amp_and_filter->gui_volume->set_value( val );
			break;

		case TRICERATOPS_MASTER_TUNE:
			self->amp_and_filter->gui_tune->set_value( val );
			break;

		case TRICERATOPS_AMP_DRIVE:
			self->amp_and_filter->gui_drive->set_value( val );
			break;

		case TRICERATOPS_FILTER_MODE:
			self->amp_and_filter->gui_filter_type->set_value( val );
			break;

		case TRICERATOPS_CUTOFF:
			self->amp_and_filter->gui_cutoff->set_value( val );
			self->amp_and_filter->gui_filter_type->val_cutoff = val;
			break;

		case TRICERATOPS_RESONANCE:
			self->amp_and_filter->gui_resonance->set_value( val );
			self->amp_and_filter->gui_filter_type->val_res = val;
			break;

		case TRICERATOPS_FILTER_KEY_FOLLOW:
			self->amp_and_filter->gui_key_follow->set_value( val );
			break;

		case TRICERATOPS_LEGATO:
			self->amp_and_filter->gui_legato->set_value( val );
			break;

		case TRICERATOPS_SYNC:
			self->amp_and_filter->gui_sync->set_value( val );	
			break;

		case TRICERATOPS_FM:
			self->amp_and_filter->gui_fm->set_value( val );
			break;

		case TRICERATOPS_PANIC:
			self->amp_and_filter->gui_panic->set_value( val );
			break;

		// envelopes

		case TRICERATOPS_ATTACK_ONE:
			self->adsr_amp->gui_attack->set_value( val );
			break;

		case TRICERATOPS_DECAY_ONE:
			self->adsr_amp->gui_decay->set_value( val );
			break;

		case TRICERATOPS_SUSTAIN_ONE:
			self->adsr_amp->gui_sustain->set_value( val );
			break;

		case TRICERATOPS_RELEASE_ONE:
			self->adsr_amp->gui_release->set_value( val );
			break;

		case TRICERATOPS_ADSR1_ROUTE_ONE:
			self->adsr_amp->gui_route1->set_value( val );
			break;

		case TRICERATOPS_ADSR1_ROUTE_ONE_DEST:
			self->adsr_amp->gui_route1->route_number = val;
			break;


		case TRICERATOPS_ADSR1_ROUTE_TWO:
			self->adsr_amp->gui_route2->set_value( val );
			break;

		case TRICERATOPS_ADSR1_ROUTE_TWO_DEST:
			self->adsr_amp->gui_route2->route_number = val;
			break;




		case TRICERATOPS_ATTACK_TWO:
			self->adsr_filter->gui_attack->set_value( val );
			break;

		case TRICERATOPS_DECAY_TWO:
			self->adsr_filter->gui_decay->set_value( val );
			break;

		case TRICERATOPS_SUSTAIN_TWO:
			self->adsr_filter->gui_sustain->set_value( val );
			break;

		case TRICERATOPS_RELEASE_TWO:
			self->adsr_filter->gui_release->set_value( val );
			break;

		case TRICERATOPS_ADSR2_ROUTE_ONE:
			self->adsr_filter->gui_route1->set_value( val );
			break;

		case TRICERATOPS_ADSR2_ROUTE_ONE_DEST:
			self->adsr_filter->gui_route1->route_number = val;
			break;

		case TRICERATOPS_ADSR2_ROUTE_TWO:
			self->adsr_filter->gui_route2->set_value( val );
			break;

		case TRICERATOPS_ADSR2_ROUTE_TWO_DEST:
			self->adsr_filter->gui_route2->route_number = val;
			break;





		case TRICERATOPS_ATTACK_THREE:
			self->adsr_lfo->gui_attack->set_value( val );
			break;

		case TRICERATOPS_DECAY_THREE:
			self->adsr_lfo->gui_decay->set_value( val );
			break;

		case TRICERATOPS_SUSTAIN_THREE:
			self->adsr_lfo->gui_sustain->set_value( val );
			break;

		case TRICERATOPS_RELEASE_THREE:
			self->adsr_lfo->gui_release->set_value( val );
			break;

		case TRICERATOPS_ADSR3_LFO1_AMOUNT:
			self->adsr_lfo->gui_lfo1->set_value( val );
			break;

		case TRICERATOPS_ADSR3_LFO2_AMOUNT:
			self->adsr_lfo->gui_lfo2->set_value( val );
			break;

		case TRICERATOPS_ADSR3_LFO3_AMOUNT:
			self->adsr_lfo->gui_lfo3->set_value( val );
			break;

		// oscillator one

		case TRICERATOPS_ACTIVE_ONE:
			self->dco1->gui_active->set_value( val );
			break;

		case TRICERATOPS_VOLUME_ONE:
			self->dco1->gui_volume->set_value( val );
			break;

		case TRICERATOPS_PULSEWIDTH_ONE:
			self->dco1->gui_pulsewidth->set_value( val );
			break;

		case TRICERATOPS_WAVE_ONE:
			self->dco1->gui_wave->set_value( val );
			break;

		case TRICERATOPS_OCTAVE_ONE:
			self->dco1->gui_octave->set_value( val );
			break;

		case TRICERATOPS_DETUNE_ONE:
			self->dco1->gui_detune->set_value( val );
			break;

		case TRICERATOPS_INERTIA_ONE:
			self->dco1->gui_inertia->set_value( val );
			break;


		// oscillator two

		case TRICERATOPS_ACTIVE_TWO:
			self->dco2->gui_active->set_value( val );
			break;

		case TRICERATOPS_VOLUME_TWO:
			self->dco2->gui_volume->set_value( val );
			break;

		case TRICERATOPS_PULSEWIDTH_TWO:
			self->dco2->gui_pulsewidth->set_value( val );
			break;

		case TRICERATOPS_WAVE_TWO:
			self->dco2->gui_wave->set_value( val );
			break;

		case TRICERATOPS_OCTAVE_TWO:
			self->dco2->gui_octave->set_value( val );
			break;

		case TRICERATOPS_DETUNE_TWO:
			self->dco2->gui_detune->set_value( val );
			break;

		case TRICERATOPS_INERTIA_TWO:
			self->dco2->gui_inertia->set_value( val );
			break;


		// oscillator three

		case TRICERATOPS_ACTIVE_THREE:
			self->dco3->gui_active->set_value( val );
			break;

		case TRICERATOPS_VOLUME_THREE:
			self->dco3->gui_volume->set_value( val );
			break;

		case TRICERATOPS_WAVE_THREE:
			self->dco3->gui_wave->set_value( val );
			break;

		case TRICERATOPS_PULSEWIDTH_THREE:
			self->dco3->gui_pulsewidth->set_value( val );
			break;

		case TRICERATOPS_OCTAVE_THREE:
			self->dco3->gui_octave->set_value( val );
			break;

		case TRICERATOPS_DETUNE_THREE:
			self->dco3->gui_detune->set_value( val );
			break;

		case TRICERATOPS_INERTIA_THREE:
			self->dco3->gui_inertia->set_value( val );
			break;

	

		// LFO ONE

		case TRICERATOPS_LFO1_RETRIG:
			self->lfo1->gui_retrig->set_value( val );
			break;
			
		case TRICERATOPS_LFO1_SPEED:
			self->lfo1->gui_speed->set_value( val );
			break;

		case TRICERATOPS_LFO1_WAVE:
			self->lfo1->gui_wave->set_value( val );
			break;

		case TRICERATOPS_LFO1_DCO1_PITCH:
			self->lfo1->gui_dco1_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO1_DCO2_PITCH:
			self->lfo1->gui_dco2_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO1_DCO3_PITCH:
			self->lfo1->gui_dco3_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO1_FILTER:
			self->lfo1->gui_cutoff->set_value( val );
			break;

		case TRICERATOPS_LFO1_ROUTE_ONE:
			self->lfo1->gui_route1->set_value( val );
			break;


		case TRICERATOPS_LFO1_ROUTE_ONE_DEST:
			self->lfo1->gui_route1->route_number = val;
			break;

		case TRICERATOPS_LFO1_ROUTE_TWO:
			self->lfo1->gui_route2->set_value( val );
			break;

		case TRICERATOPS_LFO1_ROUTE_TWO_DEST:
			self->lfo1->gui_route2->route_number = val;
			break;


		// LFO TWO

		case TRICERATOPS_LFO2_RETRIG:
			self->lfo2->gui_retrig->set_value( val );
			break;
			
		case TRICERATOPS_LFO2_SPEED:
			self->lfo2->gui_speed->set_value( val );
			break;

		case TRICERATOPS_LFO2_WAVE:
			self->lfo2->gui_wave->set_value( val );
			break;

		case TRICERATOPS_LFO2_DCO1_PITCH:
			self->lfo2->gui_dco1_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO2_DCO2_PITCH:
			self->lfo2->gui_dco2_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO2_DCO3_PITCH:
			self->lfo2->gui_dco3_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO2_FILTER:
			self->lfo2->gui_cutoff->set_value( val );
			break;

		case TRICERATOPS_LFO2_ROUTE_ONE:
			self->lfo2->gui_route1->set_value( val );
			break;

		case TRICERATOPS_LFO2_ROUTE_ONE_DEST:
			self->lfo2->gui_route1->route_number = val;
			break;

		case TRICERATOPS_LFO2_ROUTE_TWO:
			self->lfo2->gui_route2->set_value( val );
			break;

		case TRICERATOPS_LFO2_ROUTE_TWO_DEST:
			self->lfo2->gui_route2->route_number = val;
			break;

		// LFO THREE

		case TRICERATOPS_LFO3_RETRIG:
			self->lfo3->gui_retrig->set_value( val );
			break;
			
		case TRICERATOPS_LFO3_SPEED:
			self->lfo3->gui_speed->set_value( val );
			break;

		case TRICERATOPS_LFO3_WAVE:
			self->lfo3->gui_wave->set_value( val );
			break;

		case TRICERATOPS_LFO3_DCO1_PITCH:
			self->lfo3->gui_dco1_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO3_DCO2_PITCH:
			self->lfo3->gui_dco2_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO3_DCO3_PITCH:
			self->lfo3->gui_dco3_pitch->set_value( val );
			break;

		case TRICERATOPS_LFO3_FILTER:
			self->lfo3->gui_cutoff->set_value( val );
			break;

		case TRICERATOPS_LFO3_ROUTE_ONE:
			self->lfo3->gui_route1->set_value( val );
			break;

		case TRICERATOPS_LFO3_ROUTE_ONE_DEST:
			self->lfo3->gui_route1->route_number = val;
			break;

		case TRICERATOPS_LFO3_ROUTE_TWO:
			self->lfo3->gui_route2->set_value( val );
			break;

		case TRICERATOPS_LFO3_ROUTE_TWO_DEST:
			self->lfo3->gui_route2->route_number = val;
			break;


		// ECHO

		case TRICERATOPS_FX_ECHO_ACTIVE:
			self->echo->gui_active->set_value( val );
			break;

		case TRICERATOPS_FX_ECHO_SPEED:
			self->echo->gui_speed->set_value( val );
			break;

		case TRICERATOPS_FX_ECHO_DECAY:
			self->echo->gui_decay->set_value( val );
			break;

		case TRICERATOPS_FX_ECHO_EQ_LOW:
			self->echo->gui_eq_low->set_value( val );
			break;

		case TRICERATOPS_FX_ECHO_EQ_MID:
			self->echo->gui_eq_mid->set_value( val );
			break;

		case TRICERATOPS_FX_ECHO_EQ_HIGH:
			self->echo->gui_eq_high->set_value( val );
			break;


		// UNISON

		case TRICERATOPS_UNISON_ACTIVATE:
			self->unison->gui_active->set_value( val );
			break;

		case TRICERATOPS_UNISON_ONE:
			self->unison->gui_dco1_unison->set_value( val );
			break;

		case TRICERATOPS_UNISON_TWO:
			self->unison->gui_dco2_unison->set_value( val );
			break;

		case TRICERATOPS_UNISON_THREE:
			self->unison->gui_dco3_unison->set_value( val );
			break;

		// MODIFIER

		case TRICERATOPS_MODIFIER_DIRT:
			self->modifier->gui_modifier_dirt->set_value( val );
			break;

		// REVERB

		case TRICERATOPS_FX_REVERB_ACTIVE:
			self->reverb->gui_active->set_value( val );
			break;

		case TRICERATOPS_FX_REVERB_DECAY:
			self->reverb->gui_decay->set_value( val );
			break;

		case TRICERATOPS_FX_REVERB_MIX:
			self->reverb->gui_mix->set_value( val );
			break;

	// ERM ANOTHER MODIFIER PARAM, MESSY BUT NECESSARY :oO

		case TRICERATOPS_MODIFIER_STEREO_MODE:
			self->modifier->gui_modifier_stereo_mode->set_value( val );
			break;

		case TRICERATOPS_DCO1_PAN:
			self->dco1->gui_pan->set_value( val );
			break;

		case TRICERATOPS_DCO2_PAN:
			self->dco2->gui_pan->set_value( val );
			break;

		case TRICERATOPS_DCO3_PAN:
			self->dco3->gui_pan->set_value( val );
			break;

		case TRICERATOPS_MODIFIER_RING:
			self->modifier->gui_modifier_ring->set_value( val );
			break;

		case TRICERATOPS_PITCH_BEND_RANGE:
			self->modifier->gui_pitch_bend_range->set_value( val );
			break;

		case TRICERATOPS_MIDI_CHANNEL:
			self->modifier->gui_midi_channel->set_value( val );
			break;
	}

	return;
}

//------------------------------------------------------------------------------------------------

static LV2UI_Descriptor descriptors[] = {
    {triceratops_UI_URI, instantiate, cleanup, port_event, NULL}
};

const LV2UI_Descriptor * lv2ui_descriptor(uint32_t index) {
    printf("lv2ui_descriptor(%u) called\n", (unsigned int)index); 
    if (index >= sizeof(descriptors) / sizeof(descriptors[0])) {
        return NULL;
    }
    return descriptors + index;
}



