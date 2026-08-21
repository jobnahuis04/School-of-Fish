//
// Created by jobna on 19-8-2026.
//

#ifndef SCHOOL_OF_FISH_FISH_H
#define SCHOOL_OF_FISH_FISH_H
using namespace std;
#include <array>
#include <vector>
#include <random>

class Fish {
public:
    int id; // unique id of the fish in the school
    array<float,3> position = {0.0f, 0.0f, 0.0f}; // position of the fish (x, y, z)
    array<float,3> velocity = {0.0f, 0.0f, 0.0f}; // velocity of the fish (x, y, z)
    array<float,3> fish_force = {0.0f, 0.0f, 0.0f}; // force on the fish (x, y, z)
};

class School {
private:
    std::random_device rd;
    std::mt19937 gen{rd()};

public:
    int number_of_fish; // the amount of fish in the school
    float mass; // mass of each fish (school property so each fish has the same mass)
    array<float,3> ideal_velocity; // ideal velocity of the school (x, y, z)
    float ideal_separation; // ideal distance between each fish
    float fear_factor; // how afraid the fish are of supposed predators
    float fish_force_factor; // scaling factor of the force between fish
    float random_velocity_scaling; // each fish can randomly change heading;
    // this factor determines how large the randomization effect is

    vector<Fish> fish;
    void schoolDynamics(float t_step, float t_max);
    Fish* getFishById(int target_id);
    struct DistanceAndDirectionBetweenFish {
        float distance;
        array<float,3> direction;
    };
    DistanceAndDirectionBetweenFish distanceBetweenFish(array<float,3> pos1, array<float,3> pos2);
    float forceBetweenFish(float err);
    void generateFish(array<float,3> school_center, float school_radius);
    array<float,3> getRandomPosition(array<float,3> school_center, float school_radius);
    array<float, 3> getRandomVelocity();
    void saveToVTK(const std::string& filename);
};
array<float,3> multiplyArrayWithConstant(array<float,3> input, float multiplier);
array<float, 3> sumArrays(array<float, 3> input1, array<float, 3> input2);

#endif //SCHOOL_OF_FISH_FISH_H