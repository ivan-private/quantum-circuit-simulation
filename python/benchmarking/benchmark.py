import sys
sys.path.append('../src')

import numpy as np
import timeit
import argparse
import json

from quantum_circuit_v1 import QuantumCircuit_v1
from quantum_circuit_v2 import QuantumCircuit_v2
from quantum_circuit_v3 import QuantumCircuit_v3


qc_simulators = [QuantumCircuit_v1, QuantumCircuit_v2, QuantumCircuit_v3]
qc_simulators_str = {QuantumCircuit_v1: "v1", QuantumCircuit_v2: "v2", QuantumCircuit_v3: "v3"}


timer = timeit.default_timer

RESULT_DIR_PATH = "../results"

TIME_FILE_PATH = f"{RESULT_DIR_PATH}/time.csv"
MEMORY_FILE_PATH = f"{RESULT_DIR_PATH}/memory.csv"

RESULT_FILE_PATH = f"{RESULT_DIR_PATH}/benchmark_data.json"

MAX_PERFORMANCE_RESULT_FILE_PATH = f"{RESULT_DIR_PATH}/benchmark_max_performance_data.json"



def run_qft(qc, memory_data):
    for i in range(qc.n_qubits)[::-1]:
        qc.h(i)
        memory_data.append( len(qc.statevector) )
        for j in range(i-1, -1, -1):
            angle = np.pi * 2**(j-i)
            qc.cp(angle, j, i)
            memory_data.append( len(qc.statevector) )
    
    for i in range(qc.n_qubits // 2):
        qc.swap(i, qc.n_qubits-1-i)
        memory_data.append( len(qc.statevector) )


"""
    Benchmark JSON data format

    benchmark_data = {

        benchmark_id : {
            "QC_version": str,
            "qubits": int,
            "hadamards": int,
            "time": float,
            "time_limit" = float,
            "statevector_objects_after_each_gate": list[int]
        },

        benchmark_id: {
            ...
        }
    }
"""


def run_hadamards_benchmark(time_limit_seconds = 0.1):
    N_QUBITS = [i for i in range(3, 100)]

    benchmark_data = {}

    id = 1
    time_limit_exceeded = set()

    for QC in qc_simulators:

        for n in N_QUBITS:
            if QC in time_limit_exceeded:
                break

            print(f"\n{qc_simulators_str[QC]}, Qubits: {n}")

            for num_hadamard in range(n+1): 
                print(f"Hadamards: {num_hadamard}", end=",\t")
                
                qc = QC(n)

                for i in range(num_hadamard):
                    qc.h(i)
                
                memory_data = [len(qc.statevector)]

                t_start = timer()
                run_qft(qc, memory_data)
                t_end = timer()

                time = t_end - t_start

                print(f"Time: {time}")

                benchmark_data[id] = {
                    "QC_version": qc_simulators_str[QC],
                    "qubits": n,
                    "hadamards": num_hadamard,
                    "time": time,
                    "time_limit": time_limit_seconds,
                    "statevector_objects_after_each_gate": memory_data
                }
                id += 1

                if time > time_limit_seconds:
                    time_limit_exceeded.add(QC)

    
    with open(RESULT_FILE_PATH, "w+") as file:
        file.write(json.dumps(benchmark_data, indent=4))


def run_max_performance_benchmark(time_limit_seconds = 0.1):
    N_QUBITS = [i for i in range(3, 100)]

    benchmark_data = {}

    id = 1
    time_limit_exceeded = set()


    for n in N_QUBITS:
        qc = QuantumCircuit_v3(n)
        
        for i in range(qc.n_qubits):
            qc.h(i)

        memory_data = [len(qc.statevector)]

        t_start = timer()
        run_qft(qc, memory_data)
        t_end = timer()

        time = t_end - t_start

        print(f"Qubits: {n},\tTime: {time}")

        benchmark_data[id] = {
            "QC_version": qc_simulators_str[QuantumCircuit_v3],
            "qubits": n,
            "hadamards": qc.n_qubits,
            "time": time,
            "time_limit": time_limit_seconds,
            "statevector_objects_after_each_gate": memory_data
        }
        id += 1

        if time > time_limit_seconds:
            break

    
    with open(MAX_PERFORMANCE_RESULT_FILE_PATH, "w+") as file:
        file.write(json.dumps(benchmark_data, indent=4))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--time_limit", type=float, default=0.1)
    parser.add_argument("-p", "--max_performance_benchmark", default=False, action="store_true")

    args = parser.parse_args()
    time_limit = args.time_limit
    max_performance_benchmark = args.max_performance_benchmark

    if max_performance_benchmark:
        run_max_performance_benchmark(time_limit)
    else:
        run_hadamards_benchmark(time_limit)



if __name__ == "__main__":
    main()
        