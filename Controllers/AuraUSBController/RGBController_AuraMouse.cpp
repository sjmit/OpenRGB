/*-----------------------------------------*\
|  RGBController_AuraMouse.cpp              |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#include "RGBController_AuraMouse.h"

RGBController_AuraMouse::RGBController_AuraMouse(AuraMouseController* aura_ptr)
{
    aura = aura_ptr;

    name        = "ASUS Aura Mouse";
    type        = DEVICE_TYPE_MOUSE;
    description = "ASUS Aura Mouse Device";
    location    = aura->GetDeviceLocation();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = AURA_MOUSE_MODE_STATIC;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = AURA_MOUSE_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = AURA_MOUSE_MODE_COLOR_CYCLE;
    SpectrumCycle.flags      = 0;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Reactive;
    Reactive.name       = "Reactive";
    Reactive.value      = AURA_MOUSE_MODE_REACTIVE;
    Reactive.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Reactive.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Reactive);

    mode Off;
    Off.name       = "Off";
    Off.value      = AURA_MOUSE_MODE_OFF;
    Off.flags      = 0;
    Off.color_mode = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupZones();
}

RGBController_AuraMouse::~RGBController_AuraMouse()
{

}

void RGBController_AuraMouse::SetupZones()
{
    zone logo_zone;

    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;

    zones.push_back(logo_zone);

    led logo_led;

    logo_led.name           = "Logo";

    leds.push_back(logo_led);

    zone scroll_zone;

    scroll_zone.name        = "Scroll Wheel";
    scroll_zone.type        = ZONE_TYPE_SINGLE;
    scroll_zone.leds_min    = 1;
    scroll_zone.leds_max    = 1;
    scroll_zone.leds_count  = 1;
    scroll_zone.matrix_map  = NULL;

    zones.push_back(scroll_zone);

    led scroll_led;

    scroll_led.name         = "Scroll Wheel";

    leds.push_back(scroll_led);

    SetupColors();
}

void RGBController_AuraMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraMouse::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_AuraMouse::UpdateZoneLEDs(int zone)
{
    UpdateSingleLED(zone);
}

void RGBController_AuraMouse::UpdateSingleLED(int led)
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        if(led == 0)
        {
            red = RGBGetRValue(modes[active_mode].colors[led]);
            grn = RGBGetGValue(modes[active_mode].colors[led]);
            blu = RGBGetBValue(modes[active_mode].colors[led]);

            aura->SendUpdate(AURA_MOUSE_ZONE_LOGO, modes[active_mode].value, red, grn, blu);
        }
        else
        {
            red = RGBGetRValue(modes[active_mode].colors[led]);
            grn = RGBGetGValue(modes[active_mode].colors[led]);
            blu = RGBGetBValue(modes[active_mode].colors[led]);

            aura->SendUpdate(AURA_MOUSE_ZONE_SCROLL, modes[active_mode].value, red, grn, blu);
        }
    }
    else
    {
        aura->SendUpdate(AURA_MOUSE_ZONE_ALL, modes[active_mode].value, red, grn, blu);
    }
}

void RGBController_AuraMouse::SetCustomMode()
{

}

void RGBController_AuraMouse::DeviceUpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        red = RGBGetRValue(modes[active_mode].colors[0]);
        grn = RGBGetGValue(modes[active_mode].colors[0]);
        blu = RGBGetBValue(modes[active_mode].colors[0]);

        aura->SendUpdate(AURA_MOUSE_ZONE_LOGO, modes[active_mode].value, red, grn, blu);

        red = RGBGetRValue(modes[active_mode].colors[1]);
        grn = RGBGetGValue(modes[active_mode].colors[1]);
        blu = RGBGetBValue(modes[active_mode].colors[1]);

        aura->SendUpdate(AURA_MOUSE_ZONE_SCROLL, modes[active_mode].value, red, grn, blu);
    }
    else
    {
        aura->SendUpdate(AURA_MOUSE_ZONE_ALL, modes[active_mode].value, red, grn, blu);
    }
}
