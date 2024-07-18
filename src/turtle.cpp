#include "turtle.hpp"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

#include "boost/program_options.hpp"
#include "global.hpp"
#include "lexicalize.hpp"
#include "node.hpp"
#include "syntax.hpp"

int turtle_main(const boost::program_options::variables_map args) {
  const boost::filesystem::path file = args["file"].as<std::string>();
  if (!boost::filesystem::is_regular_file(file)) {
    LOG(error) << "File " << file << " not found";
    return 1;
  }
  std::string filedata;
  turtle::turtle_vector<turtle::node_t> lexemes;
  boost::filesystem::load_string_file(file, filedata);
  lexicalize(filedata, lexemes);
  return 0;
}
