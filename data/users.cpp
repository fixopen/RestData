//
// Created by up duan on 22/4/15.
//

#include <stdlib.h>
#include "users.h"
#include "../text/StringOp.h"

users users::ParseMap(MetaInfo::data const& v) {
    return users{};
}

MetaInfo::data users::toMap_() {
    return MetaInfo::data{};
}

void users::Execute(Request& request) {
    unsigned long long id = 0;
    auto paths = Util::Text::StringOp::Split(request.Uri, "/");
    if (request.Method == "GET" || request.Method == "DELETE") {
        std::string accept = request.Headers["Accept"][0];
        if (request.Headers.find("Accept") != request.Headers.end() &&
            parseMIMEType_(accept).fullType == "application/json") {
            //normal
            auto params = parseParameters_(request.Parameters);
            users user;
            //GET select
            auto users = user.model_.Select(params.filter, params.orderBy, MetaInfo::page_info{params.offset, params.count});
            request.Response.Body = user.model_.ToJson(users);
            //DELETE delete
            user.model_.Delete(id);
        } else {
            //binary
            //GET select
            //DELETE delete
        }
    }
    if (request.Method == "POST" || request.Method == "PUT" || request.Method == "PATCH") {
        std::string contentType = request.Headers["Content-Type"][0];
        if (request.Headers.find("Content-Type") != request.Headers.end() &&
            parseMIMEType_(contentType).fullType == "application/json") {
            //normal
            users user;
            user = users::ParseMap(user.model_.ParseJson(request.Body));
            //POST insert
            user.Insert();
            //PUT replace
            user.Delete();
            user.Insert();
            //PATCH update
            user.Update();
        } else {
            //binary
            //POST insert
            //PUT replace
            //PATCH update
        }
    }
}