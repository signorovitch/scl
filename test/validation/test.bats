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
