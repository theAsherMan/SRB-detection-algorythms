#pragma once

#include <CCfits/CCfits>
#include <string>
#include <filesystem>

#include "../visualSpace/VisualSpace.hpp"

using namespace std;
using namespace CCfits;

namespace fs = filesystem;


class FitsWriter
{
    private:
        fs::path path;
        std::unique_ptr<FITS> fitsObj;
    public:
        FitsWriter(fs::path filePath);
        //~FitsWriter();

        //void addKey(string name, double value, string description);

        void writeData(VisualSpace* data);
};