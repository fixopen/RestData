//
// Created by up duan on 8/5/15.
//

#ifndef RESTDATA_DISPATCHER_H
#define RESTDATA_DISPATCHER_H

#include <vector>
#include <map>
#include <string>
#include <regex>
//#include <unordered_map>
#include "CGI.h"
#include "../data/Model.h"

class Executor {
public:
    struct Parameters {
        std::vector<MetaInfo::condition> filter;
        unsigned long long offset;
        unsigned long long count;
        std::vector<MetaInfo::order_by_item> orderBy;
    };
    struct MIMEType {
        MIMEType(std::string const& type, std::string const& subtype)
        : type{type}
        , subType{subtype} {
            fullType = type + "/" + subtype;
        }
        std::string fullType;
        std::string type;
        std::string subType;
        std::map<std::string, std::string> parameters;
    };
    virtual void Execute(Request& request) = 0;
protected:
    Parameters parseParameters_(std::map<std::string, std::string> const& parameters) const;
    MIMEType parseMIMEType_(std::string const &v);
};

class Dispatcher {
public:
    static void RegisterExecutor(std::string const& uriPatterns, std::shared_ptr<Executor> const& executor);
    static void RegisterFilter(std::string const& uriPatterns, std::shared_ptr<Executor> const& filter, unsigned int order);
    static void Execute(Request& request);

private:
    static std::vector<std::pair<std::regex, std::shared_ptr<Executor>>> handles_;
};

#endif //RESTDATA_DISPATCHER_H
