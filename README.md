# My Cpp template
# with conan package manager

## install
- mkdir build
- cd build
- conan install ..
- cmake ..
- make

## test
 - googletest

## how to git clone with submodule
##### 3 way how to
 - git clone https://github.com/cpro95/conan-test.git
 - git submodule init
 - git submodule update
<hr>

#### 1 way how to
 - git clone --recurse-submodules https://github.com/cpro95/conan-test.git 

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