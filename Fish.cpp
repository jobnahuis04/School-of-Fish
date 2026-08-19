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
    int N = number_of_fish;
    // initialize an N x N matrix pre-filled with 0.0f

    std::vector<std::vector<float>> fish_separation_error(N, std::vector<float>(N, 0.0f));    for (int i = 0; i < number_of_fish; i++) {
        for (int j = i+1; j < number_of_fish; j++) { // looping over all fish without going over fish twice
            array<float,3> pos_i = getFishById(i)->position;
            array<float,3> pos_j = getFishById(j)->position;
            float error = distanceBetweenFish(pos_i, pos_j)-ideal_separation;
            fish_separation_error[i][j] = error;
            fish_separation_error[j][i] = error;
        }
    }
}

// Returns a pointer to the Fish (or nullptr if not found)
Fish* School::getFishById(int target_id) {
    auto it = ranges::find_if(fish.begin(), fish.end(), [target_id](const Fish& f) {
        return f.id == target_id;
    });

    return (it != fish.end()) ? &(*it) : nullptr;
}

float School::distanceBetweenFish(const array<float,3> pos1, const array<float,3> pos2) {
    //TODO: check if it is possible to keep a squared distance to avoid taking the square root
    const float dx = pos1[0] - pos2[0];
    const float dy = pos1[1] - pos2[1];
    const float dz = pos1[2] - pos2[2];
    const float d_squared = dx * dx + dy * dy + dz * dz;
    return sqrt(d_squared);
}

