//
// Created by up duan on 19/7/2016.
//

#ifndef RESTDATA_COLUMN_H
#define RESTDATA_COLUMN_H

#include <string>
#include "DataType.h"

template<typename T>
class Column {
public:
    Column(std::string const& type, std::string const& name)
    : name_(name)
    , type_(type) {
    }

    DataType<T> scan(std::string const& v) {
        //
    }

    std::string queryColumn() {
        //
    }

    std::string value() {
        //
    }
private:
    std::string name_;
    std::string type_;
    DataType<T> value_;
};

#endif //RESTDATA_COLUMN_H
