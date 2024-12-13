# Quantum Circuit Simulation Using Low Precision Polar Form Complex Numbers

## Steps to build project: 
To build the project you will need cmake and vcpkg. Vcpkg is used to get the google test and 
google benchmark dependencies which are needed for the unit tests and benchmarks. 

If you do not have cmake, you can use the cmake that comes bundled with vcpkg. 

If you only want to build the main executable, without unit tests and benchmarks, you can do so 
without vcpkg. Then you will have to comment out everything below the line 
"add_executable(${TARGET} ${SRC_FILES})" in the CMakeLists.txt file. 


### Step 1
Make a build directory and cd into it

```shell
mkdir build
```

```shell
cd build
```


### Step 2
If you do not have vcpkg, you can clone it from github into the build directory, or anywhere you 
would like. If you clone vcpkg into another path than the build directory, you can reuse it 
between different projects.
More information is available at [Microsoft vcpkg and cmake tutorial](https://learn.microsoft.com/sv-se/vcpkg/get_started/get-started?pivots=shell-bash#1---set-up-vcpkg).

```shell
git clone https://github.com/microsoft/vcpkg.git
```

```shell
cd vcpkg && ./bootstrap-vcpkg.sh
```

Configure the VCPKG_ROOT environment variable
```shell
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```


### Step 3
Create the project files from the CMakeLists.txt. Make sure to cd into the build directory.
You can choose if it should compile in Release mode or Debug mode by changing the 
-DCMAKE_BUILD_TYPE option to either Release or Debug
```shell
cmake -DCMAKE_BUILD_TYPE=Release ..
```


### Step 4
Build and compile the project by running the following command in the build folder:
```shell
cmake --build build
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
