/*
Copyright 2019-2021 Anders Bo Rasmussen

This file is part of circa.

    circa is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    circa is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with circa.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Display.hpp"
#include "rpn.hpp"
#include "version.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char **argv)
{
  const std::string version_str = "circa "+version;
  po::options_description desc("Allowed options");
  std::string debug_out;
  desc.add_options()
      ("help", "produce help message")
      ("version", "print version")
      ("debug_out", po::value<std::string>(&debug_out), "file to output debug to")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    std::cout << desc << "\n";
    return 0;
  }
  if (vm.count("version"))
  {
    std::cout << version_str << "\n";
    return 0;
  }

  if (vm.count("debug_out"))
  {
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(debug_out,true);
    auto my_logger= std::make_shared<spdlog::logger>("main", sink);
    spdlog::set_default_logger(my_logger);
    my_logger->set_level(spdlog::level::debug);
  }

  Display d;
  RPN rpn(d);

  d.add_line(version_str);
  d.add_line("-----------");

  while (char ch=read_char())
  {
    spdlog::debug("Got input: '{}'({})",ch >= 32 ? ch : '?',int(ch));
    rpn.process_input(ch);
  }
}
