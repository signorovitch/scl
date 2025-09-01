# SCL: Simple CAS Language

*v0.3*

SCL aims to be a human-friendly Computer Algebra System (CAS) inspired by
[maxima](https://maxima.sourceforge.io/) that feels like writing on paper. In
its current state, SCL can be used as a functional programming language capable
of performing simple arithmetic. The codebase is about 2,000 lines of
handwritten C, including a parser, interpreter, and runtime. It uses a linked
environment scoping model.

## Usage

To download and run:

```bash
git clone https://git.signorovitch.org/scl/scl -b stable && cd scl
make release
./scl.out
```

### For Development

```bash
git clone git@signorovitch.org:scl/scl --recurse-submodules && cd scl
make all test
./scl.out
```

If you wish to run tests, make sure to run `git clone --recurse-submodules` to
include the [Unity](https://github.com/ThrowTheSwitch/Unity) test framework.
*Note that tests are currently in poor use. I hope to amend this in the future.*

## Syntax

SCL's syntax will feel familiar to other functional programming languages.

```scl
> x = 3 + 3 * 3; x + 1
= 13
> f(x) x + 1
> f(1)
= 2
> (\(x) 2 * x)(5)
= 10
> f(g) g(2)
> f(\(x) 2 * x)
= 4
```

Here's a simple factorial function, using recursion:

```scl
> factorial(n) {
>   if (n == 0) { 1 }
>   else { n * factorial(n - 1) }
> }
```

SCL's syntax is quite flexible. The above function could be more concisely
written as:

```scl
> factorial(n) ? n == 0 1 n * factorial(n - 1)
```
