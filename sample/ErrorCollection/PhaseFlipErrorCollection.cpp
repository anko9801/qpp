#include "../../q++.hpp"
#include <iostream>
using namespace std;

// return value is qubit index which happened the phase flip error
int PhaseFlipErrorCollection() {
	Qubits x = Qubits(0, 3, true);
	x.H(0);

	// protect the first qubit
	x.CNOT(0, 1);
	x.CNOT(0, 2);
	x.H(0);
	x.H(1);
	x.H(2);

	// phase flip error
	x.Z(0);

	// decode
	x.H(0);
	x.H(1);
	x.H(2);
	x.CNOT(0, 2);
	x.CNOT(0, 1);
	int C1 = x.M(1);
	int C2 = x.M(2);
	if (C1 == 1 && C2 == 1) {
		x.X(0);
	}

	if (C1 == 1 && C2 == 1) return 0;
	else if (C1 == 1 && C2 == 0) return 1;
	else if (C1 == 0 && C2 == 1) return 2;
	else return -1;
}

int main() {
	printf("%d\n", PhaseFlipErrorCollection());
	return 0;
}
