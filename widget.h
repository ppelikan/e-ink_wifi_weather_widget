#pragma once

#include <GxEPD.h>
#include <GxGDEW027C44/GxGDEW027C44.h>
#include <U8g2_for_Adafruit_GFX.h>

extern GxEPD_Class display;
extern U8G2_FOR_ADAFRUIT_GFX u8g2;

//this is base class for all widgets
class Widget
{
    private:
    public:
    int dx, dy, dw, dh;
    Widget(int x, int y, int w, int h) : dx(x), dy(y), dw(w), dh(h) { };
    virtual ~Widget(){};
    virtual void redraw()
    {
        display.drawRect(dx, dy, dw /*+ dx*/, dh /*+ dy*/, GxEPD_DARKGREY);
    };
};