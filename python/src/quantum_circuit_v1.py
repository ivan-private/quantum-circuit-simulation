from base_qc import QuantumCircuitBase

import numpy as np

from line_profiler import profile


class QuantumCircuit_v1(QuantumCircuitBase):
    def __init__(self, n_qubits):
        super().__init__(n_qubits)

        self.statevector = np.zeros((self.n_states, ), dtype=np.complex128)
        self.statevector[0] = 1

    
    def get_full_statevector(self):
        return self.statevector


    @profile
    def h(self, qubit):
        H = 1 / np.sqrt(2) * np.array([[1, 1], 
                                       [1, -1]])
        I = np.eye(2)
        
        GATE = np.array([1])
        for i in range(self.n_qubits)[::-1]:
            if i == qubit:
                GATE = np.kron(GATE, H)
            else:
                GATE = np.kron(GATE, I)
        
        self.statevector = GATE @ self.statevector
        

    @profile
    def cp(self, angle, control_qubit, target_qubit):
        P = np.array([[1, 0], [0, np.exp(1j * angle)]])
        I = np.eye(2)
        I_0 = np.array([[1, 0], 
                        [0, 0]])
        
        I_1 = np.array([[0, 0], 
                        [0, 1]])

        GATE_0 = np.array([1])
        GATE_1 = np.array([1])

        for i in range(self.n_qubits)[::-1]:
            if i == target_qubit:
                GATE_0 = np.kron(GATE_0, I)
                GATE_1 = np.kron(GATE_1, P)
            elif i == control_qubit:
                GATE_0 = np.kron(GATE_0, I_0)
                GATE_1 = np.kron(GATE_1, I_1)
            else:
                GATE_0 = np.kron(GATE_0, I)
                GATE_1 = np.kron(GATE_1, I)

        GATE = GATE_0 + GATE_1

        self.statevector = GATE @ self.statevector
    

    @profile
    def x(self, qubit):
        X = np.array([[0, 1], 
                      [1, 0]])
        I = np.eye(2)

        GATE = np.array([1])

        for i in range(self.n_qubits)[::-1]:
            if i == qubit:
                GATE = np.kron(GATE, X)
            else:
                GATE = np.kron(GATE, I)
        
        self.statevector = GATE @ self.statevector


    @profile
    def swap(self, qubit1, qubit2):        
        SWAP = np.eye(self.n_states)

        for i in range(len(SWAP)):
            q1_value = (i >> qubit1) & 1
            q2_value = (i >> qubit2) & 1

            if q1_value == q2_value or q1_value != 0:
                continue
            
            # swaps the two bits for qubit1 and qubit2 in i
            j = i ^ ((1 << qubit1) | (1 << qubit2))

            SWAP[[i, j]] = SWAP[[j, i]]

        self.statevector = SWAP @ self.statevector


    @profile
    def qft(self):
        for i in range(self.n_qubits)[::-1]:
            self.h(i)
            for j in range(i-1, -1, -1):
                angle = np.pi * 2**(j-i)
                self.cp(angle, j, i)
        
        for i in range(self.n_qubits // 2):
            self.swap(i, self.n_qubits-1-i)

@profile
def main():
    # this main function is only used when profiling
    qc = QuantumCircuit_v1(11)
    qc.qft()

    print(f"QFT RAN SUCCESSFULLY WITH {qc.n_qubits} QUBITS")


if __name__ == "__main__":
    main()
