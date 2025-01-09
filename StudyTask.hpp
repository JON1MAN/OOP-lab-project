#ifndef STUDY_TASK_HPP
#define STUDY_TASK_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Task.hpp"

class StudyTask : public Task {
private:
    std::string subject;

public:
    static const std::string FILE_PATH;

    StudyTask()
        : Task(), subject("") {}

    StudyTask(
        const std::string& description,
        const std::string& when_to_do,
        const std::string& deadline,
        const std::string& priority,
        const std::string& subject)
        : Task(description, when_to_do, deadline, priority), subject(subject) {}
        
    void display() const override {
        std::cout << "Study Task:\n";
        std::cout << "  Subject: " << subject << "\n";
        std::cout << "  Description: " << description << "\n";
        std::cout << "  When To Do: " << when_to_do << "\n";
        std::cout << "  Deadline: " << deadline << "\n";
        std::cout << "  Priority: " << priority << "\n";
    }

    // Override saveToFile method
    void saveToFile(std::ofstream& file) const override {
        if (file.is_open()) {
            file << toFileString();
            file.close();
        }
    }

    // Method to load a task from an input stream
    bool loadFromStream(std::istringstream& stream) override {
        // Assuming the line format: "math, Read, 08.01.2025, 09.01.2025, high"
        std::getline(stream, subject, ',');
        std::getline(stream, description, ',');
        std::getline(stream, when_to_do, ',');
        std::getline(stream, deadline, ',');
        std::getline(stream, priority, ',');
        
        // Trim spaces if needed (implement a helper function for trimming).
        return !(subject.empty() || description.empty() || when_to_do.empty() || deadline.empty() || priority.empty());
    }

    // Method to generate a file-compatible string representation
    std::string toFileString() const override {
        return subject + ", " + description + ", " + when_to_do + ", " + deadline + ", " + priority + "\n";
    }

    const std::string& getSubject() const {
        return subject;
    }

    void setSubject(const std::string& newSubject) {
        subject = newSubject;
    }

    friend std::ostream& operator<<(std::ostream& os, const StudyTask& task) {
        os << "  Subject: " << task.getSubject() << "\n";
        os << "  Description: " << task.getDescription() << "\n";
        os << "  Deadline: " << task.getDeadline() << "\n";
        os << "  Priority: " << task.getPriority() << "\n";
        return os;
    }
};

// File path definition
const std::string StudyTask::FILE_PATH = "study.txt";

#endif