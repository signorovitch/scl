f = \(x) 2 * x
g = \(h) \(x) h(h(x))

f(2) => 4
g(f)(2) => 8

CALL
 argc: 1
 argv: [ 2 ]
 to:
  CALL
   argc: 1
   argv: [
    VREF
     name: f
   ]
   to:
    VREF
     name: g
 fname: NULL


expression + arguments = call = expression
expression + parameters = lambda = expression
expression + name = variable = expression
