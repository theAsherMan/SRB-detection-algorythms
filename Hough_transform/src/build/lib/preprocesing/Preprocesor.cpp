#include "Preprocesor.hpp"

Preprocesor::Preprocesor(VisualSpace *raw, bool delete_raw_when_deleted)
{
    this->raw = raw;
    this->processed = NULL;
    this->delete_original_space_when_deleted = delete_raw_when_deleted;
}

Preprocesor::~Preprocesor()
{
    if (processed != NULL)
    {
        delete (processed);
    }
    if (delete_original_space_when_deleted)
    {
        delete (raw);
    }
}

VisualSpace *Preprocesor::data()
{
    if (processed == NULL)
    {
        this->preprocess();
    }
    return processed;
}

void Preprocesor::preprocess()
{
    processed = new VisualSpace(raw->getWidth(), raw->getHieght());

    transferFromRawToProcessedWhileSubtractingMean();

    processed->zeroAllNegitiveValues();

    processed->standardize();

    processed->standardizeIgnoringNonPositives();

    processed->zeroAllNegitiveValues();
}

void Preprocesor::transferFromRawToProcessedWhileSubtractingMean()
{
    double mean = raw->mean();
    for (int ii = 0; ii < raw->getWidth(); ii++)
    {
        for (int jj = 0; jj < raw->getHieght(); jj++)
        {
            processed->point(ii, jj)->setValueUnsafe(raw->point(ii, jj)->getValue() - mean);
        }
    }
}