from base_qc import QuantumCircuitBase

import numpy as np
from line_profiler import profile


class PolarCoordinate:
    def __init__(self, r=0, theta=0):
        self.r = r
        self.theta = theta
    
    def __str__(self):
        return f"({self.r}, {self.theta * 180 / np.pi})"

    def __repr__(self):
        return f"({self.r}, {self.theta * 180 / np.pi})"


class QuantumCircuit_v3(QuantumCircuitBase):

    def __init__(self, n_qubits):
        self.n_qubits = n_qubits
        self.n_states = 2**n_qubits
        self.states = [PolarCoordinate() for _ in range(self.n_states)]
        self.states[0].r = 1

    
    def get_full_statevector(self):
        statevector = np.zeros((self.n_states, ), dtype=complex)
        for state in range(self.n_states):
            r = self.states[state].r
            theta = self.states[state].theta
            statevector[state] = r * np.exp(1j * theta)
        return statevector


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

            r1, theta1 = self.states[state].r, self.states[state].theta
            r2, theta2 = self.states[adjoint_state].r, self.states[adjoint_state].theta

            new_r1 = scale * np.sqrt(r1**2 + r2**2 + 2 * r1 * r2 * np.cos(theta2 - theta1))
            new_theta1 = theta1 + np.arctan2(r2 * np.sin(theta2 - theta1), r1 + r2 * np.cos(theta2 - theta1))

            new_r2 = scale * np.sqrt(r1**2 + r2**2 - 2 * r1 * r2 * np.cos(theta2 - theta1))
            new_theta2 = theta1 + np.arctan2(-r2 * np.sin(theta2 - theta1), r1 - r2 * np.cos(theta2 - theta1))

            self.states[state].r = new_r1
            self.states[state].theta = new_theta1
            self.states[adjoint_state].r = new_r2
            self.states[adjoint_state].theta = new_theta2
        


    @profile
    def cp(self, angle, control_qubit, target_qubit):
        control_mask = 1 << control_qubit
        target_mask = 1 << target_qubit

        for state in range(self.n_states):
            target_qubit = state & target_mask
            control_qubit = state & control_mask
            if control_qubit != 0 and target_qubit != 0 and self.states[state].r != 0:
                # both the target qubit and the control qubit is in state |1>
                self.states[state].theta += angle
    

    @profile
    def x(self, qubit): # done
        mask = 1 << qubit
        for state in range(self.n_states):
            if state & mask == 0:
                # the target qubit is in state |0>, 
                # with the adjoint_state being where the target qubit is |1>
                adjoint_state = state ^ mask # flips the target qubit
                self.states[state], self.states[adjoint_state] = \
                self.states[adjoint_state], self.states[state]
        

    @profile
    def cx(self, control_qubit, target_qubit):
        pass


    @profile
    def swap(self, qubit1, qubit2): # done
        for state in range(self.n_states):
            q1_state = (state >> qubit1) & 1
            q2_state = (state >> qubit2) & 1

            if q1_state == 0 and q2_state == 1:
                # only swap if qubit1 == |0> and qubit2 == |1>
                # this is because in the adjoint_state, qubit1 == |1> and qubit2 == |0>
                xor = (q1_state ^ q2_state) & 1
                adjoint_state = state ^ ((xor << qubit1) | (xor << qubit2)) # swaps the two bits
                self.states[state], self.states[adjoint_state] = self.states[adjoint_state], self.states[state]


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