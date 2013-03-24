#include<stdlib.h>
#include<stdio.h>
#include"matrices.h"

int main(int argc, char **argv){
	Matrix *m;
	m = rand_matrix(3, 3, 20);
	print(m);		
	/*TEST MANUAL ENTRY
	Matrix *m;
	manual_entry(&m);
	print(m); */
	/*TEST EIGENVALUES
	Matrix *m;
	double *arr;
	unsigned int i;
	m = rand_matrix(2, 2, 3);
	print(m);
	arr = eigenvalues(m);
	for(i = 0; i < 2; i++)
		printf("%f\n", arr[i]);
	*/
	/*TEST EIGENVALUES
	Matrix *m;
	unsigned int i;
	if(argc == 2){
		for(i = 0; i < atoi(argv[1]); i++){
			m=rand_matrix(5, 5, 5);
			orthonormal_basis(m);
		}
	}
	*/
	/*
	Matrix *m1, *m2;
	m1 = rand_matrix(3, 5, 5);
	print(m1);
	printf("==\n");
	m2 = solved_aug_matrix(m1);
	print(m2);
	*/
	/*TEST ORTHONORMAL BASIS
	Matrix *m, *o;
	m = rand_matrix(3, 3, 3);
	print(m);
	printf("==\n");
	o = orthonormal_basis(m);
	print(o);
	*/
	/*TEST DETERMINANT
	Matrix *m;
	m = rand_matrix(2, 2 ,3);
	print(m);
	printf("%f\n", determinant(m));
	*/
	/*TEST INVERSION
	Matrix *m1, *m2, *m3;
	manual_entry(m1);
	print(m1);
	printf("==\n");
	m2 = clone(m1);
	m2 = inversion(m2);
	print(m2);
	printf("==\n");
	m3 = multiply(m1, m2);
	print(m3); */
	/*TEST CLONING AND EQUATING
	Matrix *m1, *m2;
	m1 = rand_matrix(5, 5, 10);
	print(m1);
	printf("==\n");
	m2 = clone(m1);
	print(m2);
	if(equals(m1, m2) == 1)
		printf("They are equal\n");
	*/
	/*TEST FOR ZERO CHECK
	Matrix *m;
	m = rand_matrix(3, 3, 2);
	print(m);
	if(zero_vector(m) == 1)
		printf("yeah\n");
	*/
	/*TEST Scalar Multiplication of row
	Matrix *m;
	m = rand_matrix(4, 4, 4);
	print(m);
	printf("==\n");
	row_scalar_multiply(m, 2, 2);
	print(m);
	*/
	/*TEST ROW REDUCING
	Matrix *m;
	m = rand_matrix(3, 2, 4);
	print(m);
	printf("===\n");
	reduce(m, 0, 2, 4);
	print(m);
	*/
	/*TEST SCALAR MULTIPLICATION ON MATRIX
	Matrix *m;
	m = rand_matrix(3, 3, 3);
	print(m);
	printf("==\n");
	scalar_multiply(m, 3);
	print(m);
	*/
	/*TEST ROW SWAP
	Matrix *m;
	m = rand_matrix(3, 3, 3);
	print(m);
	printf("==\n");
	row_swap(m, 1, 2);
	print(m);
	*/
	/*TEST MULTIPLY
	Matrix *m1, *m2, *m3;
	m1 = rand_matrix(2, 3, 3);
	print(m1);
	m2 = rand_matrix(3, 1, 3);
	print(m2);
	m3 = multiply(m1, m2);
	print(m3);
	*/
	/*TEST GRAN-SCHMIDT	
	Matrix *m, *d;
	m = rand_matrix(3, 3, 4);
	print(m);
	printf("==\n");
	d = gram_schmidt(m);
	print(d);
	*/
	/*TEST PROJECTION
	Matrix *m1, *v;
	double *project;
	unsigned int i;
	m1 = rand_matrix(3, 2, 3);
	print(m1);
	v = rand_matrix(3, 1, 3);
	printf("==\n");
	print(v);
	printf("==\n");
	project = projection(m1, v->numbers[0], 3);
	for(i = 0; i < 3; i++)
		printf("%f\n", project[i]);
	*/
	/*TEST ADDITION/SUBTRACTION
	Matrix *m1, *m2;
	m1 = rand_matrix(3, 3, 3);
	m2 = rand_matrix(3, 3, 3);
	print(m1);
	printf("==\n");
	print(m2);
	printf("==\n");
	add(m1, m2);
	print(m1);
	printf("==\n");
	subtract(m2, m1);
	print(m2);
	*/
	/*TEST TRANSPOSE
	Matrix *m, *m2;
	m = rand_matrix(2, 4, 5);
	print(m);
	m2 = transpose(m);
	print(m2);
	*/
	/*TEST RANDOM MATRIX GENERATOR
	Matrix *m;
	m = rand_matrix(2, 3, 5);
	print(m);
	*/
	/*TEST CONSTRUCTOR && PRINT
	Matrix *m;
	m = constructor(2, 3); 
	print(m);
	*/
	/*TEST IDENTITY
	Matrix *m;
	m = identity(3);
	print(m);
	*/
	/*TEST GRAM-SCHMIDT
	Matrix *m;
	m = rand_matrix(2, 3, 5);
	print(m);
	printf("==\n");
	gram_schmidt(m);
	print(m);
	*/
	/*TEST Projection
	Matrix *m;
	double *array, *a1;
	unsigned int i;
	m = rand_matrix(3, 2, 3);
	print(m);
	printf("==\n");
	array = malloc(sizeof(double)*3);
	for(i = 0; i < 3; i++)
		printf("%f\n", array[i] = i);
	printf("==\n");
	a1 = projection(m, array, 3);
	for(i = 0; i < 3; i++)
		printf("%f\n", a1[i]);
	*/
	return 0;
}
