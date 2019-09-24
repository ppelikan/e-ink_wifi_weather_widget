#pragma once

//#include "math.h"
#include "widget.h"

struct DataPoints
{
    std::vector<std::pair<double, double>> Data;
    ~DataPoints()
    {
        Data.clear();
    }
};

class Plot : public Widget
{
private:
    double maxX = 0, maxY = 0; //real x,y values corresponding to top right corner of plot
    double minX = 0, minY = 0; //real x,y values corresponding to bottom left corner of plot
    int borderX = 5, borderY = 5;
    double scaleX = 1, scaleY = 1;
    const std::vector<uint16> ColorTable{GxEPD_BLACK, GxEPD_RED, GxEPD_DARKGREY, GxEPD_LIGHTGREY, GxEPD_BLACK, GxEPD_RED};
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

public:
    std::vector<DataPoints> Plots;
    bool FitZero = true; //true means plot will be zoomed to fit point 0,0 as well as whole graph
    bool ShowXAxis = true;
    bool ShowYAxis = true;
    bool ShowBorder = true;
    Plot(int x, int y, int w, int h) : Widget(x, y, w, h), borderX(w / 50), borderY(h / 50) { addPlot(); };
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
        if (ShowBorder)
        {
            display.drawFastHLine(dx, dy, /*dx +*/ dw, GxEPD_DARKGREY);
            display.drawFastHLine(dx, dy + dh, /*dx +*/ dw, GxEPD_DARKGREY);
            display.drawFastVLine(dx, dy, /*dy +*/ dh, GxEPD_DARKGREY);
            display.drawFastVLine(dx + dw, dy, /*dy +*/ dh, GxEPD_DARKGREY);
        }
        auto cindex = 0;
        for (auto j : Plots) //for each plot
            if (j.Data.size())
            {

                auto last = j.Data.front();
                for (auto i : j.Data) //for each data point od current plot
                {
                    display.drawLine(XtoPx(last.first),
                                     YtoPy(last.second),
                                     XtoPx(i.first),
                                     YtoPy(i.second),
                                     ColorTable.at(cindex));
                    display.drawLine(XtoPx(last.first),
                                     YtoPy(last.second)+1,
                                     XtoPx(i.first),
                                     YtoPy(i.second)+1,
                                     ColorTable.at(cindex));
                    last = i;
                }
                cindex = ++cindex % ColorTable.size(); //iteration over each color for each plot
            }
    };
    void addPlot()
    {
        DataPoints p;
        Plots.push_back(std::move(p));
    }
    void addPoint(double x, double y, int index = 0)
    {
        Plots.at(index).Data.push_back(std::make_pair(x, y));
    };
    void clear()
    {
        Plots.clear();
    }
    void clear(int index)
    {
        Plots.at(index).Data.clear();
    }
    void setData(std::vector<double> &dt, int index = 0)
    {
        int j = 0;
        for (auto i : dt)
            addPoint(j++, i, index);
    }
};