#include "turtle.hpp"

#include <boost/log/trivial.hpp>
#include <filesystem>

#include "boost/program_options.hpp"
#include "global.hpp"
#include "langs/antlr/lexicalize.hpp"
#include "langs/python/lexicalize.hpp"
#include "node.hpp"

int turtle_main(const boost::program_options::variables_map vm) {
  const std::filesystem::path file = vm["file"].as<std::string>();

  std::string action = vm["action"].as<std::string>();
  std::string type = vm["type"].as<std::string>();
  std::string language = vm["language"].as<std::string>();

  if (!std::filesystem::is_regular_file(file)) {
    BOOST_LOG_TRIVIAL(error) << "File " << file << " not found";
    return 1;
  }
  turtle::turtle_vector<turtle::node_t> lexemes;
  std::string filedata;
  turtle::load_string_file(file, filedata);
  if (language == "python") {
    turtle::langs::python::lexicalize(filedata, lexemes);
  } else if (language == "antlr") {
    turtle::langs::antlr::lexicalize(filedata, lexemes);
  } else {
    BOOST_LOG_TRIVIAL(error) << "Unknown language " << language << "\n";
    return 1;
  }
  return 0;
}
