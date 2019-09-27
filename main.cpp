#include <iostream>
#include <map>
//#include "data/Model.h"
#include "data/users.h"
#include "log/Log.h"
#include "data/TypeList.h"

using namespace std;

//int sum(int ...args) {
//    return 0 + ... + args;
//}

void fillColor(ImageData const& image, Point const& p, Color c, std::function<bool(ImageData const& image, Point const p)> isBounds) {
    if (!isBounds(image, p)) {
        auto leftPoint = Point(p.x - 1, p.y);
        fillColor(image, leftPoint, c, isBounds);
        auto topPoint = Point(p.x, p.y - 1);
        fillColor(image, topPoint, c, isBounds);
        auto rightPoint = Point(p.x + 1, p.y);
        fillColor(image, rightPoint, c, isBounds);
        auto bottomPoint = Point(p.x, p.y + 1);
        fillColor(image, bottomPoint, c, isBounds);
    }
}

struct A {
    std::string name;
    int age;

    // introduced for logging purposes only
    A() {
        std::cout << "Default ctor. ";
    }

    //explicit
    A(std::string const &s, int x) : name(s), age(x) {
        std::cout << "Ctor. ";
    }

    A(A const &a) : name(a.name), age(a.age) {
        std::cout << "Copy ctor. ";
    }

    A(A &&a) noexcept : name(std::move(a.name)), age(std::move(a.age)) {
        std::cout << "Move ctor. ";
    }

    A &operator=(A const &a) {
        std::cout << "Copy assign. ";
        name = a.name;
        age = a.age;
        return *this;
    }

    A &operator=(A &&a) noexcept {
        std::cout << "Move assign. ";
        name = std::move(a.name);
        age = std::move(a.age);
        return *this;
    }

    ~A() noexcept {
        std::cout << "Dtor. ";
    }
};

typedef void* HANDLE;
typedef unsigned int DWORD;
#define INFINITE (-1)
#define TRUE 1

// RWMUTEX
class RWMUTEX {
private:
    HANDLE hChangeMap = 0;
    std::map<DWORD, HANDLE> Threads;
    DWORD wi = INFINITE;

    RWMUTEX(const RWMUTEX &) = delete;

    RWMUTEX(RWMUTEX &&) = delete;

    operator=(const RWMUTEX &) = delete;


public:

    RWMUTEX(bool D = false) {
        if (D)
            wi = 10000;
        else
            wi = INFINITE;
        hChangeMap = CreateMutex(0, 0, 0);
    }

    ~RWMUTEX() {
        CloseHandle(hChangeMap);
        hChangeMap = 0;
        for (auto &a : Threads)
            CloseHandle(a.second);
        Threads.clear();
    }

    HANDLE CreateIf(bool KeepReaderLocked = false) {
        auto tim = WaitForSingleObject(hChangeMap, INFINITE);
        if (tim == WAIT_TIMEOUT && wi != INFINITE)
            OutputDebugString(L"LockRead debug timeout!");
        DWORD id = GetCurrentThreadId();
        if (Threads[id] == 0) {
            HANDLE e0 = CreateMutex(0, 0, 0);
            Threads[id] = e0;
        }
        HANDLE e = Threads[id];
        if (!KeepReaderLocked)
            ReleaseMutex(hChangeMap);
        return e;
    }

    HANDLE LockRead() {
        auto z = CreateIf();
        auto tim = WaitForSingleObject(z, wi);
        if (tim == WAIT_TIMEOUT && wi != INFINITE)
            OutputDebugString(L"LockRead debug timeout!");
        return z;
    }

    void LockWrite() {
        CreateIf(true);

        // Wait for all
        vector<HANDLE> AllThreads;
        AllThreads.reserve(Threads.size());
        for (auto &a : Threads) {
            AllThreads.push_back(a.second);
        }

        auto tim = WaitForMultipleObjects((DWORD) AllThreads.size(), AllThreads.data(), TRUE, wi);
        if (tim == WAIT_TIMEOUT && wi != INFINITE)
            OutputDebugString(L"LockWrite debug timeout!");

        // We don't want to keep threads, the hChangeMap is enough
        for (auto &a : Threads)
            ReleaseMutex(a.second);

        // Reader is locked
    }

    void ReleaseWrite() {
        ReleaseMutex(hChangeMap);
    }

    void ReleaseRead(HANDLE f) {
        ReleaseMutex(f);
    }
};

template<typename T>
class tlock {
private:
    mutable T t;
    mutable RWMUTEX m;

    class proxy {
        T *const p;
        RWMUTEX *m;
        int me;
    public:
        proxy(T *const _p, RWMUTEX *_m, int _me) : p(_p), m(_m), me(_me) {
            if (me == 2)
                m->LockWrite();
            else
                m->LockRead();
        }

        ~proxy() {
            if (me == 2)
                m->ReleaseWrite();
            else
                m->ReleaseRead();
        }

        T *operator->() {
            return p;
        }

        const T *operator->() const {
            return p;
        }

        T *getp() {
            return p;
        }

        const T *getpc() const {
            return p;
        }
    };

public:
    template<typename ...Args>
    tlock(Args ... args) : t(args...) {}

    const proxy r() const {
        return proxy(&t, &m, 1);
    }

    proxy w() {
        return proxy(&t, &m, 2);
    }

    void readlock(std::function<void(const T &)> f) const {
        proxy mx(&t, &m, 1);
        f(*mx.getp());
    }

    void writelock(std::function<void(T &)> f) {
        proxy mx(&t, &m, 2);
        f(*mx.getp());
    }

    proxy operator->() {
        return w();
    }

    const proxy operator->() const {
        return r();
    }
};

int main() {
    // cout << sum(3, 5) << sum(4, 7, 8) << endl;

    auto m = std::map<int, A>{};
    // Ctor. Default ctor. Move assign. Dtor. Dtor.
    m[1] = A("Ann", 63);

    // Ctor. Move ctor. Move ctor. Dtor. Dtor. Dtor.
    m.insert({1, A("Ann", 63)});

    // C++17 structured bindings style
    // auto[it, ins] = m.insert({2, "Ann"});

    // (1) Ctor. Copy ctor. Move ctor. Dtor. Dtor. Dtor.
    // explicit A(std::string const& s, int x):name(s), age(x){
    //     std::cout << "Ctor. ";
    // }
    // won't compile now, copy-list-initialization prevented
    m.insert({1, {"Ann", 63}});

    // (2) Ctor. Move ctor. Move ctor. Dtor. Dtor. Dtor.
    auto[it, ins] = m.insert(std::make_pair(1, A("Ann", 63)));

    // (3) Ctor. Move ctor. Move ctor. Dtor. Dtor. Dtor.
    m.insert({1, A("Ann", 63)});

    // (4) Ctor. Move ctor. Move ctor. Dtor. Dtor. Dtor.
    m.emplace(std::make_pair(1, A("Ann", 63))):

    // (5) Ctor. Move ctor. Dtor. Dtor.
    m.emplace(1, A("Ann", 63));

    // (6) Doesn't compile. That is why try_emplace of C++17 is of interest
    // m.emplace(1, "Ann", 63);

    // (7) Ctor. Dtor.
    m.emplace(std::piecewise_construct,
              std::forward_as_tuple(1),
              std::forward_as_tuple("Ann", 63));

    auto a = A("Ann", 63);

    // Ctor. Move ctor. Move ctor. Dtor. Dtor. Dtor.
    m.insert(std::make_pair(1, std::move(a)));

    // Ctor. Move ctor. Move ctor. Dtor. Dtor. Dtor.
    m.insert({1, std::move(a)});

    // Ctor. Move ctor. Dtor. Dtor.
    m.emplace(1, std::move(a));

    // Ctor. Move ctor. Dtor. Dtor.
    m.emplace(std::piecewise_construct,
              std::forward_as_tuple(1),
              std::forward_as_tuple(std::move(a)));

    auto[it1, ins1] = m.try_emplace(1, "Ann", 63);
    auto[it2, ins2] = m.insert_or_assign(1, std::move(a));

    // might leak if allocation fails due to insufficient memory for an object A
    std::map<int, std::unique_ptr<A>> m2;
    m2.emplace(1, std::make_unique<A>("Ann", 63));

    Util::SetWriteLog(true);
    cout << "Hello, World!" << endl;
    sqlite3 *db = nullptr;
    int r = sqlite3_open("test.sqlite", &db);
    if (r == SQLITE_OK) {
        users::Prepare();
        users::BindDatabase(db);
        users::CreateTable();
        // std::cout << std::get<0>(std::make_tuple(1, "hello", 3.1415265)) << std::endl;
        //  static_assert(std::is_same<head::type<type_list<int, double, long>>::type, >)
        // Model::Insert(Model::data{{"id", "1"}, {"name", "zhangsan"}, {"sex", "1"}, {"birthday", "2014-01-01 14:38:19.331"}, {"weight", "72.38"}, {"payment", "3983.42"}});
        // Model::Delete(1);
        // Model::Insert(Model::data{{"id", "1"}, {"name", "zhangsan"}, {"sex", "1"}, {"birthday", "2014-01-01 14:38:19.331"}, {"weight", "72.38"}, {"payment", "3983.42"}});
        // Model::Update(1, Model::data{{"id", "1"}, {"name", "lisi"}, {"sex", "1"}, {"birthday", "2014-01-01 14:38:19.321"}, {"weight", "72.8"}, {"payment", "383.42"}});
        // auto v = Model::Select(vector<Model::condition>{{"id", {"=", "1"}}}, Model::page_info{-1, -1});
        // for (auto& i : v) {
        //     for (auto& t : i) {
        //         cout << t.first << " " << t.second << endl;
        //     }
        // }
        // Model m = Model();
        // Model::DropTable();
        sqlite3_close(db);
    }
    return 0;
}
