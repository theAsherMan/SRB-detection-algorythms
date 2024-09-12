#include <iostream>
#include <time.h>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>

#include "../build/LinearHoughTransform.hpp"

using namespace std;
struct LineDescriptor{
    double m;
    double c;
};

void addNoise(vector<vector<long>*>*);
void addLines(vector<vector<long>*>*, vector<LineDescriptor>);
void printSpace(vector<vector<long>*>*);

int main(int charc, char** argv)
{
    int width = 30;
    int height = 30;

    srand(time(NULL));

//  ------------------------------------------------------------------------

    vector<LineDescriptor> lines = vector<LineDescriptor>();
    for(int ii=0; ii<3; ii++)
    {
        LineDescriptor line = LineDescriptor();
        line.m = rand()%10;
        line.c = rand()%10;
        lines.push_back(line);
    }

//  ------------------------------------------------------------------------

    vector<vector<long>*>* imageSpace = new vector<vector<long>*>();
    for(int x=0; x<width; x++)
    {
        imageSpace->push_back(new vector<long>());
        for(int y=0; y<height; y++)
        {
            imageSpace->back()->push_back(0L);
        }
    }

//  ------------------------------------------------------------------------

    string input = "none";

    string addLinesOption = "addLines";
    string addNoiseOption = "addNoise";
    string preformTransformOption = "preformTransform";

    while(input != "exit")
    {
        cout << "imageSpace" << endl;
        printSpace(imageSpace);
        cout << "please select option" << endl;
        cout << addNoiseOption << endl;
        cout << addLinesOption << endl;
        cout << preformTransformOption << endl;

        cin >> input;

        if(input == addNoiseOption){addNoise(imageSpace);}
        if(input == addLinesOption){addLines(imageSpace, lines);}
        if(input == preformTransformOption)
        {
            vector<vector<long>*>* houghSpace = imageSpaceToHoughSpace(imageSpace);
            cout << "houghSpace" << endl;
            printSpace(houghSpace);
            cout << "space contains tranforms for lines" << endl;
            for(LineDescriptor line : lines)
            {
                cout << "m:" << line.m << " c:" << line.c << endl;
            }
        }
    }

    return 0;
}

void addNoise(vector<vector<long>*>* imageSpace)
{
    for(vector<long>* row : *imageSpace)
    {
        for(int y=0; y<row->size(); y++)
        {
            row->at(y) += rand()%5;
        }
    }
}

void addLines(vector<vector<long>*>* imageSpace, vector<LineDescriptor> lines)
{
    
    for(LineDescriptor line : lines)
    {
        vector<vector<long>*>* lineSpace = new vector<vector<long>*>();
        for(int ii=0; ii<imageSpace->size(); ii++)
        {
            lineSpace->push_back(new vector<long>());
            for(int jj=0; jj<imageSpace->at(0)->size(); jj++)
            {
                lineSpace->at(ii)->push_back(0L);
            }
        }
        for(double x = 0.0; x < lineSpace->at(0)->size(); x = nextafter(x, lineSpace->at(0)->size() + 1))
        {
            int y = (int)(line.m*x+line.c);
            if(y >= 0 && y < lineSpace->size())
            {
                if(lineSpace->at(y)->at(x) == 0)
                {
                    lineSpace->at(y)->at(x) += 5;
                }
            }
        }
        for(int y=0; y<imageSpace->size(); y++)
        {
            for(int x=0; x<imageSpace->at(y)->size(); x++)
            {
                imageSpace->at(y)->at(x) += lineSpace->at(y)->at(x);
            }
            delete(imageSpace->at(y));
        }
        delete(imageSpace);
    }
}

void printSpace(vector<vector<long>*>* imageSpace)
{
    int cellwidth = 0;
    for(int y=0; y<imageSpace->size(); y++)
    {
        for(int x=0; x<imageSpace->at(y)->size(); x++)
        {
            stringstream stream = stringstream();
            stream << imageSpace->at(y)->at(x);
            string str_bucket = stream.str();
            cellwidth = max(cellwidth, (int)str_bucket.length());
        }
    }
    for(int y=imageSpace->size()-1; y>=0; y--)
    {
        cout << endl;
        for(int x=0; x < imageSpace->at(y)->size(); x++)
        {
            stringstream stream = stringstream();
            stream << imageSpace->at(y)->at(x);
            string str_bucket = stream.str();
            int stringLength = str_bucket.length();
            int length_diff = cellwidth - stringLength;

            cout << ":";
            for(int ii=0; length_diff/2; ii++)
            {
                cout << " ";
            }
            cout << str_bucket;
            for(int ii=0; length_diff/2; ii++)
            {
                cout << " ";
            }
            if(length_diff%2)
            {
                cout << " ";
            }
            cout << ":";
        }
    }
    cout << endl;
}