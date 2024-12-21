# SCL: Simple CAS Language

## Current State

The following things are possible:
  1. Floating-point numbers
  2. Negative numbers
  3. Addition
  4. Subtraction
  5. Multiplication

## Syntax (Planned)

As one would expect, you can evaluate simple infix expressions:

```scl
> 1 + 1
= 2
```

You can also define your own functions:

```scl
> f(x) = 2x
> f(2)
= 4
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
> f(x:int) = 2x
> f(2.2)
! f(x:int): x must be of type int.
```

Variables can be defined, with several attributes:

```scl
> a = 1             // Interpret type automatically.
> b:int = 1         // Must be int.
> c:const:int = 1   // Constant: value can never change.
> x:sym             // Treated symbolicaly.
```
