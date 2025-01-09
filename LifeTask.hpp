#ifndef LIFE_TASK_HPP
#define LIFE_TASK_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Task.hpp"

class LifeTask : public Task {

public:

    static const std::string FILE_PATH;

    LifeTask() : Task() {}

    LifeTask(
            const std::string& description, 
            const std::string when_to_do, 
            const std::string& deadline, 
            const std::string& priority)
        : Task(description, when_to_do, deadline, priority){}
    
    void display() const override {
        std::cout << "Life Task:\n";
        std::cout << "  Description: " << description << "\n";
        std::cout << "  When To Do: " << when_to_do << "\n";
        if (!deadline.empty()) {
            std::cout << "  Deadline: " << deadline << "\n";
        }
        if (!priority.empty()) {
            std::cout << "  Priority: " << priority << "\n";
        }
    }

    void saveToFile(std::ofstream& file) const override {
        if (file.is_open()) {
            file << toFileString();
            file.close();
        }
    }

    bool loadFromStream(std::istringstream& stream) override {
        std::getline(stream, description, ',');
        std::getline(stream, when_to_do, ',');
        std::getline(stream, deadline, ',');
        std::getline(stream, priority, ',');
        
        // Trim spaces if needed (implement a helper function for trimming).
        return !(description.empty() || when_to_do.empty() || deadline.empty() || priority.empty());
    }

    // Method to generate a file-compatible string representation
    std::string toFileString() const override {
        return  description + ", " + when_to_do + ", " + deadline + ", " + priority + "\n";
    }

        friend std::ostream& operator<<(std::ostream& os, const LifeTask& task) {
        os << "  Description: " << task.getDescription() << "\n";
        os << "  Deadline: " << task.getDeadline() << "\n";
        os << "  Priority: " << task.getPriority() << "\n";
        return os;
    }
};

const std::string LifeTask::FILE_PATH = "life.txt";

#endif