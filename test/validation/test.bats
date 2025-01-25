#!/usr/bin/env bats

bin() { ./scl.out $1 | tail -n1; }

@test "simple addition" {
    run bin "1+1"
    [ "$output" = "= 2.000000" ]

    run bin "-1+1"
    [ "$output" = "= 0.000000" ]

    run bin "1+-1"
    [ "$output" = "= 0.000000" ]

    run bin "-1+-1"
    [ "$output" = "= -2.000000" ]
}

@test "simple subtraction" {
    run bin "1-1"
    [ "$output" = "= 0.000000" ]

    run bin "-1-1"
    [ "$output" = "= -2.000000" ]

    run bin "1--1"
    [ "$output" = "= 2.000000" ]

    run bin "-1--1"
    [ "$output" = "= 0.000000" ]
}

@test "simple multiplication" {
    run bin "1*2"
    [ "$output" = "= 2.000000" ]

    run bin "-1*2"
    [ "$output" = "= -2.000000" ]

    run bin "1*-1"
    [ "$output" = "= -1.000000" ]

    run bin "-1*-1"
    [ "$output" = "= 1.000000" ]
}

@test "simple division" {
    run bin "1/2"
    [ "$output" = "= 0.500000" ]

    run bin "-1/2"
    [ "$output" = "= -0.500000" ]

    run bin "1/-1"
    [ "$output" = "= -1.000000" ]

    run bin "-1/-1"
    [ "$output" = "= 1.000000" ]
}

@test "order of operations" {
    run bin "1+2*3"
    [ "$output" = "= 7.000000" ]

    run bin "2*3+1"
    [ "$output" = "= 7.000000" ]

    run bin "6/2-1"
    [ "$output" = "= 2.000000" ]

    run bin "1-6/2"
    [ "$output" = "= -2.000000" ]
}

@test "order of operations with parenthesis" {
    run bin "(1+2)*3"
    [ "$output" = "= 9.000000" ]

    run bin "-(1+2*3)"
    [ "$output" = "= -7.000000" ]

    run bin "-(-(1+2)*3)"
    [ "$output" = "= 9.000000" ]
}

@test "multiple expressions per line" {
    run bin "1+1;2"
    [ "$output" = "= 2.000000" ]
}

@test "variable definition" {
    run bin "x = 1"
    [ "$output" = "= 1.000000" ]

    run bin "x = 1; x + 1"
    [ "$output" = "= 2.000000" ]
}
