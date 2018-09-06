#include <iostream>
//#include "data/Model.h"
#include "data/users.h"
#include "log/Log.h"
#include "data/TypeList.h"

using namespace std;

int sum(int ...args) {
    return 0 + ... + args;
}

int main() {
    cout << sum(3, 5) << sum(4, 7, 8) << endl;
    
    Util::SetWriteLog(true);
    cout << "Hello, World!" << endl;
    sqlite3* db = nullptr;
    int r = sqlite3_open("test.sqlite", &db);
    if (r == SQLITE_OK) {
        users::Prepare();
        users::BindDatabase(db);
        users::CreateTable();
        //std::cout << std::get<0>(std::make_tuple(1, "hello", 3.1415265)) << std::endl;
        // static_assert(std::is_same<head::type<type_list<int, double, long>>::type, >)
        //Model::Insert(Model::data{{"id", "1"}, {"name", "zhangsan"}, {"sex", "1"}, {"birthday", "2014-01-01 14:38:19.331"}, {"weight", "72.38"}, {"payment", "3983.42"}});
        //Model::Delete(1);
        //Model::Insert(Model::data{{"id", "1"}, {"name", "zhangsan"}, {"sex", "1"}, {"birthday", "2014-01-01 14:38:19.331"}, {"weight", "72.38"}, {"payment", "3983.42"}});
        //Model::Update(1, Model::data{{"id", "1"}, {"name", "lisi"}, {"sex", "1"}, {"birthday", "2014-01-01 14:38:19.321"}, {"weight", "72.8"}, {"payment", "383.42"}});
        //auto v = Model::Select(vector<Model::condition>{{"id", {"=", "1"}}}, Model::page_info{-1, -1});
        //for (auto& i : v) {
        //    for (auto& t : i) {
        //        cout << t.first << " " << t.second << endl;
        //    }
        //}
        //Model m = Model();
        //Model::DropTable();
        sqlite3_close(db);
    }
    return 0;
}
