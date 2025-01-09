#ifndef STUDY_TASK_HPP
#define STUDY_TASK_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Task.hpp"

/**
 * @class StudyTask
 * @brief Represents a task related to study, inheriting from the Task base class.
 * 
 * This class manages all the necessary data for a study task, including subject,
 * description, when-to-do date, deadline, and priority. It provides methods to
 * display, save, load, and convert the task data to a string format for file storage.
 */
class StudyTask : public Task {
private:
    /** @brief The subject of the study task. */
    std::string subject;

public:
    /** @brief The file path to save the study tasks. */
    static const std::string FILE_PATH;

    /** 
     * @brief Default constructor for creating an empty StudyTask.
     * 
     * Initializes a new study task with empty values.
     */
    StudyTask()
        : Task(), subject("") {}

    /** 
     * @brief Parameterized constructor for creating a StudyTask with given data.
     * 
     * @param description The description of the task.
     * @param when_to_do The date when the task should be done (DD.MM.YYYY).
     * @param deadline The deadline for the task (DD.MM.YYYY).
     * @param priority The priority of the task (low, medium, high).
     * @param subject The subject of the study task.
     */
    StudyTask(
        const std::string& description,
        const std::string& when_to_do,
        const std::string& deadline,
        const std::string& priority,
        const std::string& subject)
        : Task(description, when_to_do, deadline, priority), subject(subject) {}
    
    /** 
     * @brief Displays the study task details on the console.
     */
    void display() const override {
        std::cout << "Study Task:\n";
        std::cout << "  Subject: " << subject << "\n";
        std::cout << "  Description: " << description << "\n";
        std::cout << "  When To Do: " << when_to_do << "\n";
        std::cout << "  Deadline: " << deadline << "\n";
        std::cout << "  Priority: " << priority << "\n";
    }

    /** 
     * @brief Saves the task data to a file.
     * 
     * @param file The output file stream to write the task data to.
     */
    void saveToFile(std::ofstream& file) const override {
        if (file.is_open()) {
            file << toFileString();
            file.close();
        }
    }

    /** 
     * @brief Loads the task data from a stream (e.g., file or string).
     * 
     * @param stream The input stream from which to read the task data.
     * @return True if the task data is successfully loaded, false otherwise.
     */
    bool loadFromStream(std::istringstream& stream) override {
        std::getline(stream, subject, ',');
        std::getline(stream, description, ',');
        std::getline(stream, when_to_do, ',');
        std::getline(stream, deadline, ',');
        std::getline(stream, priority, ',');
        
        return !(subject.empty() || description.empty() || when_to_do.empty() || deadline.empty() || priority.empty());
    }

    /** 
     * @brief Converts the task data into a string for file storage.
     * 
     * @return A string representation of the task for file storage.
     */
    std::string toFileString() const override {
        return subject + ", " + description + ", " + when_to_do + ", " + deadline + ", " + priority + "\n";
    }

    const std::string& getSubject() const {
        return subject;
    }

    void setSubject(const std::string& newSubject) {
        subject = newSubject;
    }

    /** 
     * @brief Overloads the output stream operator to print task details.
     * 
     * @param os The output stream.
     * @param task The study task to print.
     * @return The output stream with the task details.
     */
    friend std::ostream& operator<<(std::ostream& os, const StudyTask& task) {
        os << "  Subject: " << task.getSubject() << "\n";
        os << "  Description: " << task.getDescription() << "\n";
        os << "  Deadline: " << task.getDeadline() << "\n";
        os << "  Priority: " << task.getPriority() << "\n";
        return os;
    }
};

/** @brief The file path for storing study tasks. */
const std::string StudyTask::FILE_PATH = "study.txt";

#endif