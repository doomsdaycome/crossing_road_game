#include "common.hpp"
#include "logger.hpp"

int main(int argc, char *argv[]) {
  crossing::Logger::makeLog(crossing::LoggerType::DEBUG, __FILE__, __LINE__,
                            "game start!");
  return 0;
}
