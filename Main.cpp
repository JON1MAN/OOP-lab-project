/**
 * @file Main.cpp
 * @brief Main entry point for the task management application.
 * 
 * This file contains the `main` function, which is the entry point of the task management application.
 * It initializes the `TaskService` class and starts the application by calling its `runApplication` method.
 */

#include <iostream>
#include "TaskService.hpp"
#include <string>
using namespace am;

/**
 * @brief The main function for running the task management application.
 * 
 * This function creates an instance of `TaskService` and invokes the `runApplication` method to start
 * the task management system. The program will continue running until the user decides to exit.
 * 
 * @return int Exit status of the program. Returns 0 if the program executes successfully.
 */
int main() {
    // Create an instance of TaskService
    TaskService taskService;

    // Start the application
    taskService.runApplication();
    return 0;
}
