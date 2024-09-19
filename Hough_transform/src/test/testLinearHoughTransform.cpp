#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>

#include "../build/LinearHoughTransform.hpp"
#include "../build/VisualSpace.hpp"

using namespace std;
struct LineDescriptor{
    double m;
    double c;

    string toString()
    {
        return "y = "+to_string(m)+"x + "+to_string(c);
    }
};

void addNoise(VisualSpace*);
LineDescriptor createRandomLine();
void addLine(VisualSpace* imageSpace, vector<LineDescriptor>*, LineDescriptor);
void outPrint(string);
void display(string);

int main(int charc, char** argv)
{
    int width = 5000;
    int height = 5000;

    srand(time(NULL));

//  ------------------------------------------------------------------------

    VisualSpace* imageSpace = new VisualSpace(width, height);

//  ------------------------------------------------------------------------

    string input = "none";

    string addLineautoOption = "addRandomLine";
    string addLineManual = "addMyLine";
    string addNoiseOption = "addNoise";
    string preformTransformOption = "preformTransform";
    string showAreaAroundLinesOption = "toggleShowArea";

    vector<LineDescriptor> lines = vector<LineDescriptor>();

    remove("..\\log.txt");

    bool showareaaroundlines = false;
    while(input != "exit")
    {
        outPrint("space contains lines:");
        for(LineDescriptor line : lines)
        {
            outPrint(line.toString());
        }
        outPrint("please select option");
        outPrint(addNoiseOption);
        outPrint(addLineautoOption);
        outPrint(addLineManual);
        outPrint(preformTransformOption);
        outPrint(showAreaAroundLinesOption);
        outPrint("");

        cin >> input;

        if(input == addNoiseOption){addNoise(imageSpace);}
        if(input == addLineautoOption){
            addLine(imageSpace, &lines, createRandomLine());
            outPrint("space contains lines");
            for(int ii=0; ii<lines.size(); ii++)
            {
                outPrint("y = " + to_string(lines.at(ii).m) + "x" + "+" + to_string(lines.at(ii).c));
            }
        }
        if(input == addLineManual)
        {
            LineDescriptor line = LineDescriptor();
            outPrint("");
            outPrint("input gradient value");
            cin >> line.m;
            outPrint("");
            outPrint("input y-intercept");
            cin >> line.c;
            addLine(imageSpace, &lines, line);
        }
        if(input == showAreaAroundLinesOption){
            showareaaroundlines = !showareaaroundlines;
            outPrint("show area around lines set to "+to_string(showareaaroundlines));
        }
        if(input == preformTransformOption)
        {
            outPrint("starting transform");
            VisualSpace* houghSpace = imageSpaceToHoughSpace(imageSpace);
            outPrint("transform complete");
            vector<VSPoint> markedPoints = vector<VSPoint>();
            VSPoint highestPoint = VSPoint(0,0,0);
            
            for(int ii=0; ii<lines.size(); ii++)
            {
                highestPoint = VSPoint(0,0,0);
                for(int x = 0; x<houghSpace->getWidth(); x++)
                {
                    for(int y=0; y<houghSpace->getHieght(); y++)
                    {
                        if(houghSpace->point(x,y)->getValue() > highestPoint.getValue())
                        {
                            bool checked = false;
                            for(VSPoint point : markedPoints)
                            {
                                if(x == point.getX() && y == point.getY())
                                {
                                    checked = true;
                                }
                            }
                            if(!checked)
                            {
                                highestPoint = VSPoint(x,y,houghSpace->point(x,y)->getValue());
                            }
                        }
                    }
                }
                VSPoint pointFound = VSPoint(highestPoint.getX(), highestPoint.getY(), highestPoint.getValue());
                markedPoints.push_back(pointFound);
            }

            outPrint("detected lines");
            for(VSPoint line : markedPoints)
            {
                outPrint("y = "+to_string(line.getX())+"x + "+to_string(line.getY()));
            }
            outPrint("correct lines");
            for(LineDescriptor line : lines)
            {
                outPrint("y = " + to_string(line.m) + "x" + "+" + to_string(line.c));
            }

            if(showareaaroundlines)
            {
                outPrint("displaying lines");
                for(VSPoint line : markedPoints)
                {
                    outPrint("y = "+to_string(line.getX())+"x + "+to_string(line.getY()));
                    
                    outPrint("in hough space");
                    int x_low = line.getX() - 10;
                    int y_low = line.getY() - 10;
                    int x_high = line.getX() + 10;
                    int y_high = line.getY() + 10;

                    for(int jj = y_high; jj>=y_low; jj--)
                    {
                        for(int ii=x_low; ii<=x_high; ii++)
                        {
                            display(": ");
                            if(ii == line.getX() && jj == line.getY())
                            {
                                display(">");
                            }
                            if(ii < 0 || ii >= houghSpace->getWidth() || jj < 0 || jj >= houghSpace->getHieght())
                            {
                                display(" ");
                            }else{
                                display(to_string(houghSpace->point(ii,jj)->getValue()));
                            }
                            if(ii == line.getX() && jj == line.getY())
                            {
                                display("<");
                            }
                            display(" :");
                        }
                        outPrint("");
                    }

                    outPrint("in normal space");
                    x_low = 0;
                    x_high = 20;
                    y_low = line.getY() - 10;
                    y_high = line.getY() + 10;

                    for(int jj = y_high; jj>=y_low; jj--)
                    {
                        for(int ii=x_low; ii<=x_high; ii++)
                        {
                            display(": ");
                            if(ii < 0 || ii >= imageSpace->getWidth() || jj < 0 || jj >= houghSpace->getHieght())
                            {
                                display(" ");
                            }else{
                                display(to_string(imageSpace->point(ii,jj)->getValue()));
                            }
                            display(" :");
                        }
                        outPrint("");
                    }
                }
            }
            delete houghSpace;
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

LineDescriptor createRandomLine()
{
    LineDescriptor line = LineDescriptor();
    line.m = rand()%10;
    line.c = rand()%100;
    return line;
}

void addLine(VisualSpace* imageSpace, vector<LineDescriptor>* lines, LineDescriptor line)
{
    lines->push_back(line);
    
    int y = line.c;
    int y_high = y + 1;
    int y_low = y - 1;

    int x=0;
    while(x < imageSpace->getWidth())
    {
        double y_true = line.m * x + line.c;

        double dy_high = abs(y_high - y_true);
        double dy = abs(y - y_true);
        double dy_low = abs(y_low - y_true);

        if(y>imageSpace->getHieght()-1)
        {
            if(line.m >= 0)
            {
                break;
            }
        }else{
            if(y<0)
            {
                if(line.m <= 0)
                {
                    break;
                }
            }else{
                imageSpace->point(x,y)->increaseValue(50);

            }
        }

        if(dy_high < dy)
        {
            y_high++;
            y++;
            y_low++;
        }else{
            if(dy_low < dy)
            {
                y_high--;
                y--;
                y_low--;
            }else{
                x++;
                y_true = line.m * x + line.c;
                dy_high = abs(y_high - y_true);
                dy = abs(y - y_true);
                dy_low = abs(y_low - y_true);
                if(dy_high < dy)
                {
                    y_high++;
                    y++;
                    y_low++;
                }else{
                    if(dy_low < dy)
                    {
                        y_high--;
                        y--;
                        y_low--;
                    }
                }
            }
        }
    }

    cout << "added line" << endl;
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