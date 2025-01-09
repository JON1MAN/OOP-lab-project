#ifndef LIFE_TASK_HPP
#define LIFE_TASK_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Task.hpp"

using namespace am;

namespace am {
    /**
     * @class LifeTask
     * @brief Represents a task related to life activities, inheriting from the Task base class.
     * 
     * This class manages a life-related task, including its description, when-to-do date, deadline, and priority.
     * It provides methods to display the task, save it to a file, load it from a stream, and convert it to a string format suitable for file storage.
     */
    class LifeTask : public Task {

    public:

        /** @brief The file path to save the life tasks. */
        static const std::string FILE_PATH;

        /** 
         * @brief Default constructor for creating an empty LifeTask.
         * 
         * Initializes a new life task with empty values.
         */
        LifeTask() : Task() {}

        /** 
         * @brief Parameterized constructor for creating a LifeTask with given data.
         * 
         * @param description The description of the task.
         * @param when_to_do The date when the task should be done (DD.MM.YYYY).
         * @param deadline The deadline for the task (DD.MM.YYYY).
         * @param priority The priority of the task (low, medium, high).
         */
        LifeTask(
                const std::string& description, 
                const std::string when_to_do, 
                const std::string& deadline, 
                const std::string& priority)
            : Task(description, when_to_do, deadline, priority){}
        
        /** 
         * @brief Displays the life task details on the console.
         */
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
            std::getline(stream, description, ',');
            std::getline(stream, when_to_do, ',');
            std::getline(stream, deadline, ',');
            std::getline(stream, priority, ',');
            
            return !(description.empty() || when_to_do.empty() || deadline.empty() || priority.empty());
        }

        /** 
         * @brief Converts the task data into a string for file storage.
         * 
         * @return A string representation of the task for file storage.
         */
        std::string toFileString() const override {
            return  description + ", " + when_to_do + ", " + deadline + ", " + priority + "\n";
        }

        /** 
         * @brief Overloads the output stream operator to print task details.
         * 
         * @param os The output stream.
         * @param task The life task to print.
         * @return The output stream with the task details.
         */
        friend std::ostream& operator<<(std::ostream& os, const LifeTask& task) {
            os << "  Description: " << task.getDescription() << "\n";
            os << "  Deadline: " << task.getDeadline() << "\n";
            os << "  Priority: " << task.getPriority() << "\n";
            return os;
        }
    };

    /** @brief The file path for storing life tasks. */
    const std::string LifeTask::FILE_PATH = "life.txt";
}

#endif