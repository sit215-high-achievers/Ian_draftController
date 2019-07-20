#include "fuzzyFunction.h"

//Triangle function constructor
FuzzyTriangle::FuzzyTriangle()
{
    left = 0;
    right = 0;
    middle = 0;
}

//Sets left, right, and middle x-values for triangle points
void FuzzyTriangle::setPoints(double l, double r, double m)
{
    left = l;
    right = r;
    middle = m;
}

double FuzzyTriangle::getValue(double v)
{
    //Membership is zero if value is lower than the left side of triangle
    if (v < left)
        return 0;
    //Calculate membership if point is between left and middle point
    else if (v < middle)
        return (v - left) / (middle - left);
    //If value is equal to middle point, membership is 1
    else if (v == middle)
        return 1;
    //Calculate membership if point is between middle and right point
    else if (v < right)
        return (right - v) / (right - middle);
    //Value is higher than right hand point, membership is zero
    else
        return 0;
}

//Trapezoid function constructor
FuzzyTrapezoid::FuzzyTrapezoid()
{
    left = 0;
    right = 0;
    lMiddle = 0;
    rMiddle = 0;
}

//Sets left, right, middle-left, and middle-right x-values for trapezoid points
void FuzzyTrapezoid::setPoints(double l, double r, double lMid, double rMid)
{
    left = l;
    right = r;
    lMiddle = lMid;
    rMiddle = rMid;
}

double FuzzyTrapezoid::getValue(double v)
{
    //Membership is zero if value is lower than the left side of trapezoid
    if (v < left)
        return 0;
    //Calculate membership if point is between left and middle-left point
    else if (v < lMiddle)
        return (v - left) / (lMiddle - left);
    //If the value is between the two middle points, membership is 1
    else if (v >= lMiddle && v <= rMiddle)
        return 1;
    //Calculate membership if point is between middle-right and right point
    else if (v < right)
        return (right - v) / (right - rMiddle);
    //Value is higher than right hand point, membership is zero
    else
        return 0;
}
