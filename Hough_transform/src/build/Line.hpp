#pragma once

class Line{
private:
    double m;
    double c;
    double t;
    double r;
public:
    Line(double, double, double, double);
    double getGradient();
    double getYintercept();
    double getTheta();
    double getRho();
};

class LineFactory{
    Line* createLineFromLinearSpace(double, double);
    Line* createLineFromRotationalSpace(double, double);
};