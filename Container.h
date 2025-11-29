#pragma once
#include <vector>
#include "IEvent.h"
#include "Selectable.h"

using namespace std;

class Container : Selectable {
protected:
    vector<unique_ptr<Selectable>> elements;

public:
};