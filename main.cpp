//#include "data/Model.h"
#include "data/users.h"
#include "log/Log.h"
#include "data/TypeList.h"

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <numeric>
#include <chrono>
#include <utility>
#include <thread>
#include <future>
#include <mutex>
#include <atomic>
#include <cstdio>

//#include <wtypes.h>

//#include <objbase.h>
//#include <msxml6.h>
//#import <msxml6.dll> raw_interfaces_only
//using namespace MSXML2;

// new COM
//class HRESULT {
//public:
//private:
//    uint32_t state_;
//};

//class GUID {
//private:
//    uint8_t value_[16];
//};

//using IID = GUID;
//using CLASSID = GUID;

template <typename T>
class SmartPtr {
public:
    T* operator->();
};

class Interface {
public:
    virtual void release() noexcept;
    virtual ~Interface() noexcept;
};

using InterfacePtr = SmartPtr<Interface>;

class Unknown : public virtual Interface {
public:
    virtual HRESULT queryInterface(IID const& interfaceId, InterfacePtr& interfacePtr) noexcept {}
    virtual ~Unknown() noexcept {}
};

class Factory {
public:
    //SmartPtr<Unknown> create(CLSID const& classId) noexcept;
};

template <typename... TInterfaces>
class BizTem : public virtual Interface, public Unknown, public TInterfaces... {
public:
    void release() noexcept final {
        //
    }
    HRESULT queryInterface(IID const& interfaceId, InterfacePtr& interfacePtr) noexcept final {
        //
    }
    HRESULT bizProc(...) final;
};

class Biz : public virtual Interface {
public:
    virtual HRESULT bizProc(...);
};

class BizClass : public Biz, public Unknown {
public:
    void release() noexcept final;
    HRESULT queryInterface(IID const& interfaceId, InterfacePtr& interfacePtr) noexcept final;
    HRESULT bizProc(...) final;
};

// new MFC or OWL or wxWidgets or QT or ATL or WTL or Vaca or nana or genericGui or FLTK
// -- based type erase & coroutine & actor & event dispatcher & channel & executor
// new Archive or protobuf or Cap'n or Arrow or BER or TLV or JSON or SQL
// -- based static reflection

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
constexpr std::size_t arraySize(T (&)[N]) noexcept {
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

//typedef void *HANDLE;
//typedef unsigned int DWORD;
//#define WAIT_TIMEOUT (1)
//#define INFINITE (-1)
//#define TRUE 1

typedef size_t dim_t;
typedef size_t rank_t;
static constexpr size_t g_rank = 10;
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
        thread_ = std::thread(&ThreadInfo::HandleRange,
                              this); // the field hold the this, when ThreadInfo moved, this invalidation
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
    std::getchar();
    return 0;
}

//using LONG = long;
//static constexpr void* INVALID_HANDLE_VALUE = nullptr;

int main() {
    // cout << sum(3, 5) << sum(4, 7, 8) << endl;
    main_thread();
    main_future();
    main_mutex();
    main_map();
    main_sqlite();
    return 0;
}

template<typename... Args>
auto add(Args&&... args)
{
    return (... + args);
}

/*
 * The type must provide operator+
 * Only same types passed to arg
 * At least two parameter are required, such that the pack is not empty.
 * operator+ should be noexcept
 * and return an object of the same type
 */

// 1 First type struct which retrieves and stores the first type of a pack
template<typename T, typename...>
struct first_type
{
    using type = T;
};

// 2 Using alias for clean TMP
template<typename... Args>
using first_type_t = typename first_type<Args...>::type;

// 3 Check whether all types are the same
template<typename T, typename... Ts>
inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

// 4 Concept to compare a type against the first type of a parameter pack
template<typename T, typename... Args>
concept same_as_first_type =
std::is_same_v<std::remove_cvref_t<T>,
        std::remove_cvref_t<first_type_t<Args...>>>;

template<typename... Args>
requires requires(Args... args)
{
    (... + args);                  // 1 Simple requirement
    requires are_same_v<Args...>;  // 2 Nested requirement with type-trait
    requires sizeof...(Args) > 1;  // 3 Nested requirement with a boolean expression asserts at least 2 parameters
    {
        (... + args)
    }
    noexcept  // 4 Compound requirement ensuring noexcept
    ->same_as_first_type<Args...>;  // 5 Same compound requirement ensuring same type
}
auto add(Args&&... args)
{
    return (... + args);
}

// 1 Class template mock to create the different needed properties
template<bool NOEXCEPT, bool hasOperatorPlus, bool validReturnType>
class ObjectMock
{
public:
    ObjectMock() = default;

    // 2 Operator plus with controlled noexcept can be enabled
    ObjectMock& operator+(const ObjectMock& rhs) noexcept(NOEXCEPT) requires(
            hasOperatorPlus&& validReturnType)
    {
        return *this;
    }

    // 3 Operator plus with invalid return type
    int operator+(const ObjectMock& rhs) noexcept(NOEXCEPT) requires(
            hasOperatorPlus && not validReturnType)
    {
        return 3;
    }
};

// 4 Create the different mocks from the class template
using NoAdd               = ObjectMock<true, false, true>;
using ValidClass          = ObjectMock<true, true, true>;
using NotNoexcept         = ObjectMock<false, true, true>;
using DifferentReturnType = ObjectMock<false, true, false>;

template<typename... Args>
concept TestAdd =
requires(Args... args) // 1 Define a variadic concept as helper
{
add(args...); // 2 Call add by expanding the pack
};

// 1 Assert that type has operator+
static_assert(TestAdd<int, int, int>);
static_assert(not TestAdd<NoAdd, NoAdd>);

// 2 Assert, that no mixed types are allowed
static_assert(not TestAdd<int, double>);

// 3 Assert that pack has at least one parameter
static_assert(not TestAdd<int>);

// 4 Assert that operator+ is noexcept
static_assert(not TestAdd<NotNoexcept, NotNoexcept>);

// 5 Assert that operator+ returns the same type
static_assert(not TestAdd<DifferentReturnType, DifferentReturnType>);

// 6 Assert that a valid class works
static_assert(TestAdd<ValidClass, ValidClass>);

template<typename T, bool enable = true>
class SampleUseEnableIfWithError
{
public:
    std::enable_if_t<enable, int> DisableThisMethodOnRequest() { return 42; }
};

template<typename T, bool enable = true>
class Sample
{
public:
    int DisableThisMethodOnRequest() requires(enable) { return 42; }
};

template<typename T, bool enable = true>
class SampleUseEnableIf
{
public:
    template<typename Dummy = void>
    std::enable_if_t<enable, int> DisableThisMethodOnRequest()
    {
        return 42;
    }
};

template<typename T>
concept YourRequirementOrConcept = true;

template<typename T>
requires YourRequirementOrConcept<T>
void func();

template<typename T>
void func() requires YourRequirementOrConcept<T>;

template<YourRequirementOrConcept T>
void func();

#include <type_traits>

template<bool WithDefaultCtor>
class ClassWithOptionalDefaultCtorUseEnableIf
{
public:
    template<typename std::enable_if<WithDefaultCtor, int>::type = 0>
    ClassWithOptionalDefaultCtorUseEnableIf() // 1  we cannot say =default here
    {
    }
};

template<bool WithDefaultCtor>
class ClassWithOptionalDefaultCtor
{
public:
    ClassWithOptionalDefaultCtor() requires(WithDefaultCtor) =
    default; // 1  way better
};

int mainTest()
{
    ClassWithOptionalDefaultCtorUseEnableIf<true> te{};
    ClassWithOptionalDefaultCtor<true> t{};
}

// lambdaUnevaluatedContext.cpp

#include <cmath>
#include <iostream>
#include <memory>
#include <set>
#include <string>

template <typename Cont>
void printContainer(const Cont& cont) {
    for (const auto& c: cont) std::cout << c << "  ";
    std::cout << "\n";
}

int main2() {
    std::cout << std::endl;

    std::set<std::string> set1 = {"scott", "Bjarne", "Herb", "Dave", "michael"};
    printContainer(set1);

    using SetDecreasing = std::set<std::string, decltype([](const auto& l, const auto& r){ return l > r; })>;           // (1)
    SetDecreasing set2 = {"scott", "Bjarne", "Herb", "Dave", "michael"};
    printContainer(set2);     // (2)

    using SetLength = std::set<std::string, decltype([](const auto& l, const auto& r){ return l.size() < r.size(); })>; // (1)
    SetLength set3 = {"scott", "Bjarne", "Herb", "Dave", "michael"};
    printContainer(set3);     // (2)

    std::cout << std::endl;

    std::set<int> set4 = {-10, 5, 3, 100, 0, -25};
    printContainer(set4);

    using setAbsolute = std::set<int, decltype([](const auto& l, const auto& r){ return  std::abs(l)< std::abs(r); })>; // (1)
    setAbsolute set5 = {-10, 5, 3, 100, 0, -25};
    printContainer(set5);    // (2)

    std::cout << "\n\n";
}
