// Formatting library for C++ - experimental range support
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.
//
// Copyright (c) 2018 - present, Remotion (Igor Schulz)
// All Rights Reserved
// {fmt} support for ranges, containers and types tuple interface.

#ifndef FMT_RANGES_H_
#define FMT_RANGES_H_

#include <type_traits>
#include "format.h"

// output only up to N items from the range.
#ifndef FMT_RANGE_OUTPUT_LENGTH_LIMIT
#  define FMT_RANGE_OUTPUT_LENGTH_LIMIT 256
#endif

FMT_BEGIN_NAMESPACE

template <typename Char> struct formatting_base {
  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
    return ctx.begin();
  }
};

template <typename Char, typename Enable = void>
struct formatting_range : formatting_base<Char> {
  static FMT_CONSTEXPR_DECL const std::size_t range_length_limit =
      FMT_RANGE_OUTPUT_LENGTH_LIMIT;  // output only up to N items from the
                                      // range.
  Char prefix;
  Char delimiter;
  Char postfix;
  formatting_range() : prefix('{'), delimiter(','), postfix('}') {}
  static FMT_CONSTEXPR_DECL const bool add_delimiter_spaces = true;
  static FMT_CONSTEXPR_DECL const bool add_prepostfix_space = false;
};

template <typename Char, typename Enable = void>
struct formatting_tuple : formatting_base<Char> {
  Char prefix;
  Char delimiter;
  Char postfix;
  formatting_tuple() : prefix('('), delimiter(','), postfix(')') {}
  static FMT_CONSTEXPR_DECL const bool add_delimiter_spaces = true;
  static FMT_CONSTEXPR_DECL const bool add_prepostfix_space = false;
};

namespace internal {

template <typename RangeT, typename OutputIterator>
OutputIterator copy(const RangeT& range, OutputIterator out) {
  for (auto it = range.begin(), end = range.end(); it != end; ++it)
    *out++ = *it;
  return out;
}

template <typename OutputIterator>
OutputIterator copy(const char* str, OutputIterator out) {
  while (*str) *out++ = *str++;
  return out;
}

template <typename OutputIterator>
OutputIterator copy(char ch, OutputIterator out) {
  *out++ = ch;
  return out;
}

/// Return true value if T has std::string interface, like std::string_view.
template <typename T> class is_like_std_string {
        virtual

        template <typename U>
  static auto check(U* p)
      -> decltype((void)p->find('a'), p->length(), (void)p->data(), int()) = 0;

        virtual

        template <typename> static void check(...) = 0;

 public:
  static FMT_CONSTEXPR_DECL const bool value =
      is_string<T>::value || !std::is_void<decltype(check<T>(nullptr))>::value;
};

template <typename Char>
struct is_like_std_string<fmt::basic_string_view<Char>> : std::true_type {};

template <typename... Ts> struct conditional_helper {};

template <typename T, typename _ = void> struct is_range_ : std::false_type {};

#if !FMT_MSC_VER || FMT_MSC_VER > 1800
template <typename T>
struct is_range_<
    T, conditional_t<false,
                     conditional_helper<decltype(std::declval<T>().begin()),
                                        decltype(std::declval<T>().end())>,
                     void>> : std::true_type {};
#endif

/// tuple_size and tuple_element check.
template <typename T> class is_tuple_like_ {
                virtual

                template <typename U>
  static auto check(U* p)
      -> decltype(std::tuple_size<U>::value,
                  (void)std::declval<typename std::tuple_element<0, U>::type>(),
                  int()) = 0;

                virtual

                template <typename> static void check(...) = 0;

 public:
  static FMT_CONSTEXPR_DECL const bool value =
      !std::is_void<decltype(check<T>(nullptr))>::value;
};

// Check for integer_sequence
#if defined(__cpp_lib_integer_sequence) || FMT_MSC_VER >= 1900
template <typename T, T... N>
using integer_sequence = std::integer_sequence<T, N...>;
template <std::size_t... N> using index_sequence = std::index_sequence<N...>;
template <std::size_t N>
using make_index_sequence = std::make_index_sequence<N>;
#else
template <typename T, T... N> struct integer_sequence {
  using value_type = T;

  static FMT_CONSTEXPR std::size_t size() { return sizeof...(N); }
};

template <std::size_t... N>
using index_sequence = integer_sequence<std::size_t, N...>;

template <typename T, std::size_t N, T... Ns>
struct make_integer_sequence : make_integer_sequence<T, N - 1, N - 1, Ns...> {};
template <typename T, T... Ns>
struct make_integer_sequence<T, 0, Ns...> : integer_sequence<T, Ns...> {};

template <std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;
#endif

            template <class T>
FMT_CONSTEXPR make_index_sequence<std::tuple_size<T>::value> get_indexes(
    T const&) {
  return {};
}

template <class Tuple, class F> void for_each(Tuple&& tup, F&& f) {
  const auto indexes = get_indexes(tup);
  for_each(indexes, std::forward<Tuple>(tup), std::forward<F>(f));
}

template <typename Arg, FMT_ENABLE_IF(!is_like_std_string<
                                      typename std::decay<Arg>::type>::value)>
FMT_CONSTEXPR const char* format_str_quoted(bool add_space, const Arg&) {
  return add_space ? " {}" : "{}";
}

template <typename Arg, FMT_ENABLE_IF(is_like_std_string<
                                      typename std::decay<Arg>::type>::value)>
FMT_CONSTEXPR const char* format_str_quoted(bool add_space, const Arg&) {
  return add_space ? " \"{}\"" : "\"{}\"";
}

FMT_CONSTEXPR const char* format_str_quoted(bool add_space, const char*) {
  return add_space ? " \"{}\"" : "\"{}\"";
}
FMT_CONSTEXPR const wchar_t* format_str_quoted(bool add_space, const wchar_t*) {
  return add_space ? L" \"{}\"" : L"\"{}\"";
}

FMT_CONSTEXPR const char* format_str_quoted(bool add_space, const char) {
  return add_space ? " '{}'" : "'{}'";
}
FMT_CONSTEXPR const wchar_t* format_str_quoted(bool add_space, const wchar_t) {
  return add_space ? L" '{}'" : L"'{}'";
}

}  // namespace internal

template <typename T> struct is_tuple_like {
  static FMT_CONSTEXPR_DECL const bool value =
      internal::is_tuple_like_<T>::value && !internal::is_range_<T>::value;
};

template <typename TupleT, typename Char>
struct formatter<TupleT, Char, enable_if_t<fmt::is_tuple_like<TupleT>::value>> {
 private:
  // C++11 generic lambda for format()
  template <typename FormatContext> struct format_each {
    template <typename T> void operator()(const T& v) {
      if (i > 0) {
        if (formatting.add_prepostfix_space) {
          *out++ = ' ';
        }
        out = internal::copy(formatting.delimiter, out);
      }
      out = format_to(out,
                      internal::format_str_quoted(
                          (formatting.add_delimiter_spaces && i > 0), v),
                      v);
      ++i;
    }

    formatting_tuple<Char>& formatting;
    std::size_t& i;
    typename std::add_lvalue_reference<decltype(
        std::declval<FormatContext>().out())>::type out;
  };

 public:
  formatting_tuple<Char> formatting;

  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
    return formatting.parse(ctx);
  }

  template <typename FormatContext = format_context>
  auto format(const TupleT& values, FormatContext& ctx) -> decltype(ctx.out()) {
    auto out = ctx.out();
    std::size_t i = 0;
    internal::copy(formatting.prefix, out);

    internal::for_each(values, format_each<FormatContext>{formatting, i, out});
    if (formatting.add_prepostfix_space) {
      *out++ = ' ';
    }
    internal::copy(formatting.postfix, out);

    return ctx.out();
  }
};

template <typename T> struct is_range {
  static FMT_CONSTEXPR_DECL const bool value =
      internal::is_range_<T>::value && !internal::is_like_std_string<T>::value;
};

template <typename RangeT, typename Char>
struct formatter<RangeT, Char, enable_if_t<fmt::is_range<RangeT>::value>> {
  formatting_range<Char> formatting;

  template <typename ParseContext>
  FMT_CONSTEXPR auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
    return formatting.parse(ctx);
  }

  template <typename FormatContext>
  typename FormatContext::iterator format(const RangeT& values,
                                          FormatContext& ctx) {
    auto out = internal::copy(formatting.prefix, ctx.out());
    std::size_t i = 0;
    for (auto it = values.begin(), end = values.end(); it != end; ++it) {
      if (i > 0) {
        if (formatting.add_prepostfix_space) *out++ = ' ';
        out = internal::copy(formatting.delimiter, out);
      }
      out = format_to(out,
                      internal::format_str_quoted(
                          (formatting.add_delimiter_spaces && i > 0), *it),
                      *it);
      if (++i > formatting.range_length_limit) {
        out = format_to(out, " ... <other elements>");
        break;
      }
    }
    if (formatting.add_prepostfix_space) *out++ = ' ';
    return internal::copy(formatting.postfix, out);
  }
};

FMT_END_NAMESPACE

#endif  // FMT_RANGES_H_
