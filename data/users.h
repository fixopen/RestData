//
// Created by up duan on 22/4/15.
//

#ifndef RESTDATA_USERS_H
#define RESTDATA_USERS_H


#include "Model.h"
#include "../cgi/Dispatcher.h"

class users : public Model<users>, Executor {
public:
    static void Prepare() {
        model_.MetaPrepare("users", std::map<std::string, std::string>{{"photo", "blob"}, {"description", "text"}, {"payment", "numeric(8, 2)"}, {"weight", "real"}});
    }
    static users ParseMap(MetaInfo::data const& v);
    virtual void Execute(Request& request) override;

protected:
    virtual MetaInfo::data toMap_() override;
};


#endif //RESTDATA_USERS_H
