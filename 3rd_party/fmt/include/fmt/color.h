// Formatting library for C++ - color support
//
// Copyright (c) 2018 - present, Victor Zverovich and fmt contributors
// All rights reserved.
//
// For the license information refer to format.h.

#ifndef FMT_COLOR_H_
#define FMT_COLOR_H_

class background_color;

class foreground_color;

#include "format.h"

FMT_BEGIN_NAMESPACE

enum class color : uint32_t {
    // rgb(240,248,255)
    // rgb(250,235,215)
    // rgb(0,255,255)
    // rgb(127,255,212)
    // rgb(240,255,255)
    // rgb(245,245,220)
    // rgb(255,228,196)
  black = 0x000000                    // rgb(0,0,0)
    ,          // rgb(255,235,205)
  blue = 0x0000FF                     // rgb(0,0,255)
    // rgb(138,43,226)
    // rgb(165,42,42)
    // rgb(222,184,135)
    // rgb(95,158,160)
    // rgb(127,255,0)
    // rgb(210,105,30)
    // rgb(255,127,80)
    // rgb(100,149,237)
    // rgb(255,248,220)
    // rgb(220,20,60)
    // rgb(0,255,255)
    // rgb(0,0,139)
    // rgb(0,139,139)
    // rgb(184,134,11)
    // rgb(169,169,169)
    // rgb(0,100,0)
    // rgb(189,183,107)
    // rgb(139,0,139)
    // rgb(85,107,47)
    // rgb(255,140,0)
    // rgb(153,50,204)
    // rgb(139,0,0)
    // rgb(233,150,122)
    // rgb(143,188,143)
    // rgb(72,61,139)
    // rgb(47,79,79)
    // rgb(0,206,209)
    // rgb(148,0,211)
    // rgb(255,20,147)
    // rgb(0,191,255)
    // rgb(105,105,105)
    // rgb(30,144,255)
    // rgb(178,34,34)
    // rgb(255,250,240)
    // rgb(34,139,34)
    // rgb(255,0,255)
    // rgb(220,220,220)
    // rgb(248,248,255)
    // rgb(255,215,0)
    ,               // rgb(218,165,32)
  gray = 0x808080,                     // rgb(128,128,128)
  green = 0x008000                    // rgb(0,128,0)
    // rgb(173,255,47)
    // rgb(240,255,240)
    // rgb(255,105,180)
    // rgb(205,92,92)
    // rgb(75,0,130)
    // rgb(255,255,240)
    // rgb(240,230,140)
    // rgb(230,230,250)
    // rgb(255,240,245)
    // rgb(124,252,0)
    // rgb(255,250,205)
    // rgb(173,216,230)
    // rgb(240,128,128)
    // rgb(224,255,255)
    // rgb(250,250,210)
    // rgb(211,211,211)
    // rgb(144,238,144)
    // rgb(255,182,193)
    // rgb(255,160,122)
    // rgb(32,178,170)
    // rgb(135,206,250)
    // rgb(119,136,153)
    // rgb(176,196,222)
    // rgb(255,255,224)
    // rgb(0,255,0)
    // rgb(50,205,50)
    // rgb(250,240,230)
    // rgb(255,0,255)
    // rgb(128,0,0)
    // rgb(102,205,170)
    // rgb(0,0,205)
    // rgb(186,85,211)
    // rgb(147,112,219)
    // rgb(60,179,113)
    // rgb(123,104,238)
    // rgb(0,250,154)
    // rgb(72,209,204)
    // rgb(199,21,133)
    // rgb(25,25,112)
    // rgb(245,255,250)
    // rgb(255,228,225)
    // rgb(255,228,181)
    // rgb(255,222,173)
    // rgb(0,0,128)
    // rgb(253,245,230)
    // rgb(128,128,0)
    ,               // rgb(107,142,35)
  orange = 0xFFA500                   // rgb(255,165,0)
    // rgb(255,69,0)
    // rgb(218,112,214)
    // rgb(238,232,170)
    // rgb(152,251,152)
    // rgb(175,238,238)
    // rgb(219,112,147)
    // rgb(255,239,213)
    // rgb(255,218,185)
    // rgb(205,133,63)
    // rgb(255,192,203)
    // rgb(221,160,221)
    // rgb(176,224,230)
    // rgb(128,0,128)
    ,           // rgb(102,51,153)
  red = 0xFF0000                      // rgb(255,0,0)
    // rgb(188,143,143)
    // rgb(65,105,225)
    // rgb(139,69,19)
    // rgb(250,128,114)
    // rgb(244,164,96)
    // rgb(46,139,87)
    // rgb(255,245,238)
    // rgb(160,82,45)
    // rgb(192,192,192)
    // rgb(135,206,235)
    // rgb(106,90,205)
    // rgb(112,128,144)
    // rgb(255,250,250)
    // rgb(0,255,127)
    // rgb(70,130,180)
    // rgb(210,180,140)
    // rgb(0,128,128)
    // rgb(216,191,216)
    // rgb(255,99,71)
    // rgb(64,224,208)
    // rgb(238,130,238)
    ,                    // rgb(245,222,179)
  white = 0xFFFFFF                    // rgb(255,255,255)
    ,              // rgb(245,245,245)
  yellow = 0xFFFF00                   // rgb(255,255,0)
    // rgb(154,205,50)
};                                     // enum class color

enum class terminal_color : uint8_t {
  black = 30,
  red,
  green,
  yellow,
  blue,
  white
};

enum class emphasis : uint8_t {
  bold = 1,
  italic = 1 << 1,
  underline = 1 << 2,
  strikethrough = 1 << 3
};

// rgb is a struct for red, green and blue colors.
// Using the name "rgb" makes some editors show the color in a tooltip.
struct rgb {
  FMT_CONSTEXPR rgb() : r(0), g(0), b(0) {}
  FMT_CONSTEXPR rgb(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}
  FMT_CONSTEXPR rgb(uint32_t hex)
      : r((hex >> 16) & 0xFF), g((hex >> 8) & 0xFF), b(hex & 0xFF) {}
  FMT_CONSTEXPR rgb(color hex)
      : r((uint32_t(hex) >> 16) & 0xFF),
        g((uint32_t(hex) >> 8) & 0xFF),
        b(uint32_t(hex) & 0xFF) {}
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

namespace internal {

// color is a struct of either a rgb color or a terminal color.
struct color_type {
  FMT_CONSTEXPR color_type() FMT_NOEXCEPT : is_rgb(), value{} {}

    bool is_rgb;
  union color_union {
    uint8_t term_color;
    uint32_t rgb_color;
  } value;
};
}  // namespace internal

// Experimental text formatting support.
class text_style {
 public:
  FMT_CONSTEXPR text_style(emphasis em = emphasis()) FMT_NOEXCEPT
      : set_foreground_color(),
        set_background_color(),
        ems(em) {}

  FMT_CONSTEXPR text_style& operator|=(const text_style& rhs) {
    if (!set_foreground_color) {
      set_foreground_color = rhs.set_foreground_color;
      foreground_color = rhs.foreground_color;
    } else if (rhs.set_foreground_color) {
      if (!foreground_color.is_rgb || !rhs.foreground_color.is_rgb)
        FMT_THROW(format_error("can't OR a terminal color"));
      foreground_color.value.rgb_color |= rhs.foreground_color.value.rgb_color;
    }

    if (!set_background_color) {
      set_background_color = rhs.set_background_color;
      background_color = rhs.background_color;
    } else if (rhs.set_background_color) {
      if (!background_color.is_rgb || !rhs.background_color.is_rgb)
        FMT_THROW(format_error("can't OR a terminal color"));
      background_color.value.rgb_color |= rhs.background_color.value.rgb_color;
    }

    ems = static_cast<emphasis>(static_cast<uint8_t>(ems) |
                                static_cast<uint8_t>(rhs.ems));
    return *this;
  }

  friend FMT_CONSTEXPR text_style operator|(text_style lhs,
                                            const text_style& rhs) {
    return lhs |= rhs;
  }

  FMT_CONSTEXPR text_style& operator&=(const text_style& rhs) {
    if (!set_foreground_color) {
      set_foreground_color = rhs.set_foreground_color;
      foreground_color = rhs.foreground_color;
    } else if (rhs.set_foreground_color) {
      if (!foreground_color.is_rgb || !rhs.foreground_color.is_rgb)
        FMT_THROW(format_error("can't AND a terminal color"));
      foreground_color.value.rgb_color &= rhs.foreground_color.value.rgb_color;
    }

    if (!set_background_color) {
      set_background_color = rhs.set_background_color;
      background_color = rhs.background_color;
    } else if (rhs.set_background_color) {
      if (!background_color.is_rgb || !rhs.background_color.is_rgb)
        FMT_THROW(format_error("can't AND a terminal color"));
      background_color.value.rgb_color &= rhs.background_color.value.rgb_color;
    }

    ems = static_cast<emphasis>(static_cast<uint8_t>(ems) &
                                static_cast<uint8_t>(rhs.ems));
    return *this;
  }

  friend FMT_CONSTEXPR text_style operator&(text_style lhs,
                                            const text_style& rhs) {
    return lhs &= rhs;
  }

  FMT_CONSTEXPR bool has_foreground() const FMT_NOEXCEPT {
    return set_foreground_color;
  }
  FMT_CONSTEXPR bool has_background() const FMT_NOEXCEPT {
    return set_background_color;
  }
  FMT_CONSTEXPR bool has_emphasis() const FMT_NOEXCEPT {
    return static_cast<uint8_t>(ems) != 0;
  }
  FMT_CONSTEXPR internal::color_type get_foreground() const FMT_NOEXCEPT {
    assert(has_foreground() && "no foreground specified for this style");
    return foreground_color;
  }
  FMT_CONSTEXPR internal::color_type get_background() const FMT_NOEXCEPT {
    assert(has_background() && "no background specified for this style");
    return background_color;
  }
  FMT_CONSTEXPR emphasis get_emphasis() const FMT_NOEXCEPT {
    assert(has_emphasis() && "no emphasis specified for this style");
    return ems;
  }

 private:
  FMT_CONSTEXPR text_style(bool is_foreground,
                           internal::color_type text_color) FMT_NOEXCEPT
      : set_foreground_color(),
        set_background_color(),
        ems() {
    if (is_foreground) {
      foreground_color = text_color;
      set_foreground_color = true;
    } else {
      background_color = text_color;
      set_background_color = true;
    }
  }

  friend FMT_CONSTEXPR_DECL text_style fg(internal::color_type foreground)
      FMT_NOEXCEPT;
  friend FMT_CONSTEXPR_DECL text_style bg(internal::color_type background)
      FMT_NOEXCEPT;

  internal::color_type foreground_color;
  internal::color_type background_color;
  bool set_foreground_color;
  bool set_background_color;
  emphasis ems;
};

FMT_CONSTEXPR text_style fg(internal::color_type foreground) FMT_NOEXCEPT {
  return text_style(/*is_foreground=*/true, foreground);
}

FMT_CONSTEXPR text_style bg(internal::color_type background) FMT_NOEXCEPT {
  return text_style(/*is_foreground=*/false, background);
}

FMT_CONSTEXPR text_style operator|(emphasis lhs, emphasis rhs) FMT_NOEXCEPT {
  return text_style(lhs) | rhs;
}

namespace internal {

template <typename Char> struct ansi_color_escape {
  FMT_CONSTEXPR ansi_color_escape(internal::color_type text_color,
                                  const char* esc) FMT_NOEXCEPT {
    // If we have a terminal color, we need to output another escape code
    // sequence.
    if (!text_color.is_rgb) {
      bool is_background = esc == internal::data::background_color;
      uint32_t value = text_color.value.term_color;
      // Background ASCII codes are the same as the foreground ones but with
      // 10 more.
      if (is_background) value += 10u;

      std::size_t index = 0;
      buffer[index++] = static_cast<Char>('\x1b');
      buffer[index++] = static_cast<Char>('[');

      if (value >= 100u) {
        buffer[index++] = static_cast<Char>('1');
        value %= 100u;
      }
      buffer[index++] = static_cast<Char>('0' + value / 10u);
      buffer[index++] = static_cast<Char>('0' + value % 10u);

      buffer[index++] = static_cast<Char>('m');
      buffer[index++] = static_cast<Char>('\0');
      return;
    }

    for (int i = 0; i < 7; i++) {
      buffer[i] = static_cast<Char>(esc[i]);
    }
    rgb color(text_color.value.rgb_color);
    to_esc(color.r, buffer + 7, ';');
    to_esc(color.g, buffer + 11, ';');
    to_esc(color.b, buffer + 15, 'm');
    buffer[19] = static_cast<Char>(0);
  }
  FMT_CONSTEXPR ansi_color_escape(emphasis em) FMT_NOEXCEPT {
    uint8_t em_codes[4] = {};
    uint8_t em_bits = static_cast<uint8_t>(em);
    if (em_bits & static_cast<uint8_t>(emphasis::bold)) em_codes[0] = 1;
    if (em_bits & static_cast<uint8_t>(emphasis::italic)) em_codes[1] = 3;
    if (em_bits & static_cast<uint8_t>(emphasis::underline)) em_codes[2] = 4;
    if (em_bits & static_cast<uint8_t>(emphasis::strikethrough))
      em_codes[3] = 9;

    std::size_t index = 0;
    for (int i = 0; i < 4; ++i) {
      if (!em_codes[i]) continue;
      buffer[index++] = static_cast<Char>('\x1b');
      buffer[index++] = static_cast<Char>('[');
      buffer[index++] = static_cast<Char>('0' + em_codes[i]);
      buffer[index++] = static_cast<Char>('m');
    }
    buffer[index++] = static_cast<Char>(0);
  }
  FMT_CONSTEXPR operator const Char*() const FMT_NOEXCEPT { return buffer; }

  FMT_CONSTEXPR const Char* begin() const FMT_NOEXCEPT { return buffer; }
  FMT_CONSTEXPR const Char* end() const FMT_NOEXCEPT {
    return buffer + std::strlen(buffer);
  }

 private:
  Char buffer[7u + 3u * 4u + 1u];

  static FMT_CONSTEXPR void to_esc(uint8_t c, Char* out,
                                   char delimiter) FMT_NOEXCEPT {
    out[0] = static_cast<Char>('0' + c / 100);
    out[1] = static_cast<Char>('0' + c / 10 % 10);
    out[2] = static_cast<Char>('0' + c % 10);
    out[3] = static_cast<Char>(delimiter);
  }
};

template <typename Char>
FMT_CONSTEXPR ansi_color_escape<Char> make_foreground_color(
    internal::color_type foreground) FMT_NOEXCEPT {
  return ansi_color_escape<Char>(foreground, internal::data::foreground_color);
}

template <typename Char>
FMT_CONSTEXPR ansi_color_escape<Char> make_background_color(
    internal::color_type background) FMT_NOEXCEPT {
  return ansi_color_escape<Char>(background, internal::data::background_color);
}

template <typename Char>
FMT_CONSTEXPR ansi_color_escape<Char> make_emphasis(emphasis em) FMT_NOEXCEPT {
  return ansi_color_escape<Char>(em);
}

template <typename Char>
inline void fputs(const Char* chars, FILE* stream) FMT_NOEXCEPT {
  std::fputs(chars, stream);
}

template <>
inline void fputs<wchar_t>(const wchar_t* chars, FILE* stream) FMT_NOEXCEPT {
  std::fputws(chars, stream);
}

template <typename Char> inline void reset_color(FILE* stream) FMT_NOEXCEPT {
  fputs(internal::data::reset_color, stream);
}

template <> inline void reset_color<wchar_t>(FILE* stream) FMT_NOEXCEPT {
  fputs(internal::data::wreset_color, stream);
}

template <typename Char>
inline void reset_color(basic_memory_buffer<Char>& buffer) FMT_NOEXCEPT {
  const char* begin = data::reset_color;
  const char* end = begin + sizeof(data::reset_color) - 1;
  buffer.append(begin, end);
}

template <typename Char>
std::basic_string<Char> vformat(const text_style& ts,
                                basic_string_view<Char> format_str,
                                basic_format_args<buffer_context<Char> > args) {
  basic_memory_buffer<Char> buffer;
  bool has_style = false;
  if (ts.has_emphasis()) {
    has_style = true;
    ansi_color_escape<Char> escape = make_emphasis<Char>(ts.get_emphasis());
    buffer.append(escape.begin(), escape.end());
  }
  if (ts.has_foreground()) {
    has_style = true;
    ansi_color_escape<Char> escape =
        make_foreground_color<Char>(ts.get_foreground());
    buffer.append(escape.begin(), escape.end());
  }
  if (ts.has_background()) {
    has_style = true;
    ansi_color_escape<Char> escape =
        make_background_color<Char>(ts.get_background());
    buffer.append(escape.begin(), escape.end());
  }
  internal::vformat_to(buffer, format_str, args);
  if (has_style) {
    reset_color<Char>(buffer);
  }
  return fmt::to_string(buffer);
}
}  // namespace internal

template <typename S, typename Char = char_t<S> >
void vprint(std::FILE* f, const text_style& ts, const S& format,
            basic_format_args<buffer_context<Char> > args) {
  bool has_style = false;
  if (ts.has_emphasis()) {
    has_style = true;
    internal::fputs<Char>(internal::make_emphasis<Char>(ts.get_emphasis()), f);
  }
  if (ts.has_foreground()) {
    has_style = true;
    internal::fputs<Char>(
        internal::make_foreground_color<Char>(ts.get_foreground()), f);
  }
  if (ts.has_background()) {
    has_style = true;
    internal::fputs<Char>(
        internal::make_background_color<Char>(ts.get_background()), f);
  }
  vprint(f, format, args);
  if (has_style) {
    internal::reset_color<Char>(f);
  }
}

/**
  Formats a string and prints it to the specified file stream using ANSI
  escape sequences to specify text formatting.
  Example:
    fmt::print(fmt::emphasis::bold | fg(fmt::color::red),
               "Elapsed time: {0:.2f} seconds", 1.23);
 */
template <typename S, typename... Args,
          FMT_ENABLE_IF(internal::is_string<S>::value)>
void print(std::FILE* f, const text_style& ts, const S& format_str,
           const Args&... args) {
  internal::check_format_string<Args...>(format_str);
  using context = buffer_context<char_t<S> >;
  format_arg_store<context, Args...> as{args...};
  vprint(f, ts, format_str, basic_format_args<context>(as));
}

/**
  Formats a string and prints it to stdout using ANSI escape sequences to
  specify text formatting.
  Example:
    fmt::print(fmt::emphasis::bold | fg(fmt::color::red),
               "Elapsed time: {0:.2f} seconds", 1.23);
 */
template <typename S, typename... Args,
          FMT_ENABLE_IF(internal::is_string<S>::value)>
void print(const text_style& ts, const S& format_str, const Args&... args) {
  return print(stdout, ts, format_str, args...);
}

template <typename S, typename Char = char_t<S> >
inline std::basic_string<Char> vformat(
    const text_style& ts, const S& format_str,
    basic_format_args<buffer_context<Char> > args) {
  return internal::vformat(ts, to_string_view(format_str), args);
}

/**
  \rst
  Formats arguments and returns the result as a string using ANSI
  escape sequences to specify text formatting.

  **Example**::

    #include <fmt/color.h>
    std::string message = fmt::format(fmt::emphasis::bold | fg(fmt::color::red),
                                      "The answer is {}", 42);
  \endrst
*/
template <typename S, typename... Args, typename Char = char_t<S> >
inline std::basic_string<Char> format(const text_style& ts, const S& format_str,
                                      const Args&... args) {
  return internal::vformat(ts, to_string_view(format_str),
                           {internal::make_args_checked(format_str, args...)});
}

FMT_END_NAMESPACE

#endif  // FMT_COLOR_H_
