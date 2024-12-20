#include <string>
#include <Task.hpp>

class WorkTask : public Task {
private:
    std::string projectName;
    std::string assignedBy;
    std::string team;
    int estimatedHours;

public:
    WorkTask(const std::string& name, const std::string& description, const std::string& deadline, const std::string& priority,
             const std::string& projectName, const std::string& assignedBy, const std::string& team, int estimatedHours)
        : Task(name, description, deadline, priority), projectName(projectName), assignedBy(assignedBy), team(team), estimatedHours(estimatedHours) {}

    const std::string& getProjectName() const {
        return projectName;
    }

    const std::string& getAssignedBy() const {
        return assignedBy;
    }

    const std::string& getTeam() const {
        return team;
    }

    int getEstimatedHours() const {
        return estimatedHours;
    }

    void setProjectName(const std::string& newProjectName) {
        projectName = newProjectName;
    }

    void setAssignedBy(const std::string& newAssignedBy) {
        assignedBy = newAssignedBy;
    }

    void setTeam(const std::string& newTeam) {
        team = newTeam;
    }

    void setEstimatedHours(int newEstimatedHours) {
        estimatedHours = newEstimatedHours;
    }

    void execute() override {
    }
};
