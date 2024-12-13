# Quantum Circuit Simulation Using Low Precision Polar Form Complex Numbers

## Steps to build project: 
To build the project you will need cmake and vcpkg. Vcpkg is used to get the google test and 
google benchmark dependencies which are needed for the unit tests and benchmarks. 

If you only want to build the main executable, without unit tests and benchmarks, you can do so 
without vcpkg. Then you will have to comment out everything below the line 
"add_executable(${TARGET} ${SRC_FILES})" in the CMakeLists.txt file. 


### Step 1
Follow the [tutorial](https://learn.microsoft.com/sv-se/vcpkg/get_started/get-started?pivots=shell-bash#1---set-up-vcpkg) from Microsoft on how to install and setup cmake and vcpkg.


### Step 2
```shell
mkdir build
```

```shell
cd build
```

### Step 3
Create the project files from the CMakeLists.txt. Make sure to cd into the build directory.
You can choose if it should compile in Release mode or Debug mode by changing the 
-DCMAKE_BUILD_TYPE option to either Release or Debug
```shell
cmake -DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg/scripts/buildsystems/vcpkg.cmake> -DCMAKE_BUILD_TYPE=Release ..
```


### Step 4
Build and compile the project by running the following command in the build folder:
```shell
cmake --build .
```

This will generate three executables in the build directory: 
- main_executable
- tests
- benchmarks

### Step 5
Run the desired executable in the build directory, for example
```shell
./main_executable
```
