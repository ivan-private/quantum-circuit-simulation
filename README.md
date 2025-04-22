# Quantum Circuit Simulation Using Low Precision Polar Form Complex Numbers

Different kinds of state vector based quantum circuit simulators written in Python and C++.

## Building the project: 
To build the project you will need to use make. This will generate a build directory with two executables: 
- main_executable
- test_executable

## src directory
The src directory contains the quantum circuit simulator implementation together with a main.cpp file which shows an example use case of this simulator. This is what is compiled to the main_executable. 

## tests directory
The tests directory contains the QC_Tester.cpp file which implements the tests. These are not tests of correctness but comparison tests meant to show the difference in accuracy between using std::complex and SmallPolar in the QuantumCircuit simulator. This compiles into test_executable. The recomended way to run the tests and save the results is to use:
```shell
make test > results/<filename>.txt
```

## results directory
The results directory contains test results from tests that I have run and saved, together with a Jupyter Notebook "generate_latex_table.ipynb" which implements a Python script for parsing the results and creating a LaTeX formatted table. 