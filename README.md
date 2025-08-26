# SCL: Simple CAS Language

Version v1.0-beta

SCL aims to be a human-friendly Computer Algebra System (CAS) inspired by
[maxima](https://maxima.sourceforge.io/) that feels like writing on paper. In
its current state, SCL can be used as a functional programming language capable
of performing simple arithmetic. The codebase is about 2,000 lines of C,
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
> f(x) 2 * x
> n = 3
> f(n)
= 6
```

Being a functional programming language at heart, one can of course use lambda functions:

```scl
> (\(x) 2 * x)(5)
= 10
> f(g) g(2)
> f(\(x) 2 * x)
= 4
```
