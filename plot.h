#pragma once

//#include "math.h"
#include "widget.h"

struct DataPoints
{
    std::vector<std::pair<double, double>> Data;   // <x value, y value>
    std::vector<std::pair<String, String>> Labels; // <x label, y label>
    bool Xlabels = false;
    bool Ylabels = false;
    ~DataPoints()
    {
        Data.clear();
        Labels.clear();
    }
    uint32_t getXLabelWidth(const uint8_t *font)
    {
        uint32_t w = 0;
        u8g2.setFont(font);
        for (auto i : Labels)
            w += (u8g2.getUTF8Width(i.first.c_str())); // * 2);
        return w;
    }
};

class Plot : public Widget
{
private:
    double maxX = 0, maxY = 0; //real x,y values corresponding to top right corner of plot
    double minX = 0, minY = 0; //real x,y values corresponding to bottom left corner of plot
    double scaleX = 1, scaleY = 1;
    bool hasXlabels = false;
    bool hasYlabels = false;
    const uint8_t *axisLabelsFont;
    const std::vector<uint16> ColorTable{GxEPD_RED, GxEPD_BLACK, GxEPD_DARKGREY, GxEPD_LIGHTGREY, GxEPD_BLACK, GxEPD_RED};
    inline int XtoPx(double x)
    {
        return dx + borderX + (x + abs(minX)) * scaleX; //convert real x value to pixel x position
    };
    inline int YtoPy(double y)
    {
        return dy + dh - borderY - (y - (minY)) * scaleY; //convert real y value to pixel y position
    };
    inline void fixMinMax(double x, double y)
    {
        maxX = fmax(maxX, x); //check if point x,y is located outside of plot
        maxY = fmax(maxY, y); //if so, then enlarge displayed area (zoom out)
        minX = fmin(minX, x);
        minY = fmin(minY, y);
    }
    uint8_t getFontHeight()
    {
        u8g2.setFont(axisLabelsFont);
        return u8g2.getFontAscent();
    }
    uint16_t getXLabelsStep(DataPoints &plot)
    {
        uint16_t fit_labels = trunc(dw / plot.getXLabelWidth(axisLabelsFont)) * plot.Data.size(); //how many labels can we fit in x axis
        return (plot.Data.size() / fit_labels);
    }
    // uint16_t getYLabelsStep(DataPoints &plot)
    // {
    //     uint16_t fit_labels = trunc(dw / plot.getXLabelWidth(axisLabelsFont)) * plot.Data.size(); //how many labels can we fit in x axis
    //     return (plot.Data.size() / fit_labels);
    // }

public:
    std::vector<DataPoints> Plots;
    bool FitZero = true; //true means plot will be zoomed to fit point 0,0 as well as whole graph
    bool ShowXAxis = true;
    bool ShowYAxis = true;
    bool ShowBorder = true;
    int borderX = 5, borderY = 5;
    Plot(int x, int y, int w, int h) : Widget(x, y, w, h), borderX(w / 30), borderY(h / 30) { addPlot(); };
    ~Plot()
    {
        Plots.clear();
    }
    void redraw()
    {
        if (!Plots.size()) //if no plots, then exit
            return;
        if (FitZero)
            maxX = maxY = minX = minY = 0;
        else
        {
            maxX = minX = Plots.front().Data.front().first;
            maxY = minY = Plots.front().Data.front().second;
        }
        for (auto j : Plots)
            for (auto i : j.Data)
                fixMinMax(i.first, i.second);
        scaleX = ((double)(dw - borderX * 2)) / (maxX - minX); //calculate plot scale based on datapoints
        scaleY = ((double)(dh - borderY * 2)) / (maxY - minY);

        //QuickCG::drawRect(dx, dy, dw + dx, dh + dy, QuickCG::RGB_Black);
        if (ShowYAxis)
        {
            int off = YtoPy(0);
            if ((off > dy) && (off < dy + dh))
                display.drawFastHLine(dx, off, /*dx +*/ dw, GxEPD_DARKGREY); //if should visible, draw x axis
        }
        if (ShowXAxis)
        {
            int off = XtoPx(0);
            if ((off > dx) && (off < dx + dw))
                display.drawFastVLine(off, dy, /*dy +*/ dh, GxEPD_DARKGREY); //if  should visible, draw y axis
        }
#ifdef WIDGETS_SHOW_DEBUG_FRAMES
        ShowBorder = true;
#endif
        if (ShowBorder)
        {
            display.drawFastHLine(dx, dy, /*dx +*/ dw, GxEPD_DARKGREY);
            display.drawFastHLine(dx, dy + dh, /*dx +*/ dw, GxEPD_DARKGREY);
            display.drawFastVLine(dx, dy, /*dy +*/ dh, GxEPD_DARKGREY);
            display.drawFastVLine(dx + dw, dy, /*dy +*/ dh, GxEPD_DARKGREY);
        }
        u8g2.setFont(axisLabelsFont);
        auto cindex = 0;
        for (auto j : Plots) //for each plot
            if (j.Data.size())
            {
                uint16_t LabelXstep = getXLabelsStep(j);
                Serial.println("LabelXstep: " + String(LabelXstep));
                auto last = j.Data.front();
                uint16_t n = 0;
                for (auto i : j.Data) //for each data point od current plot
                {
                    uint16_t x1 = XtoPx(last.first);
                    uint16_t x2 = XtoPx(i.first);
                    uint16_t y1 = YtoPy(last.second);
                    uint16_t y2 = YtoPy(i.second);
                    display.drawLine(x1, y1, x2, y2, ColorTable.at(cindex));
                    display.drawLine(x1, y1 - 1, x2, y2 - 1, ColorTable.at(cindex));
                    display.drawLine(x1, y1 + 1, x2, y2 + 1, ColorTable.at(cindex));

                    last = i;
                    n++;
                }
                u8g2.setForegroundColor(GxEPD_BLACK);
                n = 0;
                for (auto i : j.Data) //for each data point od current plot
                {
                    if (hasYlabels)
                    {
                        uint16_t strw = u8g2.getUTF8Width(j.Labels.at(n).second.c_str()) / 2;
                        uint16_t curx = XtoPx(i.first) - strw;
                        uint16_t cury = YtoPy(i.second) + getFontHeight() - getFontHeight() / 2;
                        display.fillCircle(XtoPx(i.first), YtoPy(i.second), strw + 1, GxEPD_WHITE);
                        //display.drawCircle(XtoPx(i.first), YtoPy(i.second), strw + 2, GxEPD_BLACK);
                        u8g2.setCursor(curx, cury + 1);
                        u8g2.print(j.Labels.at(n).second);
                    }
                    if (hasXlabels)
                    {
                        if (!(n % LabelXstep))
                        {
                            uint16_t curx = XtoPx(i.first) - u8g2.getUTF8Width(j.Labels.at(n).first.c_str()) / 2;
                            uint16_t cury = dy + dh + getFontHeight();
                            // Serial.println("x,y: " + String(curx)+", "+String(cury));
                            // Serial.println("["+j.Labels.at(n).first+"]");
                            u8g2.setCursor(curx, cury + 2);
                            u8g2.print(j.Labels.at(n).first);
                        }
                    }
                    n++;
                }
                cindex = ++cindex % ColorTable.size(); //iteration over each color for each plot
            }
    };
    void addPlot()
    {
        DataPoints p;
        Plots.push_back(std::move(p));
    }
    void addPoint(double x, double y, int index)
    {
        Plots.at(index).Data.push_back(std::make_pair(x, y));
    };
    void addPoint(double x, double y, String XLabel = "", String YLabel = "", int index = 0)
    {
        Plots.at(index).Data.push_back(std::make_pair(x, y));
        Plots.at(index).Labels.push_back(std::make_pair(XLabel, YLabel));
        if (XLabel != "")
        {
            hasXlabels = true;
            Plots.at(index).Xlabels = true;
        }
        if (YLabel != "")
        {
            Plots.at(index).Ylabels = true;
            hasYlabels = true;
        }
    };
    void clear()
    {
        Plots.clear();
        hasXlabels = false;
        hasYlabels = false;
    }
    void clear(int index)
    {
        Plots.at(index).Data.clear();
        Plots.at(index).Labels.clear();
    }
    void setData(std::vector<double> &dt, int index = 0)
    {
        int j = 0;
        for (auto i : dt)
            addPoint(j++, i, index);
    }
    void setAxisLabelsFont(const uint8_t *font)
    {
        axisLabelsFont = font;
    }
};