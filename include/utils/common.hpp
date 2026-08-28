#pragma once

#include <cstdint>

constexpr uint64_t operator""_KiB(unsigned long long v) { return v << 10; }
constexpr uint64_t operator""_MiB(unsigned long long v) { return v << 20; }
constexpr uint64_t operator""_GiB(unsigned long long v) { return v << 30; }

namespace crossing {
enum class LoggerType : uint8_t {
  UNKNOWN_LOGGER = 0,
  DEBUG = 1,
  INFO = 2,
  WARN = 3,
  ERROR = 4,
};

inline constexpr LoggerType DEBUG = LoggerType::DEBUG;
inline constexpr LoggerType INFO = LoggerType::INFO;
inline constexpr LoggerType WARN = LoggerType::WARN;
inline constexpr LoggerType ERROR = LoggerType::ERROR;
} // namespace crossing
