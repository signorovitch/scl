n: Int = 3

- 'Int': integer constructor
- '3': integer literal

v: Vec(3, Int) = <1, 2, 3>

Point: Struct = { x: Int, y: Int }

p: Point = { .x = 1, .y = 2 }
p.x + p.y

VecOf(n: Int, t: Type): Type = Vec(n, t)
strings: VecOf(3, Str) = <"Hello", ",", " world.">

f(g) = g(2)

g(n: Int): Int = n * 2
g: Lambda(Int, Int) = \(n: Int):Int n * 2

Int, Vec, Str

f(g: \(Int):Int ):Int = g(2)
f(g: Lambda(Int, Int)): Int = g(2)

f: Lambda(Int, Str, Str) = \(s1: Str, s2: Str) length(s) + length(s2)

Bad: Type = if Until.time() % 2 == 0 Str Int

- Types are code.
- All types inherit from `Type` (the type of `Type` is `Type`).
