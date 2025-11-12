#pragma once

class ITask {
public:
    virtual ~ITask() = default;
    virtual bool Update(float deltaTime) = 0;
};
