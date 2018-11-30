function f (x) {
    let g = lambda (y) { return x * y * 2; };
    return g;
}

let z = 3;
let g = f(z);

g(42);

