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
        listeners.push_back({ ++nextId, move(listener) });
        return nextId;
    }

    void RemoveListener(int id) {
        listeners.erase(
            remove_if(listeners.begin(), listeners.end(),
                [id](auto& l) { return l.id == id; }),
            listeners.end()
        );
    }

    void Invoke(T... args) {
        for (auto& l : listeners)
            l.func(args...);
    }

private:
    struct Entry { int id; Listener func; };
    vector<Entry> listeners;
    int nextId = 0;
};
