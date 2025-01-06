#ifndef TASK_SERVICE_HPP
#define TASK_SERVICE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "StudyTask.hpp"
#include "WorkTask.hpp"
#include "LifeTask.hpp"

class TaskService {
public:
    void runApplication() {
        loadAndDisplayTasksForToday();
        int choice = 0;

        while (true) {
            std::cout << "\nOptions:" << std::endl;
            std::cout << "1 - Add task for today" << std::endl;
            std::cout << "2 - Add task (custom date)" << std::endl;
            std::cout << "3 - Mark task as done" << std::endl;
            std::cout << "4 - Exit" << std::endl;

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
                    std::cout << "Exiting application..." << std::endl;
                    return;
                default:
                    std::cout << "Invalid option. Please choose between 1 and 4." << std::endl;
                    break;
            }
        }
    }

private:

    std::string getTodayDate() {
        time_t t = time(0);
        tm* now = localtime(&t);
        char buf[11];
        strftime(buf, sizeof(buf), "%d.%m.%Y", now);
        return std::string(buf);
    }

    void loadAndDisplayTasksForToday() {
        std::string today = getTodayDate();
        std::cout << "\nTasks for today (" << today << "):\n";

        std::vector<StudyTask> studyTasks = loadTasks<StudyTask>(StudyTask::FILE_PATH, today);
        std::vector<LifeTask> lifeTasks = loadTasks<LifeTask>(LifeTask::FILE_PATH, today);
        std::vector<WorkTask> workTasks = loadTasks<WorkTask>(WorkTask::FILE_PATH, today);

        displayTasks("Study Tasks", studyTasks);
        displayTasks("Life Tasks", lifeTasks);
        displayTasks("Work Tasks", workTasks);
    }

    template <typename T>
        std::vector<T> loadTasks(const std::string& filePath, const std::string& date) {
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

            if (task.loadFromStream(ss) && task.getWhenToDo() == date) {
                tasks.push_back(task);
            }
        }

        return tasks;
    }


    template <typename TaskType>
    void displayTasks(const std::string& title, const std::vector<TaskType>& tasks) {
        std::cout << "----- " << title << " -----\n";
        if (tasks.empty()) {
            std::cout << "No tasks.\n";
        } else {
            for (size_t i = 0; i < tasks.size(); ++i) {
                std::cout << i + 1 << ". " << tasks[i] << "\n";
            }
        }
    }

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

    int chooseTaskType() {
        int type;
        while (true) {
            std::cout << "What type of task?\n";
            std::cout << "1 - Study\n2 - Life\n3 - Work\n";
            std::cout << "Choose an option: ";
            std::cin >> type;
            if (type >= 1 && type <= 3) continue;;
            std::cout << "Invalid choice. Please select 1, 2, or 3.\n";
        }
        return type;
    }

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

    void rescheduleUnfinishedTasks() {
        std::string today = getTodayDate();
        std::string nextDay = getNextDay(today);

        rescheduleTasks<StudyTask>(StudyTask::FILE_PATH, today, nextDay);
        rescheduleTasks<LifeTask>(LifeTask::FILE_PATH, today, nextDay);
        rescheduleTasks<WorkTask>(WorkTask::FILE_PATH, today, nextDay);
    }

    std::string getNextDay(const std::string& today) {
        tm date = {};
        strptime(today.c_str(), "%d.%m.%Y", &date);
        time_t t = mktime(&date) + 86400; // Add 24 hours
        tm* nextDay = localtime(&t);

        char buf[11];
        strftime(buf, sizeof(buf), "%d.%m.%Y", nextDay);
        return std::string(buf);
    }

    template <typename TaskType>
    void rescheduleTasks(const std::string& filePath, const std::string& today, const std::string& nextDay) {
        std::ifstream file(filePath);
        if (!file) return;

        std::vector<TaskType> tasks;
        std::string line;
        while (std::getline(file, line)) {
            TaskType task;
            std::istringstream ss(line);
            if (task.loadFromStream(ss) && task.getWhenToDo() == today) {
                task.setWhenToDo(nextDay);
            }
            tasks.push_back(task);
        }
        file.close();

        std::ofstream outFile(filePath);
        for (const auto& task : tasks) {
            outFile << task.toFileString() << "\n";
        }
    }

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
            }
        }
    }
    
    void createStudyTask() {
        std::string description, when_to_do, deadline, priority, subject;

        collectCommonTaskDetails(description, when_to_do, deadline, priority);
        std::cout << "What subject? ";
        std::cin.ignore();
        std::getline(std::cin, subject);

        StudyTask studyTask(description, when_to_do, deadline, priority, subject);
        saveTaskToFile(studyTask, StudyTask::FILE_PATH);
    }

    void createWorkTask() {
        std::string description, when_to_do, deadline, priority, assignedBy;

        collectCommonTaskDetails(description, when_to_do, deadline, priority);
        std::cout << "Who is the assignee? ";
        std::cin.ignore();
        std::getline(std::cin, assignedBy);

        WorkTask workTask(description, when_to_do, deadline, priority, assignedBy);
        saveTaskToFile(workTask, WorkTask::FILE_PATH);
    }

    void createLifeTask() {
        std::string description, when_to_do, deadline, priority;

        collectCommonTaskDetails(description, when_to_do, deadline, priority);

        LifeTask lifeTask(description, when_to_do, deadline, priority);
        saveTaskToFile(lifeTask, LifeTask::FILE_PATH);
    }

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
    saveTaskToFile(studyTask, StudyTask::FILE_PATH);
    std::cout << "Study task added for today: " << when_to_do << "\n";
}

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
    saveTaskToFile(lifeTask, LifeTask::FILE_PATH);
    std::cout << "Life task added for today: " << when_to_do << "\n";
}

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
    saveTaskToFile(workTask, WorkTask::FILE_PATH);
    std::cout << "Work task added for today: " << when_to_do << "\n";
}


    void collectCommonTaskDetails(std::string& description, std::string& when_to_do, std::string& deadline, std::string& priority) {
        std::cin.ignore();

        std::cout << "Provide description: ";
        std::getline(std::cin, description);

        std::cout << "When do you want to do it? (DD.MM.YYYY): ";
        std::getline(std::cin, when_to_do);

        std::cout << "What is your deadline? (DD.MM.YYYY): ";
        std::getline(std::cin, deadline);

        std::cout << "What is the priority? (from 0 to 10): ";
        std::getline(std::cin, priority);
    }

    template <typename TaskType>
    void saveTaskToFile(const TaskType& task, const std::string& filePath) {
        std::ofstream file(filePath, std::ios::app);
        if (!file) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }
        task.saveToFile(file);
        file.close();
        std::cout << "Task saved successfully to " << filePath << "!" << std::endl;
    }
};

#endif