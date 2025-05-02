#include "raytracing.h"

void test_individual_transformations()
{
    tuple p = point(1, 0, 1);

    matrix A = rotation_x(90);
    matrix B = scaling(5, 5, 5);
    matrix C = translation(10, 5, 7);

    tuple p2 = matrix_multiply_tuple(A, p);
    tuple expected_p2 = point(1, -1, 0);
    printf("p2 = ");
    print_tuple(p2);
    if (!equal_tuple(p2, expected_p2))
        printf("❌ p2 does not match expected\n");

    tuple p3 = matrix_multiply_tuple(B, p2);
    tuple expected_p3 = point(5, -5, 0);
    printf("p3 = ");
    print_tuple(p3);
    if (!equal_tuple(p3, expected_p3))
        printf("❌ p3 does not match expected\n");

    tuple p4 = matrix_multiply_tuple(C, p3);
    tuple expected_p4 = point(15, 0, 7);
    printf("p4 = ");
    print_tuple(p4);
    if (!equal_tuple(p4, expected_p4))
        printf("❌ p4 does not match expected\n");
}

void test_chained_transformations()
{
    tuple p = point(1, 0, 1);

    matrix A = rotation_x(90);
    matrix B = scaling(5, 5, 5);
    matrix C = translation(10, 5, 7);

    // Matrix multiplication: reverse order
    matrix T = matrix_multiply(C, matrix_multiply(B, A));
    tuple result = matrix_multiply_tuple(T, p);
    tuple expected = point(15, 0, 7);

    printf("T * p = ");
    print_tuple(result);
    if (!equal_tuple(result, expected))
        printf("❌ result does not match expected\n");
}

int main(void)
{
    printf("Test 1: Individual transformations:\n");
    test_individual_transformations();

    printf("\nTest 2: Chained transformations:\n");
    test_chained_transformations();

    return (0);
}
