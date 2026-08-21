//
// Created by jobna on 19-8-2026.
//

#include "Fish.h"
#include <array>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include  <filesystem>

void School::schoolDynamics(float t_step, float t_max) {
    std::filesystem::path folder = "vtk_files";
    if (std::filesystem::exists(folder)) {
        std::filesystem::remove_all(folder);
    }
    std::filesystem::create_directory(folder);
    int loop_count = 0;
    for (float t = 0.0; t < t_max; t += t_step) {
        for (int i = 0; i < number_of_fish; i++) {
            for (int j = i+1; j < number_of_fish; j++) { // looping over all fish without going over fish twice
                // finding the positions of the two interacting fish
                array<float,3> pos_i = getFishById(i)->position;
                array<float,3> pos_j = getFishById(j)->position;
                // finding the distance and direction vector between the fish
                DistanceAndDirectionBetweenFish d_fish = distanceBetweenFish(pos_i, pos_j);
                float fish_separation_error = d_fish.distance-ideal_separation;
                float F = forceBetweenFish(fish_separation_error);
                getFishById(i)->fish_force = multiplyArrayWithConstant(d_fish.direction, F);
                getFishById(j)->fish_force = multiplyArrayWithConstant(d_fish.direction, -F);;
            }
        }
        for (int i = 0; i < number_of_fish; i++) {
            array<float,3> acceleration = multiplyArrayWithConstant(getFishById(i)->fish_force, 1/mass);
            getFishById(i)->velocity = sumArrays(getFishById(i)->velocity, multiplyArrayWithConstant(acceleration, t_step));
            getFishById(i)->position = sumArrays(getFishById(i)->position, multiplyArrayWithConstant(getFishById(i)->velocity, t_step));
        }
        cout<<"t = "<<t<<" / "<<t_max<<endl;
        loop_count += 1;
        ostringstream filename;
        filename << "vtk_files/fish_"
                 << setfill('0')
                 << setw(4)
                 << loop_count
                 << ".vtk";

        saveToVTK(filename.str());
    }
}

float School::forceBetweenFish(float err) {
    return fish_force_factor / (err);
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
    float dist = std::sqrt(d_squared);
    if (dist < 1e-6){dist = 1e-6;} // making sure there is no division by 0
    DistanceAndDirectionBetweenFish result;
    result.distance = std::sqrt(d_squared);
    result.direction = {dx/dist, dy/dist, dz/dist}; // normalized direction from pos1 to pos2
    return result;
}

void School::generateFish(array<float,3> school_center, float school_radius) {
    fish.clear();
    fish.reserve(number_of_fish); // Pre-allocates memory to avoid reallocations

    for (int i = 0; i < number_of_fish; ++i) {
        Fish new_fish;
        new_fish.id = i;
        new_fish.position = getRandomPosition(school_center, school_radius);
        new_fish.velocity = getRandomVelocity();
        fish.push_back(new_fish);
    }
}

array<float, 3> School::getRandomPosition(array<float,3> school_center, float school_radius) {
    // setup random engine
    uniform_real_distribution<float> unit_dist(0.0, 1.0);

    // ample radius and angle
    float r = school_radius * sqrt(unit_dist(gen));
    float theta = unit_dist(gen) * 2 * numbers::pi_v<float>;
    // float phi = unit_dist(gen) * 2 * numbers::pi_v<float> - numbers::pi_v<float>;

    float x = r * std::cos(theta);
    float y = r * std::sin(theta);
    // float z = sin(phi);
    float z = 0;

    return {x+school_center[0], y+school_center[1], z+school_center[2]};
}
array<float, 3> School::getRandomVelocity() {
    uniform_real_distribution<float> unit_dist(0.0, 1.0);
    float vx = unit_dist(gen) * random_velocity_scaling + ideal_velocity[0];
    float vy = unit_dist(gen) * random_velocity_scaling + ideal_velocity[1];
    float vz = unit_dist(gen) * random_velocity_scaling + ideal_velocity[2];
    return {vx, vy, vz};
}

void School::saveToVTK(const std::string& filename)
{
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open VTK file: " << filename << std::endl;
        return;
    }

    // VTK header
    file << "# vtk DataFile Version 3.0\n";
    file << "Fish school simulation\n";
    file << "ASCII\n";
    file << "DATASET POLYDATA\n";

    // Fish positions
    file << "POINTS " << fish.size() << " float\n";

    for (const Fish& f : fish) {
        file << f.position[0] << " "
             << f.position[1] << " "
             << f.position[2] << "\n";
    }

    // Make every fish a vertex
    file << "VERTICES " << fish.size() << " " << fish.size() * 2 << "\n";

    for (size_t i = 0; i < fish.size(); ++i) {
        file << "1 " << i << "\n";
    }

    // Fish data
    file << "POINT_DATA " << fish.size() << "\n";

    // Velocity
    file << "VECTORS velocity float\n";

    for (const Fish& f : fish) {
        file << f.velocity[0] << " "
             << f.velocity[1] << " "
             << f.velocity[2] << "\n";
    }

    // Fish ID
    file << "SCALARS fish_id int 1\n";
    file << "LOOKUP_TABLE default\n";

    for (const Fish& f : fish) {
        file << f.id << "\n";
    }

    file.close();
}

array<float, 3> multiplyArrayWithConstant(array<float, 3> input, float multiplier) {
    array<float,3> output;
    for (int i = 0; i < 3; i++) {
        output[i] = input[i]*multiplier;
    }
    return output;
}
array<float, 3> sumArrays(array<float, 3> input1, array<float, 3> input2) {
    return {input1[0] + input2[0], input1[1] + input2[1], input1[2] + input2[2]};
}
