//
// Created by jobna on 19-8-2026.
//

#ifndef PRIVE_GIT_PROJECTEN_FISH_H
#define PRIVE_GIT_PROJECTEN_FISH_H
using namespace std;
#include <array>
#include <vector>

class Fish {
public:
    int id; // unique id of the fish in the school
    array<float,3> position; // position of the fish (x, y, z)
    array<float,3> velocity; // velocity of the fish (x, y, z)
};

class School {
public:
    int number_of_fish; // the amount of fish in the school
    float mass; // mass of each fish (school property so each fish has the same mass)
    array<float,3> ideal_velocity; // ideal velocity of the school (x, y, z)
    float ideal_separation; // ideal distance between each fish
    float fear_factor; // how afraid the fish are of supposed predators
    float nearby_fish_radius; // the radius around a fish of other fish that it wants to swim next to
    float random_velocity_scaling; // each fish can randomly change heading;
                                   // this factor determines how large the randomization effect is

    vector<Fish> fish;
    void schoolDynamics();
    Fish* getFishById(int target_id);
    static float distanceBetweenFish(array<float,3> pos1, array<float,3> pos2);
};


#endif //PRIVE_GIT_PROJECTEN_FISH_H
