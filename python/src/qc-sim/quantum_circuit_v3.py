from base_qc import QuantumCircuitBase

import numpy as np
from line_profiler import profile


class QuantumCircuit_v3(QuantumCircuitBase):

    def __init__(self, n_qubits):
        super().__init__(n_qubits)

        self.states = {0: 1+0j}


    def get_full_statevector(self):
        statevector = np.zeros((self.n_states, ), dtype=np.complex128)
        for state, amplitude in self.states.items():
            statevector[state] = amplitude

        return statevector


    @profile
    def h(self, qubit):
        scale = 1 / np.sqrt(2)
        new_states = {}

        mask = 1 << qubit
        
        for state, amplitude in self.states.items():
            adjoint_state = state ^ mask  # flips the target qubit

            if state in new_states or adjoint_state in new_states:
                continue

            # Fetch the amplitude of the adjacent state, if it exists
            adjoint_amplitude = self.states.get(adjoint_state, 0)

            if (state & mask) != 0:
                state, adjoint_state = adjoint_state, state
                amplitude, adjoint_amplitude = adjoint_amplitude, amplitude

            new_a0 = scale * (amplitude + adjoint_amplitude)
            new_a1 = scale * (amplitude - adjoint_amplitude)
                        
            if new_a0 != 0:
                new_states[state] = new_a0
            
            if new_a1 != 0:
                new_states[adjoint_state] = new_a1
        
        self.states = new_states


    @profile
    def cp(self, angle, control_qubit, target_qubit):
        control_mask = 1 << control_qubit
        target_mask = 1 << target_qubit

        phase_factor = np.exp(1j * angle)

        for state in self.states.keys():
            if (state & target_mask) and (state & control_mask):
                # both the target qubit and the control qubit is in state |1>
                self.states[state] *= phase_factor


    @profile
    def x(self, qubit):
        new_states = {}
        mask = 1 << qubit
        for state in self.states.keys():
            new_state = state ^ mask # flips the target qubit
            new_states[new_state] = self.states[state]
        
        self.states = new_states


    @profile
    def cx(self, control_qubit, target_qubit):
        pass


    @profile
    def swap(self, qubit1, qubit2):
        new_states = {}

        for state in self.states.keys():
            q1_value = (state >> qubit1) & 1
            q2_value = (state >> qubit2) & 1

            if q1_value == q2_value:
                new_states[state] = self.states[state]
                continue

            # swaps the two bits only if they are different
            adjoint_state = state ^ ((1 << qubit1) | (1 << qubit2)) 
            
            a0 = self.states[state]
            a1 = self.states.get(adjoint_state, 0.0)

            a0, a1 = a1, a0

            if a0 != 0:
                new_states[state] = a0
            
            if a1 != 0:
                new_states[adjoint_state] = a1

        self.states = new_states


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
    qc = QuantumCircuit_v3(18)
    # for i in range(10):
    #     qc.h(i)
    qc.qft()

    print(f"QFT RAN SUCCESSFULLY WITH {qc.n_qubits} QUBITS")

if __name__ == "__main__":
    main()