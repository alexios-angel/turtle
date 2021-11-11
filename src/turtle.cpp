//
// Created by alex on 11/11/21.
//

#include "turtle.h"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

#include "boost/program_options.hpp"
#include "global.h"

int turtle(const boost::program_options::variables_map args) {
  const boost::filesystem::path file = args["file"].as<std::string>();
  if (!boost::filesystem::is_regular_file(file)) {
    LOG(error) << "File " << file << " not found";
    return 1;
  }
  std::string filedata;
  boost::filesystem::load_string_file(file, filedata);
  std::vector<int> lexmes;

  return 0;
}