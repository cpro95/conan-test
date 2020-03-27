#include <spdlog/spdlog.h>
#include <cstdio>
#include <fstream>
#include <string>
#include "Helper.h"
#include "SFML/Graphics/Texture.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "my_SqliteCpp.h"

static std::string db_filename = "../../data/MyVideos116.db";
static nlohmann::json json;
static std::string search_queryFromSTD;
static int json_size = 0;
static int selected = 0;

void mainLoop() {
  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Search Movies", NULL, ImGuiWindowFlags_MenuBar)) {
    // top
    static char search_query[128] = "";
    ImGui::InputText("", search_query, IM_ARRAYSIZE(search_query));
    ImGui::SameLine();
    if (ImGui::Button("Search")) {
      spdlog::info("Search Button Clicked : {}", search_query);
      if ((unsigned)strlen(search_query) == 0) {
        spdlog::info("length is 0");
        search_queryFromSTD = "";
      } else {
        search_queryFromSTD = search_query;
      }
      json = getDBData(db_filename, search_queryFromSTD);
      json_size = json.size();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
      spdlog::info("Clear Button Clicked");
      search_query[0] = '\0';
      json.clear();
      json.push_back({{"idMovie", 0},
                      {"c01", "No Data"},
                      {"c01", "No Data"},
                      {"c02", "No Data"},
                      {"c08", "No Data"},
                      {"c19", "No Data"},
                      {"c20", "No Data"},
                      {"premiered", "No Data"},
                      {"strPath", "No Data"},
                      {"rating", 0},
                      {"uniqueid_value", "No Data"}});
      json_size = json.size();
      selected = 0;
    }
    if (json[selected]["c00"].is_null()) {
      ImGui::Text("Found :0");
    } else {
      ImGui::Text("Found :%d", json_size);
    }

    // left

    ImGui::BeginChild("Search Result", ImVec2(300, 0), true);
    // spdlog::info("json size {}", json.size());

    if (json[0]["idMovie"] == 0) {
      std::string label = {"No Data"};
      if (ImGui::Selectable(label.c_str(), selected == 0)) selected = 0;
    } else {
      for (int i = 0; i < json_size; i++) {
        std::string label;
        label = json[i]["c00"];
        if (ImGui::Selectable(label.c_str(), selected == i)) selected = i;
      }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // right panel
    ImGui::BeginGroup();

    ImGui::BeginChild(
        "item view",
        ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));  // Leave room for 1
                                                          // line below us
    if (json[selected]["c00"].is_null()) {
      ImGui::Text("No Data");
    } else {
      ImGui::Text("%s", json[selected]["c00"].dump().c_str());
    }
    ImGui::Separator();

    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
      if (ImGui::BeginTabItem("Description")) {
        if (json[selected]["c01"].is_null()) {
          ImGui::TextWrapped("No Data");
        } else {
          ImGui::TextWrapped("%s", json[selected]["c01"].dump().c_str());
        }
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("strPath")) {
        if (json[selected]["c01"].is_null()) {
          ImGui::TextWrapped("No Data");
        } else {
          ImGui::TextWrapped("%s", json[selected]["strPath"].dump().c_str());
        }
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
    ImGui::EndChild();

    if (ImGui::Button("Trailer")) {
      spdlog::info("Trailer Button Clicked");
      spdlog::info("{}", json[selected]["c08"].dump());
      std::string img_string =
          getImageDataFromUrl(json[selected]["c08"].dump());
      // std::ofstream o("test.jpg");
      // o << img_string;
      sf::Texture texture;
      long int Dim = img_string.length();
      texture.loadFromMemory(img_string.c_str(), Dim);
      ImGui::Image(texture, sf::Vector2f(500,500));
    }
    ImGui::SameLine();
    if (ImGui::Button("IMDB Info")) {
      spdlog::info("IMDB Info Button Clicked");
    }

    ImGui::EndGroup();  // right panel
  }
  ImGui::End();
}