var x = 10;

var f1 = lambda (y) { y = y + x * 2; return y; };

var f2 = lambda (z, u) { u = lambda (v) { return v + z; }; return u(5); };

f1 (3);
f2 (10, f1);
