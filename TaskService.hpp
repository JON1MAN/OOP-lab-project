#ifndef TASK_SERVICE_HPP
#define TASK_SERVICE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "StudyTask.hpp"
#include "WorkTask.hpp"
#include "LifeTask.hpp"
using namespace am;

namespace am {
    class TaskService {
    public:

        /**
         * @brief Main loop of the To-Do List application.
         *
         * This function runs the main application loop, allowing the user to interact
         * with the To-Do List application through a set of menu options. The user can
         * view tasks for the current day, add new tasks, mark tasks as done, reschedule
         * unfinished tasks, or exit the application.
         *
         * @details The function operates in an infinite loop, presenting the user with
         * a menu of options. The user selects an option by entering a number corresponding
         * to the desired action:
         * - **1**: Add a new task for today.
         * - **2**: Add a task with a custom date.
         * - **3**: Mark a task as completed.
         * - **4**: Reschedule unfinished tasks from today to the next day.
         * - **5**: Exit the application.
         *
         * Depending on the user's choice, the function invokes corresponding helper
         * functions to perform the requested actions.
         *
         * - **Invalid Input Handling**: If the user enters an invalid choice, a message
         *   is displayed, and the menu is shown again.
         * - **Exit**: Choosing option 5 exits the loop and terminates the application.
         *
         * @see loadAndDisplayTasksForToday()
         * @see addTaskForToday()
         * @see runTaskCreation()
         * @see markTaskAsDone()
         * @see rescheduleUnfinishedTasks()
         */
        void runApplication() {
            int choice = 0;

            while (true) {
                loadAndDisplayTasksForToday();
                std::cout << "\nOptions:" << std::endl;
                std::cout << "1 - Add task for today" << std::endl;
                std::cout << "2 - Add task (custom date)" << std::endl;
                std::cout << "3 - Mark task as done" << std::endl;
                std::cout << "4 - Reschedule tasks from today to next day" << std::endl;
                std::cout << "5 - Exit" << std::endl;

                std::cout << "Choose an option: ";
                std::cin >> choice;

                switch (choice) {
                    case 1:
                        addTaskForToday();
                        break;
                    case 2:
                        runTaskCreation();
                        break;
                    case 3:
                        markTaskAsDone();
                        break;
                    case 4:
                        rescheduleUnfinishedTasks();
                        break;
                    case 5:
                        std::cout << "Exiting application..." << std::endl;
                        return;
                    default:
                        std::cout << "Invalid option. Please choose between 1 and 4." << std::endl;
                        break;
                }
            }
        }

    private:

        /**
         * @brief Retrieves the current date in the format "DD.MM.YYYY".
         *
         * This function gets the current date by using the system's local time and formats it
         * into a string representation with the format "day.month.year" (e.g., "09.01.2025").
         * The formatted date string is returned as a `std::string`.
         *
         * @return A `std::string` containing the current date in the format "DD.MM.YYYY".
         *
         * @note The function uses the system's local time and relies on the `strftime` function
         *       to format the date.
         */
        std::string getTodayDate() {
            time_t t = time(0);
            tm* now = localtime(&t);
            char buf[11];
            strftime(buf, sizeof(buf), "%d.%m.%Y", now);
            return std::string(buf);
        }

        /**
         * @brief Loads and displays tasks for today.
         *
         * This function retrieves the current date and uses it to load and display tasks
         * for today from three categories: Study, Life, and Work. It uses the `loadTasks` 
         * function to load tasks from corresponding file paths for each category and 
         * then displays them using the `displayTasks` function.
         *
         * The function performs the following actions:
         * - Retrieves the current date using `getTodayDate()`.
         * - Loads tasks for today from files specific to Study, Life, and Work categories.
         * - Displays the loaded tasks for each category with the appropriate labels.
         *
         * @see getTodayDate()
         * @see loadTasks()
         * @see displayTasks()
         */
        void loadAndDisplayTasksForToday() {
            std::string today = getTodayDate();
            std::cout << "\nTasks for today (" << today << "):\n";

            std::vector<StudyTask> studyTasks = loadTasks<StudyTask>(StudyTask::FILE_PATH, today);
            std::vector<LifeTask> lifeTasks = loadTasks<LifeTask>(LifeTask::FILE_PATH, today);
            std::vector<WorkTask> workTasks = loadTasks<WorkTask>(WorkTask::FILE_PATH, today);

            displayTasks("Study Tasks", studyTasks, 31);
            displayTasks("Life Tasks", lifeTasks, 33);
            displayTasks("Work Tasks", workTasks, 32);
        }

        /**
         * @brief Loads tasks from a file for a specific date.
         *
         * This function loads tasks from the specified file for a given date. The tasks are filtered
         * based on the provided date, and only those that are scheduled for today (as determined by
         * `getTodayDate()`) are returned. It ensures that the template type `T` is a subclass of `Task`.
         *
         * The function performs the following steps:
         * - Opens the specified file and reads each line.
         * - For each line, attempts to load the task using the `loadFromStream` method.
         * - Checks if the task is scheduled for today by comparing the date with `getTodayDate()`.
         * - Returns a vector of tasks that are scheduled for today.
         *
         * @tparam T The type of task to load. It must derive from the `Task` class.
         *
         * @param filePath The path to the file containing the task data.
         * @param date The date for which tasks should be loaded.
         *
         * @return A `std::vector<T>` containing tasks for the specified date.
         *
         * @throws std::ios_base::failure If there is an error opening the file.
         *
         * @see Task
         * @see getTodayDate()
         * @see loadFromStream()
         */
        template <typename T>
        std::vector<T> loadTasks(const std::string& filePath, const std::string& date) {
                static_assert(std::is_base_of<Task, T>::value, "T must derive from Task");

                std::ifstream file(filePath);
                std::vector<T> tasks;

                if (!file.is_open()) {
                    std::cerr << "Error: Could not open file: " << filePath << "\n";
                    return tasks;
                }

                std::string line;
                
                while (std::getline(file, line)) {
                    T task;
                    std::istringstream ss(line);
                    
                    if (task.loadFromStream(ss) && trim_left(task.getWhenToDo()) == getTodayDate()) {
                        tasks.push_back(task);
                    }
            }

            return tasks;
        }

        /**
         * @brief Trims leading spaces from a string.
         *
         * This function removes any leading spaces (whitespace characters) from the input string.
         * It finds the first non-space character and returns a substring starting from that character.
         * If the string contains only spaces, it returns an empty string.
         *
         * @param str The string to be trimmed.
         *
         * @return A `std::string` with leading spaces removed. If the input string consists only of spaces,
         *         an empty string is returned.
         *
         * @note This function only removes spaces from the beginning of the string.
         */
        std::string trim_left(const std::string& str) {
            size_t start = str.find_first_not_of(' ');
            return (start == std::string::npos) ? "" : str.substr(start);
        }

        /**
         * @brief Resets the console text color to the default.
         *
         * This function resets the text color of the console to its default color by sending
         * the appropriate ANSI escape sequence. It is typically used to clear any custom text
         * color formatting applied in previous console output.
         *
         * @note This function assumes that the console supports ANSI escape codes for text color formatting.
         */
        void resetColor() {
            std::cout << "\033[0m";
        }

        /**
         * @brief Displays a list of tasks with a title and color formatting.
         *
         * This function displays a list of tasks with a given title, applying color formatting
         * to the title text using ANSI escape codes. If no tasks are available, it prints "No tasks".
         * For each task, it prints its index and the task itself. The tasks are displayed in the order
         * they appear in the provided vector.
         *
         * @tparam Task The type of tasks in the vector. This function works with any task type that
         *              supports streaming (i.e., can be printed using `std::cout`).
         *
         * @param title The title to display above the task list.
         * @param tasks The vector of tasks to be displayed.
         * @param color The color code for the title text (e.g., 31 for red, 32 for green).
         *
         * @note The function uses ANSI escape codes to apply color formatting to the title.
         *       The `resetColor` function is called after displaying the title to reset the text color.
         */
        template <typename Task>
        void displayTasks(const std::string& title, const std::vector<Task>& tasks, int color) {
            std::cout << "\033[" << color << "m";
            std::cout << "----- " << title << " -----\n\n";
            resetColor();

            if (tasks.empty()) {
                std::cout << "No tasks.\n";
            } else {
                for (size_t i = 0; i < tasks.size(); ++i) {
                    std::cout << i + 1 << ". " << tasks[i] << "\n";
                }
            }
        }

        /**
         * @brief Adds a task for today based on user input.
         *
         * This function prompts the user to choose a task type (Study, Life, or Work) and creates
         * the corresponding task for today. It retrieves today's date and then calls the appropriate
         * function to create the selected task.
         *
         * @note If the user enters an invalid task type, an error message is displayed.
         */
        void addTaskForToday() {
            std::string today = getTodayDate();
            std::cout << "Adding a task for today (" << today << ").\n";

            int type = chooseTaskType();
            switch (type) {
                case 1:
                    createStudyTask(today);
                    break;
                case 2:
                    createLifeTask(today);
                    break;
                case 3:
                    createWorkTask(today);
                    break;
                default:
                    std::cout << "Invalid task type. Please choose 1, 2, or 3.\n";
                    break;
            }
        }

        /**
         * @brief Prompts the user to choose a task type (Study, Life, or Work).
         *
         * This function repeatedly asks the user to select a task type until a valid option (1, 2, or 3) is entered.
         * It returns the chosen task type as an integer.
         *
         * @return The task type chosen by the user (1 for Study, 2 for Life, or 3 for Work).
         *
         * @note If the user enters an invalid choice, the function will prompt them again until a valid option is selected.
         */
        int chooseTaskType() {
            int type = 0;
            while (type < 1 || type > 3) {
                std::cout << "What type of task?\n";
                std::cout << "1 - Study\n2 - Life\n3 - Work\n";
                std::cout << "Choose an option: ";
                std::cin >> type;
                if (type < 1 && type > 3) {
                    std::cout << "Invalid choice. Please select 1, 2, or 3.\n";
                }
            }
            return type;
        }

        /**
         * @brief Marks a task as done by removing it from the task list.
         *
         * This function allows the user to select a task type (Study, Life, or Work) 
         * and presents a list of tasks of the chosen type. The user can then choose a 
         * task to mark as done. The selected task is removed from the corresponding 
         * task file, and the updated list is saved back to the file.
         * 
         * - Prompts the user to choose a task type.
         * - Displays a list of tasks for the selected type.
         * - Allows the user to select a task by its number.
         * - Removes the selected task from the file.
         * - Displays a confirmation message once the task is marked as done.
         *
         * @note If no tasks are available or the user enters an invalid task number, 
         *       an appropriate message is shown and the operation is aborted. 
         *       The task files are updated accordingly after a task is removed.
         */
        void markTaskAsDone() {
            int type = chooseTaskType();

            std::string filePath;
            switch (type) {
                case 1:
                    filePath = StudyTask::FILE_PATH;
                    break;
                case 2:
                    filePath = LifeTask::FILE_PATH;
                    break;
                case 3:
                    filePath = WorkTask::FILE_PATH;
                    break;
            }

            std::ifstream file(filePath);
            if (!file) {
                std::cout << "No tasks of this type.\n";
                return;
            }

            std::vector<std::string> lines;
            std::string line;
            while (std::getline(file, line)) {
                lines.push_back(line);
            }
            file.close();

            if (lines.empty()) {
                std::cout << "No tasks to mark as done.\n";
                return;
            }

            std::cout << "Select the task to mark as done:\n";
            for (size_t i = 0; i < lines.size(); ++i) {
                std::cout << i + 1 << ". " << lines[i] << "\n";
            }

            int taskNumber;
            while (true) {
                std::cout << "Enter task number: ";
                std::cin >> taskNumber;
                if (taskNumber > 0 && taskNumber <= lines.size()) break;
                std::cout << "Invalid task number.\n";
            }

            lines.erase(lines.begin() + taskNumber - 1);
            std::ofstream outFile(filePath);
            for (const auto& l : lines) {
                outFile << l << "\n";
            }
            std::cout << "Task marked as done and removed from the list.\n";
        }

        /**
         * @brief Reschedules unfinished tasks to the next day.
         *
         * This function retrieves today's date and calculates the next day's date.
         * It then reschedules any unfinished tasks (Study, Life, Work) by updating 
         * their due dates to the next day.
         *
         * @note After rescheduling, a confirmation message is displayed.
         */
        void rescheduleUnfinishedTasks() {
            std::string today = getTodayDate();
            std::string nextDay = getNextDay(today);

            rescheduleTasks<StudyTask>(StudyTask::FILE_PATH, today, nextDay);
            rescheduleTasks<LifeTask>(LifeTask::FILE_PATH, today, nextDay);
            rescheduleTasks<WorkTask>(WorkTask::FILE_PATH, today, nextDay);

            std::cout << "Rescheduled tasks for tomorrow!" << std::endl;
        }

        /**
         * @brief Returns the next day's date.
         *
         * This function takes today's date as input, calculates the next day's date, 
         * and returns it as a string in the format "dd.mm.yyyy".
         *
         * @param today The current date in "dd.mm.yyyy" format.
         * @return The next day's date in "dd.mm.yyyy" format.
         */
        std::string getNextDay(const std::string& today) {
            tm date = {};
            strptime(today.c_str(), "%d.%m.%Y", &date);
            time_t t = mktime(&date) + 86400;
            tm* nextDay = localtime(&t);

            char buf[11];
            strftime(buf, sizeof(buf), "%d.%m.%Y", nextDay);
            return std::string(buf);
        }

        /**
         * @brief Reschedules tasks from today to the next day.
         *
         * This function loads tasks from a specified file, checks if their due date is today, 
         * and reschedules them by updating their due date to the next day. The tasks are then 
         * saved back to the file with the updated due dates.
         *
         * @tparam T The type of task to reschedule (must derive from Task).
         * @param filePath The file path where the tasks are stored.
         * @param today The current date in "dd.mm.yyyy" format.
         * @param nextDay The next day's date in "dd.mm.yyyy" format.
         */
        template <typename T>
        void rescheduleTasks(const std::string& filePath, const std::string& today, const std::string& nextDay) {
            static_assert(std::is_base_of<Task, T>::value, "T must derive from Task");

            std::ifstream file(filePath);
            std::vector<T> tasks;

            if (!file.is_open()) {
                std::cerr << "Error: Could not open file: " << filePath << "\n";
            }

            std::string line;
            while (std::getline(file, line)) {
                T task;
                std::istringstream ss(line);
                if (task.loadFromStream(ss) && trim_left(task.getWhenToDo()) == today) {
                    task.setWhenToDo(nextDay);
                }
                tasks.push_back(task);
            }
            file.close();

            std::ofstream outFile(filePath);
            for (const auto& task : tasks) {
                outFile << task.toFileString();
            }
        }

        /**
         * @brief Runs the task creation menu.
         *
         * This function presents a menu to the user for creating different types of tasks 
         * (Study, Life, Work). The user selects the task type, and the corresponding task 
         * creation function is called. The user can exit the menu by selecting option 4.
         *
         * @note If an invalid option is selected, the user is prompted to choose a valid option.
         */
        void runTaskCreation() {
            short type = 0;

            while (true) {
                std::cout << "\nWhat type of task?" << std::endl;
                std::cout << "1 - Study" << std::endl;
                std::cout << "2 - Life" << std::endl;
                std::cout << "3 - Work" << std::endl;
                std::cout << "4 - Exit" << std::endl;

                std::cout << "Choose an option: ";
                std::cin >> type;

                if (type == 4) {
                    std::cout << "Exiting task creation..." << std::endl;
                    break;
                } else if (type < 1 || type > 4) {
                    std::cout << "Invalid option. Please choose between 1 and 4." << std::endl;
                    continue;
                }

                switch (type) {
                    case 1:
                        createStudyTask();
                        break;
                    case 2:
                        createLifeTask();
                        break;
                    case 3:
                        createWorkTask();
                        break;
                    case 4:
                        return;
                }
            }
        }
        
        /**
         * @brief Creates and saves a study task.
         *
         * This function prompts the user to provide details for a study task, such as 
         * description, date, deadline, priority, and subject. It then creates a 
         * `StudyTask` object with the provided details and appends it to the appropriate 
         * file for storage.
         *
         * @note If the file cannot be opened for writing, an error message is displayed.
         */
        void createStudyTask() {
            std::string description, when_to_do, deadline, priority, subject;

            std::cout << "Provide description: ";
            std::cin.ignore();
            std::getline(std::cin, description);

            std::cout << "When do you want to do it? (DD.MM.YYYY): ";
            std::getline(std::cin, when_to_do);

            std::cout << "What is your deadline? (DD.MM.YYYY): ";
            std::getline(std::cin, deadline);

            std::cout << "What is the priority? (low, medium, high): ";
            std::getline(std::cin, priority);

            std::cout << "What subject? ";
            std::getline(std::cin, subject);

            StudyTask studyTask(description, when_to_do, deadline, priority, subject);
            
            std::ofstream outFile(StudyTask::FILE_PATH, std::ios::app);
            if (outFile.is_open()) {
                outFile << studyTask.toFileString();
                outFile.close();
                std::cout << "Study task added to file for: " << when_to_do << "\n";
            } else {
                std::cerr << "Error: Unable to open file for writing.\n";
            }
        }

        /**
         * @brief Creates and saves a work task.
         *
         * This function prompts the user to provide details for a work task, such as 
         * description, date, deadline, priority, and assignee. It then creates a 
         * `WorkTask` object with the provided details and appends it to the appropriate 
         * file for storage.
         *
         * @note If the file cannot be opened for writing, an error message is displayed.
         */
        void createWorkTask() {
            std::string description, when_to_do, deadline, priority, assignedBy;

            std::cout << "Provide description: ";
            std::cin.ignore();
            std::getline(std::cin, description);

            std::cout << "When do you want to do it? (DD.MM.YYYY): ";
            std::getline(std::cin, when_to_do);

            std::cout << "What is your deadline? (DD.MM.YYYY): ";
            std::getline(std::cin, deadline);

            std::cout << "What is the priority? (low, medium, high): ";
            std::getline(std::cin, priority);

            std::cout << "Who is the assignee? ";
            std::getline(std::cin, assignedBy);

            WorkTask workTask(description, when_to_do, deadline, priority, assignedBy);
            
            std::ofstream outFile(WorkTask::FILE_PATH, std::ios::app);
            if (outFile.is_open()) {
                outFile << workTask.toFileString();
                outFile.close();
                std::cout << "Work task added to file for : " << when_to_do << "\n";
            } else {
                std::cerr << "Error: Unable to open file for writing.\n";
            }
        }

        /**
         * @brief Creates and saves a life task.
         *
         * This function prompts the user to input details for a life task, including 
         * description, date, deadline, and priority. It creates a `LifeTask` object 
         * with the provided data and appends it to the relevant file for storage.
         *
         * @note If the file cannot be opened for writing, an error message is displayed.
         */
        void createLifeTask() {
            std::string description, when_to_do, deadline, priority;

            std::cout << "Provide description: ";
            std::cin.ignore();
            std::getline(std::cin, description);

            std::cout << "When do you want to do it? (DD.MM.YYYY): ";
            std::getline(std::cin, when_to_do);

            std::cout << "What is your deadline? (DD.MM.YYYY): ";
            std::getline(std::cin, deadline);

            std::cout << "What is the priority? (low, medium, high): ";
            std::getline(std::cin, priority);

            LifeTask lifeTask(description, when_to_do, deadline, priority);

            std::ofstream outFile(LifeTask::FILE_PATH, std::ios::app);
            if (outFile.is_open()) {
                outFile << lifeTask.toFileString();
                outFile.close();
                std::cout << "Life task added to file for: " << when_to_do << "\n";
            } else {
                std::cerr << "Error: Unable to open file for writing.\n";
            }
        }

        /**
         * @brief Creates and saves a study task for a given date.
         *
         * This function prompts the user to input details for a study task, including 
         * description, deadline, priority, and subject. It creates a `StudyTask` object 
         * with the provided data and appends it to the relevant file for storage. 
         * The task is associated with the specified date (`when_to_do`).
         *
         * @param when_to_do The date the task is scheduled to be done (format: DD.MM.YYYY).
         * 
         * @note If the file cannot be opened for writing, an error message is displayed.
         */
        void createStudyTask(const std::string& when_to_do) {
            std::string description, deadline, priority, subject;

            std::cout << "Enter task description: ";
            std::cin.ignore();
            std::getline(std::cin, description);

            std::cout << "Enter deadline date (DD.MM.YYYY): ";
            std::getline(std::cin, deadline);

            std::cout << "Enter priority (low, medium, high): ";
            std::getline(std::cin, priority);

            std::cout << "What subject? ";
            std::getline(std::cin, subject);

            StudyTask studyTask(description, when_to_do, deadline, priority, subject);
            std::ofstream outFile(StudyTask::FILE_PATH, std::ios::app);
            if (outFile.is_open()) {
                outFile << studyTask.toFileString();
                outFile.close();
                std::cout << "Study task added to file for today: " << when_to_do << "\n";
            } else {
                std::cerr << "Error: Unable to open file for writing.\n";
            }
        }

        /**
         * @brief Creates and saves a life task for a given date.
         *
         * This function prompts the user to input details for a life task, including 
         * description, deadline, and priority. It creates a `LifeTask` object with 
         * the provided data and appends it to the relevant file for storage. 
         * The task is associated with the specified date (`when_to_do`).
         *
         * @param when_to_do The date the task is scheduled to be done (format: DD.MM.YYYY).
         * 
         * @note If the file cannot be opened for writing, an error message is displayed.
         */
        void createLifeTask(const std::string& when_to_do) {
            std::string description, deadline, priority;

            std::cout << "Enter task description: ";
            std::cin.ignore();
            std::getline(std::cin, description);

            std::cout << "Enter deadline date (DD.MM.YYYY): ";
            std::getline(std::cin, deadline);

            std::cout << "Enter priority (low, medium, high): ";
            std::getline(std::cin, priority);

            LifeTask lifeTask(description, when_to_do, deadline, priority);
            std::ofstream outFile(LifeTask::FILE_PATH, std::ios::app);
            if (outFile.is_open()) {
                outFile << lifeTask.toFileString();
                outFile.close();
                std::cout << "Life task added to file for today: " << when_to_do << "\n";
            } else {
                std::cerr << "Error: Unable to open file for writing.\n";
            }
        }

        /**
         * @brief Creates and saves a work task for a given date.
         *
         * This function prompts the user to input details for a work task, including 
         * description, deadline, priority, and assignee. It creates a `WorkTask` object 
         * with the provided data and appends it to the relevant file for storage. 
         * The task is associated with the specified date (`when_to_do`).
         *
         * @param when_to_do The date the task is scheduled to be done (format: DD.MM.YYYY).
         * 
         * @note If the file cannot be opened for writing, an error message is displayed.
         */
        void createWorkTask(const std::string& when_to_do) {
            std::string description, deadline, priority, assignedBy;

            std::cout << "Enter task description: ";
            std::cin.ignore();
            std::getline(std::cin, description);

            std::cout << "Enter deadline date (DD.MM.YYYY): ";
            std::getline(std::cin, deadline);

            std::cout << "Enter priority (low, medium, high): ";
            std::getline(std::cin, priority);

            std::cout << "Who is the assignee? ";
            std::getline(std::cin, assignedBy);

            WorkTask workTask(description, when_to_do, deadline, priority, assignedBy);

            std::ofstream outFile(WorkTask::FILE_PATH, std::ios::app);
            if (outFile.is_open()) {
                outFile << workTask.toFileString();
                outFile.close();
                std::cout << "Work task added to file for today: " << when_to_do << "\n";
            } else {
                std::cerr << "Error: Unable to open file for writing.\n";
            }
        }

    };
}


#endif