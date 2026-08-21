#include <iostream>
#include "Fish.h"

int main() {
    // initial parameters of the school:
    School school;
    school.number_of_fish = 200;
    school.mass = 0.5;
    school.ideal_velocity = {1, 0, 0};
    school.ideal_separation = 0.1;
    school.random_velocity_scaling = 0;
    school.fish_force_factor = 5;
    school.generateFish({0, 0, 0}, 50); // generating the fish in the school

    school.schoolDynamics(0.2, 500);


    return 0;
}