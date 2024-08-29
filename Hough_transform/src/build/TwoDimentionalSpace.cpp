#include "TwoDimentionalSpace.hpp"

ITwoDimentionalSpace::ITwoDimentionalSpace(int startingX, int startingY, TwoDimentionalSpace* space)
{
    this->width = space->getWidth();
    this->height = space->getHieght();
    this->space = space;

    this->x = startingX;
    this->y = startingY;
}

TwoDimentionalSpaceLocation ITwoDimentionalSpace::operator*()
{
    return {this->x, this->y, this->space->get(this->x, this->y)};
}

void ITwoDimentionalSpace::operator++()
{
    if(x == this->width - 1)
    {
        this->x = 0;
        this->y++;
    }else{
        this->x++;
    }
}

bool ITwoDimentionalSpace::operator!=(ITwoDimentionalSpace other)
{
    if(this->x != other.x)
    {
        return true;
    }
    if(this->y != other.y)
    {
        return true;
    }
    if(this->space != other.space)
    {
        return true;
    }
    return false;
}

bool ITwoDimentionalSpace::operator==(ITwoDimentionalSpace other)
{
    return !(*this != other);
}

// =============================================================================

TwoDimentionalSpace::TwoDimentionalSpace(int width, int height)
{

    this->width = width;
    this->height = height;

    this->array = new long*[width];
    for(int ii=0; ii<width; ii++)
    {
        this->array[ii] = new long[height];
        for(int jj=0; jj<height; jj++)
        {
            this->array[ii][jj] = 0;
        }
    }
}

TwoDimentionalSpace::~TwoDimentionalSpace()
{
    for(int ii=0; ii<this->width; ii++)
    {
        delete(this->array[ii]);
    }
    delete(this->array);
}

long TwoDimentionalSpace::get(int x, int y)
{
    return this->array[x][y];
}

void TwoDimentionalSpace::set(int x, int y, long value)
{
    this->array[x][y] = value;
}

int TwoDimentionalSpace::getWidth()
{
    return width;
}

int TwoDimentionalSpace::getHieght()
{
    return height;
}

ITwoDimentionalSpace TwoDimentionalSpace::begin()
{
    return ITwoDimentionalSpace(0, 0, this);
}

ITwoDimentionalSpace TwoDimentionalSpace::end()
{
    return ITwoDimentionalSpace(0, this->height, this);
}

void TwoDimentionalSpace::print()
{
    int symbolLength = 1;
    for(auto point : *this)
    {
        if(point.value)
        {
            while(point.value/(10*symbolLength))
            {
                symbolLength++;
            }
        }
    }
    for(int ii=0; ii<width; ii++)
    {
        for(int jj=0; jj<height; jj++)
        {
            if(array[ii][jj])
            {
                std::cout << std::Tostring(array[ii][jj]);
            }
            else
            {
                for(int _=0; _<symbolLength; _++)
                {
                    std::cout << "_";
                }
            }
            if(jj != height - 1)
            {
                std::cout << " :: ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "space is of width " << width << " and hieght " << height << std::endl;
}