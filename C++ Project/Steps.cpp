#include "Steps.h"
#include <fstream>
#include <iostream>

// Add a step to the recording
void Steps::addStep(size_t iteration, char step) {
    steps.push_back(std::make_pair(iteration, step));
}

// Load steps from file
Steps Steps::loadSteps(const std::string& filename) {
    Steps loadedSteps;
    std::ifstream steps_file(filename);

    if (!steps_file.is_open()) {
        std::cerr << "Error: Could not open steps file: " << filename << std::endl;
        return loadedSteps;
    }

    // Read number of steps (first line)
    size_t size;
    steps_file >> size;

    // Read each step: iteration and key pressed
    while (!steps_file.eof() && size-- != 0) {
        size_t iteration;
        char step;
        steps_file >> iteration >> step;

        // Only add if read was successful
        if (steps_file.good()) {
            loadedSteps.addStep(iteration, step);
        }
    }

    steps_file.close();

    std::cout << "Loaded " << loadedSteps.getStepCount()
        << " steps from " << filename << std::endl;

    return loadedSteps;
}

// Save steps to file
void Steps::saveSteps(const std::string& filename) const {
    std::ofstream steps_file(filename);

    if (!steps_file.is_open()) {
        std::cerr << "Error: Could not create steps file: " << filename << std::endl;
        return;
    }

    // Write number of steps (first line)
    steps_file << steps.size();

    // Write each step: iteration and key
    for (const auto& step : steps) {
        steps_file << '\n' << step.first << ' ' << step.second;
    }

    steps_file.close();

    std::cout << "Saved " << steps.size()
        << " steps to " << filename << std::endl;
}

// Get the next step for replay
bool Steps::getNextStep(size_t currentIteration, char& outStep) {
    // Check if we've exhausted all steps
    if (currentStepIndex >= steps.size()) {
        return false;
    }

    // Check if the current iteration matches the next step's iteration
    if (steps[currentStepIndex].first == currentIteration) {
        outStep = steps[currentStepIndex].second;
        currentStepIndex++;
        return true;
    }

    // This iteration has no recorded step
    return false;
}

// Reset replay to beginning
void Steps::resetReplay() {
    currentStepIndex = 0;
}

// Check if there are more steps to replay
bool Steps::hasMoreSteps() const {
    return currentStepIndex < steps.size();
}

// Get total number of steps
size_t Steps::getStepCount() const {
    return steps.size();
}

// Clear all steps
void Steps::clear() {
    steps.clear();
    currentStepIndex = 0;
}

// Check if steps vector is empty
bool Steps::isEmpty() const {
    return steps.empty();
}

// Get a specific step by index (for debugging/verification)
std::pair<size_t, char> Steps::getStepAt(size_t index) const {
    if (index < steps.size()) {
        return steps[index];
    }
    return std::make_pair(0, '\0');
}
