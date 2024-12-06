#include "preprocesor.hpp"

Preprocesor::Preprocesor(VisualSpace* raw, bool delete_raw_when_deleted)
{
    this->raw = raw;
    this->processed = NULL;
    this->delete_original_space_when_deleted = delete_raw_when_deleted;
}

Preprocesor::~Preprocesor()
{
    if(processed != NULL)
    {
        delete(processed);
    }
    if(delete_original_space_when_deleted)
    {
        delete(raw);
    }
}

VisualSpace* Preprocesor::data()
{
    if(processed == NULL)
    {
        this->preprocess();
    }
    return processed;
}

void Preprocesor::preprocess()
{
    //FitsWriter(fs::path("./beforeProcessing.fits")).writeData(raw);
    processed = new VisualSpace(raw->getWidth(), raw->getHieght());

    transferFromRawToProcessedWhileSubtractingMean();
    //FitsWriter(fs::path("./afterTransfer.fits")).writeData(processed);

    processed->zeroAllNegitiveValues();
    //FitsWriter(fs::path("./afterfirstzeroing.fits")).writeData(processed);

    processed->standardize();
    //FitsWriter(fs::path("./afterStandardization.fits")).writeData(processed);

    processed->standardizeIgnoringNonPositives();
    //FitsWriter(fs::path("./afterSpecializedStandadization.fits")).writeData(processed);

    processed->zeroAllNegitiveValues();
    //FitsWriter(fs::path("./finalProduct.fits")).writeData(processed);
}

void Preprocesor::transferFromRawToProcessedWhileSubtractingMean()
{
    double mean = raw->mean();
    for(int ii=0; ii<raw->getWidth(); ii++)
    {
        for(int jj=0; jj<raw->getHieght(); jj++)
        {
            processed->point(ii, jj)->setValueUnsafe(raw->point(ii, jj)->getValue() - mean);
        }
    }
    processed->setAsDirty();
}