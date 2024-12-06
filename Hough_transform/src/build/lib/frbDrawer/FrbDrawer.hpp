#pragma once

#include "../visualSpace/VisualSpace.hpp"
#include "../includes/Constants.hpp"

#include <vector>

using namespace std;

class FrbDrawer
{
    private:
        struct Frb{
            bool isDrawn;
            double dm;
            int start_time;
            double brightness;
        };
    private:
        VisualSpace* image;
        double min_freq;
        double max_freq;
        double time_step;
        vector<Frb> frbs;

    public:
        FrbDrawer(VisualSpace* image, double min_freq, double max_freq, double time_step);
        vector<Frb> getFrbs();
        FrbDrawer* draw(double dm, int start_time, double brightness);
        VisualSpace* data();
    
    private:
        void drawFRB(double dispersion_measure, int start_x, double brightness);
        double calculateTimeDelay(int y, int height, double dispersion_measure);
};