function f (x) {
    let z = 2;
    let g = lambda (y) { return x * y * z; };

    return g;
}

let z = 3;
let g = f(z);

g(42);
