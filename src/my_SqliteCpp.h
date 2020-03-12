/**
 * @file  myDB_sqlite3.h
 * @brief A few short function about sqlite3 query
 *
 *  how-to use the Sqlite3pp wrapper
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "SQLiteCpp/SQLiteCpp.h"
#include "nlohmann/json.hpp"
#include "rapidxml.hpp"
#include "spdlog/spdlog.h"
#include "tabulate/table.hpp"

std::string getImageUrlFromXML(std::string path, unsigned int depth) {
  //   std::string xml_string = movies_json[0]["c08"];
  //   rapidxml::xml_document<> doc;
  //   doc.parse<0>((char *)xml_string.c_str());
  //   rapidxml::xml_node<> *node = doc.first_node(doc.first_node()->name());
  //   std::cout << "first node's value : " << node->value() << "\n";
  //   for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr;
  //        attr = attr->next_attribute()) {
  //     std::cout << "Node foobar has attribute " << attr->name() << " ";
  //     std::cout << "with value " << attr->value() << "\n";
  //   }

  // if path which is xml data is empty!!!
  if (path.length() == 0) {
    spdlog::debug("xml is empty : args is {}", path);
    return "";
  } else {
    rapidxml::xml_document<> doc;
    doc.parse<0>((char*)path.c_str());
    rapidxml::xml_node<>* node = doc.first_node();
    if (depth == 1) {
      // std::cout << node->value() << '\n';
      return node->value();
    } else if (depth == 2) {
      node = node->first_node("thumb");
      // std::cout << node->value() << '\n';
      return node->value();
    } else {
      return "No Images";
    }
  }
}

int getDBTotal(std::string db_filename) {
  try {
    SQLite::Database db(db_filename, SQLite::OPEN_READONLY);
    spdlog::info("SQLite database file '{}' opened successfully",
                 db.getFilename().c_str());
    int total = db.execAndGet("select count(*) from movie_view");
    return total;
  } catch (std::exception& e) {
    spdlog::info("SQLite exeception: {}", e.what());
    spdlog::info(
        "This error is probably due to a problem with the file you requested.");
    exit(1);
  }
}

nlohmann::json getDBData(std::string db_filename, std::string search_query) {
  nlohmann::json movies_json;
  nlohmann::json movie_json;
  nlohmann::json err_json = {
      {"idMovie", 0},  {"c01", ""},   {"c01", ""},           {"c02", ""},
      {"c08", ""},     {"c19", ""},   {"c20", ""},           {"premiered", ""},
      {"strPath", ""}, {"rating", 0}, {"uniqueid_value", ""}};

  try {
    SQLite::Database db(db_filename, SQLite::OPEN_READONLY);
    spdlog::info("SQLite database file '{}' opened successfully",
                 db.getFilename().c_str());

    std::string query_string;
    if (search_query.length() == 0) {
      query_string =
          "select idMovie, c00, c01, c03, c08, c19, c20, premiered, strPath, "
          "rating, uniqueid_value "
          "from movie_view order by premiered desc";
    } else {
      query_string =
          "select idMovie, c00, c01, c03, c08, c19, c20, premiered, strPath, "
          "rating, uniqueid_value "
          "from movie_view where c00 like '%";
      query_string += search_query;
      query_string += "%' order by premiered desc";
    }

    spdlog::debug("query: {}", query_string);

    SQLite::Statement query(db, query_string);
    while (query.executeStep()) {
      movie_json["idMovie"] = query.getColumn(0).getInt();
      movie_json["c00"] = query.getColumn(1).getString();
      movie_json["c01"] = query.getColumn(2).getString();
      movie_json["c03"] = query.getColumn(3).getString();
      movie_json["c08"] = getImageUrlFromXML(query.getColumn(4).getString(), 1);
      movie_json["c19"] = query.getColumn(5).getString();
      movie_json["c20"] = getImageUrlFromXML(query.getColumn(6).getString(), 2);
      movie_json["premiered"] = query.getColumn(7).getString();
      movie_json["strPath"] = query.getColumn(8).getString();
      movie_json["rating"] = query.getColumn(9).getDouble();
      movie_json["uniqueid_value"] = query.getColumn(10).getString();

      movies_json.push_back(movie_json);
    }

    if (movies_json.size() == 0) {
      spdlog::info("No data found");
      movies_json.push_back(err_json);
    }
  } catch (std::exception& e) {
    spdlog::info("SQLite exeception: {}", e.what());
    spdlog::info(
        "This error is probably due to a problem with the file you requested.");
    exit(1);
  }

  return movies_json;
}