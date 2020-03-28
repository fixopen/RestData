//
// Created by up duan on 8/5/15.
//

#include <stdlib.h>
#include "Dispatcher.h"

Executor::Parameters Executor::parseParameters_(std::map<std::string, std::string> const& parameters) const {
    std::vector<MetaInfo::condition> filter;
    std::vector<MetaInfo::order_by_item> orderBy;
    auto offset = (unsigned long long)-1ll;
    auto count = (unsigned long long)-1ll;
    for (auto& param : parameters) {
        if (param.first == "filter") {
            //
        } else if (param.first == "orderBy") {
            //
        } else if (param.first == "offset") {
            offset = (unsigned long long)atoll(param.second.c_str());
        } else if (param.first == "count") {
            count = (unsigned long long)atoll(param.second.c_str());
        } else {
            //
        }
    }
    return Executor::Parameters{filter, offset, count, orderBy};
};

Executor::MIMEType Executor::parseMIMEType_(std::string const &v) {
    Executor::MIMEType result{"application", "octet-stream"};
    return result;
}

std::vector<std::pair<std::regex, std::shared_ptr<Executor>>> Dispatcher::handles_;

void Dispatcher::RegisterExecutor(std::string const& uriPatterns, std::shared_ptr<Executor> const& executor) {
    handles_.emplace_back(std::regex(uriPatterns), executor);
}

void Dispatcher::Execute(Request& request) {
    for (auto& i : handles_) {
        if (std::regex_match(request.Uri, i.first)) {
            i.second->Execute(request);
            break;
        }
    }
}
