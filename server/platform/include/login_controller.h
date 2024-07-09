#ifndef LOGIN_CONTROLLER_H
#define LOGIN_CONTROLLER_H

#include <string>

#include "json/json.h"

#include "controller.h"

class LoginController : public Controller{
public:
    virtual Json::Value HandleRequest(const Json::Value& value) override;
};

#endif