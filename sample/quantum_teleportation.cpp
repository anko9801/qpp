#include "../q++.hpp"
#include <iostream>

using namespace std;

int main() {
	Qubits x = Qubits(0, 3);

	x.SetEx(4);
	x.H(1);
	x.CNOT(1, 0);
	x.CNOT(2, 1);
	x.H(2);
	int M1 = x.M(2);
	int M2 = x.M(1);
	x.CX(0, M2);
	x.CZ(0, M1);
	int M3 = x.M(0);

	printf("%d\n", M3);
	return M3;
}
