//
// Created by up duan on 22/4/15.
//

#include "users.h"

users users::ParseMap(MetaInfo::data const& v) {
    return users{};
}

MetaInfo::data users::toMap_() {
    return MetaInfo::data{};
}

void users::Execute(Request& request) {
    if (request.Method == "GET" || request.Method == "DELETE") {
        if (request.Headers.find("Accept") && request.Headers["Accept"] == "application/json") {
            //normal
            //GET select
            //DELETE delete
        } else {
            //binary
            //GET select
            //DELETE delete
        }
    }
    if (request.Method == "POST" || request.Method == "PUT" || request.Method == "PATCH") {
        if (request.Headers.find("Content-Type") && request.Headers["Content-Type"] == "application/json") {
            //normal
            //POST insert
            //PUT replace
            //PATCH update
        } else {
            //binary
            //POST insert
            //PUT replace
            //PATCH update
        }
    }
}