#include "FrbDrawer.hpp"


FrbDrawer::FrbDrawer(VisualSpace* image, double min_freq, double max_freq, double time_step)
{
    this->image = image;
    this->max_freq = min_freq;
    this->max_freq = max_freq;
    this->time_step = time_step;

    this->frbs = vector<FrbDrawer::Frb>();
}

vector<FrbDrawer::Frb> FrbDrawer::getFrbs()
{
    return frbs;
}

FrbDrawer* FrbDrawer::draw(double dispersion_measure, int start_x, double brightness)
{
    frbs.push_back(Frb{
        /*isDrawn = */false,
        /*dm = */dispersion_measure,
        /*start_time = */start_x,
        /*brightness = */brightness
    });
    return this;
}

VisualSpace* FrbDrawer::data()
{
    for(auto frb : frbs)
    {
        if(!frb.isDrawn)
        {
            drawFRB(frb.dm, frb.start_time, frb.brightness);
            frb.isDrawn = true;
        }
    }
    return image;
}

void FrbDrawer::drawFRB(double dispersion_measure, int start_x, double brightness)
{
    int y = image->getHieght() - 1;
    int x = start_x;

    while(y >= 0 && x < image->getWidth())
    {
        double d_x = calculateTimeDelay(y, image->getHieght(), dispersion_measure);
        while(double(x) <= round(start_x + d_x))
        {
            if(x >= image->getWidth()) break;
            image->point(x,y)->increaseValue(brightness);
            x++;
        }
        if(x >= image->getWidth()) break;
        x--;
        y--;
        d_x = calculateTimeDelay(y, image->getHieght(), dispersion_measure);
        while(double(x) == round(start_x + d_x))
        {
            if(y < 0) break;
            image->point(x,y)->increaseValue(1);
            y--;
            d_x = calculateTimeDelay(y, image->getHieght(), dispersion_measure);
        }
    }
}

double FrbDrawer::calculateTimeDelay(int y, int height, double dispersion_measure)
{
    double frequencyStep = (max_freq - min_freq)/height;
    double curr_frequency = max_freq - ((height - y + 1) * frequencyStep);
    double timeDelay = K_DM*dispersion_measure*((1/(curr_frequency*curr_frequency))-(1/(max_freq*max_freq)));
    double d_x = timeDelay / time_step;
    return d_x;
}