#pragma once

#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

template<typename... T>
class IEvent {
public:
    using Listener = function<void(T...)>;

    int AddListener(Listener listener) {
        this->listeners.push_back({ ++this->nextId, move(listener) });
        return this->nextId;
    }

    void RemoveListener(int id) {
        this->listeners.erase(
            remove_if(this->listeners.begin(), this->listeners.end(),
                [id](auto& l) { return l.id == id; }),
            this->listeners.end()
        );
    }

    void Invoke(T... args) {
        for (auto& l : this->listeners)
            l.func(args...);
    }

private:
    struct Entry { int id; Listener func; };
    vector<Entry> listeners;
    int nextId = 0;
};
