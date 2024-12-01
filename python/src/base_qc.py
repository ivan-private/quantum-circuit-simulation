from abc import ABC, abstractmethod

import numpy as np


class QuantumCircuitBase(ABC):

    @abstractmethod
    def __init__(self, n_qubits):
        self.n_qubits: int = n_qubits
        self.n_states: int = 2**n_qubits
        self.statevector = None
    
    @abstractmethod
    def get_full_statevector(self) -> np.ndarray[np.complex128]:
        pass

    
    @abstractmethod
    def h(self, qubit) -> None:
        pass
        
    
    @abstractmethod
    def cp(self, angle, control_qubit, target_qubit) -> None:
        pass
    
    
    @abstractmethod
    def x(self, qubit) -> None:
        pass

    
    @abstractmethod
    def swap(self, qubit1, qubit2) -> None:        
        pass
    
    
    @abstractmethod
    def qft(self) -> None:
        pass

