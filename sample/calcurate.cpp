#include "../q++.hpp"
#include <iostream>

using namespace std;

void HalfAdder(Qubits *qubits, int a, int b, int c){
	qubits->CCNOT(a, b, c);
	qubits->CNOT (b, a);
}

void FullAdder(Qubits *qbits, int a, int b, int c0, int c1) {
	qbits->CCNOT(a, b, c1);
	qbits->CNOT (a, b);
	qbits->CCNOT(b, c0, c1);
	qbits->CNOT (b, c0);
}

int Add(int bit, int a, int b) {
	Qubits qbits (b << bit | a, bit*3+1);
	for (int i = 0;i < bit;i++) {
		FullAdder(&qbits, i, i+bit, i+2*bit, i+2*bit+1);
	}
	int res = qbits.M_all();
	res >>= 2 * bit;
	return res;
}

void HalfSuber(Qubits *qubits, int a, int b, int c){
	qubits->CNOT (b, a);
	qubits->CCNOT(a, b, c);
}

void FullSuber(Qubits *qbits, int a, int b, int c0, int c1) {
	qbits->CNOT (b, c0);
	qbits->CCNOT(b, c0, c1);
	qbits->CNOT (a, b);
	qbits->CCNOT(a, b, c1);
}

int Sub(int bit, int a, int b) {
	//Qubits qbits (a << 2*bit | b, bit*3+1);
	Qubits qbits (a, 4);
	for (int i = 0;i < bit;i++) {
		FullSuber(&qbits, i, i+bit, i+2*bit, i+2*bit+1);
	}
	int res = qbits.M_all();
	//res >>= bit;
	//res &= 0b001;
	return res;
}

int main() {
	for (int i = 0;i < 4;i++) {
		Qubits x = Qubits(i, 3);
		HalfAdder(&x, 1, 0, 2);
		int add = x.M_all();
		HalfSuber(&x, 1, 0, 2);
		int sub = x.M_all();
		printf("%s => %s => %s\n", btoS(x.bits, i).c_str(), btoS(x.bits, add).c_str(), btoS(x.bits, sub).c_str());
	}
	for (int i = 0;i < 4;i++) {
		Qubits x = Qubits(i, 4);
		FullAdder(&x, 0, 1, 2, 3);
		int after_add = x.M_all();
		FullSuber(&x, 0, 1, 2, 3);
		int after_sub = x.M_all();
		printf("%s => %s => %s\n", btoS(x.bits, i).c_str(), btoS(x.bits, after_add).c_str(), btoS(x.bits, after_sub).c_str());
	}
	for (int a = 0; a < 2; a++) {
		for (int b = 0; b < 2; b++) {
			printf("%x - %x = %s\n", a, b, btoS(4, Sub(1, a, b)).c_str());
		}
	}
	for (int a = 0;a < 8;a++) {
		printf("%x - %x = %s\n", a, 0, btoS(4, Sub(1, a, 0)).c_str());
	}
	return 0;
}
