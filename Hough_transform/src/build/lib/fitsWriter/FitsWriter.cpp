#include "FitsWriter.hpp"

FitsWriter::FitsWriter(fs::path filePath)
{
    this->path = filePath;
    this->fitsObj = std::unique_ptr<FITS>(nullptr);
}

void FitsWriter::writeData(VisualSpace* data)
{
    if(fs::exists(path))
    {
        fs::remove(path);
    }
    fs::create_directories(path.parent_path());
    long naxes[2] = {data->getWidth(), data->getHieght()};
    fitsObj.reset(new FITS(path, DOUBLE_IMG, 2, naxes));
    int elementCount = data->count();
    fitsObj->pHDU().write(1, elementCount, data->raw());
}