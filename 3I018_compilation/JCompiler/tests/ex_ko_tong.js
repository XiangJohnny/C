
function foo (x) {
    return
      (lambda (y) {
            let z = y;
            return
              lambda (v) { return v + x + y + z; };

    }) (x)
    (2 * x)

        + (x * 2);
}

foo(5);
