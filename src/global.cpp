#include "global.hpp"
#include <boost/stacktrace.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

void terminate_handler(void) {
  try {
    std::cerr << boost::stacktrace::stacktrace();
  } catch (...) {
  }
  std::abort();
}

namespace turtle {

void load_string_file(const std::filesystem::path &path, std::string str) {
  // Sanity check
  if (!std::filesystem::is_regular_file(path)) {
    return;
  }

  // Open the file
  // Note that we have to use binary mode as we want to return a string
  // representing matching the bytes of the file on the file system.
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    return;
  }

  // Read contents
  str.assign((std::istreambuf_iterator<char>(file)),
             std::istreambuf_iterator<char>());

  // Close the file
  file.close();
}

} // namespace turtle
