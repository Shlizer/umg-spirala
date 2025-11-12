#pragma once

#include <vector>
#include <memory>
#include "ITask.h"

using namespace std;

class TaskManager {
    vector<unique_ptr<ITask>> tasks;

public:
    void AddTask(std::unique_ptr<ITask> task) {
        tasks.push_back(std::move(task));
    }

    void Update(float deltaTime) {
        for (auto it = tasks.begin(); it != tasks.end();) {
            if ((*it)->Update(deltaTime))
                it = tasks.erase(it);
            else
                ++it;
        }
    }
};
