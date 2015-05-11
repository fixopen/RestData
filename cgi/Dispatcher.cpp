//
// Created by up duan on 8/5/15.
//

#include "Dispatcher.h"
#include <regex>

std::map<std::string, Executor> Dispatcher::handles_;

void Dispatcher::RegisterExecutor(std::string const& uriPatterns, Executor const& executor) {
    handles_.insert(std::make_pair(uriPatterns, executor));
}

void Dispatcher::Execute(Request& request) {
    for (auto& i : handles_) {
        if (regex_match(request.Uri, i.first)) {
            i.second.Execute(request);
            break;
        }
    }
}
