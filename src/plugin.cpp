// Based on Daniel Davies VCV modules (https://github.com/danieldavies99)
// Modified by AndyClotta - 2026
#include "plugin.hpp"

Plugin *pluginInstance;

void init(Plugin *p)
{
	pluginInstance = p;

	// Add modules here
	p->addModel(modelInteractiveRhythmComposer);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}