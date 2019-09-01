#include "../q++.hpp"
#include <iostream>

using namespace std;

int Add(int bit, int a, int b) {
	Qubits qbits (b << bit | a, 3*bit+1, false);
	for (int i = 0;i < bit;i++) {
		qbits.FullAdder(i, i+bit, i+2*bit, i+2*bit+1);
	}
	int res = qbits.M_all();
	res >>= 2 * bit;
	return res;
}

int Sub(int bit, int a, int b) {
	int rev_a = 0;
	for (int i = 0;i < bit;i++) {
		rev_a |= (a & (1 << i)) >> i;
	}
	Qubits qbits = Qubits(b << bit | rev_a, 3*bit+1, false);

	for (int i = 0;i < bit;i++) {
		qbits.FullSubtractor(i, i+bit, i+2*bit, i+2*bit+1);
	}
	int res = qbits.M_all();
	res >>= 2 * bit;
	res = a+1-res;
	return res;
}

void test_add() {
	printf("Add\n");
	for (int a = 0; a < 16; a++) {
		for (int b = 0; b < 16; b++) {
			printf("%d + %d = %d\n", a, b, Add(4, a, b));
		}
	}
}

void test_sub() {
	printf("Sub\n");
	for (int a = 0; a < 16; a++) {
		for (int b = 0; b < 16; b++) {
			printf("%d - %d = %d\n", a, b, Sub(4, a, b));
		}
	}
}

void test_fibo() {
	int a = 1;
	int b = 1;
	int c = 0;
	printf("1\n1\n");
	for (int i = 0;i < 10;i++) {
		c = a;
		a = Add(5, a, b);
		b = c;
		printf("%d\n", a);
	}
}

/*
void CarryGateR(Qubits *q, int a, int b, int c, int d) {
	q->CCNOT(c, b, a);
	q->CNOT(c, b);
	q->CCNOT(d, b, a);
}

void CarryGateL(Qubits *q, int a, int b, int c, int d) {
	q->CCNOT(d, b, a);
	q->CNOT(c, b);
	q->CCNOT(c, b, a);
}

void SumGate(Qubits *q, int a, int b, int c) {
	q->CNOT(b, a);
	q->CNOT(c, a);
}

void add_3bit(Qubits *q) {
	// a 0:2 b 3:5 c 6:9
	for (int i = 0;i < 3;i++) {
		CarryGateR(q, 6+i, i, 3+i, 7+i);
	}
	q->CNOT(2, 5);
	SumGate(q, 8, 2, 5);
	for (int i = 1;i >= 0;i--) {
		CarryGateL(q, 6+i, i, 3+i, 7+i);
		SumGate(q, 6+i, i, 3+i);
	}
}

void sub_3bit(Qubits *q) {
	for (int i = 0;i < 3;i++) {
		CarryGateL(q, 6+i, i, 3+i, 7+i);
	}
}

int add(int a, int b) {
	//printf("        %s\n", btoS(10, a | b << 3).c_str());
	Qubits q = Qubits(a | b << 3, 10);
	add_3bit(&q);
	int ans = q.M_all();
	//ans >>= 6;
	return ans;
}*/

int main() {
	test_add();
	test_sub();
	test_fibo();
	return 0;
}
