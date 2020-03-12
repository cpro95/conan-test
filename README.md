# My Cpp template
# with conan package manager

## install
- mkdir build
- cd build
- conan install ..
- cmake ..
- make

## binary file saved to /bin folder.

## conanfile.txt
```
[requires]
imgui-sfml/2.1@bincrafters/stable
sqlitecpp/2.4.0@bincrafters/stable
rapidxml/1.13@bincrafters/stable
fmt/6.1.2
nlohmann_json/3.7.3
spdlog/1.5.0
docopt.cpp/0.6.2

[generators]
cmake

```