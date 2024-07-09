#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "json/json.h"

class Controller {
public:
    virtual Json::Value HandleRequest(const Json::Value& value) = 0;
};

#endif