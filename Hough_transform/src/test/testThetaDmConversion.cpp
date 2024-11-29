#include <time.h>
#include <iostream>

#include "../build/lib/regriding/LinearToSquareRegrider.hpp"
#include "../build/lib/visualSpace/VisualSpace.hpp"


int main()
{
    auto regridder = LinearToSquareRegrider(new VisualSpace(5,5), 10, 13.5, 15.2, true);
    double dm_1 = 50;
    auto theta = regridder.dmToTheta(dm_1, 500, 5);
    auto dm_2 = regridder.thetaToDm(theta, 500, 5);

    cout << "original dm: " << dm_1 << endl;
    cout << "converts to theta: " << theta << endl;
    cout << "converts back to: " << dm_2 << endl;
}