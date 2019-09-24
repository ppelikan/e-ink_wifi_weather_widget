#pragma once

#include "widget.h"

enum tTxtPos
{
    jCenter = 0x00,
    jCenterTop = 0x01,
    jLeftCenter = 0x04,
    jLeftTop = 0x05,
    jCenterBottom = 0x02,
    jRightCenter = 0x08,
    jRightBottom = 0x0A,
    jLeftBottom = 0x06,
    jRightTop = 0x09
};

class Label : public Widget
{
private:
    //int borderX = 5, borderY = 5;
    int16_t getTextWidth()
    {
        //u8g2.setFont(fnt);
        return u8g2.getUTF8Width(txt.c_str());
    };

    int8_t getFontHeight()
    {
        //u8g2.setFont(fnt);
        return u8g2.getFontAscent();
    };

public:
    String txt = "";
    tTxtPos pos = jCenter;
    uint16_t fGcolor = GxEPD_BLACK, bGcolor = GxEPD_WHITE;
    const uint8_t *fnt;

    Label(int x, int y, int w, int h, String text = "") : Widget(x, y, w, h) /*, borderX(w / 50), borderY(h / 50)*/, txt(text){};
    //~Label(){}
    Label &setFont(const uint8_t *font)
    {
        fnt = font;
        return *this;
    };
    Label &setTextPos(tTxtPos TextPos)
    {
        pos = TextPos;
        return *this;
    }
    Label &setText(String text)
    {
        txt = text;
        return *this;
    }
    void redraw(String text)
    {
        setText(text);
        redraw();
    }
    void redraw()
    {
        int16_t xpos, ypos;
        u8g2.setFont(fnt);
        if (pos & 0x04) //left
            xpos = dx;
        else if (pos & 0x08) //right
            xpos = dx + dw - getTextWidth();
        else //center
            xpos = dx + dw / 2 - getTextWidth() / 2;
        if (pos & 0x01) //top
            ypos = dy;
        else if (pos & 0x02) //bottom
            ypos = dy + dh - getFontHeight();
        else //center
            ypos = dy + dh / 2 - getFontHeight() / 2;

        display.fillRect(dx, dy, dw, dh, bGcolor);
#ifdef WIDGETS_SHOW_DEBUG_FRAMES
        display.drawRect(dx, dy, dw, dh, fGcolor);
#endif
        u8g2.setForegroundColor(fGcolor);
        u8g2.setBackgroundColor(bGcolor);
        u8g2.setCursor(xpos, ypos + getFontHeight());
        u8g2.print(txt);
    };
};