OPENQASM 2.0;
include "qelib1.inc";

qreg q[3];
creg c[3];

x q[2];
h q[1];
cx q[1], q[0];
cx q[2], q[1];
h q[2];
measure q[2] -> c[2];
measure q[1] -> c[1];
measure q[0] -> c[0];
