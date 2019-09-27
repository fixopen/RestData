//
// Created by up duan on 23/6/2017.
//

#ifndef RESTDATA_TYPELIST_H
#define RESTDATA_TYPELIST_H

#include <cstddef>
#include <tuple>

template<typename... Types>
struct type_list;

template<template <typename...> class List, typename Head, typename... Rest>
struct erase_head {
    using type = typename std::enable_if<(List<Head, Rest...>::size > 0), type_list<Rest...>>::type;
};

template<template <typename...> class List, typename Head, typename... Rest>
struct erase_tail {
    using type = typename std::enable_if<(List<Head, Rest...>::size > 0), type_list<Rest...>>::type;
};

template<typename... Types>
struct type_list {
    using type = type_list<Types...>;

    static constexpr std::size_t size = sizeof...(Types);
};

struct head {
    template<typename... Types>
    using type = typename std::tuple_element<0, std::tuple<Types...>>::type;
};

struct rest {
    template<typename... Types>
    using type = typename erase_head<type_list, Types...>::type;
};

template<std::size_t index, typename... Types> struct at {
    using type = typename std::invoke_result<std::get<index>(std::tuple<Types...>)>::type;
//    using type = typename std::conditional<
//            index == 0,
//            typename head::type<Types...>,
//            typename rest::type<Types...>::template at<index - 1, Types...>::type
//    >::type;
};

template<typename T, typename... Types> struct find {
    static constexpr std::size_t value = std::conditional<
            std::is_same<T, typename head::type<Types...>>::value,
            std::integral_constant<std::size_t, 0>,
            std::integral_constant<std::size_t, 1 + rest::type<Types...>::template find<T, Types...>::value>
    >::type::value;
};

template<std::size_t start, std::size_t end, typename... Types> struct slice {
    using type = std::conditional<
            start == 0,
            std::conditional<
                    end == 0,
                    typename type_list<Types...>::type,
                    typename erase_tail<rest::type<Types...>::slice<0, end - 1, Types...>::type>::type
            >,
            typename rest::type::template slice<start - 1, end, Types...>::type
    >;
};

template<std::size_t start, std::size_t count, typename ...NewTypes> struct splice {
    //for insert, append, remove, update
};

//template<typename FirstType, typename ...TRest>
//struct type_list {
//    using type = type_list<FirstType, TRest...>;
//
//    static constexpr std::size_t size = 1 + sizeof...(TRest);
//
//    struct head {
//        using type = FirstType; //typename std::tuple_element<0, std::tuple<Types...>>::type;
//    };
//
//    struct rest {
//        using type = type_list<TRest...>;
//    };
//
//    template<std::size_t index> struct at {
//        //using type = std::result_of<std::get<index>(std::tuple<Types...>)>;
//    };
//
//    template<typename T> struct find {
//        static constexpr std::size_t value = std::conditional<
//                std::is_same<T, typename head::type>::value,
//                std::integral_constant<std::size_t, 0>,
//                std::integral_constant<std::size_t, 1 + rest::type::find<T>::value>
//        >::value;
//    };
//
//    template<std::size_t start, std::size_t end> struct slice {
//        //
//    };
//
//    template<std::size_t start, std::size_t count, typename ...NewTypes> struct splice {
//        //for insert, append, remove, update
//    };
//};

#endif //RESTDATA_TYPELIST_H
