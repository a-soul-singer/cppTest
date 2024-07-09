#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <string>

#include "json/json.h"

#include "controller.h"

class UserController : public Controller {
public:
    virtual Json::Value HandleRequest(const Json::Value& value) override;
};

#endif