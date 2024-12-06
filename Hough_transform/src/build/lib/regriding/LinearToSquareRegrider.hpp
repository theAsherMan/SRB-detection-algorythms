#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "../VisualSpace/VisualSpace.hpp"
#include "../includes/Constants.hpp"

class LinearToSquareRegrider
{
private:
    VisualSpace *originalSpace;
    VisualSpace *regridedSpace;
    int regrid_value;
    float y_max;
    float y_min;
    bool delete_original_space_on_finish;

public:
    LinearToSquareRegrider(VisualSpace *source_space, int regrid_value, float y_min, float y_max, bool delete_original_space_when_deleted);
    ~LinearToSquareRegrider();
    VisualSpace *data();
    double dmToTheta(double dm, int number_of_frequencies, double time_step);
    double thetaToDm(double theta, int number_of_frequencies, double time_step);

    private:
        void regrid_space();
        VisualSpace* generateExpandedImage(VisualSpace* orig);
        VisualSpace* preformRegridingAcrossTempArrays(VisualSpace*);
        void projectTempArrayOntoRegridedSpace(VisualSpace*);
        int calculateRegridedY(int, double);
};