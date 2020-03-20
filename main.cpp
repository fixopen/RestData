#include <iostream>
#include <map>
#include <utility>
#include <array>
//#include "data/Model.h"
#include "data/users.h"
#include "log/Log.h"
#include "data/TypeList.h"

#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
#include <mutex>
#include <atomic>

void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise) {
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum);  // Notify future
}

void do_work(std::promise<void> barrier) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barrier.set_value();
}

int main_future() {
    // Demonstrate using promise<int> to transmit a result between threads.
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(accumulate, numbers.begin(), numbers.end(),
                            std::move(accumulate_promise));

    // future::get() will wait until the future has a valid result and retrieves it.
    // Calling wait() before get() is not needed
    //accumulate_future.wait();  // wait for result
    std::cout << "result=" << accumulate_future.get() << '\n';
    work_thread.join();  // wait for thread completion

    // Demonstrate using promise<void> to signal state between threads.
    std::promise<void> barrier;
    std::future<void> barrier_future = barrier.get_future();
    std::thread new_work_thread(do_work, std::move(barrier));
    barrier_future.wait();
    new_work_thread.join();
    return 0;
}

#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;

void save_page(const std::string &url) {
    // simulate a long page fetch
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::string result = "fake content";

    std::lock_guard<std::mutex> guard(g_pages_mutex);
    g_pages[url] = result;
}

int main_mutex() {
    std::thread t1(save_page, "http://foo");
    std::thread t2(save_page, "http://bar");
    t1.join();
    t2.join();

    // safe to access g_pages without lock now, as the threads are joined
    for (const auto &pair : g_pages) {
        std::cout << pair.first << " => " << pair.second << '\n';
    }
    return 0;
}

using namespace std;

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept { // constexpr
    return N;
}

//int sum(int ...args) {
//    return 0 + ... + args;
//}

// void fillColor(ImageData const& image, Point const& p, Color c, std::function<bool(ImageData const& image, Point const p)> isBounds) {
//     if (!isBounds(image, p)) {
//         auto leftPoint = Point(p.x - 1, p.y);
//         fillColor(image, leftPoint, c, isBounds);
//         auto topPoint = Point(p.x, p.y - 1);
//         fillColor(image, topPoint, c, isBounds);
//         auto rightPoint = Point(p.x + 1, p.y);
//         fillColor(image, rightPoint, c, isBounds);
//         auto bottomPoint = Point(p.x, p.y + 1);
//         fillColor(image, bottomPoint, c, isBounds);
//     }
// }

struct A {
    std::string name;
    int age;

    // introduced for logging purposes only
    A() : age(-1) {
        std::cout << "Default ctor. ";
    }

    //explicit
    A(std::string s, int x) : name(std::move(s)), age(x) {
        std::cout << "Ctor. ";
    }

    A(A const &a) : name(a.name), age(a.age) {
        std::cout << "Copy ctor. ";
    }

    A(A &&a) noexcept : name(std::move(a.name)), age(a.age) {
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
        age = a.age;
        return *this;
    }

    ~A() noexcept {
        std::cout << "Dtor. ";
    }
};

typedef void *HANDLE;
typedef unsigned int DWORD;
#define INFINITE (-1)
#define TRUE 1

// RWMUTEX
class ReadWriteMutex {
private:
    HANDLE hChangeMap = nullptr;
    std::map<DWORD, HANDLE> Threads;
    DWORD wi = INFINITE;
public:
    ReadWriteMutex(const ReadWriteMutex &) = delete;

    ReadWriteMutex(ReadWriteMutex &&) = delete;

    ReadWriteMutex const &operator=(const ReadWriteMutex &) = delete;

    explicit ReadWriteMutex(bool D = false) {
        if (D) {
            wi = 10000;
        } else {
            wi = INFINITE;
        }
        hChangeMap = CreateMutex(0, 0, 0);
    }

    ~ReadWriteMutex() {
        CloseHandle(hChangeMap);
        hChangeMap = 0;
        for (auto &a : Threads) {
            CloseHandle(a.second);
        }
        Threads.clear();
    }

    HANDLE CreateIf(bool KeepReaderLocked = false) {
        auto tim = WaitForSingleObject(hChangeMap, INFINITE);
        if (tim == WAIT_TIMEOUT && wi != INFINITE) {
            OutputDebugString(L"LockRead debug timeout!");
        }
        DWORD id = GetCurrentThreadId();
        if (Threads[id] == 0) {
            HANDLE e0 = CreateMutex(0, 0, 0);
            Threads[id] = e0;
        }
        HANDLE e = Threads[id];
        if (!KeepReaderLocked) {
            ReleaseMutex(hChangeMap);
        }
        return e;
    }

    HANDLE LockRead() {
        auto z = CreateIf();
        auto tim = WaitForSingleObject(z, wi);
        if (tim == WAIT_TIMEOUT && wi != INFINITE) {
            OutputDebugString(L"LockRead debug timeout!");
        }
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
        if (tim == WAIT_TIMEOUT && wi != INFINITE) {
            OutputDebugString(L"LockWrite debug timeout!");
        }

        // We don't want to keep threads, the hChangeMap is enough
        for (auto &a : Threads) {
            ReleaseMutex(a.second);
        }

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
class lock {
private:
    mutable T t;
    mutable ReadWriteMutex m;

    class proxy {
        T *const p;
        ReadWriteMutex *m;
        int me;
        // append
        HANDLE f; // @@
    public:
        proxy(T *const _p, ReadWriteMutex *_m, int _me) : p(_p), m(_m), me(_me) {
            if (me == 2) {
                m->LockWrite();
            } else {
                f = m->LockRead(); // @@
            }
        }

        ~proxy() {
            if (me == 2) {
                m->ReleaseWrite();
            } else {
                m->ReleaseRead(f); // @@
            }
        }

        T *operator->() {
            return p;
        }

        const T *operator->() const {
            return p;
        }

        T *get() {
            return p;
        }

        const T *get() const {
            return p;
        }
    };

public:
    template<typename ...Args>
    explicit lock(Args ... args) : t(args...) {}

    proxy r() const {
        return proxy(&t, &m, 1);
    }

    proxy w() {
        return proxy(&t, &m, 2);
    }

    void readlock(std::function<void(const T &)> f) const {
        proxy mx(&t, &m, 1);
        f(*mx.get());
    }

    void writelock(std::function<void(T &)> f) {
        proxy mx(&t, &m, 2);
        f(*mx.get());
    }

    proxy operator->() {
        return w();
    }

    proxy operator->() const {
        return r();
    }
};

typedef size_t dim_t;
typedef size_t rank_t;
constexpr size_t g_rank = 10;
const size_t dims[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

template<typename... I>
dim_t offset(I... args) {
    static_assert(sizeof...(I) == g_rank, "invalid number of indexes");

    // TODO: expand the expression
    const std::array<dim_t, g_rank> offs{static_cast<dim_t>(args)...};
    dim_t off = 0;
    for (rank_t i = 0; i < g_rank; ++i) {
        off = off * dims[i] + offs[i];
    }
    return off;
}

int main_map() {
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
    m.emplace(std::make_pair(1, A("Ann", 63)));

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
    return 0;
}

int main_sqlite() {
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

class ThreadInfo;

std::vector<ThreadInfo> g_thread_info;
std::mutex print_m;

class ThreadInfo {
public:
    int begin_, end_;
    std::thread thread_;

    ThreadInfo(int begin, int end) : begin_(begin), end_(end) {
        thread_ = std::thread(&ThreadInfo::HandleRange, this); // the field hold the this, when ThreadInfo moved, this invalidation
    }
public:
    void HandleRange() {
        std::lock_guard<std::mutex> lk(print_m);
        std::cout << " " << begin_ << " " << end_ << std::endl;
    }
};

int main_thread() {
    // g_thread_info.reserve(10);
    for (int i = 0; i < 20; i += 5) {
        g_thread_info.emplace_back(i, i + 5);
    }
    getchar();
    return 0;
}

int main() {
    // cout << sum(3, 5) << sum(4, 7, 8) << endl;
    main_thread();
    main_future();
    main_mutex();
    main_map();
    main_sqlite();
    return 0;
}
