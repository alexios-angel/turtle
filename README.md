# Turtle

A Python to C++ transcompiler

## Build
### Prerequisites
If you would like to test it out you would need:

 - `cmake`
 - `clang`
 - `libboost-all-dev`
 - `git`
 - `build-essential`

```Bash
sudo apt install -y clang libboost-all-dev build-essential
pip install cmake # To install the latest CMake; The apt repo is old
```


### Build instructions

Clone the repository and build the project using CMake:

```Bash
git clone https://github.com/alexios-angel/turtle
cd turtle
cmake . -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++
# If you would like to use the Mold linker
# cmake . -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_LINKER_TYPE=MOLD
cmake --build . -j $(nproc)
```
