function foo (x) {
    function fii (y) {
	return x + y;
    }

    var z = 100;

    return fii(5);
}

foo(4);
fii(5);
foo(4);
fii(5);

var z = 200;
z;

/*****

$ ../../NativeVM/runvm tests/erreur3_td7_exo2.js.bc
runVM v3 (native)
-----------------
loading bytecode file: tests/erreur3_td7_exo2.js.bc
-------------------
9
9
-------------------
VM stopping

*****/
