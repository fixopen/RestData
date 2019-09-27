#pragma once

#include <string>
#include <chrono>
#include <iostream>

#include <thread>
#include <future>

struct modern_call_return_void_t {
    void get() {}
};

template<typename TCallable, typename TSignature>
struct modern_callback_adapter_t {
    using callback_type = TCallable;
    using return_type = modern_call_return_void_t;

    static std::tuple<callback_type, return_type> traits(TCallable&& callback) {
        return { std::forward<TCallable>(callback), {} };
    }
};

template<typename TPromiseTraits, typename TResult>
struct use_future_callback_base_t {
    using result_type = TResult;
    using promise_type = typename TPromiseTraits::template promise_type<result_type>;

    mutable promise_type promise_;

    auto get_future() const {
        return promise_.get_future();
    }
};

template<typename...>
struct use_future_callback_t;

template<typename TPromiseTraits>
struct use_future_callback_t<TPromiseTraits> : use_future_callback_base_t<TPromiseTraits, void> {
    using use_future_callback_base_t<TPromiseTraits, void>::use_future_callback_base_t;

    void operator()() const {
        promise_.set_value();
    }
};

template<typename TPromiseTraits>
struct use_future_callback_t<TPromiseTraits, std::exception_ptr> : use_future_callback_base_t<TPromiseTraits, void> {
    using use_future_callback_base_t<TPromiseTraits, void>::use_future_callback_base_t;

    void operator()(std::exception_ptr exceptionPtr) const {
        if (exceptionPtr == nullptr)
            promise_.set_exception(std::forward<std::exception_ptr>(exceptionPtr));
        else
            promise_.set_value(std::forward<TResult>(value)); // @@
    }
};

template<typename TPromiseTraits, typename TResult>
struct use_future_callback_t<TPromiseTraits, TResult> : use_future_callback_base_t<TPromiseTraits, TResult> {
    using use_future_callback_base_t<TPromiseTraits, TResult>::use_future_callback_base_t;

    template<typename TArg>
    void operator()(TArg&& arg) const {
        promise_.set_value(std::forward<TArg>(arg));
    }
};

template<typename TPromiseTraits, typename TResult>
struct use_future_callback_t<TPromiseTraits, std::exception_ptr, TResult> : use_future_callback_base_t<TPromiseTraits, TResult> {
    using use_future_callback_base_t<TPromiseTraits, TResult>::use_future_callback_base_t;

    template<typename TArg>
    void operator()(std::exception_ptr exceptionPtr, TArg&& arg) const {
        if (exceptionPtr)
            promise_.set_exception(std::forward<std::exception_ptr>(exceptionPtr));
        else
            promise_.set_value(std::forward<TArg>(arg));
    }
};

template<typename TPromiseTraits, typename... TResult>
struct use_future_callback_t<TPromiseTraits, TResult...> : use_future_callback_base_t<TPromiseTraits, std::tuple<TResult...>> {
    using use_future_callback_base_t<TPromiseTraits, std::tuple<TResult...>>::use_future_callback_base_t;

    template<typename... TArg>
    void operator()(TArg&&... arg) const {
        static_assert (sizeof...(TArg) == sizeof...(TResult), "");
        promise_.set_value(std::make_tuple(std::forward<TArg>(arg)...));
    }
};

template<typename TPromiseTraits, typename... TResult>
struct use_future_callback_t<TPromiseTraits, std::exception_ptr, TResult...> : use_future_callback_base_t<TPromiseTraits, std::tuple<TResult...>> {
    using use_future_callback_base_t<TPromiseTraits, std::tuple<TResult...>>::use_future_callback_base_t;

    template<typename... TArg>
    void operator()(std::exception_ptr exceptionPtr, TArg&&... arg) const {
        static_assert (sizeof...(TArg) == sizeof...(TResult), "");
        if (exceptionPtr)
            promise_.set_exception(std::forward<std::exception_ptr>(exceptionPtr));
        else
            promise_.set_value(std::make_tuple(std::forward<TArg>(arg)...));
    }
};

template<typename TFutureTraits, typename TResult>
struct use_future_return_t {
    using result_type = TResult;
    using future_type = typename TFutureTraits::template future_type<result_type>;

    future_type future_;

    use_future_return_t(future_type&& future) : future_(future) {}

    future_type get() {
        return std::move(future_);
    }
};

template<typename TCallback, typename... TResult>
struct modern_callback_adapter_impl_t {
    using traits_type = TCallback;
    using callback_type = use_future_callback_t<traits_type, TResult...>;
    using result_type = typename callback_type::result_type;
    using return_type = use_future_return_t<traits_type, result_type>;

    static std::tuple<callback_type, return_type> traits(const TCallback&) {
        callback_type callback{};
        auto future = callback.get_future();
        return { std::move(callback), std::move(future) };
    }
};

struct std_future_t {
    template<typename TResult>
    using promise_type = std::promise<TResult>;

    template<typename TResult>
    using future_type = std::future<TResult>;
};

inline constexpr std_future_t std_future{};

template<typename TR, typename... TResult>
struct modern_callback_adapter_t<std_future_t, TR(TResult...)> : modern_callback_adapter_impl_t<std_future_t, TResult...> {};

//struct use_librf_t {
//    template<typename TResult>
//    using promise_type = resumef::promise_t<TResult>;
//
//    template<typename TResult>
//    using future_type = resumef::future_t<TResult>;
//};

//inline constexpr use_librf_t use_librf{};

//template<typename TR, typename... TResult>
//struct modern_callback_adapter_t<use_librf_t, TR(TResult...)> : modern_callback_adapter_impl_t<use_librf_t, TResult...> {};
