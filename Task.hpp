#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace am {
    /**
     * @class Task
     * @brief Abstract base class representing a general task with a description, when-to-do date, deadline, and priority.
     * 
     * This class provides a blueprint for different types of tasks (e.g., study, work, life) that share common properties such as description, when-to-do date, deadline, and priority.
     * It also includes methods for displaying, saving, and loading tasks, as well as converting them to file-friendly formats.
     */
    class Task {
    protected:
        /** @brief The description of the task. */
        std::string description;
        
        /** @brief The date when the task should be done (DD.MM.YYYY). */
        std::string when_to_do;

        /** @brief The deadline for the task (DD.MM.YYYY). */
        std::string deadline;

        /** @brief The priority of the task (low, medium, high). */
        std::string priority;

    public:

        /** 
         * @brief Default constructor for creating an empty task.
         */
        Task() = default;

        /** 
         * @brief Constructor for creating a task with description and when-to-do date.
         * 
         * @param description The description of the task.
         * @param when_to_do The date when the task should be done (DD.MM.YYYY).
         */
        Task(const std::string& description, const std::string when_to_do) 
            : description(description), when_to_do(when_to_do) {}
            
        /** 
         * @brief Constructor for creating a task with description, when-to-do date, deadline, and priority.
         * 
         * @param description The description of the task.
         * @param when_to_do The date when the task should be done (DD.MM.YYYY).
         * @param deadline The deadline for the task (DD.MM.YYYY).
         * @param priority The priority of the task (low, medium, high).
         */
        Task(
                const std::string& description, 
                const std::string when_to_do, 
                const std::string& deadline, 
                const std::string& priority)
            : description(description), when_to_do(when_to_do), deadline(deadline), priority(priority) {}

        /** 
         * @brief Virtual destructor for cleaning up derived classes.
         */
        virtual ~Task() {}

        /** 
         * @brief Pure virtual method to display task details.
         * 
         * This method must be implemented by derived classes.
         */
        virtual void display() const = 0;

        /** 
         * @brief Pure virtual method to save task data to a file.
         * 
         * @param file The output file stream where task data should be saved.
         */
        virtual void saveToFile(std::ofstream& file) const = 0;

        /** 
         * @brief Pure virtual method to load task data from a stream.
         * 
         * @param ss The input stream from which to load the task data.
         * @return True if the task was successfully loaded, false otherwise.
         */
        virtual bool loadFromStream(std::istringstream& ss) = 0;

        /** 
         * @brief Pure virtual method to convert task data to a string suitable for file storage.
         * 
         * @return A string representing the task's data in a file-compatible format.
         */
        virtual std::string toFileString() const = 0;

        const std::string& getDescription() const {
            return description;
        }

        std::string getWhenToDo() const {
            return when_to_do; 
        }

        const std::string& getDeadline() const {
            return deadline;
        }

        const std::string& getPriority() const {
            return priority;
        }

        void setDescription(const std::string& newDescription) {
            description = newDescription;
        }

        void setWhenToDo(const std::string& newWhenToDo) {
            when_to_do = newWhenToDo;
        }

        void setDeadline(const std::string& newDeadline) {
            deadline = newDeadline;
        }

        void setPriority(const std::string& newPriority) {
            priority = newPriority;
        }
    };
}


#endif