#include "LinearToSquareRegrider.hpp"

#include <iostream>

LinearToSquareRegrider::LinearToSquareRegrider(VisualSpace* originalSpace, int regrid_value, float y_min, float y_max, bool delete_original_space_when_deleted)
{
    this->originalSpace = originalSpace;
    this->regridedSpace = NULL;
    this->regrid_value = regrid_value;
    this->y_max = y_max;
    this->y_min = y_min;
    this->delete_original_space_on_finish = delete_original_space_when_deleted;
}

VisualSpace* LinearToSquareRegrider::data()
{
    if(regridedSpace != NULL)
    {
        return regridedSpace;
    }
    regrid_space();
    return regridedSpace;
}

double LinearToSquareRegrider::dmToTheta(double dm, int number_of_frequencies, double time_step)
{
    double theta;
    theta = atan((number_of_frequencies*time_step)/(dm*K_DM*(pow(y_min, -2) - pow(y_max, -2)))) - M_PI/2;
    return theta;
}
double LinearToSquareRegrider::thetaToDm(double theta, int number_of_frequencies, double time_step)
{
    double dm;
    dm = (number_of_frequencies*time_step)/(tan(theta + M_PI/2) * K_DM * (pow(y_min,-2) - pow(y_max,-2)));
    return dm;
}

LinearToSquareRegrider::~LinearToSquareRegrider()
{
    if(delete_original_space_on_finish)
    {
        delete this->originalSpace;
    }
    if(regridedSpace != NULL)
    {
        delete regridedSpace;
    }
}

void LinearToSquareRegrider::regrid_space()
{
    int width = originalSpace->getWidth() * regrid_value;
    int height = originalSpace->getHieght() * regrid_value;

    VisualSpace* temp1 = generateExpandedImage(width, height);
    VisualSpace* temp2 = preformRegridingAcrossTempArrays(temp1);
    delete temp1;
    projectTempArrayOntoRegridedSpace(temp2);
    delete temp2;
}

VisualSpace* LinearToSquareRegrider::generateExpandedImage(int width, int height)
{
    VisualSpace* temp = new VisualSpace(width, height);
    for(int row = 0; row < originalSpace->getHieght(); row++)
    {
        for(int col = 0; col < originalSpace->getWidth(); col++)
        {
            double value = originalSpace->point(col, row)->getValue() / (regrid_value*regrid_value);
            for(int ii=row*regrid_value; ii<row*regrid_value+regrid_value; ii++)
            {
                for(int jj=col*regrid_value; jj<col*regrid_value + regrid_value; jj++)
                {
                    temp->point(jj, ii)->setValueUnsafe(value);
                }
            }
        }
    }
    return temp;
}

VisualSpace* LinearToSquareRegrider::preformRegridingAcrossTempArrays(VisualSpace* temp1)
{
    int y_range = temp1->getHieght();
    int x_range = temp1->getWidth();
    VisualSpace* temp2 = new VisualSpace(x_range, y_range);

    double y_step = (y_max - y_min)/y_range;

    for(int row = 0; row < y_range; row++)
    {
        int actual_y = y_min + row*y_step;
        int regrided_y = calculateRegridedY(actual_y, y_range-1);
        for(int col = 0; col < x_range; col++)
        {
            float value = temp1->point(col, row)->getValue();
            temp2->point(col, regrided_y)->setValueUnsafe(value);
        }
    }
    return temp2;
}

void LinearToSquareRegrider::projectTempArrayOntoRegridedSpace(VisualSpace* temp)
{
    this->regridedSpace = new VisualSpace(originalSpace->getWidth(), originalSpace->getHieght());
    for(int col = 0; col < this->regridedSpace->getWidth(); col++)
    {
        for(int row = 0; row < this->regridedSpace->getHieght(); row++)
        {
            for(int ii = col*regrid_value; ii < col*regrid_value+regrid_value; ii++)
            {
                for(int jj = row*regrid_value; jj < row*regrid_value+regrid_value; jj++)
                {
                    regridedSpace->point(col, row)->setValueUnsafe(temp->point(ii, jj)->getValue());
                }
            }
        }
    }
}


int LinearToSquareRegrider::calculateRegridedY(int row, double scale)
{
    double row_in = double(row);
    double y_max_component = 1/(y_max * y_max);
    double temp = (
        (
            1/(row_in*row_in)
        ) - (
            y_max_component
        )
    ) / (
        (
            1/(y_min*y_min)
        ) - (
            y_max_component
        )
    );
    int result = int(round(temp*scale));
    return result;
}