//
// Created by up duan on 18/4/15.
//

#include "Model.h"
#include "../text/StringOp.h"
#include "../log/Log.h"
#include "../yajl/api/yajl_tree.h"

#include <iostream>

using namespace Util::Text::StringOp;

namespace {
    const std::wstring logFilename = L"log.txt";
    const std::string logType = "db error";
    const std::string jsonLogType = "json error";
    int rowProcess(void *parameter, int argc, char **argv, char **columnName) {
        int result = 0;
        auto p = (std::vector<MetaInfo::data>*) (parameter);
        auto item = MetaInfo::data{};
        for (int i = 0; i < argc; ++i) {
            std::string v;
            if (argv[i]) {
                v = argv[i];
            }
            item.insert(std::make_pair(columnName[i], v));
        }
        p->push_back(item);
        return result;
    };
    auto arguments = std::vector<MetaInfo::data>();
    const std::string delimeter = ", ";
    const std::string conditionSep = " AND ";
}

void MetaInfo::CreateTable() {
    //using namespace std;

    auto columns = std::vector<std::string>(columns_.size());
    size_t index = 0;
    for (auto column : columns_) {
        columns[index++] = Quote(column.first) + " " + column.second;
    }
    auto columnDefines = std::string();
    for (auto column : columns) {
        columnDefines += column + delimeter;
    }
    columnDefines = columnDefines.substr(0, columnDefines.length() - delimeter.length());
    std::string command = "CREATE TABLE " + Quote(tableName_) + " ( " + columnDefines + " ) ";
    //cout << command << endl;
    char* error = nullptr;
    sqlite3_exec(database_, command.c_str(), 0, 0, &error);
    if (error) {
        Util::WriteLog(logFilename, error, logType);
        sqlite3_free(error);
    }
}

void MetaInfo::DropTable() {
    std::string command = "DROP TABLE " + Quote(tableName_);
    char* error = nullptr;
    sqlite3_exec(database_, command.c_str(), 0, 0, &error);
    if (error) {
        Util::WriteLog(logFilename, error, logType);
        sqlite3_free(error);
    }
}

std::vector<MetaInfo::data> MetaInfo::Select(std::vector<MetaInfo::condition> const& filter, std::vector<MetaInfo::order_by_item> const& orderBy, page_info const& pageInfo) {
    std::string columnNames;
    for (auto& column : columns_) {
        columnNames += Quote(column.first) + delimeter;
    }
    columnNames = columnNames.substr(0, columnNames.length() - delimeter.length());
    std::string whereClause;
    for (auto& condition : filter) {
        whereClause += Quote(condition.first) + " " + condition.second.first + " " + QuoteForDatabase(condition.second.second, columns_[condition.first]) + conditionSep;
    }
    if (whereClause != "") {
        whereClause = " WHERE " + whereClause.substr(0, whereClause.length() - conditionSep.length());
    }
    std::string orderByClause;
    for (auto& orderItem : orderBy) {
        orderByClause += Quote(orderItem.first) + " " + (orderItem.second == MetaInfo::Asc ? "ASC" : "DESC") + ", ";
    }
    if (orderByClause != "") {
        orderByClause = " ORDER BY " + orderByClause.substr(0, orderByClause.length() - delimeter.length());
    }
    std::string pageClause;
    if (pageInfo.first != -1 && pageInfo.second != -1) {
        pageClause = " LIMIT " + FromIntU((int const)pageInfo.second) + " OFFSET " + FromIntU((int const)pageInfo.first);
    }
    std::string command = "SELECT " + columnNames + " FROM " + Quote(tableName_) + whereClause + orderByClause + pageClause; //construct from filter and pageInfo
    //std::cout << command << std::endl;
    char* error = nullptr;
    arguments.clear();
    sqlite3_exec(database_, command.c_str(), rowProcess, &arguments, &error);
    if (error) {
        Util::WriteLog(logFilename, error, logType);
        sqlite3_free(error);
    }
    return arguments;
}

bool MetaInfo::Insert(data const& value) {
    bool result = true;
    std::string columnNames; //construct from value
    std::string columnValues; //construct from value
    for (auto& field : value) {
        columnNames += Quote(field.first) + delimeter;
        columnValues += QuoteForDatabase(field.second, columns_[field.first]) + delimeter;
    }
    columnNames = columnNames.substr(0, columnNames.length() - delimeter.length());
    columnValues = columnValues.substr(0, columnValues.length() - delimeter.length());
    std::string command = "INSERT INTO " + Quote(tableName_) + " ( " + columnNames + " ) VALUES ( " + columnValues + " )";
    //std::cout << command << std::endl;
    char* error = nullptr;
    sqlite3_exec(database_, command.c_str(), rowProcess, &arguments, &error);
    if (error) {
        Util::WriteLog(logFilename, error, logType);
        sqlite3_free(error);
        result = false;
    }
    return result;
}

bool MetaInfo::Delete(unsigned long long id) {
    bool result = true;
    std::string command = "DELETE FROM " + Quote(tableName_) + " WHERE " + Quote("id") + " = " + FromIntU((int const)id);
    char* error = nullptr;
    sqlite3_exec(database_, command.c_str(), rowProcess, &arguments, &error);
    if (error) {
        Util::WriteLog(logFilename, error, logType);
        sqlite3_free(error);
        result = false;
    }
    return result;
}

bool MetaInfo::Update(unsigned long long id, data const& value) {
    auto result = false;
    std::string columnsSet; //construct from value
    for (auto& field : value) {
        columnsSet += Quote(field.first) + " = " + QuoteForDatabase(field.second, columns_[field.first]) + delimeter;
    }
    columnsSet = columnsSet.substr(0, columnsSet.length() - delimeter.length());
    std::string command = "UPDATE " + Quote(tableName_) + " SET " + columnsSet + " WHERE " + Quote("id") + " = " + FromIntU((int const)id);
    //std::cout << command << std::endl;
    char* error = nullptr;
    sqlite3_exec(database_, command.c_str(), rowProcess, &arguments, &error);
    if (error) {
        Util::WriteLog(logFilename, error, logType);
        sqlite3_free(error);
    } else {
        result = true;
    }
    return result;
}

std::string MetaInfo::QuoteForJson(std::string const& value, std::string const& type) {
    std::string result = value;
    if (type == "text") {
        result = Quote(value);
    }
    return result;
}

std::string MetaInfo::ToJson(data const& value) {
    std::string result = "{";
    for (auto field : value) {
        result += Quote(field.first) + ": " + QuoteForJson(field.second, columns_[field.first]) + delimeter;
    }
    result = result.substr(0, result.length() - delimeter.length());
    result += "}";
    return result;
}

std::string MetaInfo::ToJson(std::vector<data> const& values) {
    std::string result = "[";
    for (auto& value : values) {
        result += ToJson(value) + delimeter;
    }
    result = result.substr(0, result.length() - delimeter.length());
    result += "]";
    return result;
}

namespace {
    void fillMapByJson(std::map<std::string, std::string> const& types, yajl_val json, MetaInfo::data& data) {
        auto obj = YAJL_GET_OBJECT(json);
        for (size_t i = 0; i < obj->len; ++i) {
            std::string name = obj->keys[i];
            auto v = obj->values[i];
            std::string type = types.find(name)->second;
            std::string value;
            if (type == "integer") {
                value = YAJL_GET_NUMBER(v);
            } else if (type == "real") {
                value = YAJL_GET_NUMBER(v);
            } else if (type.compare(0, 7, "numeric") == 0) {
                value = YAJL_GET_NUMBER(v);
            }  else if (type == "text") {
                value = YAJL_GET_STRING(v);
            } else if (type == "blob") {
                value = "";
            }
            data.insert(std::make_pair(name, value));
        }
    }
}

MetaInfo::data MetaInfo::ParseJson(std::string const& v) {
    MetaInfo::data result;
    size_t const ErrorBufferLength = 32;
    char errorBuffer[ErrorBufferLength];
    yajl_val d = yajl_tree_parse(v.c_str(), errorBuffer, ErrorBufferLength);
    if (d) {
        fillMapByJson(columns_, d, result);
        yajl_tree_free(d);
    } else {
        Util::WriteLog(logFilename, errorBuffer, jsonLogType);
    }
    return result;
}

std::vector<MetaInfo::data> MetaInfo::ParseArrayJson(std::string const& v) {
    std::vector<data> result;
    size_t const ErrorBufferLength = 32;
    char errorBuffer[ErrorBufferLength];
    yajl_val d = yajl_tree_parse(v.c_str(), errorBuffer, ErrorBufferLength);
    if (d) {
        auto arr = YAJL_GET_ARRAY(d);
        for (size_t i = 0; i < arr->len; ++i) {
            MetaInfo::data item;
            auto v = arr->values[i];
            fillMapByJson(columns_, v, item);
            result.push_back(item);
        }
        yajl_tree_free(d);
    } else {
        Util::WriteLog(logFilename, errorBuffer, jsonLogType);
    }
    return result;
}
