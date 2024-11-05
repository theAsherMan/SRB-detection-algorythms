#include <iostream>
#include <time.h>
#include <math.h>

#include "../build/regriding/LinearToSquareRegrider.hpp"
#include "../build/visualSpace/VisualSpace.hpp"

using namespace std;

const double k_dm = 4.148 * 1000;

void drawDM(VisualSpace*, double, int, int, int, double, double, double);
double calculateTimeDelay(int, int, double, double, double, double);

int main(int charc, char** argv)
{
    srand(time(NULL));

    int width = 10;
    int height = 10;
    double min_frequency = 100;
    double max_frequency = 200;

    VisualSpace* image = new VisualSpace(width, height);

    double dispersion_mesaure = (rand() % 100000) / 1000.0;
    //double dispersion_mesaure = 20;
    drawDM(image, dispersion_mesaure, rand() % (width/2), width, height, 100, 200, 1);

    cout << "image space" << endl;
    cout << image->toString("int") << endl;

    LinearToSquareRegrider regridder = LinearToSquareRegrider(image, 2, min_frequency, max_frequency, false);

    regridder.data();
    cout << "regriddedSpace" << endl;
    cout << regridder.data()->toString() << endl;

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
            image->point(x,y)->increaseValue(5);
            x++;
        }
        if(x >= width) break;
        x--;
        y--;
        d_x = calculateTimeDelay(y, height, max_frequency, min_frequency, dispersion_mesaure, time_step);
        while(double(x) == round(start_x + d_x))
        {
            if(y < 0) break;
            image->point(x,y)->increaseValue(5);
            y--;
            d_x = calculateTimeDelay(y, height, max_frequency, min_frequency, dispersion_mesaure, time_step);
        }
    }
}

double calculateTimeDelay(int y, int height, double max_frequency, double min_frequency, double dispersion_mesaure, double time_step)
{
    double frequencyStep = (max_frequency - min_frequency)/height;
    double curr_frequency = max_frequency - ((height - y + 1) * frequencyStep);
    double timeDelay = k_dm*dispersion_mesaure*((1/(curr_frequency*curr_frequency))-(1/(max_frequency*max_frequency)));
    double d_x = timeDelay / time_step;
    return d_x;
}