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
sudo apt install -y cmake clang libboost-all-dev build-essential
```

If you would like to contribute you would need

```Bash
sudo apt install -y cmake clang libboost-all-dev build-essential
```
### Build instructions

Clone the repository and build the project using CMake:

```Bash
git clone https://github.com/alexios-angel/turtle
cd turtle
cmake . -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++
make
```
