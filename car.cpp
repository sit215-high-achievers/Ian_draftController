#include "car.h"
#include <iostream>
#include <cmath>
#include <vector>

Car::Car(int speedLimit)
{
    //Power, speed, acceleration values for the car
    power = 0;
    speed = 0;
    accel = 0;
    //Control functions
    //Four parameters for trapezoid-shaped functions (left, right, middle-left, middle-right)
    //belowSetSpeedBig and deceleratingBig have extreme negative values for left-hand points to catch values off 
    //the left side of the graph.
    //overSetSpeedBig and acceleratingBig have large positive values for the same reason on the right-hand side.
    //Three parameters for triangular-shaped functions(left, middle, right)
    belowSetSpeedBig.setPoints(-10000, -10, -10000, -20);
    belowSetSpeedSmall.setPoints(-11, 0, -5.5);
    atSetSpeed.setPoints(-1, 1, 0);
    overSetSpeedSmall.setPoints(0, 11, 5.5);
    overSetSpeedBig.setPoints(10, 10000, 20, 10000);

    deceleratingBig.setPoints(-1000, -10, -1000, -20);
    deceleratingSmall.setPoints(-11, 0, -5.5);
    stableChange.setPoints(-1, 1, 0);
    acceleratingSmall.setPoints(0, 11, 5.5);
    acceleratingBig.setPoints(10, 1000, 20, 1000);
}

double
Car::getSpeed(double roadAngle)
{
    //Store current speed to use later to calculate current rate of change
    double prevSpeed = speed;
    //Deduct argument passed from applied engine power
    double effectivePower = power - roadAngle;

    //This function is called 10 times per second, divide effectivePower by 10 before adding to speed
    speed += (effectivePower / 10);
    //Calculate acceleration, timespan between speed updates is 0.1 seconds
    accel = (speed - prevSpeed) / 0.1;

    return speed;
}

void Car::setPower(int speedLimit)
{
    //Get memberships for each function
    double belowBig, belowSmall, at, overSmall, overBig, decelBig, decelSmall, stable, accelSmall, accelBig;
    belowBig = belowSetSpeedBig.getValue(speed - speedLimit);
    belowSmall = belowSetSpeedSmall.getValue(speed - speedLimit);
    at = atSetSpeed.getValue(speed - speedLimit);
    overSmall = overSetSpeedSmall.getValue(speed - speedLimit);
    overBig = overSetSpeedBig.getValue(speed - speedLimit);
    decelBig = deceleratingBig.getValue(accel);
    decelSmall = deceleratingSmall.getValue(accel);
    stable = stableChange.getValue(accel);
    accelSmall = acceleratingSmall.getValue(accel);
    accelBig = acceleratingBig.getValue(accel);

    //Print function memberships
    std::cout << "Memberships:\n";
    std::cout << "belowBig: " << belowBig << std::endl;
    std::cout << "belowSmall: " << belowSmall << std::endl;
    std::cout << "at: " << at << std::endl;
    std::cout << "overSmall: " << overSmall << std::endl;
    std::cout << "overBig: " << overBig << std::endl;
    std::cout << "decelBig: " << decelBig << std::endl;
    std::cout << "decelSmall: " << decelSmall << std::endl;
    std::cout << "stable: " << stable << std::endl;
    std::cout << "accelSmall: " << accelSmall << std::endl;
    std::cout << "accelBig: " << accelBig << "\n\n";

    //Array of double, double pairs. Stores the average of the speed and acceleration inputs, and the value of the
    //relevant output rule.
    std::vector<std::pair<double, double>> memberships;
    //Lambda function to add new pair to the above vector.
    auto addMember = [&memberships](double d1, double d2, double d3) { memberships.push_back(std::make_pair((d1 + d2) / 2, d3)); };

    //Find speed and acceleration values which have nonzero memberships, average them, and pair them with
    //their relevant output rule using the lambda function defined above.
    if (belowBig > 0)
    {
        if (accelBig > 0)
            addMember(belowBig, accelBig, MAX_SMALL_ACCEL_VAL);
        if (accelSmall > 0)
            addMember(belowBig, accelSmall, MAX_BIG_ACCEL_VAL);
        if (stable > 0)
            addMember(belowBig, stable, MAX_BIG_ACCEL_VAL);
        if (decelBig > 0)
            addMember(belowBig, decelBig, NO_CHANGE_VAL);
        if (decelSmall > 0)
            addMember(belowBig, decelSmall, MAX_BIG_ACCEL_VAL);
    }
    if (belowSmall > 0)
    {
        if (accelBig > 0)
            addMember(belowSmall, accelBig, MAX_BIG_DECEL_VAL);
        if (accelSmall > 0)
            addMember(belowSmall, accelSmall, NO_CHANGE_VAL);
        if (stable > 0)
            addMember(belowSmall, stable, MAX_SMALL_ACCEL_VAL);
        if (decelBig > 0)
            addMember(belowSmall, decelBig, MAX_BIG_ACCEL_VAL);
        if (decelSmall > 0)
            addMember(belowSmall, decelSmall, MAX_SMALL_ACCEL_VAL);
    }
    if (at > 0)
    {
        if (accelBig > 0)
            addMember(at, accelBig, MAX_BIG_DECEL_VAL);
        if (accelSmall > 0)
            addMember(at, accelSmall, MAX_SMALL_DECEL_VAL);
        if (stable > 0)
            addMember(at, stable, NO_CHANGE_VAL);
        if (decelBig > 0)
            addMember(at, decelBig, MAX_BIG_ACCEL_VAL);
        if (decelSmall > 0)
            addMember(at, decelSmall, MAX_SMALL_ACCEL_VAL);
    }
    if (overSmall > 0)
    {
        if (accelBig > 0)
            addMember(overSmall, accelBig, MAX_BIG_DECEL_VAL);
        if (accelSmall > 0)
            addMember(overSmall, accelSmall, MAX_SMALL_DECEL_VAL);
        if (stable > 0)
            addMember(overSmall, stable, MAX_SMALL_ACCEL_VAL);
        if (decelBig > 0)
            addMember(overSmall, decelBig, MAX_SMALL_ACCEL_VAL);
        if (decelSmall > 0)
            addMember(overSmall, decelSmall, NO_CHANGE_VAL);
    }
    if (overBig > 0)
    {
        if (accelBig > 0)
            addMember(overBig, accelBig, MAX_BIG_DECEL_VAL);
        if (accelSmall > 0)
            addMember(overBig, accelSmall, MAX_BIG_DECEL_VAL);
        if (stable > 0)
            addMember(overBig, stable, MAX_BIG_DECEL_VAL);
        if (decelBig > 0)
            addMember(overBig, decelBig, NO_CHANGE_VAL);
        if (decelSmall > 0)
            addMember(overBig, decelSmall, MAX_BIG_DECEL_VAL);
    }
    
    //Get area of output function
    //Replace the first value in each pair (which at the moment stores the average of the speed
    //and acceleration memberships) with the area of the trapezoid defined from the output function.
    //i.e. the output triangle cut off at the height of the average input members

    //Get the area of the output shape (the output triangle cutoff at height p.first)
    for (auto &p : memberships)        
        p.first = 5 * p.first * (1 - (p.first / 2));
    //Calculate centre of gravity
    //Centre of gravity is the sum of the areas of the trapezoids weighted by their base values, divided by
    //the sum of their areas.
    //((Area of trapezoid A*Output value of trapezoid A)+(area of trapizoid B*Output value of trapezoid B)) / (Sum of areas)
    double weightedAreaSum = 0, areaSum = 0;
    for (auto &p : memberships)
    {
        weightedAreaSum += p.first * p.second;
        areaSum += p.first;
    }
    if (areaSum != 0)
        power += weightedAreaSum / areaSum;

    //If the new applied power limit is over the available power of the engine, set power to maximum engine power
    if (power > CAR_POWER_LIMIT)
        power = CAR_POWER_LIMIT;
}
