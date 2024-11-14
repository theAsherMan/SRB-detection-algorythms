#include <iostream>
#include <time.h>
#include <math.h>

#include "../build/lib/regriding/LinearToSquareRegrider.hpp"
#include "../build/lib/visualSpace/VisualSpace.hpp"

using namespace std;

const double k_dm = 4.148 * 1000;

void drawDM(VisualSpace*, double, int, int, int, double, double, double);
double calculateTimeDelay(int, int, double, double, double, double);

int main(int charc, char** argv)
{
    srand(time(NULL));

    int width = 100;
    int height = 100;

    double f_min = 199.04 * 1000000;
    double f_max = 229.76 * 1000000;

    double dm_max = 1500;
    double dm_min = 1200;

    VisualSpace* image = new VisualSpace(width, height);

    double dispersion_measure = (rand() % int(dm_max - dm_min)) + dm_min;
    //int start_x = rand() % (width/2);
    int start_x = 0;
    drawDM(image, dispersion_measure, start_x, width, height, f_min, f_max, 1);

    cout << "image space" << endl;
    cout << image->toString("mark") << endl;

    LinearToSquareRegrider regridder = LinearToSquareRegrider(image, 1, f_min, f_max, false);

    regridder.data();
    cout << "regriddedSpace" << endl;
    cout << regridder.data()->toString("mark") << endl;

    return 1;
}

void drawDM(VisualSpace* image, double dispersion_mesaure, int start_x, int width, int height, double min_frequency, double max_frequency, double time_step)
{
    int y = height - 1;
    int x = start_x;

    while(y >= 0 && x < width)
    {
        double d_x = calculateTimeDelay(y, height, max_frequency, min_frequency, dispersion_mesaure, time_step);
        while(double(x) <= round(start_x + d_x))
        {
            if(x >= width) break;
            image->point(x,y)->increaseValue(1);
            x++;
        }
        if(x >= width) break;
        x--;
        y--;
        d_x = calculateTimeDelay(y, height, max_frequency, min_frequency, dispersion_mesaure, time_step);
        while(double(x) == round(start_x + d_x))
        {
            if(y < 0) break;
            image->point(x,y)->increaseValue(1);
            y--;
            d_x = calculateTimeDelay(y, height, max_frequency, min_frequency, dispersion_mesaure, time_step);
        }
    }
}

double calculateTimeDelay(int y, int height, double max_frequency, double min_frequency, double dispersion_measure, double time_step)
{
    double frequencyStep = (max_frequency - min_frequency)/height;
    double curr_frequency = max_frequency - ((height - y + 1) * frequencyStep);
    double timeDelay = k_dm*dispersion_measure*((1/(curr_frequency*curr_frequency))-(1/(max_frequency*max_frequency)));
    double d_x = timeDelay / time_step;
    return d_x;
}