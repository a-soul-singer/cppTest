#ifndef OPT_LOG_CONTROLLER_H
#define OPT_LOG_CONTROLLER_H

#include <string>

#include "json/json.h"

#include "controller.h"

class OptLogController : public Controller
{
public:
    virtual Json::Value HandleRequest(const Json::Value& value) override;
};

#endif