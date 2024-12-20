#include <string>
#include <Task.hpp>

class StudiesTask : public Task {
private:
    std::string subject;
    bool isGroupWork;
    std::string materialsNeeded;

public:
    StudiesTask(const std::string& name, const std::string& description, const std::string& deadline, const std::string& priority,
                const std::string& subject, bool isGroupWork, const std::string& materialsNeeded)
        : Task(name, description, deadline, priority), subject(subject), isGroupWork(isGroupWork), materialsNeeded(materialsNeeded) {}

    const std::string& getSubject() const {
        return subject;
    }

    bool getIsGroupWork() const {
        return isGroupWork;
    }

    const std::string& getMaterialsNeeded() const {
        return materialsNeeded;
    }

    void setSubject(const std::string& newSubject) {
        subject = newSubject;
    }

    void setIsGroupWork(bool newIsGroupWork) {
        isGroupWork = newIsGroupWork;
    }

    void setMaterialsNeeded(const std::string& newMaterialsNeeded) {
        materialsNeeded = newMaterialsNeeded;
    }

    void execute() override {
    }
};