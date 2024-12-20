#include <string>
#include <Task.hpp>

class LifeTask : public Task {
private:
    std::string location;
    std::string companion;
    std::string repeatFrequency;
    std::string notes;

public:
    LifeTask(const std::string& name, const std::string& description, const std::string& deadline, const std::string& priority,
             const std::string& location, const std::string& companion, const std::string& repeatFrequency, const std::string& notes)
        : Task(name, description, deadline, priority), location(location), companion(companion), repeatFrequency(repeatFrequency), notes(notes) {}

    const std::string& getLocation() const {
        return location;
    }

    const std::string& getCompanion() const {
        return companion;
    }

    const std::string& getRepeatFrequency() const {
        return repeatFrequency;
    }

    const std::string& getNotes() const {
        return notes;
    }

    void setLocation(const std::string& newLocation) {
        location = newLocation;
    }

    void setCompanion(const std::string& newCompanion) {
        companion = newCompanion;
    }

    void setRepeatFrequency(const std::string& newRepeatFrequency) {
        repeatFrequency = newRepeatFrequency;
    }

    void setNotes(const std::string& newNotes) {
        notes = newNotes;
    }

    void execute() override {

    }
};
