#pragma once
#include <rack.hpp>
#include "plugin.hpp"

struct RedKnob : rack::RoundKnob
{
    RedKnob()
    {
        setSvg(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/widgets/RedKnob.svg")));
    }
};

struct SteppedRedKnob : rack::RoundKnob
{
    SteppedRedKnob()
    {
        setSvg(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/widgets/RedKnob.svg")));
        snap = true;
    }
};