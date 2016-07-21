//
// Created by up duan on 15/11/2015.
//

#ifndef RESTDATA_DATATYPE_H
#define RESTDATA_DATATYPE_H


#include <string>

class DataType {
public:
    template<typename T>
    T const toCpp(){
        T result;
        return result;
    }

    std::string const toDatabase() {
        return std::string("");
    }

    std::string toQueryColumn() {
        return std::string("");
    }

    std::string toInsertValue() {
        return std::string("");
    }

    std::string const toJson() {
        return std::string("");
    }

    virtual ~DataType() = default;
private:
    //
};


#endif //RESTDATA_DATATYPE_H
