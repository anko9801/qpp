#include "q++.hpp"
#include <iostream>
using namespace std;

int main() {
	for (int i = 0;i < 4;i++) {
		Qubits x = Qubits(i, 3);
		x.CCNOT(1, 0, 2);
		x.CNOT(0, 1);
		x.SWAP(0, 1);
		printf("i = %d\n", x.M());
	}
	return 0;
}
