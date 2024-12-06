#include "../build/lib/csv/Csv.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

#define FILENAME "temp.csv"

int main()
{
    if(fs::exists(FILENAME))
    {
        fs::remove(FILENAME);
    }

    ofstream writingFile(FILENAME);
    writingFile << "firstCol,secondCol,thirdCol" << endl;
    writingFile << "bruh,moment,fr" << endl;
    writingFile << "og,og,og" << endl;
    writingFile.close();

    ifstream readingFile(FILENAME);
    string line;
    while(getline(readingFile, line))
    {
        cout << line << endl;
    }
    readingFile.close();

    auto headers = Csv::getHeaders(FILENAME);
    Csv csv(FILENAME);

    cout << endl << endl << "headers" << endl;;
    for(auto header : headers)
    {
        cout << header << ",";
    }
    cout << endl << "number of headers: " << headers.size();
    cout << endl << "number of lines: " << csv.count() << endl;
    cout << endl << "data" << endl;
    for(auto row : csv)
    {
        for(auto header : headers)
        {
            cout << row[header] << ",";
        }
        cout << endl;
    }
    cout << "---===+++===---" << endl;
    fs::remove(FILENAME);
}
