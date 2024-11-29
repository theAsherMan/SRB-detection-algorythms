#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>

#include "../build/lib/linearTransformer/LinearHoughTransformer.hpp"
#include "../build/lib/VisualSpace/VisualSpace.hpp"

using namespace std;

void addNoise(VisualSpace*);
HoughLineDescriptor createRandomLine();
void addLine(VisualSpace* imageSpace, vector<HoughLineDescriptor>*, HoughLineDescriptor);
void outPrint(string);
void display(string);

VisualSpace* imageSpace;

#define MAX_THETA (M_PI)
#define MIN_THETA (0.5*M_PI)

#define MAX_RHO (100)
#define MIN_RHO (0)

int main(int charc, char** argv)
{
    int width = 1000;
    int height = 1000;

    srand(time(NULL));

//  ------------------------------------------------------------------------

    imageSpace = new VisualSpace(width, height);

//  ------------------------------------------------------------------------

    string input = "none";

    string addLineautoOption = "addRandomLine";
    string addLineManual = "addMyLine";
    string addNoiseOption = "addNoise";
    string preformTransformOption = "preformTransform";
    string compareLinesOption = "compareLines";
    string showHoughSpaceOption = "showHoughSpace";
    string showImageSpaceOption = "showImageSpace";

    auto lines = vector<HoughLineDescriptor>();
    bool compareLines = true;
    bool showHoughSpace = true;

    remove("..\\log.txt");

    while(input != "exit")
    {
        if(input == showImageSpaceOption)
        {
            outPrint("imageSpace");
            outPrint(imageSpace->toString());
        }
        outPrint("space contains lines:");
        for(auto line : lines)
        {
            outPrint(line.toString());
        }
        outPrint("please select option");
        outPrint(addNoiseOption);
        outPrint(addLineautoOption);
        outPrint(addLineManual);
        outPrint(preformTransformOption);
        outPrint(compareLinesOption);
        outPrint(showHoughSpaceOption);
        outPrint(showImageSpaceOption);
        outPrint("");

        cin >> input;

        if(input == addNoiseOption){addNoise(imageSpace);}
        if(input == addLineautoOption){
            addLine(imageSpace, &lines, createRandomLine());
            outPrint("space contains lines");
            for(int ii=0; ii<lines.size(); ii++)
            {
                outPrint(lines.at(ii).toString());
            }
        }
        if(input == addLineManual)
        {
            double theta;
            double rho;
            outPrint("");
            outPrint("input theta value");
            cin >> theta;
            if(theta > MAX_THETA || theta < MIN_THETA){
                outPrint("invalid theta");
                continue;
            }
            outPrint("");
            outPrint("input rho");
            cin >> rho;
            if(rho > MAX_RHO || rho < MIN_RHO)
            {
                outPrint("invalid rho");
                continue;
            }
            addLine(imageSpace, &lines, HoughLineDescriptor(theta, rho, 10));
        }
        if(input == preformTransformOption)
        {
            auto transformer = LinearHoughTransformer(0.5*M_PI, M_PI, 100, 5, imageSpace, 1, 1, 2);
            
            outPrint("starting transform");
            auto detectedLines = transformer.getLinesOfHighestBrightness(1);
            outPrint("transform complete");
            if(showHoughSpace)
            {
                outPrint("resulting hough space");
                outPrint(transformer.showHoughSpace());
            }
            if(compareLines)
            {
                outPrint("predicted lines");
                auto predictions = VisualSpace(width, height);
                for(auto line : detectedLines)
                {
                    addLine(&predictions, NULL, line);
                }
                outPrint(predictions.toString());
                outPrint("actual lines");
                outPrint(imageSpace->toString());
            }
            outPrint("detected lines");
            for(auto line : detectedLines)
            {
                outPrint(line.toString());
            }
            outPrint("correct lines");
            for(auto line : lines)
            {
                outPrint(line.toString());
            }
            outPrint("result is predicted to be in range:");
            double low_theta = INFINITY;
            double high_theta = -INFINITY;
            double low_rho = INFINITY;
            double high_rho = -INFINITY;            
            for(auto line : detectedLines)
            {
                if(line.theta < low_theta)
                {
                    low_theta = line.theta;
                }if(line.theta > high_theta)
                {
                    high_theta = line.theta;
                }
                if(line.rho < low_rho)
                {
                    low_rho = line.rho;
                }if(line.rho > high_rho)
                {
                    high_rho = line.rho;
                }
            }

            outPrint("theta: " + to_string(low_theta) + " to " + to_string(high_theta));
            outPrint("rho: " + to_string(low_rho) + " to " + to_string(high_rho));
        }
        if(input == compareLinesOption){
            compareLines = !compareLines;
            outPrint("compare lines set to: "+to_string(compareLines));
        }
        if(input == showHoughSpaceOption){
            showHoughSpace = !showHoughSpace;
            outPrint("show hough space set to: "+to_string(showHoughSpace));
        }
    }


    return 0;
}

void addNoise(VisualSpace* imageSpace)
{
    for(int x=0; x<imageSpace->getWidth(); x++)
    {
        for(int y=0; y<imageSpace->getHieght(); y++)
        {
            imageSpace->point(x,y)->increaseValue((rand()%50)+10);
        }
    }

    cout << "added noise" << endl;
}

HoughLineDescriptor createRandomLine()
{
    double theta_seed = (rand() % 100) / 100.0;
    double theta = (MAX_THETA - MIN_THETA) * theta_seed + MIN_THETA;
    double rho_seed = (rand() % 100) / 100.0;
    double rho = rho_seed * (MAX_RHO - MIN_RHO) + MIN_RHO;

    auto line = HoughLineDescriptor(theta, rho, 60);
    outPrint("new line: " + line.toString());
    return line;
}

void addLine(VisualSpace* imageSpace, vector<HoughLineDescriptor>* lines, HoughLineDescriptor line)
{
    if(lines != NULL)
    {
        lines->push_back(line);
    }

    int x = max(0.0, line.rho/cos(line.theta));
    int y = max(0.0, line.rho/sin(line.theta));

    double cosTheta = cos(line.theta);
    double sinTheta = sin(line.theta);

    bool paint = true;

    while(x < imageSpace->getWidth() && x >= 0 && y < imageSpace->getHieght() && y >= 0)
    {
        if(paint){
            imageSpace->point(x, y)->increaseValue(line.brightness);
        }
        paint =true;

        int y_true = round((line.rho - x*cosTheta)/sinTheta);
        if(y_true < y)
        {
            y--;
        }else if(y_true > y){
            y++;                
        }else{
            x++;
            y_true = round((line.rho - x*cosTheta)/sinTheta);
            if(y_true < y)
            {
                y--;
            }else if(y_true > y)
            {
                y++;
            }
        }
    }
}

void outPrint(string message)
{
        ofstream writer("../log.txt", ios::app);
        cout << message << endl;
        writer << message << endl;
        writer.close();
}

void display(string message)
{
    ofstream writer("../log.txt", ios::app);
    cout << message;
    writer << message;
    writer.close();
}