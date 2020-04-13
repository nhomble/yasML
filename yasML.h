#ifndef yasML_h
#define yasML_h

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define SUCC 1
#define FAIL -1
/*
* a matrix is 
		columns
	pointer   .   .   .   .
rows	  |	
	  |
	  V
	  .
	  .
	  .

the matrix is an array of array pointers where each array pointer corresponds to a vector
*/
#ifdef __cplusplus

extern "C" {

#endif

/* current representation of a matrix in my mind  */
typedef struct Matrix{
	int rows;
	int columns;
	double **numbers;
} Matrix;
Matrix *identity(int length);
Matrix *inversion(Matrix *m);
Matrix *constructor(int r, int c);
int destroy_matrix(Matrix *m);
int print(Matrix *m);
int row_swap(Matrix *m, int a, int b);
int scalar_multiply(Matrix *m, float f);
int reduce(Matrix *m, int a, int b, float factor);
int equals(Matrix *m1, Matrix *m2);
/* we shouldn`t use clone keyword because it`s extensively used in c++ */
Matrix *clonemx(Matrix *m);
Matrix *transpose(Matrix *m);
Matrix *multiply(Matrix *m1, Matrix *m2);
int add(Matrix *m1, Matrix *m2);
int subtract(Matrix *, Matrix *);
Matrix *gram_schmidt(Matrix *);
double *projection(Matrix *, double *, int length);
int zero_vector(Matrix *);
Matrix *orthonormal_basis(Matrix *);
double determinant(Matrix *m);
Matrix *solved_aug_matrix(Matrix *);
void manual_entry(Matrix **m);
double *eigenvalues(Matrix *m);

static int row_scalar_multiply(Matrix *m, int row, float factor);
static double vector_multiply(double *col, double *row, int length);
static void vector_addition(double *v1, double *v2, int length);
static void scalar_vector_multiplication(double factor, double *vector, int length);
static void vector_subtraction(double *v1, double *v2, int length);

/* return success if there is at least one zero vector in the matrix */
int zero_vector(Matrix *m){
	unsigned int i, j, counter;
	if(m == NULL)
		return FAIL;
	for(i = 0; i < m->columns; i++){
		counter = 0;	
		for(j = 0; j < m->rows; j++){
			if(m->numbers[i][j] == 0)
				counter++;
		}
		if(counter == 3)
			return SUCC;
	}
	return FAIL;
}

/* make a zero matrix of given dimensions */
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
	m->numbers = malloc(sizeof(double *)*c);
	for(i = 0; i < c; i++)
		m->numbers[i] = calloc(sizeof(double), r);
	return m;
}

/* enter 1s along the main diagonal */
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
	for(i = 0; i < m->columns; i++)
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
			printf("%f ", m->numbers[j][i]);
		}
		printf("\n");
	}
	return SUCC;
}

int row_swap(Matrix *m, int a, int b){
	double temp;
	unsigned int i;
	if(m == NULL)
		return FAIL;
	if(m->rows <= a || m->rows <= b)
		return FAIL;	
	for(i = 0; i < m->columns; i++){
		temp = m->numbers[i][a];
		m->numbers[i][a] = m->numbers[i][b];
		m->numbers[i][b] = temp;	
	}		
	return SUCC;	
}

int scalar_multiply(Matrix *m, float scalar){
	int i, j;
	if(m == NULL)
		return FAIL;
	for(i = 0; i < m->columns; i++){
		for(j = 0; j < m->rows; j++)
			(m->numbers[i])[j] *= scalar;
	}
	return SUCC;
}

/* reduce row b by factor*a  */
int reduce(Matrix *m, int a, int b, float factor){
	int i;
	if(m == NULL)
		return FAIL;
	if(m->rows < a || m->rows < b)
		return FAIL;
	for(i = 0; i < m->columns; i++){
		m->numbers[i][b]  -= m->numbers[i][a]*factor;
	}

	return SUCC;
}

/* matrix m will become the identity so the caller must save their matrix themselves  */
Matrix *inversion(Matrix *m){
	Matrix *invert;
	unsigned int i, j, l;
	double factor;
	if(m == NULL)
		return NULL;
	if((m)->columns != (m)->rows)
		return NULL;
	invert = identity((m)->rows);

	/* reduce each of the rows to get a lower triangle */	
	for(i = 0; i < (m)->columns; i++){
		for(j = i + 1; j < (m)->rows; j++){
			if((m)->numbers[i][i] == 0){
				for(l=i+1; l < m->rows; l++){
					if(m->numbers[l][l] != 0){
						row_swap(m, i, l);
						break;
					}
				}	
				continue;
			}
			factor = (m)->numbers[i][j]/((m)->numbers[i][i]);
			reduce(invert, i, j, factor);
			reduce((m), i, j, factor);
		}
	}
	/* now finish the upper triangle  */
	for(i = (m)->columns - 1; i > 0; i--){
		for(j = i-1; j>=0; j--){
			if((m)->numbers[i][i] == 0)
				continue;
			if(j == -1)
				break;
			factor = (m)->numbers[i][j]/((m)->numbers[i][i]);
			reduce(invert, i, j, factor);
			reduce((m), i, j, factor);
		}
	}
	/* scale everything to 1 */
	for(i = 0; i < (m)->columns; i++){
		if((m)->numbers[i][i]==0)
			continue;
		factor = 1/((m)->numbers[i][i]);
		row_scalar_multiply(invert, i, factor);
		row_scalar_multiply((m), i, factor);
	}
	return invert;
}

static int row_scalar_multiply(Matrix *m, int row, float factor){
	int i;
	if(m == NULL)
		return FAIL;
	if(m->rows <= row)
		return FAIL;
	for(i = 0; i < m->columns; i++)
		m->numbers[i][row] *= factor;
	return SUCC;
}

int equals(Matrix *m1, Matrix *m2){
	unsigned int i, j;
	if(m1 == NULL || m2 == NULL)
		return FAIL;
	if(m1->columns != m2->columns || m1->rows != m2->rows)
		return FAIL;
	for(i = 0; i < m1->columns; i++){
		for(j = 0; j < m1->rows; j++){
			if(m1->numbers[i][j] != m2->numbers[i][j])
				return FAIL;
		}
	}
	return SUCC;
}

Matrix *clonemx(Matrix *m){
	Matrix *copy;
	unsigned int i, j;
	copy = constructor(m->rows, m->columns);
	for(i = 0; i < m->columns; i++)
		for(j = 0; j < m->rows; j++)
			copy->numbers[i][j] = m->numbers[i][j];
	return copy;
}

Matrix *transpose(Matrix *m){
	Matrix *trans;
	unsigned int i, j;
	if(m == NULL)
		return NULL;
	trans = constructor(m->columns, m->rows);
	for(i = 0; i < trans->columns; i++){
		for(j = 0; j < trans->rows; j++)
			trans->numbers[i][j] = m->numbers[j][i];
	}
	return trans;	
}

/* m1 x m2  */
Matrix *multiply(Matrix *m1, Matrix *m2){
	Matrix *product, *trans;
	unsigned int i, j;
	if(m1 == NULL || m2 == NULL)
		return NULL;
	if(m1->columns != m2->rows)
		return NULL;
	trans = transpose(m1);
	product = constructor(m1->rows, m2->columns);
	for(i = 0; i < product->columns; i++){
		for(j = 0; j < product->rows; j++){
			product->numbers[i][j] = vector_multiply(trans->numbers[j], m2->numbers[i], m2->rows);
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
		sum += col[i] * row[i];
	}
	return sum;
}

/* m1 += m2  */
int add(Matrix *m1, Matrix *m2){
	unsigned int i, j;
	if(m1 == NULL || m2 == NULL)
		return FAIL;
	if(m1->rows != m2->rows || m1->columns != m2->columns)
		return FAIL;
	for(i = 0; i < m1->columns; i++){
		for(j = 0; j < m1->rows; j++)
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
	for(i = 0; i < m1->columns; i++){
		for(j = 0; j < m1->rows; j++)
			m1->numbers[i][j] -= m2->numbers[i][j];
	}
	return SUCC;
}

/* change m into an orthogonal matrix  */
Matrix *gram_schmidt(Matrix *m){
	Matrix *ortho;
	double *ortho_vector, *temp;
	unsigned int i, j;
	if(m != NULL || m->rows == m->columns || zero_vector(m) != 1){
		/* create my empy matrix to have new orthogonal vector be added to */
		ortho = constructor(m->rows, 1);
		/* initialize with the first vector */
		free(ortho->numbers[0]);
		ortho_vector = malloc(sizeof(double)*m->rows);
		for(i = 0; i < m->rows; i++)
			ortho_vector[i] = m->numbers[0][i];
		ortho->numbers[0] = ortho_vector;	
		/* now loop and go through the gs system */
		for(i = 1; i < m->columns; i++){
			/* first initialize to the regular vector */
			ortho_vector = malloc(sizeof(double)*m->rows);
			for(j = 0; j < m->rows; j++)
				ortho_vector[j] = m->numbers[i][j];
			/* get the subtracting factor */
			temp = projection(ortho, ortho_vector, m->rows);
			/* expand the matrix */
			ortho->columns++;
			ortho->numbers = realloc(ortho->numbers, sizeof(double *)*ortho->columns);
			ortho->numbers[ortho->columns - 1] = ortho_vector;
			vector_subtraction(ortho_vector, temp, m->rows);
		}
		return ortho;
	}
	return NULL;
}

double *projection(Matrix *m, double *v, int length){
	unsigned int i, j;
	double *sum, *copy, *vector, factor;
	if(m->rows != length)
		return NULL;
	if(m == NULL || v == NULL)
		return NULL;
	sum = calloc(sizeof(double), m->rows);
	copy = malloc(sizeof(double)*m->rows);
	for(i = 0; i < m->columns; i++){
		for(j = 0; j < m->rows; j++)
			copy[j] = m->numbers[i][j];
		vector = copy;
		factor = vector_multiply(v, vector, m->rows)/vector_multiply(vector, vector, m->rows);
		scalar_vector_multiplication(factor, vector, m->rows);
		vector_addition(sum, vector, m->rows);
	}
	free(copy);
	return sum;
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

static void vector_subtraction(double *v1, double *v2, int length){
	unsigned int i;
	for(i = 0; i < length; i++){
		v1[i] -= v2[i];
	}
}

double determinant(Matrix *m){
	Matrix *copy;
	unsigned int i, j;
	double det, factor;
	if(m == NULL)
		return -1;
	if(m->columns != m->rows)
		return -1;
	copy = clonemx(m);
	det = 1;

	/* reduce each of the rows to get a lower triangle */	
	for(i = 0; i < copy->columns; i++){
		for(j = i + 1; j < copy->rows; j++){
			if(copy->numbers[i][i] == 0)
				continue;
			factor = copy->numbers[i][j]/(copy->numbers[i][i]);
			reduce(copy, i, j, factor);
		}
	}
	for(i = 0; i < copy->columns; i++)
		det *= copy->numbers[i][i];
	destroy_matrix(copy);
	return det;
}

Matrix *orthonormal_basis(Matrix *m){
	Matrix *orthog;
	unsigned int i, j;
	double factor;
	if(m == NULL)
		return NULL;
	orthog = gram_schmidt(m);
	for(i = 0; i < m->columns; i++){
		factor = 0;
		for(j = 0; j < m->rows; j++)
			factor += orthog->numbers[i][j]*orthog->numbers[i][j];
		factor = sqrt(factor);
		for(j = 0; j < m->rows; j++)
			orthog->numbers[i][j] /= factor;
	}
	return orthog;
}

Matrix *solved_aug_matrix(Matrix *m){
	Matrix *low;
	double factor, absolute;
	unsigned int i, j, k, l;
	if(m == NULL)
		return NULL;
	low = clonemx(m);
	absolute = abs(m->rows - m->columns);
	for(k = 0; k < absolute; k++){
		/* reduce each of the rows to get a lower triangle */	
		for(i = 0; i < low->columns && i<low->rows; i++){
			for(j = i + 1; j < low->rows; j++){
				if(low->numbers[i][i] == 0){
					for(l = i+1; l < low->rows; l++){
						if(m->numbers[l][l]!=0){
							row_swap(low, i, l);
							break;
						}
					}
					continue;
				}
				factor = low->numbers[i][j]/(low->numbers[i][i]);
				reduce(low, i, j, factor);
			}
		}
		/* now finish the upper triangle  */
		for(i = (low->rows>low->columns)?low->columns-1:low->rows-1; i > 0; i--){
			for(j = i-1; j>=0; j--){
				if(low->numbers[i][i] == 0)
					continue;
				if(j == -1)
					break;
				factor = low->numbers[i][j]/(low->numbers[i][i]);
				reduce(low, i, j, factor);
			}
		}
		/* scale everything to 1 */
		for(i = 0; i < low->columns; i++){
			if(low->numbers[i][i]==0)
				continue;
			factor = 1/(low->numbers[i][i]);
			row_scalar_multiply(low, i, factor);
		}
	}
	return low;
}

/* Return an array of all of the possible eigenvalues */
double *eigenvalues(Matrix *m){
	double *values, factor;
	Matrix *red;
	unsigned int i, j, l;
	if(m == NULL)
		return NULL;
	if(m->rows != m->columns)
		return NULL;
	values = malloc(sizeof(double)*m->rows);
	red = clonemx(m);
	/* reduce each of the rows to get a lower triangle */	
	for(i = 0; i < red->columns; i++){
		for(j = i + 1; j < red->rows; j++){
			if(red->numbers[i][i] == 0){
				for(l = i+1; l < red->rows; l++){
					if(red->numbers[l][l] != 0){
						row_swap(red, i, l);
						break;
					}
				}
				continue;
			}
			factor = red->numbers[i][j]/(red->numbers[i][i]);
			reduce(red, i, j, factor);
		}
	}
	for(i = 0; i < red->columns; i++)
		values[i] = red->numbers[i][i];
	return values;
}

/* make your own matrix */
void manual_entry(Matrix **m){
	Matrix *temp;
	int i, j, rows, cols;
	double number;
	char buffer[12];
	printf("Rows | Columns\n");
	/* should only execute once but I need to do error detection */
	rows = -1; cols = -1;
	while(fgets(buffer, 12, stdin) != NULL){
		sscanf(buffer, "%d | %d", &rows, &cols);
		break;
	}
	if(rows == -1 || cols == -1)
		perror("bad input");
	temp = constructor(rows, cols);
	if(temp == NULL)
		return;
	i = 0; j = 0;
	printf("start entering numbers from left to right, top to bottom\nand use either EOF to end input\n");
	while(fgets(buffer, 6, stdin) != NULL){
		number = atof(buffer);
		temp->numbers[i%cols][(int) floor(j/rows)] = number;
		i++;
		j++;
	}
	*m = temp;
}

#ifdef __cplusplus
}
#endif

#endif // yasML
