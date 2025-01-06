#ifndef WORK_TASK_HPP
#define WORK_TASK_HPP

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Task.hpp"

class WorkTask : public Task {
private:
    std::string assignedBy;

public:

    static const std::string FILE_PATH;

    WorkTask()
        : Task(), assignedBy("") {}

    WorkTask(const std::string& description,
            const std::string when_to_do,
            const std::string& deadline,
            const std::string& priority,
            const std::string& assignedBy)
        : Task(description, when_to_do, deadline, priority), assignedBy(assignedBy) {}

    void display() const override {
        std::cout << "Work Task:\n";
        std::cout << "  Description: " << description << "\n";
        std::cout << "  Assignee: " << assignedBy << "\n";
        std::cout << "  When To Do: " << when_to_do << "\n";
        std::cout << "  Deadline: " << deadline << "\n";
        std::cout << "  Priority: " << priority << "\n";
    }

    void saveToFile(std::ofstream& file) const override {
        if (file.is_open()) {
            file << description << "," << when_to_do << "," << deadline << "," << priority << "," << assignedBy << "\n";
        }
    }

    bool loadFromStream(std::istream& stream) override {
        if (std::getline(stream, assignedBy) &&
            std::getline(stream, description) &&
            std::getline(stream, when_to_do) &&
            std::getline(stream, deadline) &&
            std::getline(stream, priority)) {
            return !description.empty();
        }
        return false;
    }

    // Method to generate a file-compatible string representation
    std::string toFileString() const override {
        return assignedBy + "\n" + description + "\n" + when_to_do + "\n" + deadline + "\n" + priority;
    }

    const std::string& getAssignedBy() const {
        return assignedBy;
    }

    void setAssignedBy(const std::string& newAssignedBy) {
        assignedBy = newAssignedBy;
    }

    friend std::ostream& operator<<(std::ostream& os, const WorkTask& task) {
        os << "Work Task:\n";
        os << "  Description: " << task.getDescription() << "\n";
        os << "  When To Do: " << task.getWhenToDo() << "\n";
        os << "  Deadline: " << task.getDeadline() << "\n";
        os << "  Priority: " << task.getPriority() << "\n";
        return os;
    }

};

const std::string WorkTask::FILE_PATH = "work_tasks.txt";

#endif