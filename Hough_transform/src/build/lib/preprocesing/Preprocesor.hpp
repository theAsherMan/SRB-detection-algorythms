#pragma once

#include "../visualSpace/VisualSpace.hpp"
#include "../fitsWriter/FitsWriter.hpp"

class Preprocesor{
    private:
        VisualSpace* raw;
        VisualSpace* processed;
        bool delete_original_space_when_deleted;

    public:
        Preprocesor(VisualSpace* raw, bool delete_original_space_when_deleted);
        ~Preprocesor();
        VisualSpace* data();
    
    private:
        void preprocess();
        void transferFromRawToProcessedWhileSubtractingMean();
};