# Benchmarking the quantum circuit simulators

Run the benchmark with 
> python benchmark.py -t \<time limit in seconds\>

The paramter -t sets the threshold running time for all testcases. If the threshold is exceeded, 
the remaining testcases for the specific simulator and number of qubits is also run, but the number
of qubits will not be incremented in the next iteration. The output is saved in a file called 
"python/results/benchmark_data.json".