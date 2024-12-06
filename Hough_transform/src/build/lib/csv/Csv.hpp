#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <climits>

using namespace std;

namespace fs = filesystem;

class CsvTuple;

class Csv{
    private:
        ifstream file;
        fs::path path;
        vector<CsvTuple> data;

        vector<string> columnHeaders;

        class iterator;
    public:
        Csv(fs::path path);
        ~Csv();
        int count();
        CsvTuple operator[](int idx);
        bool isFileOpen();
        static vector<string> getHeaders(fs::path path);

        
        iterator begin();
        iterator end();
        iterator iterateFrom(int idx);
        //mutators

    
    private:
        void readFile();
        void readFileUpToIdx(int idx);
        class iterator{
            public:
                int idx;
                vector<CsvTuple>* data;
                iterator(int start_idx, vector<CsvTuple>* data);
                iterator operator++();
                bool operator!=(iterator other);
                CsvTuple operator*();
        };

};

class CsvTuple {
    private:
        map<string, string> elements;

    public:
        CsvTuple();
        void addElement(string key, string value);
        string operator[](string name);
        //mutators
};