#include "Steps.h"
#include <fstream>
#include <iostream>


// Add a step to the recording
void Steps::addStep(size_t iteration, int playerNum, char step) {
    steps.push_back({iteration, playerNum, step});
}

// Load steps from file
Steps Steps::loadSteps(const std::string& filename) {
    Steps loadedSteps;
    std::ifstream steps_file(filename);

	if (!steps_file.is_open()) {// Check if file opened successfully
        std::cout << "Error: Could not open steps file: " << filename << std::endl;
        return loadedSteps;
    }
	// Read screen files line (first line)
    std::string screensLine;
    if (std::getline(steps_file, screensLine)) {
        loadedSteps.setScreenFiles(screensLine);
    }
    // Read number of steps (first line)
    size_t size;
    if (!(steps_file >> size)) {
        steps_file.close();
        return loadedSteps;
    }

    // Read each step: iteration and key pressed
    for (size_t i = 0; i < size; ++i) {
        size_t iteration;
        int playerNum;
        char step;

		// Read iteration, player number, and key
        if (steps_file >> iteration >> playerNum >> step) {
            loadedSteps.addStep(iteration, playerNum, step);
        }
    }

    steps_file.close();
    return loadedSteps;
}

// Save steps to file
void Steps::saveSteps(const std::string& filename) const {
    std::ofstream steps_file(filename);

    if (!steps_file.is_open()) {
        std::cout << "Error: Could not create steps file: " << filename << std::endl;
        return;
    }

	steps_file << screenFiles << '\n'; // Write screen files line (first line)
 
	steps_file << steps.size() << "\n";  // Write number of steps (second line)

    // Write each step: iteration and key
    for (const auto& step : steps) {
        steps_file << "TIME: " << step.iteration
            << " PLAYER: " << step.PlayerNum
            << " KEY: " << step.key << "\n";
    }

	steps_file.close(); // Close the file

    std::cout << "Saved " << steps.size()
        << " steps to " << filename << std::endl;
}

// Get the next step for replay
bool Steps::getNextStep(size_t currentIteration, Step& outStep) {
    // Check if we've exhausted all steps
    if (currentStepIndex >= steps.size()) {
        return false;
    }

    // Check if the current iteration matches the next step's iteration
    if (steps[currentStepIndex].iteration == currentIteration) {
        outStep = steps[currentStepIndex];
        currentStepIndex++;
        return true;
    }
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


// Clear all steps
void Steps::clearSteps() {
    steps.clear();
    currentStepIndex = 0;
    screenFiles = "";
}
// Initialize for recording with screen file names
void Steps::initForRecording(const std::vector<std::string>& fileNames) {
   
	clearSteps(); // Clear any existing steps

  
	std::string screensString = ""; // Concatenate file names with '|' delimiter
	for (size_t i = 0; i < fileNames.size(); i++) {  // Iterate through file names
		if (i > 0) screensString += "|"; // Add delimiter for all but first
		screensString += fileNames[i]; // Append file name
    }

	screenFiles = screensString; // Set the concatenated string
}
// Add step if key belongs to a player
void Steps::addStepIfValid(size_t iteration, char ch, const Player& p1, const Player& p2) {
	if (p1.isMyKey(ch)) {// player 1 is identified with the sign '&' - we will register it as playerNum 1
        addStep(iteration, 1, ch);
    }
	else if (p2.isMyKey(ch)) { // player 2 is identified with the sign '$' - we will register it as playerNum 2
        addStep(iteration, 2, ch);
    }
}