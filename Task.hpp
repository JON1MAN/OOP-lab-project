#include <string>

class Task {
protected:
    std::string name;
    std::string description;
    std::string deadline;
    std::string priority;

public:
    Task(const std::string& name, const std::string& description, const std::string& deadline, const std::string& priority)
        : name(name), description(description), deadline(deadline), priority(priority) {}

    virtual ~Task() = default;

    virtual void execute() = 0;

    const std::string& getName() const {
        return name;
    }

    const std::string& getDescription() const {
        return description;
    }

    const std::string& getDeadline() const {
        return deadline;
    }

    const std::string& getPriority() const {
        return priority;
    }

    void setName(const std::string& newName) {
        name = newName;
    }

    void setDescription(const std::string& newDescription) {
        description = newDescription;
    }

    void setDeadline(const std::string& newDeadline) {
        deadline = newDeadline;
    }

    void setPriority(const std::string& newPriority) {
        priority = newPriority;
    }
};