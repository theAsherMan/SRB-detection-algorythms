#include "Csv.hpp"

Csv::Csv(fs::path path)
{
    this->columnHeaders = getHeaders(path);
    this->data = vector<CsvTuple>();
    this->path = path;
    this->file = ifstream(path);
    
    // skip header row
    string _;
    getline(file, _);
}

Csv::~Csv()
{
    if(file.is_open())
    {
        file.close();
    }
}

vector<string> Csv::getHeaders(fs::path path)
{
    vector<string> headers = vector<string>();
    ifstream file(path);
    if(!file.is_open())
    {
        throw "failed to open file";
    }
    string headerLine;
    getline(file, headerLine);
    stringstream parser(headerLine);
    string header;
    while(getline(parser, header, ','))
    {
        headers.push_back(header);
    }
    file.close();
    return headers;
}

bool Csv::isFileOpen()
{
    return file.is_open();
}

CsvTuple Csv::operator[](int idx)
{
    if(idx >= this->data.size())
    {
        readFileUpToIdx(idx);
    }
    return data[idx];
}

int Csv::count()
{
    readFile();
    return data.size();
}

void Csv::readFile()
{
    readFileUpToIdx(INT_MAX);
}

void Csv::readFileUpToIdx(int idx)
{
    string line;
    stringstream parser;
    string value;

    if(!file.is_open())
    {
        throw "failed to read file";
    }
    while(data.size() <= idx && getline(file, line))
    {
        int ii=0;
        parser = stringstream(line);
        auto row = CsvTuple();
        while(getline(parser, value, ','))
        {
            string key = columnHeaders.at(ii++);
            row.addElement(key, value);
        }
        data.push_back(row);
    }
}

Csv::iterator Csv::begin()
{
    return iterateFrom(0);
}
Csv::iterator Csv::end()
{
    return iterateFrom(count());
}
Csv::iterator Csv::iterateFrom(int idx)
{
    return iterator(idx, &this->data);
}

Csv::iterator::iterator(int idx, vector<CsvTuple>* data)
{
    this->data = data;
    this->idx = idx;
}

// pre increment
Csv::iterator Csv::iterator::operator++()
{
    this->idx++;
    return *this;
}

CsvTuple Csv::iterator::operator*()
{
    return data->at(this->idx);
}

bool Csv::iterator::operator!=(Csv::iterator other)
{
    if(other.data != this->data)
    {
        return false;
    }
    return other.idx != this->idx;
}


CsvTuple::CsvTuple()
{
    this->elements = map<string, string>();
}

void CsvTuple::addElement(string key, string value)
{
    elements.insert({key, value});
}

string CsvTuple::operator[](string key)
{
    return elements[key];
}