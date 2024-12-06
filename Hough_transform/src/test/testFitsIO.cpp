
#include <CCfits/CCfits>
#include <iostream>
#include <filesystem>
#include <valarray>

#include "../build/lib/visualSpace/VisualSpace.hpp"
#include "../build/lib/fitsWriter/FitsWriter.hpp"

#define WIDTH 10
#define HEIGHT 15
#define FILENAME "./temp.fits"

using namespace std;
using namespace CCfits;

void initualiseData(VisualSpace* data);
void saveData(VisualSpace* data);
VisualSpace* recoverData();

int main()
{
    auto data1 = new VisualSpace(WIDTH, HEIGHT);
    initualiseData(data1);
    cout << data1->toString() << endl;
    cout << "======================" << endl << endl;
    saveData(data1);
    cout << "======================" << endl;
    auto data2 = recoverData();
    cout << data2->toString() << endl;
    //filesystem::remove(FILENAME);
    cout << "---===+++===---" << endl;
}

void initualiseData(VisualSpace* data)
{
    for(int ii=0; ii<data->getWidth(); ii++)
    {
        for(int jj=0; jj<data->getHieght(); jj++)
        {
            data->point(ii, jj)->setValue(ii+jj*WIDTH);
        }
    }
}

void saveData(VisualSpace* data)
{
    FitsWriter(FILENAME).writeData(data);
}

VisualSpace* recoverData()
{
    auto fits_file = FITS(FILENAME, Read, true);
    PHDU& pHDU = fits_file.pHDU();

    int width = pHDU.axis(0);
    int height = pHDU.axis(1);
    int count = width * height;

    auto raw = valarray<double>(count);
    pHDU.read(raw);
    return new VisualSpace(width, height, &raw);
}