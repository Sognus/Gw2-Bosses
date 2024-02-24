#include "Globals.h"
#include "nlohmann/json.hpp"

#ifndef IJSONHANDLED_H
#define IJSONHANDLED_H
using json = nlohmann::json;

class IJsonHandled {
public:

    virtual ~IJsonHandled() {}

    virtual json ToJson() const = 0;
    virtual void FromJson(const json& json) = 0;
};

#endif // !IJSONHANDLED_H
