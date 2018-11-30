function f (x) {
    return lambda (y) { return x * y * 2; };
}

let z = 3;
let g = f(z);

g(42);

