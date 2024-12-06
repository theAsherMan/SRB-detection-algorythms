#include "../build/lib/visualSpace/VisualSpace.hpp"
#include "../build/lib/fitsWriter/FitsWriter.hpp"

#include <iostream>
#include <valarray>

#define FACTOR 2
#define OG_WIDTH 15
#define OG_HEIGHT 10

using namespace std;

VisualSpace* generateExpandedImage(VisualSpace* orig)
{
    orig->setAsDirty();
    int width = orig->getWidth() * FACTOR;
    int height = orig->getHieght() * FACTOR;
    double regrid_square = FACTOR * FACTOR;

    VisualSpace* temp = new VisualSpace(width, height);
    for(int x=0; x<width; x++)
    {
        for(int y=0; y<height; y++)
        {
            int orig_x = x / FACTOR;
            int orig_y = y / FACTOR;

            temp->point(x, y)->setValueUnsafe(orig->point(orig_x, orig_y)->getValue());
        }
    }
    return temp;
}

VisualSpace* regrid(VisualSpace* orig)
{
    return generateExpandedImage(orig);
}

int main()
{
    auto orig = VisualSpace(OG_WIDTH, OG_HEIGHT);
    for(int ii=0; ii<OG_WIDTH; ii++)
    {
        for(int jj=0; jj<OG_HEIGHT; jj++)
        {
            orig.point(ii, jj)->setValue(ii + jj*OG_WIDTH);
        }
    }

    cout << orig.toString("int") << endl;

    FitsWriter("./temp.fits").writeData(&orig);


    auto raw_fits = FITS("./fitsFiles/out_genfrb0001/out_genfrb0001.fits", Read, true);
    //auto raw_fits = FITS("./temp.fits", Read, true);
    PHDU& fits = raw_fits.pHDU();
    auto flat_data = valarray<double>(fits.axis(0) * fits.axis(1));
    fits.read(flat_data);
    auto reread = VisualSpace(fits.axis(0), fits.axis(1), &flat_data);

    cout << reread.toString("int") << endl;

    auto regridded = regrid(&reread);

    cout << regridded->toString("int");

    FitsWriter("./before.fits").writeData(&reread);
    FitsWriter("./after.fits").writeData(regridded);

    cout << "original width: " << reread.getWidth() << endl;
    cout << "became: " << regridded->getWidth() << endl;
    cout << "original height: " << reread.getHieght() << endl;
    cout << "became: " << regridded->getHieght() << endl;

    cout << "---===+++===---" << endl;
    fs::remove("./temp.fits");
}