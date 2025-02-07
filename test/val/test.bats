#!/usr/bin/env bats

bin() { ./scl.out $1 | tail -n1; }

@test "simple addition" {
    run bin "1+1"
    [ "$output" = "= 2.000000" ]

    run bin "-1+1"
    echo $output
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

@test "basic blocks" {
    run bin "{1}"
    [ "$output" = "= 1.000000" ]

    run bin "2+{3;4}"
    [ "$output" = "= 6.000000" ]

    run bin "5*{1+1;5*2}"
    echo $output
    [ "$output" = "= 50.000000" ]
}

@test "variable definition" {
    run bin "x=1"
    [ "$output" = "= 1.000000" ]

    run bin "x=1;x+1"
    [ "$output" = "= 2.000000" ]

    run bin "h=7;j=2;k=8;l=4;h*h-l+j*k"
    echo $output
    [ "$output" = "= 61.000000" ]
}

#@test "function definition" {
#    run bin "f(n)=2*n; f(2)"
#    [ "$output" = "= 4.000000" ]
#}

@test "integer arithmetic" {
    run bin "x:int=1"
    [ "$output" = "= 1" ]
}
