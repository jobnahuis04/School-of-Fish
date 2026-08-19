//
// Created by jobna on 19-8-2026.
//

#include "Fish.h"
#include <array>
#include <cmath>
#include <math.h>
#include <vector>
using namespace std;

void School::schoolDynamics() {
    for (int i = 0; i < number_of_fish; i++) {
        for (int j = i+1; j < number_of_fish; j++) { // looping over all fish without going over fish twice
            array<float,3> pos_i = getFishById(i)->position;
            array<float,3> pos_j = getFishById(j)->position;
            DistanceAndDirectionBetweenFish d_fish = distanceBetweenFish(pos_i, pos_j);
            float fish_separation_error = d_fish.distance-ideal_separation;
            float F = forceBetweenFish(fish_separation_error);
            getFishById(i)->fish_force = {F*d_fish.direction[0],F*d_fish.direction[1],F*d_fish.direction[2]};
            getFishById(j)->fish_force = {-F*d_fish.direction[0],-F*d_fish.direction[1],-F*d_fish.direction[2]};
        }
    }
}

float School::forceBetweenFish(float err) {
    return fish_force_factor / (err * err);
}

// Returns a pointer to the Fish (or nullptr if not found)
Fish* School::getFishById(int target_id) {
    auto it = ranges::find_if(fish.begin(), fish.end(), [target_id](const Fish& f) {
        return f.id == target_id;
    });

    return (it != fish.end()) ? &(*it) : nullptr;
}


School::DistanceAndDirectionBetweenFish School::distanceBetweenFish(const array<float,3> pos1, const array<float,3> pos2) {
    //TODO: check if it is possible to keep a squared distance to avoid taking the square root
    const float dx = pos2[0] - pos1[0];
    const float dy = pos2[1] - pos1[1];
    const float dz = pos2[2] - pos1[2];
    const float d_squared = dx * dx + dy * dy + dz * dz;
    const float dist = sqrt(d_squared);
    DistanceAndDirectionBetweenFish result;
    result.distance = sqrt(d_squared);
    result.direction = {dx/dist, dy/dist, dz/dist}; // normalized direction from pos1 to pos2
    return result;
}

