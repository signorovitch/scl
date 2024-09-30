#include <stdio.h>

#include "include/util.h"

int main(int argc, char** argv) {
    printf("2 is even: %s\n", is_even(2) ? "true" : "false");
    printf("5 is even: %s\n", is_even(5) ? "true" : "false");
    return 0;
}
