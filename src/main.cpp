#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <iostream>
#include <string_view>

#include "boost/program_options.hpp"
#include "global.hpp"
#include "node.hpp"
#include "turtle.hpp"

int main(int argc, char **argv) {
    namespace po = boost::program_options;
    // Initialize logging
    boost::log::add_console_log(std::cout, boost::log::keywords::format = "%Message%");

    // Declare the supported options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", po::bool_switch(), "produce help message")
        ("file,f", po::value<std::string>()->default_value("main.py"), "filename to transcompile")
        ("action,a", po::value<std::string>()->default_value("generate"), "action to perform")
        ("type,t", po::value<std::string>()->default_value("grammar"), "type of item to generate")
        ("language,l", po::value<std::string>()->defualt_value("python"), "language for grammar");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm["help"].as<bool>()) {
        boost::filesystem::path program_path{argv[0]};
        auto program_name = boost::filesystem::basename(program_path);
        BOOST_LOG_TRIVIAL(info) << program_name << " <arg> <filename>";
        BOOST_LOG_TRIVIAL(info) << desc;
        return 0;
    }

    return turtle_main(vm);
}
