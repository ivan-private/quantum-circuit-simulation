import sys
sys.path.append('../src')

import numpy as np
import pytest

from qiskit import QuantumCircuit, transpile
from qiskit_aer import AerSimulator
from qiskit.circuit.library import QFT

from quantum_circuit_v1 import QuantumCircuit_v1
from quantum_circuit_v2 import QuantumCircuit_v2
from quantum_circuit_v3 import QuantumCircuit_v3

TOL = 1e-6

simulator = AerSimulator(method="statevector")

quantum_circuits_to_test = [QuantumCircuit_v1, QuantumCircuit_v2, QuantumCircuit_v3]

n_qubits = 5



def test_cp_gate():
    for QC in quantum_circuits_to_test:
        run_cp_gate_test(QC)


def run_cp_gate_test(MyQC):
    testcases = 2
    for angle in [np.pi, np.pi / 2, np.pi / 4]:
        for testcase in range(testcases):
            my_statevectors = np.zeros((4, 2**n_qubits), dtype=complex)
            qiskit_statevectors = np.zeros((4, 2**n_qubits), dtype=complex)

            qiskit_qc = QuantumCircuit(n_qubits)
            my_qc = MyQC(n_qubits)

            # init circuit
            if testcase == 0:
                qiskit_qc.x(0)
                qiskit_qc.h(1)

                my_qc.x(0)
                my_qc.h(1)

            if testcase == 1:
                qiskit_qc.x(0)
                qiskit_qc.h(1)
                qiskit_qc.h(2)

                my_qc.x(0)
                my_qc.h(1)
                my_qc.h(2)

            my_statevectors[0, :] = my_qc.get_full_statevector()
            qiskit_qc.save_statevector(label="0")


            # test 1
            if testcase == 0:
                qiskit_qc.cp(angle, 0, 1)
                my_qc.cp(angle, 0, 1)

            if testcase == 1:
                qiskit_qc.cp(angle, 0, 1)
                my_qc.cp(angle, 0, 1)

            my_statevectors[1, :] = my_qc.get_full_statevector()
            qiskit_qc.save_statevector(label="1")


            # test 2
            if testcase == 0:
                qiskit_qc.cp(angle, 0, 1)
                my_qc.cp(angle, 0, 1)

            if testcase == 1:
                qiskit_qc.cp(angle, 0, 2)
                my_qc.cp(angle, 0, 2)

            my_statevectors[2, :] = my_qc.get_full_statevector()
            qiskit_qc.save_statevector(label="2")


            # test 3
            if testcase == 0:
                qiskit_qc.cp(angle, 0, 1)
                my_qc.cp(angle, 0, 1)

            if testcase == 1:
                qiskit_qc.cp(angle, 1, 2)
                my_qc.cp(angle, 1, 2)

            my_statevectors[3, :] = my_qc.get_full_statevector()
            qiskit_qc.save_statevector(label="3")


            qiskit_results = simulator.run([qiskit_qc], shots=1).result()
            qiskit_data = qiskit_results.data(0)

            for i in range(4):
                qiskit_statevectors[i, :] = qiskit_data[str(i)].data
            
            np.testing.assert_allclose(my_statevectors, qiskit_statevectors, atol=TOL, \
                                    err_msg=f"Error produced during testcase {testcase} for {MyQC.__name__}")


