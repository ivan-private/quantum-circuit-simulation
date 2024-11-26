# Quantum circuit simulators written in Python

## QuantumCircuitBase
- Abstract base class
- All quantum circuit simulator versions will inherit from this
- Acts as an interface

## QuantumCircuit_v1 
- Dense state vector as numpy array
- States as np.complex128 type
- Gates as dense np.complex128 numpy matrices, applied using matrix multiplication

## QuantumCircuit_v2 
- Dense state vector as numpy array
- States as np.complex128 type
- Gates implemented by iterating through all states and performing bit wise operations to the state's binary label 

## QuantumCircuit_v3
- Sparse state vector as Python dictionary
- States as Python complex type
- Gates implemented by iterating through all states and performing bit wise operations to the state's binary label

