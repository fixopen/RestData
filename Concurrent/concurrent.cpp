#include "concurrent.h"

#include <string>
#include <chrono>
#include <iostream>

#include <thread>
#include <future>

template<typename T1, typename T2, typename TCallback>
auto add_async(T1&& v1, T2&& v2, TCallback&& token) {
    using adapter_type = modern_callback_adapter_t<std::decay<TCallback>, void(decltype(v1 + v2))>;
    auto adapter = typename adapter_type::traits(std::forward<TCallback>(token));
    std::thread([=, callback = std::move(std::get<0>(adapter))]{
        using namespace std::literals;
        std::this_thread::sleep_for(0.1s);
        callback(v1 + v2);
    }).detach();
    return std::move(std::get<1>(adapter)).get();
}

template<typename T1, typename T2, typename TCallback>
auto muldiv_async(T1&& v1, T2&& v2, TCallback&& token) {
    using adapter_type = modern_callback_adapter_t<std::decay<TCallback>, void(std::exception_ptr, decltype(v1 * v2), decltype(v1 / v2))>;
    auto adapter = typename adapter_type::traits(std::forward<TCallback>(token));
    std::thread([=, callback = std::move(std::get<0>(adapter))]{
        using namespace std::literals;
        std::this_thread::sleep_for(0.1s);
        auto v = v1 * v2;
        if (v2) {
            callback(nullptr, v, v1 / v2);
        } else {
            callback(std::make_exception_ptr(std::logic_error("divided by zero")), v, 0);
        }
    }).detach();
    return std::move(std::get<1>(adapter)).get();
}

//演示如何通过回调模型，扩展到支持future模式，调用链模式，以及协程。
//
//在其他线程里，将ValueT的输入值value，转化为std::string。然后调用callback(std::string &&)。
//
//具体做法是，并不直接保存callback进行回调，
//而是通过respone_callback_traits_t<>来获取真正的回调，以及本函数的返回值。
//本演示函数的CallableT是支持型如void(std::string)函数类型的可调用类型。
template<typename TValue, typename TCallable>
auto tostring_async(TValue&& value, TCallable&& callback)
/*-> typename modern_callback_adapter_t<std::decay_t<TCallable>, std::string>::return_type*/ {
    using callable_type = std::decay_t<TCallable>;
    using result_type = std::string;
    using adapter_type = modern_callback_adapter_t<callable_type, result_type>;

    auto adapter = typename adapter_type::traits(std::forward<TCallable>(callback));

    std::thread([callback = std::move(std::get<0>(adapter)), value = std::forward<TValue>(value)]{
        using namespace std::literals;
        callback(std::to_string(value));
    }).detach();

    return std::move(std::get<1>(adapter)).get();
}

void resumable_main_modern_callback() {
    using namespace std::literals;

    //auto nocare = tostring_async(1.0, [](std::string && value) {
    //    std::cout << value << std::endl;
    //});
    //std::cout << nocare << std::endl;

    //std::this_thread::sleep_for(1s);
    //std::cout << "......" << std::endl;

    std::future<std::string> f = tostring_async(5, std_future);
    std::cout << f.get() << std::endl;
}
