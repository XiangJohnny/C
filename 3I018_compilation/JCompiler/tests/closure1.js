var f = lambda(x) {
    return lambda (y) {
        return x + y;
    };
};

var g = f(2);

g(3);
