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
            file << description << "," << when_to_do << ",";
            if (!deadline.empty()) {
                file << deadline << ",";
            }
            if (!priority.empty()) {
                file << priority << ",";
            }
            file << "\n";
        }
    }

    bool loadFromStream(std::istream& stream) override {
        if (std::getline(stream, description) &&
            std::getline(stream, when_to_do) &&
            std::getline(stream, deadline) &&
            std::getline(stream, priority)) {
            return !description.empty();
        }
        return false;
    }

    // Method to generate a file-compatible string representation
    std::string toFileString() const override {
        return description + "\n" + when_to_do + "\n" + deadline + "\n" + priority;
    }

        friend std::ostream& operator<<(std::ostream& os, const LifeTask& task) {
        os << "Life Task:\n";
        os << "  Description: " << task.getDescription() << "\n";
        os << "  When To Do: " << task.getWhenToDo() << "\n";
        os << "  Deadline: " << task.getDeadline() << "\n";
        os << "  Priority: " << task.getPriority() << "\n";
        return os;
    }
};

const std::string LifeTask::FILE_PATH = "life_tasks.txt";

#endif