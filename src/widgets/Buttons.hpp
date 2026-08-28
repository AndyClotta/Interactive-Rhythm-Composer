
#pragma once
#include "plugin.hpp"

struct ShiftArrowButtonLeft : rack::app::SvgSwitch
{
    ShiftArrowButtonLeft()
    {
        momentary = true;
        addFrame(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/widgets/LightLeftArrow.svg")));
        addFrame(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/widgets/DarkLeftArrow.svg")));
    }
};

struct ShiftArrowButtonRight : rack::app::SvgSwitch
{
    ShiftArrowButtonRight()
    {
        momentary = true;
        addFrame(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/widgets/LightRightArrow.svg")));
        addFrame(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/widgets/DarkRightArrow.svg")));
    }
};

struct CKD6Latch : rack::app::SvgSwitch
{
    CKD6Latch()
    {
        momentary = false;
        latch = false;
        addFrame(rack::Svg::load(rack::asset::system("res/ComponentLibrary/CKD6_0.svg")));
        addFrame(rack::Svg::load(rack::asset::system("res/ComponentLibrary/CKD6_1.svg")));
    }
};

struct CKD6InvisibleLatch : rack::app::SvgSwitch
{
    CKD6InvisibleLatch()
    {
        momentary = false;
        latch = true;
        addFrame(rack::Svg::load(rack::asset::system("res/ComponentLibrary/CKD6_0.svg")));
        addFrame(rack::Svg::load(rack::asset::system("res/ComponentLibrary/CKD6_1.svg")));
    }
};
