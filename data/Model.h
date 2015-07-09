//
// Created by up duan on 18/4/15.
//

#ifndef RESTDATA_MODEL_H
#define RESTDATA_MODEL_H

#include <iosfwd>
#include <map>
#include <string>
#include <vector>
#include "../sqlite/sqlite3.h"

template<typename T> class Model;

class MetaInfo {
    template<typename T> friend class Model;
public:
    enum Direction {
        Asc,
        Desc
    };
    typedef std::pair<std::string, Direction> order_by_item;
    typedef std::pair<std::string, std::string> condition_value;
    typedef std::pair<std::string, condition_value> condition;
    typedef std::map<std::string, std::string> data;
    typedef std::pair<unsigned long long, unsigned long long> page_info;
public:
    std::string GetTypeByName(std::string const& name) const {
        std::string result;
        auto i = columns_.find(name);
        if (i != columns_.end()) {
            result = i->first;
        }
        return result;
    }
    std::string Quote(std::string const &name) {
        return "\"" + name + "\"";
    }
    std::string QuoteForDatabase(std::string const& value, std::string const& type) {
        std::string result = value;
        if (type == "text") {
            result = "'" + value + "'";
        }
        return result;
    }
public:
    void MetaPrepare(std::string const& tableName, std::map<std::string, std::string> const& columns) {
        tableName_ = tableName;
        columns_ = columns;
        columns_.insert(std::make_pair("id", "integer primary key"));
    }
    void CreateTable();
    void DropTable();
    std::vector<data> Select(std::vector<condition> const& filter, std::vector<order_by_item> const& orderBy, page_info const& pageInfo);
    bool Insert(data const& value);
    bool Delete(unsigned long long id);
    bool Update(unsigned long long id, data const& value);
public:
    std::string QuoteForJson(std::string const& value, std::string const& type);
    std::string ToJson(MetaInfo::data const& value);
    std::string ToJson(std::vector<MetaInfo::data> const& values);
    MetaInfo::data ParseJson(std::string const& v);
    std::vector<MetaInfo::data> ParseArrayJson(std::string const& v);
protected:
    std::map<std::string, std::string> columns_;
    std::string tableName_;
    sqlite3* database_;
};

template<typename T>
class Model {
public:
    static void BindDatabase(sqlite3* database) {
        model_.database_ = database;
    }
    static void CreateTable() {
        model_.CreateTable();
    }
    static void DropTable() {
        model_.DropTable();
    }
    static std::vector<T> Select(std::vector<MetaInfo::condition> const& filter, std::vector<MetaInfo::order_by_item> const& orderBy, MetaInfo::page_info const& pageInfo) {
        std::vector<T> result;
        auto data = model_.Select(filter, orderBy, pageInfo);
        for (auto& datum : data) {
            auto item = T::ParseMap(data);
            //@@item.id_ = atoi(data["id"]);
            result.push_back(item);
        }
        return result;
    }
    bool Insert() {
        MetaInfo::data value = toMap_();
        //@@value.insert(std::make_pair("id", FromIntU(id_)));
        return model_.Insert(value);
    }
    bool Delete() {
        return model_.Delete(id_);
    }
    bool Update() {
        MetaInfo::data value = toMap_();
        //@@value.insert(std::make_pair("id", FromIntU(id_)));
        return model_.Update(id_, value);
    }

protected:
    virtual MetaInfo::data toMap_() const = 0;
    unsigned long long int id_ = 0;
    static MetaInfo model_;
};

template<typename T> MetaInfo Model<T>::model_;

#endif //RESTDATA_MODEL_H
