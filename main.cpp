#include "q++.hpp"
#include <iostream>
using namespace std;

void add(){
	for (int i = 0;i < 4;i++) {
		Qubits x = Qubits(i, 3);
		x.CCNOT(1, 0, 2);
		x.CNOT(0, 1);
		x.SWAP(0, 1);
		printf("i = %d\n", x.M_all());
	}
}

int main() {
	Qubits x = Qubits(0, 3);
	x.Set(1);
	x.H(1);
	x.CNOT(1, 2);
	x.CNOT(0, 1);
	x.H(0);
	int M1 = x.M(0);
	int M2 = x.M(1);
	x.X()
	return 0;
}
