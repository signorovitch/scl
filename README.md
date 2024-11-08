# SCL: Simple Calculator Language

## Syntax

As one would expect, you can evaluate simple infix expressions:

```scl
> 1 + 1
= 2
```

You can also define your own functions:

```scl
> f(x) 2x
> f(2)
= 4
```

SCL will dynamically decide on types, but you can state them explicitly as
well:

```scl
> f(x:int) 2x
> f(2.2)
! f(x:int): x must be of type int.
```

Variables can be defined, with several attributes:

```scl
> a = 1             // Interpret type automatically.
> b:int = 1         // Must be int.
> c:const:int = 1   // Constant: value can never change.
> d:lazy = (1 + 1)  // Interpreter will wait as long as possible before
                    // evaluating.
```

