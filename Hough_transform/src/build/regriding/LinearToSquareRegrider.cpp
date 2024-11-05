#include "LinearToSquareRegrider.hpp"

#include <iostream>

LinearToSquareRegrider::LinearToSquareRegrider(VisualSpace* originalSpace, int regrid_value, float y_min, float y_max, bool delete_original_space_on_finish)
{
    this->originalSpace = originalSpace;
    this->regridedSpace = NULL;
    this->regrid_value = regrid_value;
    this->y_max = y_max;
    this->y_min = y_min;
    this->delete_original_space_on_finish = delete_original_space_on_finish;
}

VisualSpace* LinearToSquareRegrider::data()
{
    cout << "getting data" << endl;
    if(regridedSpace != NULL)
    {
        cout << "regriding already completed.  returning data" << endl;
        return regridedSpace;
    }
    cout << "preforming regriding" << endl;
    regrid_space();
    cout << "returning data" << endl;
    return regridedSpace;
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

    cout << "generating expanded image" << endl;
    VisualSpace* temp1 = generateExpandedImage(width, height);
    cout << temp1->toString("float") << endl;
    cout << "regridding accross temp arrays" << endl;
    VisualSpace* temp2 = preformRegridingAcrossTempArrays(temp1);
    cout << temp2->toString("float") << endl;
    cout << "collapsing image onto regrided space" << endl; 
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
            float value = originalSpace->point(col, row)->getValue() / (regrid_value*regrid_value);
            
            for(int ii=row*regrid_value; ii<row*regrid_value+regrid_value; ii++)
            {
                for(int jj=col*regrid_value; jj<col*regrid_value + regrid_value; jj++)
                {
                    temp->point(jj, ii)->increaseValue(value);
                }
            }
        }
    }
    return temp;
}

VisualSpace* LinearToSquareRegrider::preformRegridingAcrossTempArrays(VisualSpace* temp1)
{
    int y_range = temp1->getWidth();
    int x_range = temp1->getHieght();
    VisualSpace* temp2 = new VisualSpace(x_range, y_range);

    double y_increment = (y_max-y_min)/y_range;

    for(int row = 0; row < y_range; row++)
    {
        double current_y = y_increment * row + y_min;
        for(int col = 0; col < y_range; col++)
        {
            int regrided_y = calculateRegridedY(current_y, y_range);
            if(regrided_y >= y_range){continue;}
            if(regrided_y < 0){continue;}
            float value = temp1->point(col, row)->getValue();
            temp2->point(regrided_y, row)->increaseValue(value);
        }
    }
}

void LinearToSquareRegrider::projectTempArrayOntoRegridedSpace(VisualSpace* temp)
{
    this->regridedSpace = new VisualSpace(originalSpace->getWidth(), originalSpace->getHieght());
    for(int col; col < temp->getWidth(); col++)
    {
        for(int row; row < temp->getHieght(); row++)
        {
            for(int ii = col*regrid_value; ii < col*regrid_value+regrid_value; ii++)
            {
                for(int jj = row*regrid_value; jj < row*regrid_value+regrid_value; jj++)
                {
                    regridedSpace->point(col, row)->increaseValue(temp->point(ii, jj)->getValue());
                }
            }
        }
    }
}

int LinearToSquareRegrider::calculateRegridedY(int current_y, int y_range)
{
    double y_max_component = 1/(y_max * y_max);
    double temp = (
        (
            1/(current_y*current_y)
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

    return int(round(temp * y_range));
}