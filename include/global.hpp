#ifndef GLOBAL_H
#define GLOBAL_H

#include <boost/exception/all.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/stacktrace.hpp>
#define LOG BOOST_LOG_TRIVIAL

#ifndef NDEBUG
#define DEBUG_CPP
#endif

namespace boost {
inline void assertion_failed_msg(char const *expr, char const *msg,
                                 char const *function, char const * /*file*/,
                                 long /*line*/) {
  std::cerr << "Expression '" << expr << "' is false in function '" << function
            << "': " << (msg ? msg : "<...>") << ".\n"
            << "Backtrace:\n"
            << boost::stacktrace::stacktrace() << '\n';

  std::abort();
}

inline void assertion_failed(char const *expr, char const *function,
                             char const *file, long line) {
  ::boost::assertion_failed_msg(expr, nullptr /*nullptr*/, function, file,
                                line);
}
} // namespace boost

// The definition of terminate_handler is in global.cpp
// in order to avoid multiple definitions when linking
void terminate_handler(void);

typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace>
    stacktrace_error_info;
template <class E> void BOOST_THROW(const E &e) {
  throw boost::enable_error_info(e)
      << stacktrace_error_info(boost::stacktrace::stacktrace());
}

namespace turtle {
auto distance(const auto &first, const auto &last) {
    size_t i = 0;
    for (auto it = first; it != last; ++it) {
      ++i;
    }
    return i;
}
}
#endif
