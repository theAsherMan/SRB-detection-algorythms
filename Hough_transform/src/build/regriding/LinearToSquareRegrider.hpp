#pragma once

#include <math.h>

#include "../VisualSpace/VisualSpace.hpp"

class LinearToSquareRegrider
{
    private:
        VisualSpace* originalSpace;
        VisualSpace* regridedSpace;
        int regrid_value;
        float y_max;
        float y_min;
        bool delete_original_space_on_finish;
    
    public:
        LinearToSquareRegrider(VisualSpace*, int, float, float, bool);
        ~LinearToSquareRegrider();
        VisualSpace* data();

    private:
        void regrid_space();
        VisualSpace* generateExpandedImage(int, int);
        VisualSpace* preformRegridingAcrossTempArrays(VisualSpace*);
        void projectTempArrayOntoRegridedSpace(VisualSpace*);
        int calculateRegridedY(int, double);
};