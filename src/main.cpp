/**
 * @file  main.cpp
 * @brief A few short examples in a row.
 *
 *  Demonstrates how-to use the SQLite++ wrapper
 *
 * Copyright (c) 2012-2020 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "SQLiteCpp/SQLiteCpp.h"
#include "docopt/docopt.h"
#include "fmt/core.h"
#include "my_SqliteCpp.h"
#include "tabulate/table.hpp"

static const char USAGE[] =
    R"(conanTest.

Usage:
      conanTest [-d dbName] [-s search_string] [-j outFileName] [--count]
      conanTest (-h | --help)
      conanTest --version

    Options:
      -h --help             Show this screen.
      -v --version          Show version.
      -d dbName             DB File Name ( Required! )
      -s search_string      Search String.
      -j outFileName        JSON output FileName.
      --count               Show Total Count.
)";

void make_Table(tabulate::Table& table, nlohmann::json& movies_json) {
  const uint movie_table_size = 12;
  std::vector<std::string> movie_table_names{
      "idMovie", "c00",       "c01",     "c03",    "c08",           "c19",
      "c20",     "premiered", "strPath", "rating", "uniqueid_value"};

  // code snippet
  // for (auto val : movies_json) {
  //   for (auto val2 : val) {
  //     std::cout << val2 << std::setw(4) << '\n';
  //   }
  // }

  // even easier with structured bindings (C++17)
  // for (auto& [key, value] : movies_json[0].items()) {
  //   table.add_row({key, value});
  // }

  for (int i = 0; i < movies_json.size(); ++i) {
    table.add_row({"key", "values"});
    for (auto val : movie_table_names) {
      table.add_row({val, movies_json[i][val].dump()});
    }
  }

  // table style
  for (int i = 0; i < movies_json.size(); ++i) {
    table[i * movie_table_size]
        .format()
        .font_background_color(tabulate::Color::blue)
        .font_color(tabulate::Color::white);
  }
  // table width
  table[0][0].format().width(11);
  table[0][1].format().width(100);
}

int main(int argc, char** argv) {
  std::map<std::string, docopt::value> args =
      docopt::docopt(USAGE, {argv + 1, argv + argc}, true, "conanTest");

  for (auto const& arg : args) {
    std::cout << arg.first << " : " << arg.second << std::endl;
  }

  std::string db_filename;

  if (!args["-d"].isString()) {
    fmt::print("Arguments required : -d <DBFILENAME>\n");
    exit(1);
  } else {
    db_filename = args["-d"].asString();
    fmt::print("db_file_name : {}\n", db_filename);
  }

  if (args["--count"].asBool()) {
    fmt::print("Print Count of all items\n");
    spdlog::info("Total Count : {}", getDBTotal(db_filename));
    exit(0);
  }

  std::string query_string;
  if (!args["-s"].isString()) {
    fmt::print("-s option is empty.\n");
  } else {
    query_string = args["-s"].asString();
  }

  std::string output_filename;
  if (args["-j"].isString()) {
    output_filename = args["-j"].asString();
  }

  spdlog::info("Search Movies with...");



  nlohmann::json movies_json = getDBData(db_filename, query_string);

  if (output_filename.length() == 0) {
    tabulate::Table table;
    make_Table(table, movies_json);
    table.print(std::cout);
  } else {
    std::ofstream o(output_filename);
    o << movies_json.dump(2) << std::endl;
    spdlog::info("Exporting to JSON is complete : {}", output_filename);
  }
  spdlog::info("Total Count : {}", movies_json.size());

  return EXIT_SUCCESS;
}
