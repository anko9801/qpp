#include "q++.hpp"
#include <iostream>

using namespace std;

void add(Qubit a, Qubit b){
	for (int i = 0;i < 4;i++) {
		Qubits x = Qubits(i, 3);
		x.CCNOT(1, 0, 2);
		x.CNOT(0, 1);
		x.SWAP(0, 1);
		printf("i = %d\n", x.M_all());
	}
}

int test() {
	Qubits x = Qubits(2, 2);
	x.print();
	x.CNOT(0, 1);
	x.print();
	x.CNOT(0, 1);
	x.print();

	return 0;
}

int main() {

	return 0;
}
