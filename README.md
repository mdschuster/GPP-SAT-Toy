## SAT Toy
This is a toy program that illustrates the separating axis theorem
in 2D. It goes along with a lecture in Game Physics Programming taught
by Micah Schuster at Wentworth Institute of Technology.

### Getting Started
This is a CMake project that will compile in Linux, Windows, and MacOS.
There are two required external libraries that are included in the repository:
* Raylib: www.raylib.com
* Dear ImGUI: https://github.com/ocornut/imgui

There is also a small internal math library that is used to represent
vector objects.

### Organization
* `bin/` contains the binary and a small configuration file for creating the shapes in the program
* `external/` contains Raylib and Dear ImGUI. Both include CMakeLists and are compiled as dependencies to the application
* `lib/mm/` contains the small math library for the class. It is a stripped down version that contains the functionality necessary for this application.
* `src/` contains the main source code for the project.
* `Lecture.pdf` contains the lecture that accompanies the project.

The completed version of `main.cpp` that is live-coded during the lecture is included in `src/finished` for reference.