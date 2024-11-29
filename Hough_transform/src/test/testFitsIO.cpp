
#include <CCfits/CCfits>
#include <iostream>
#include <filesystem>


using namespace std;
using namespace CCfits;

int main()
{
    string prefix = "./fitsFiles";
    for(const auto & entry : std::filesystem::__cxx11::recursive_directory_iterator(prefix))
    {
        if(entry.is_regular_file() && entry.path().extension() == ".fits")
        {

            string fitsPath = entry.path();
            unique_ptr<FITS> pInfile(new FITS(fitsPath, Read, true));

            PHDU &image = pInfile->pHDU();
            image.readAllKeys();

            valarray<float> contents;

            image.read(contents);

            cout << "=======================================" << endl;
            cout << entry.path() << endl;
            cout<<image<<endl;
        }
    }
}