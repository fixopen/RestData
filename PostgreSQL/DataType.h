//
// Created by up duan on 15/11/2015.
//

#ifndef RESTDATA_DATATYPE_H
#define RESTDATA_DATATYPE_H

#include <fmt/format.h>
#include <string>

template<typename T>
class DataType {
public:
    typedef typename T native_type;

    std::string const toDatabase() {
        return fmt::format("{}", value);
        //--or--
        //return "NULL"
    }

    native_type const fromDatabase(std::string const& vp) {
        native_type result;
        return result;
    }

    bool isNull() {
        return true;
    }

    ~DataType() = default;
private:
    native_type value;
};

template<>
class DataType<std::string> {
public:
    typedef typename std::string native_type;

    std::string const toDatabase() {
        return fmt::format("'{}'", value);
        //--or--
        //return "NULL"
    }

    native_type const fromDatabase(std::string const& vp) {
        return vp;
    }

    bool isNull() {
        return true;
    }

    ~DataType() = default;
private:
    native_type value;
};

#endif //RESTDATA_DATATYPE_H
