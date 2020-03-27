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

// #include <fstream>
#include "SFML/Network.hpp"

#include "spdlog/spdlog.h"

#include "Uri.h"

// static std::string img_url = {
//     "http://image.tmdb.org/t/p/original/iTQHKziZy9pAAY4hHEDCGPaOvFC.jpg"};

std::string getImageDataFromUrl(std::string img_url) {
  std::string return_string;

  // first ", last " delete
  img_url.erase(0, 1);
  img_url.erase(img_url.length() - 1, 1);

  spdlog::info("img_url : {}", img_url);
  Uri u0 = Uri::Parse(img_url);
  // QueryString, Path, Protocol, Host, Port
  spdlog::info("Host {}", u0.Host);
  spdlog::info("Path {}", u0.Path);
  // spdlog::info("QueryString {}", u0.QueryString);
  // spdlog::info("Protocol {}", u0.Protocol);
  // spdlog::info("Port {}", u0.Port);

  sf::Http http;
  http.setHost(u0.Host);

  sf::Http::Request request;
  request.setMethod(sf::Http::Request::Get);
  request.setUri(u0.Path);
  request.setHttpVersion(1, 1);  // HTTP 1.1

  sf::Http::Response response = http.sendRequest(request);

  if (response.getStatus() == sf::Http::Response::Ok) {
    return_string = response.getBody();
    // spdlog::info("test.jpeg save");
    // save to file
    // std::ofstream o("test.jpeg");
    // o << response.getBody();
  } else {
    spdlog::info("status {}", response.getStatus());
    return_string = "No Image";
  }

  return return_string;
}
