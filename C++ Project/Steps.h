#pragma once
#include <vector>
#include <string>
#include <utility>

class Steps {
private:
    // Vector of pairs: (game cycle / iteration, key pressed)
    std::vector<std::pair<size_t, char>> steps;

    // Current step index for replay mode
    size_t currentStepIndex;

public:
    // Constructor
    Steps() : currentStepIndex(0) {
    }

    // Destructor
    ~Steps() {
    }

    // Add a step to the recording
    void addStep(size_t iteration, char step);

    // Load steps from file (static factory method)
    static Steps loadSteps(const std::string& filename);

    // Save steps to file
    void saveSteps(const std::string& filename) const;

    // Get the next step for replay mode
    bool getNextStep(size_t currentIteration, char& outStep);

    // Reset replay to the beginning
    void resetReplay();

    // Check if there are more steps to replay
    bool hasMoreSteps() const;

    // Get total number of recorded steps
    size_t getStepCount() const;

    // Clear all steps (reset everything)
    void clear();

    // Check if steps were loaded successfully
    bool isEmpty() const;

    // Get a specific step by index (for debugging/verification)
    std::pair<size_t, char> getStepAt(size_t index) const;
};