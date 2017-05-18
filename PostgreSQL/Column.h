//
// Created by up duan on 19/7/2016.
//

#ifndef RESTDATA_COLUMN_H
#define RESTDATA_COLUMN_H

#include <map>
#include <string>
#include "DataType.h"

template<typename T>
class Column {
public:
    typedef typename T native_type;

    Column(std::string const& type, std::string const& name)
    : name_(name)
    , type_(type) {
    }

    DataType<T> scan(std::string const& v) {
        //
    }

    native_type const fromDatabase(std::string const& vp) {
        native_type result;
        return result;
    }

    std::string queryColumn() {
        //
    }

    std::string value() {
        //
    }

    std::string const toDatabase() {
        return fmt::format("{}", value_);
        //--or--
        //return "NULL"
    }

    bool isNull() {
        return true;
    }

    ~Column() = default;
private:
    std::string name_;
    std::string type_;
    std::map<std::string, std::string> constraits_;
    native_type value_;
    //DataType<T> value_;
};

#endif //RESTDATA_COLUMN_H
