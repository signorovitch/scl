# SCL: Simple CAS Language

Version v1.0-beta

SCL aims to be a human-friendly Computer Algebra System (CAS) inspired by
[maxima](https://maxima.sourceforge.io/) that feels like writing on paper. In
its current state, SCL can be used as a basic 4-function calculator with order
of operations and local variables. The codebase is about 1,400 lines of C,
including a parser, interpreter, and runtime. It uses a linked environment
scoping model.

## Usage

To download and run:

```bash
git clone https://git.signorovitch.org/jacob/scl -b stable && cd scl
make release
./scl.out
```

### For Development

```bash
git clone git@signorovitch.org:jacob/scl --recurse-submodules && cd scl
make all test
./scl.out
```

If you wish to run tests, make sure to run `git clone --recurse-submodules` to
include the [Unity](https://github.com/ThrowTheSwitch/Unity) test framework.

## Syntax

As one would expect, you can evaluate simple infix expressions:

```scl
> 1 + 1
= 2
```

You can also define your own functions and variables:

```scl
> f(x) = 2x
> n = 3
> f(n)
= 6
```

As SCL uses a linked environment model for scope, arguments are passed by
reference by default. If you would like to pass by value (i.e., a copy) you may
use the syntax:

```scl
> f(x) = x = 1
> n = 4
> f($n) # Pass a copy of n.
= 1
> n
= 4
> f(n) # Pass a reference to n.
= 1
> n
> 1
```

Symbolic algebra is done in the following manner:

```scl
> f(x) = x^4
> diff(f, x:sym, 2)
= 12x^2
```

SCL will dynamically decide on types, but you can state them explicitly as
well:

```scl
> f(x: int): int = 2x
> f(3)
= 6
> f(3.1)
! Traceback:
! In call to `f(x: int): int`:
! TypeError (58): Argument `x` must be of type `int`.
```

Variables can be defined, with several attributes:

```scl
> a = 1             // Interpret type automatically.
> b:int = 1         // Must be int.
> c:const:int = 1   // Constant: value can never change.
> x:sym             // Treated symbolicaly.
```
