#include<math.h>
#include<stdlib.h>
#include"matrices.h"
#include<stdio.h>
#include<err.h>
#include<time.h>

#define SUCC 1
#define FAIL -1

/*
* a matrix is 
		columns
	.   .   .   .   .
rows	.	
	.
	.
where each numbers->numbers is an array of length rows which contains double pointers
each entry points to an array length columns that actually contains the values

common practice to attain a vector is to index the transpose of the matrix
*/
 
static double vector_multiply(double *col, double *row, int length);
static void vector_addition(double *v1, double *v2, int length);
static void scalar_vector_multiplication(double factor, double *vector, int length);
/* make a zero matrix of given dimensions  */
Matrix *constructor(int r, int c){
	unsigned int i;
	Matrix *m;
	if(r <= 0 || c <= 0){
		perror("Give me positive values for dimensions genius");
		return NULL;
	}
	m = malloc(sizeof(Matrix));
	m->rows = r;
	m->columns = c;
	m->numbers = malloc(sizeof(double *)*r);
	for(i = 0; i < r; i++)
		m->numbers[i] = calloc(sizeof(double), c);	
	return m;
}

/* enter 1s along the main diagonal  */
Matrix *identity(int length){
	unsigned int i, j;
	Matrix *m;
	m = constructor(length, length);
	for(i = 0; i < length; i++){
		j = i;
		(m->numbers[i])[j] = 1;
	}
	return m;
}

/* free memory associated with the matrix  */
int destroy_matrix(Matrix *m){
	unsigned int i;
	if(m == NULL)
		return FAIL;
	for(i = 0; i < m->rows; i++)
		free(m->numbers[i]);
	free(m->numbers);
	free(m);
	return SUCC;
}

/* print the matrix  */
int print(Matrix *m){
	unsigned int i, j;
	if(m == NULL)
		return FAIL;
	for(i = 0; i < m->rows; i++){
		for(j = 0; j < m->columns; j++){
			printf("%f ", m->numbers[i][j]);
		}
		printf("\n");
	}
	return SUCC;
}

int row_swap(Matrix *m, int a, int b){
	double *temp;
	if(m == NULL)
		return FAIL;
	if(m->rows <= a || m->rows <= b)
		return FAIL;	
	temp = m->numbers[a];
	m->numbers[a] = m->numbers[b];
	m->numbers[b] = temp;
	return SUCC;	
}

int scalar_multiply(Matrix *m, float scalar){
	int i, j;
	if(m == NULL)
		return FAIL;
	for(i = 0; i < m->rows; i++){
		for(j = 0; j < m->columns; j++)
			(m->numbers[i])[j] *= scalar;
	}
	return SUCC;
}

/* reduce row b by factor*a  */
int reduce(Matrix *m, int a, int b, float factor){
	int i;
	double *to_be_factored;
	if(m == NULL)
		return FAIL;
	if(m->rows < a || m->rows < b)
		return FAIL;
	to_be_factored = malloc(sizeof(double) * m->columns);
	for(i = 0; i < m->rows; i++)
		to_be_factored[i] = m->numbers[a][i];
	/* scalar multiply  */
	for(i = 0; i < m->columns; i++)
		to_be_factored[i] *= factor;
	/* reduce  */
	for(i = 0; i < m->columns; i++)
		(m->numbers[b])[i] -= to_be_factored[i];
	free(to_be_factored);
	return SUCC;
}

/* matrix m will become the identity so the caller must save their matrix themselves  */
Matrix *inversion(Matrix *m){
	Matrix *invert, *goal;
	unsigned int i, j;
	double factor;
	if(m == NULL)
		return NULL;
	if(m->columns != m->rows)
		return NULL;
	invert = identity(m->rows);

	/* reduce each of the rows to get a lower triangle */	
	for(i = 0; i < m->columns; i++){
		for(j = i + 1; j < m->rows; j++){
			if(m->numbers[i][i] == 0)
				continue;
			factor = m->numbers[j][i]/m->numbers[i][i];
			reduce(invert, i, j, factor);
			reduce(m, i, j, factor);
		}
	}
	/* now finish the upper triangle  */
	for(i = m->columns - 1; i > 0; i--){
		for(j = i-1; j>=0; j--){
			if(m->numbers[i][i] == 0)
				continue;
			if(j == -1)
				break;
			factor = m->numbers[j][i]/m->numbers[i][i];
			reduce(invert, i, j, factor);
			reduce(m, i, j, factor);
		}
	}
	/* scale everything to 1 */
	for(i = 0; i < m->columns; i++){
		if(m->numbers[i][i]==0)
			continue;
		factor = 1/m->numbers[i][i];
		row_scalar_multiply(invert, i, factor);
		row_scalar_multiply(m, i, factor);
	}
	goal = identity(m->rows);
	if(equals(goal, m))
		return invert;
	return NULL;
}

int row_scalar_multiply(Matrix *m, int row, float factor){
	int i;
	if(m == NULL)
		return FAIL;
	if(m->rows <= row)
		return FAIL;
	for(i = 0; i < m->columns; i++)
		m->numbers[row][i] *= factor;
	return SUCC;
}

int equals(Matrix *m1, Matrix *m2){
	unsigned int i, j;
	if(m1 == NULL || m2 == NULL)
		return FAIL;
	if(m1->columns != m2->columns || m1->rows != m2->rows)
		return FAIL;
	for(i = 0; i < m1->rows; i++){
		for(j = 0; j < m2->columns; j++){
			if(m1->numbers[i][j] != m2->numbers[i][j])
				return FAIL;
		}
	}
	return SUCC;
}

Matrix *clone(Matrix *m){
	Matrix *copy;
	unsigned int i, j;
	copy = constructor(m->rows, m->columns);
	for(i = 0; i < m->rows; i++)
		for(j = 0; j < m->columns; j++)
			copy->numbers[i][j] = m->numbers[i][j];
	return copy;
}

Matrix *transpose(Matrix *m){
	Matrix *trans;
	unsigned int i, j;
	if(m == NULL)
		return NULL;
	trans = constructor(m->columns, m->rows);
	for(i = 0; i < trans->rows; i++){
		for(j = 0; j < trans->columns; j++)
			trans->numbers[i][j] = m->numbers[j][i];
	}
	return trans;	
}

/* create random matrix of integers  */
Matrix *rand_matrix(int rows, int columns, int modulo){
	Matrix *m;
	unsigned int i, j;
	srand(time(NULL));
	m = constructor(rows, columns);
	for(i = 0; i < rows; i++)
		for(j = 0; j < columns; j++)
			m->numbers[i][j] = rand()%modulo;
	return m; 
}

/* m1 x m2  */
Matrix *multiply(Matrix *m1, Matrix *m2){
	Matrix *product, *trans;
	unsigned int i, j;
	if(m1 == NULL || m2 == NULL)
		return NULL;
	if(m1->columns != m2->rows)
		return NULL;
	trans = transpose(m2);
	product = constructor(m1->rows, m2->columns);
	for(i = 0; i < product->columns; i++){
		for(j = 0; j < product->rows; j++){
			product->numbers[j][i] = vector_multiply(trans->numbers[i], m1->numbers[j], m1->columns);
		}
	}
	destroy_matrix(trans);
	return product;
}
/* v1 x v2  -- simply a helper function -- computes dot product between two vectors*/
static double vector_multiply(double *col, double *row, int length){
	double sum;
	unsigned int i;
	sum = 0;
	for(i = 0; i < length; i++){
		printf("col[%d] %f, row[%d] %f \n", i, col[i], i, row[i]);
		sum += col[i] * row[i];
	}
	printf("\n");
	return sum;
}

/* m1 += m2  */
int add(Matrix *m1, Matrix *m2){
	unsigned int i, j;
	if(m1 == NULL || m2 == NULL)
		return FAIL;
	if(m1->rows != m2->rows || m1->columns != m2->columns)
		return FAIL;
	for(i = 0; i < m1->rows; i++){
		for(j = 0; j < m1->columns; j++)
			m1->numbers[i][j] += m2->numbers[i][j];
	}
	return SUCC;
}

int subtract(Matrix *m1, Matrix *m2){
	unsigned int i, j;
	if(m1 == NULL || m2 == NULL)
		return FAIL;
	if(m1->rows != m2->rows || m1->columns != m2->columns)
		return FAIL;
	for(i = 0; i < m1->rows; i++){
		for(j = 0; j < m1->columns; j++)
			m1->numbers[i][j] -= m2->numbers[i][j];
	}
	return SUCC;
}

Matrix *gram_schmidt(Matrix *m){
	Matrix *set, *basis, *temp;
	double *vector_of_interest;
	unsigned int i;
	if(m == NULL)
		return NULL;
	return NULL;
	set = transpose(m);
	basis = NULL;
	basis = constructor(1, m->columns);
	for(i = 1; i < set->columns; i++){
		/* determine the new orthogonal vector */				
	}

	destroy_matrix(set);
	return basis;
}

double *get_vector(Matrix *m, int index){
	Matrix *copy;
	if(m == NULL)
		return NULL;
	if (index < 0)
		return NULL;
	copy = clone(m);
	copy = transpose(copy);
	return copy->numbers[index];
}

Matrix *projection(Matrix *m, double *v, int length){
	unsigned int i;
	Matrix *project, *trans, *finish;
	double *vector, factor;
	if(m->rows != length)
		return NULL;
	if(m == NULL || v == NULL)
		return NULL;
	trans = transpose(m);
	project = constructor(1, length);	
	for(i = 0; i < m->columns; i++){
		vector = trans->numbers[i];
		factor = vector_multiply(v, vector, m->rows)/vector_multiply(vector, vector, m->rows);
		scalar_vector_multiplication(factor, vector, m->rows);
		vector_addition(project->numbers[0], vector, m->rows);
	}
	destroy_matrix(trans);
	finish = transpose(project);
	free(project);
	return finish;
}

/* v1 *= v2  */
static void scalar_vector_multiplication(double factor, double *vector, int length){
	unsigned int i;
	for(i = 0; i < length; i++)
		vector[i] *= factor;
}

/* v1 += v2  */
static void vector_addition(double *v1, double *v2, int length){
	unsigned int i;
	for(i = 0; i < length; i++){
		v1[i] += v2[i];
	}
}
