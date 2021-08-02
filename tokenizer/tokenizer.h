#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "error/error.h"
#include "tokenizer/token.h"
#include "tokenizer/utils.hpp"

namespace miniplc0 {

class Tokenizer final {
    using uint64_t = std::uint64_t;

public:
  Tokenizer(std::istream &ifs)
      : _rdr(ifs), _initialized(false), _ptr(0, 0), _lines_buffer() {}
  Tokenizer(Tokenizer &&tkz) = delete;
  Tokenizer(const Tokenizer &) = delete;
  Tokenizer &operator=(const Tokenizer &) = delete;

    virtual // 核心函数，返回下一个 token
  std::pair<std::optional<Token>, std::optional<CompilationError>> NextToken() = 0;

    virtual // 一次返回所有 token
  std::pair<std::vector<Token>, std::optional<CompilationError>> AllTokens() = 0;

 private:

    // 从这里开始其实是一个基于行号的缓冲区的实现
  // 为了简单起见，我们没有单独拿出一个类实现
  // 核心思想和 C 的文件输入输出类似，就是一个 buffer 加一个指针，有三个细节
  // 1.缓冲区包括 \n
  // 2.指针始终指向下一个要读取的 char
  // 3.行号和列号从 0 开始

    virtual // 一个简单的总结
  // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9  | 偏移
  // | = | = | = | = | = | = | = | = | = | =  |
  // | h | a | 1 | 9 | 2 | 6 | 0 | 8 | 1 | \n |（缓冲区第0行）
  // | 7 | 1 | 1 | 4 | 5 | 1 | 4 |             （缓冲区第1行）
  // 这里假设指针指向第一行的 \n，那么有
  // nextPos() = (1, 0)
  // currentPos() = (0, 9)
  // previousPos() = (0, 8)
  // nextChar() = '\n' 并且指针移动到 (1, 0)
  // unreadLast() 指针移动到 (0, 8)
  std::pair<uint64_t, uint64_t> nextPos() = 0;

    virtual std::pair<uint64_t, uint64_t> currentPos() = 0;

    virtual std::pair<uint64_t, uint64_t> previousPos() = 0;

    virtual std::optional<char> nextChar() = 0;

    virtual void unreadLast() = 0;

};
}  // namespace miniplc0
