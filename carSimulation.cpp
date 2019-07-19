#include <ctime>
#include <iostream>
#include <iomanip>
#include "car.h"

#define NUMBER_OF_LINEBREAKS 5
#define SIM_ITER_COUNT 100
#define SPEED_LIMIT 50

void performSimulation(Car car, int roadAngle)
{
	auto stamp = std::clock();
	int count = 0;
	while (count < SIM_ITER_COUNT)
    {
        if (std::clock() >= stamp + 100)
        {
            std::cout << std::string(NUMBER_OF_LINEBREAKS, '\n');
            std::cout << "Effective Road Angle: " << roadAngle << "\tSpeed Limit: " << SPEED_LIMIT << std::endl;
            car.setPower(SPEED_LIMIT);
            std::cout << "Speed: " << car.getSpeed(roadAngle) << std::endl;
            std::cout << "Acceleration: " << car.accel << std::endl;
            count++;
            stamp = std::clock();
        }
    }
}

int main()
{
    Car car(SPEED_LIMIT);

    std::cout << std::fixed;
    std::cout << std::setprecision(2);

	performSimulation(car, 0);
	performSimulation(car, 5);
	performSimulation(car, 10);
}
