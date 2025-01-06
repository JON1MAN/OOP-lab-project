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
            file << subject << "\n"
                 << description << "\n"
                 << when_to_do << "\n"
                 << deadline << "\n"
                 << priority << "\n";
        }
    }

    // Method to load a task from an input stream
    bool loadFromStream(std::istream& stream) override {
        if (std::getline(stream, subject) &&
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
        return subject + "\n" + description + "\n" + when_to_do + "\n" + deadline + "\n" + priority;
    }

    const std::string& getSubject() const {
        return subject;
    }

    void setSubject(const std::string& newSubject) {
        subject = newSubject;
    }

    friend std::ostream& operator<<(std::ostream& os, const StudyTask& task) {
        os << "Study Task:\n";
        os << "  Subject: " << task.subject << "\n";
        os << "  Description: " << task.getDescription() << "\n";
        os << "  When To Do: " << task.getWhenToDo() << "\n";
        os << "  Deadline: " << task.getDeadline() << "\n";
        os << "  Priority: " << task.getPriority() << "\n";
        return os;
    }
};

// File path definition
const std::string StudyTask::FILE_PATH = "study_tasks.txt";

#endif
