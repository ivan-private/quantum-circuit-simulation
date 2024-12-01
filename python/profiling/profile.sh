#!/bin/bash

# Check for the correct number of arguments
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 {v1|v2|v3}"
    exit 1
fi

# Capture the input argument
VERSION=$1

# Verify the input argument is valid
if [[ "$VERSION" != "v1" && "$VERSION" != "v2" && "$VERSION" != "v3" ]]; then
    echo "Error: Input must be one of {v1, v2, v3}"
    exit 1
fi

# Set the filenames based on the input
PYTHON_FILE="../src/quantum_circuit_${VERSION}.py"
LPROF_FILE="quantum_circuit_${VERSION}.py.lprof"
RESULT_FILE="../results/profiling_results_qc_${VERSION}.txt"

# Run kernprof to generate the .lprof file
kernprof -l "$PYTHON_FILE"
if [ $? -ne 0 ]; then
    echo "Error: Failed to run kernprof on $PYTHON_FILE"
    exit 1
fi

# Use line_profiler to generate the .txt file
python -m line_profiler "$LPROF_FILE" > "$RESULT_FILE"
if [ $? -ne 0 ]; then
    echo "Error: Failed to generate line_profiler output"
    exit 1
fi

# Delete the .lprof file
rm "$LPROF_FILE"
if [ $? -ne 0 ]; then
    echo "Warning: Failed to delete $LPROF_FILE"
else
    echo "Successfully deleted $LPROF_FILE"
fi

echo "Profiling completed successfully. Results saved to $RESULT_FILE"
