//
// Created by up duan on 8/5/15.
//

#ifndef RESTDATA_DISPATCHER_H
#define RESTDATA_DISPATCHER_H

#include "CGI.h"

class Executor {
public:
    virtual void Execute(Request& request) = 0;
};

class Dispatcher {
public:
    static void RegisterExecutor(std::string const& uriPatterns, Executor const& executor);
    static void Execute(Request& request);

private:
    static std::map<std::string, Executor> handles_;
};

#endif //RESTDATA_DISPATCHER_H
