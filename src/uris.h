

#ifndef triceratops_URIS_H
#define triceratops_URIS_H

#include "lv2/lv2plug.in/ns/ext/state/state.h"
#include "lv2/lv2plug.in/ns/ext/log/log.h"

#define LV2_MIDI__MidiEvent "http://lv2plug.in/ns/ext/midi#MidiEvent"
#define triceratops_URI "http://nickbailey.co.nr/triceratops"

typedef struct {
	LV2_URID midi_Event;
} triceratopsURIs;

static inline void
map_triceratops_uris(LV2_URID_Map* map, triceratopsURIs* uris)
{
	uris->midi_Event         = map->map(map->handle, LV2_MIDI__MidiEvent);
}




#endif  /* triceratops_URIS_H */




