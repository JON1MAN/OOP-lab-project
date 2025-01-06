#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Task {
protected:
    std::string description;
    std::string when_to_do;
    std::string deadline;
    std::string priority;

    bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    std::string incrementDate(const std::string& date) {

        if (date.empty() || date.size() != 10) return date;

        int day = std::stoi(date.substr(0, 2));
        int month = std::stoi(date.substr(3, 2));
        int year = std::stoi(date.substr(6, 4));

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if (isLeapYear(year)) {
            daysInMonth[1] = 29;
        }

        day++;

        if (day > daysInMonth[month - 1]) {
            day = 1;
            month++;
        }

        if (month > 12) {
            month = 1;
            year++;
        }

        std::ostringstream oss;
        oss << (day < 10 ? "0" : "") << day << "."
            << (month < 10 ? "0" : "") << month << "."
            << year;

        return oss.str();
    }

    void rescheduleToNextDay() {
        when_to_do = incrementDate(when_to_do);
        if (!deadline.empty()) {
            deadline = incrementDate(deadline);
        }
    }

public:

    Task() {}

    Task(const std::string& description, const std::string when_to_do) 
        : description(description), when_to_do(when_to_do) {}
        
    Task(
            const std::string& description, 
            const std::string when_to_do, 
            const std::string& deadline, 
            const std::string& priority)
        : description(description), when_to_do(when_to_do), deadline(deadline), priority(priority) {}

    virtual ~Task() = default;

    virtual void display() const = 0;

    virtual void saveToFile(std::ofstream& file) const = 0;

    virtual bool loadFromStream(std::istream& stream) = 0;

    virtual std::string toFileString() const;

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

#endif