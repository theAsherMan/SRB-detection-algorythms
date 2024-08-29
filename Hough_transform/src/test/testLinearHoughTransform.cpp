#include <iostream>
#include <time.h>

#include "../build/LinearHoughTransform.hpp"
#include "../build/TwoDimentionalSpace.hpp"

struct LineDescriptor{
    double m;
    double c;
};

LineDescriptor* addThreeStraightLines(TwoDimentionalSpace*);
void addNoise(TwoDimentionalSpace*);

int width = 30;
int height = 30;

int main(int argc, char** argv, char** envp)
{

    long** a = new long*[10];
    for(int ii=0; ii<10; ii++)
    {
        a[ii] = new long[10];
        for(int jj=0; jj<10; jj++)
        {
            a[ii][jj] = 100 * ii + jj;
        }
    }

    for(int ii = 0; ii<10; ii++)
    {
        for(int jj=0; jj<10; jj++)
        {
            std::cout << a[ii][jj] << "::";
        }
        std::cout << std::endl;
    }

    srand(time(NULL));

    TwoDimentionalSpace* imageSpace = new TwoDimentionalSpace(width, height);

    LineDescriptor* lines = addThreeStraightLines(imageSpace);

    addNoise(imageSpace);

    auto start = time(NULL);
    std::cout << "started transform" << std::endl;

    TwoDimentionalSpace* houghSpace = ImageSpaceToHoughSpace(width, height, imageSpace);

    auto end = time(NULL);
    std::cout << "transform finnished" << std::endl;

    std::cout << "transformation took:: " << end-start << " seconds" << std::endl;

    std::cout << "transformation converted space:: " << std::endl;

    imageSpace->print();

    std::cout << "into::" << std::endl;

    houghSpace->print();

    return 0;
}

LineDescriptor* addThreeStraightLines(TwoDimentionalSpace* space)
{
    LineDescriptor* lines = new LineDescriptor[3];
    for(int ii=0; ii<3; ii++)
    {
        lines[ii].m = (rand()%10) + 1;
        lines[ii].c = (rand()%10) + 1;
    }

    long*** paintedLines = new long**[3];
    paintedLines[0] = new long*[width];
    for(int ii=0; ii<width; ii++)
    {
        paintedLines[0][ii] = new long[height];
        for(int jj=0; jj<height; jj++)
        {
            paintedLines[0][ii][jj] = 0;
        }
    }

    paintedLines[1] = new long*[width];
    for(int ii=0; ii<width; ii++)
    {
        paintedLines[1][ii] = new long[height];
        for(int jj=0; jj<height; jj++)
        {
            paintedLines[1][ii][jj] = 0;
        }
    }

    paintedLines[2] = new long*[width];
    for(int ii=0; ii<width; ii++)
    {
        paintedLines[2][ii] = new long[height];
        for(int jj=0; jj<height; jj++)
        {
            paintedLines[2][ii][jj] = 0;
        }
    }

    for(TwoDimentionalSpaceLocation point : *space)
    {
        for(int ii=0; ii<3; ii++)
        {
            if(point.y == lines[ii].m * point.x + lines[ii].c)
            {
                paintedLines[ii][point.x][point.y] = 5;
            }
        }
    }

    for(int ii=0; ii<3; ii++)
    {
        for(int x=0; x<width-1; x++)
        {
            bool painting = false;
            for(int y=0; y<height; y++)
            {
                if(paintedLines[ii][x+1][y])
                {
                    break;
                }
                if(paintedLines[ii][x][y])
                {
                    painting = true;
                }
                if(painting)
                {
                    paintedLines[ii][x][y] = 5;
                }
            }
        }
    }

    for(int x=0; x<3; x++)
    {
        for(int ii=0; ii<width; ii++)
        {
            for(int jj=0; jj<height; jj++)
            {
                if(paintedLines[x][ii][jj] == 0)
                {
                    std::cout << "_" << "::";
                }
                else{
                    std::cout << paintedLines[x][ii][jj] << "::";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl << std::endl;
    }

    for(auto point : *space)
    {
        for(int ii=0; ii<3; ii++)
        {
            space->set(point.x, point.y, space->get(point.x, point.y) + paintedLines[ii][point.x][point.y]);
        }
    }

    space->print();
    return lines;
}

void addNoise(TwoDimentionalSpace* space)
{
    for(TwoDimentionalSpaceLocation point : *space)
    {
        space->set(point.x, point.y, space->get(point.x, point.y) + rand()%5);
    }
}