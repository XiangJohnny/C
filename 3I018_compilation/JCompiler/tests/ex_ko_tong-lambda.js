
function foo (x) {
    return
      (lambda (y) {
          return (lambda(z) {
            return
              lambda (v) { return v + x + y + z; };
          })(y);

    }) (x)
    (2 * x)

        + (x * 2);
}

foo(5);
