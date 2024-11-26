from base_qc import QuantumCircuitBase

import numpy as np
from line_profiler import profile


class QuantumCircuit_v2(QuantumCircuitBase):
    def __init__(self, n_qubits):
        super().__init__(n_qubits)

        self.statevector = np.zeros((self.n_states, ), dtype=np.complex128)
        self.statevector[0] = 1

    
    def get_full_statevector(self):
        return self.statevector


    @profile
    def h(self, qubit):
        scale = 1 / np.sqrt(2)
        
        mask = 1 << qubit
        
        for state in range(self.n_states):
            if state & mask != 0:
                continue
            # only apply Hadamard to states where the target qubit == |0>
            # this is since the adjoint_state will have the target qubit == |1>

            adjoint_state = state ^ mask # flips the target qubit

            amplitude = self.statevector[state]
            adjoint_amplitude = self.statevector[adjoint_state]

            new_amplitude = scale * (amplitude + adjoint_amplitude)
            new_adjoint_amplitude = scale * (amplitude - adjoint_amplitude)
                        
            self.statevector[state] = new_amplitude
            self.statevector[adjoint_state] = new_adjoint_amplitude
        

    @profile
    def cp(self, angle, control_qubit, target_qubit):
        control_mask = 1 << control_qubit
        target_mask = 1 << target_qubit

        phase_factor = np.exp(1j * angle)

        for state in range(self.n_states):
            target_qubit = state & target_mask
            control_qubit = state & control_mask
            if control_qubit != 0 and target_qubit != 0:
                # both the target qubit and the control qubit is in state |1>
                self.statevector[state] *= phase_factor
    

    @profile
    def x(self, qubit):
        mask = 1 << qubit
        for state in range(self.n_states):
            if state & mask == 0:
                # the target qubit is in state |0>, 
                # with the adjoint_state being where the target qubit is |1>
                adjoint_state = state ^ mask # flips the target qubit
                self.statevector[state], self.statevector[adjoint_state] = self.statevector[adjoint_state], self.statevector[state]
        

    @profile
    def cx(self, control_qubit, target_qubit):
        pass


    @profile
    def swap(self, qubit1, qubit2):
        for state in range(self.n_states):
            q1_value = (state >> qubit1) & 1
            q2_value = (state >> qubit2) & 1
            
            if q1_value == 0 and q2_value == 1:
                # only swap if qubit1 == |0> and qubit2 == |1>
                # this is because in the adjoint_state, qubit1 == |1> and qubit2 == |0>
                adjoint_state = state ^ ((1 << qubit1) | (1 << qubit2)) 
                self.statevector[state], self.statevector[adjoint_state] = self.statevector[adjoint_state], self.statevector[state]


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
    def ghz(self):
        pass


@profile
def main():
    # this main function is only used when profiling
    qc = QuantumCircuit_v2(5)
    qc.qft()
    print(f"QFT RAN SUCCESSFULLY WITH {qc.n_qubits} QUBITS")

if __name__ == "__main__":
    main()

