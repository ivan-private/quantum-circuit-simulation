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


@pytest.mark.filterwarnings("ignore") # from qiskit.circuit.library import QFT has a deprecation warning, this silences it
def test_qft():
    for QC in quantum_circuits_to_test:
        run_qft_test(QC)


def run_qft_test(MyQC):
    testcases = 3
    for testcase in range(testcases):
        my_statevectors = np.zeros((2, 2**n_qubits), dtype=complex)
        qiskit_statevectors = np.zeros((2, 2**n_qubits), dtype=complex)

        qiskit_qc = QuantumCircuit(n_qubits)
        my_qc = MyQC(n_qubits)

        
        if testcase == 0:
            qiskit_qc.x(0)
            qiskit_qc.h(1)
            qiskit_qc.cp(np.pi / 2, 0, 1)

            my_qc.x(0)
            my_qc.h(1)
            my_qc.cp(np.pi / 2, 0, 1)

        if testcase == 1:
            qiskit_qc.x(0)
            qiskit_qc.h(1)
            qiskit_qc.h(2)
            qiskit_qc.cp(np.pi / 4, 0, 1)
            qiskit_qc.cp(np.pi / 5, 0, 2)
            qiskit_qc.cp(np.pi / 6, 1, 2)

            my_qc.x(0)
            my_qc.h(1)
            my_qc.h(2)
            my_qc.cp(np.pi / 4, 0, 1)
            my_qc.cp(np.pi / 5, 0, 2)
            my_qc.cp(np.pi / 6, 1, 2)

        if testcase == 2:
            qiskit_qc.x(1)

            my_qc.x(1)

        my_statevectors[0, :] = my_qc.get_full_statevector()
        qiskit_qc.save_statevector(label="0")


        # test 1
        qiskit_qc.append(QFT(n_qubits), list(range(n_qubits)))
        my_qc.qft()
        my_statevectors[1, :] = my_qc.get_full_statevector()
        qiskit_qc.save_statevector(label="1")

        qiskit_results = simulator.run([transpile(qiskit_qc, simulator)], shots=1).result()
        qiskit_data = qiskit_results.data(0)

        for i in range(2):
            qiskit_statevectors[i, :] = qiskit_data[str(i)].data
        
        np.testing.assert_allclose(my_statevectors, qiskit_statevectors, atol=TOL, \
                                    err_msg=f"Error produced during testcase {testcase} for {MyQC.__name__}")


