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
static inline Matrix *identity(int length);
static inline Matrix *inversion(Matrix *m);
static inline Matrix *constructor(int r, int c);
static inline int destroy_matrix(Matrix *m);
static inline int print(Matrix *m);
static inline int row_swap(Matrix *m, int a, int b);
static inline int scalar_multiply(Matrix *m, double f);
static inline int reduce(Matrix *m, int a, int b, double factor);
static inline int equals(Matrix *m1, Matrix *m2);
/* we shouldn`t use clone keyword because it`s extensively used in c++ */
static inline Matrix *clonemx(Matrix *m);
static inline Matrix *transpose(Matrix *m);
static inline Matrix *multiply(Matrix *m1, Matrix *m2);
static inline int add(Matrix *m1, Matrix *m2);
static inline int subtract(Matrix *, Matrix *);
static inline Matrix *gram_schmidt(Matrix *);
static inline double *projection(Matrix *, double *, int length);
static inline int zero_vector(Matrix *);
static inline Matrix *orthonormal_basis(Matrix *);
static inline double determinant(Matrix *m);
static inline Matrix *solved_aug_matrix(Matrix *);
static inline void manual_entry(Matrix **m);
static inline double *eigenvalues(Matrix *m);

static inline int row_scalar_multiply(Matrix *m, int row, double factor);
static inline double vector_multiply(double *col, double *row, int length);
static inline void vector_addition(double *v1, double *v2, int length);
static inline void scalar_vector_multiplication(double factor, double *vector, int length);
static inline void vector_subtraction(double *v1, double *v2, int length);

/* return success if there is at least one zero vector in the matrix */
static inline int zero_vector(Matrix *m){
	unsigned int i, j, counter;
	if(m == NULL)
		return FAIL;
	for(i = 0; i < m->columns; i++){
		counter = 0;
		for(j = 0; j < m->rows; j++){
			if(fabs(m->numbers[i][j]) < 1e-9)
				counter++;
		}
		if(counter == m->rows)
			return SUCC;
	}
	return FAIL;
}

/* make a zero matrix of given dimensions */
static inline Matrix *constructor(int r, int c){
	unsigned int i, k;
	Matrix *m;
	if(r <= 0 || c <= 0){
		perror("Give me positive values for dimensions genius");
		return NULL;
	}
	m = malloc(sizeof(Matrix));
	if(m == NULL)
		return NULL;
	m->rows = r;
	m->columns = c;
	m->numbers = malloc(sizeof(double *)*c);
	if(m->numbers == NULL){
		free(m);
		return NULL;
	}
	for(i = 0; i < c; i++){
		m->numbers[i] = calloc(sizeof(double), r);
		if(m->numbers[i] == NULL){
			for(k = 0; k < i; k++)
				free(m->numbers[k]);
			free(m->numbers);
			free(m);
			return NULL;
		}
	}
	return m;
}

/* enter 1s along the main diagonal */
static inline Matrix *identity(int length){
	unsigned int i, j;
	Matrix *m;
	m = constructor(length, length);
	if(m == NULL)
		return NULL;
	for(i = 0; i < length; i++){
		j = i;
		(m->numbers[i])[j] = 1;
	}
	return m;
}

/* free memory associated with the matrix  */
static inline int destroy_matrix(Matrix *m){
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
static inline int print(Matrix *m){
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

static inline int row_swap(Matrix *m, int a, int b){
	double temp;
	unsigned int i;
	if(m == NULL)
		return FAIL;
	if(a < 0 || b < 0 || a >= m->rows || b >= m->rows)
		return FAIL;
	for(i = 0; i < m->columns; i++){
		temp = m->numbers[i][a];
		m->numbers[i][a] = m->numbers[i][b];
		m->numbers[i][b] = temp;	
	}		
	return SUCC;	
}

static inline int scalar_multiply(Matrix *m, double scalar){
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
static inline int reduce(Matrix *m, int a, int b, double factor){
	int i;
	if(m == NULL)
		return FAIL;
	if(a < 0 || b < 0 || a >= m->rows || b >= m->rows)
		return FAIL;
	for(i = 0; i < m->columns; i++){
		m->numbers[i][b]  -= m->numbers[i][a]*factor;
	}

	return SUCC;
}

static inline Matrix *inversion(Matrix *m){
	Matrix *invert, *work;
	unsigned int i, l, pivot_row;
	int j;
	double factor, max_val;
	if(m == NULL)
		return NULL;
	if(m->columns != m->rows)
		return NULL;
	work = clonemx(m);
	invert = identity(work->rows);

	/* reduce each of the rows to get a lower triangle, picking the
	   largest-magnitude candidate in the column as pivot (partial
	   pivoting) instead of only swapping on an exact zero */
	for(i = 0; i < work->columns; i++){
		pivot_row = i;
		max_val = fabs(work->numbers[i][i]);
		for(l = i + 1; l < work->rows; l++){
			if(fabs(work->numbers[i][l]) > max_val){
				max_val = fabs(work->numbers[i][l]);
				pivot_row = l;
			}
		}
		if(max_val < 1e-12){
			/* no usable pivot: m is singular, there is no inverse */
			destroy_matrix(work);
			destroy_matrix(invert);
			return NULL;
		}
		if(pivot_row != i){
			row_swap(work, i, pivot_row);
			row_swap(invert, i, pivot_row);
		}
		for(j = i + 1; j < work->rows; j++){
			factor = work->numbers[i][j]/(work->numbers[i][i]);
			reduce(invert, i, j, factor);
			reduce(work, i, j, factor);
		}
	}
	/* now finish the upper triangle  */
	for(i = work->columns - 1; i > 0; i--){
		for(j = i-1; j>=0; j--){
			factor = work->numbers[i][j]/(work->numbers[i][i]);
			reduce(invert, i, j, factor);
			reduce(work, i, j, factor);
		}
	}
	/* scale everything to 1 */
	for(i = 0; i < work->columns; i++){
		factor = 1/(work->numbers[i][i]);
		row_scalar_multiply(invert, i, factor);
		row_scalar_multiply(work, i, factor);
	}
	destroy_matrix(work);
	return invert;
}

static inline int row_scalar_multiply(Matrix *m, int row, double factor){
	int i;
	if(m == NULL)
		return FAIL;
	if(m->rows <= row)
		return FAIL;
	for(i = 0; i < m->columns; i++)
		m->numbers[i][row] *= factor;
	return SUCC;
}

static inline int equals(Matrix *m1, Matrix *m2){
	unsigned int i, j;
	if(m1 == NULL || m2 == NULL)
		return FAIL;
	if(m1->columns != m2->columns || m1->rows != m2->rows)
		return FAIL;
	for(i = 0; i < m1->columns; i++){
		for(j = 0; j < m1->rows; j++){
			if(fabs(m1->numbers[i][j] - m2->numbers[i][j]) > 1e-9)
				return FAIL;
		}
	}
	return SUCC;
}

static inline Matrix *clonemx(Matrix *m){
	Matrix *copy;
	unsigned int i, j;
	if(m == NULL)
		return NULL;
	copy = constructor(m->rows, m->columns);
	for(i = 0; i < m->columns; i++)
		for(j = 0; j < m->rows; j++)
			copy->numbers[i][j] = m->numbers[i][j];
	return copy;
}

static inline Matrix *transpose(Matrix *m){
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
static inline Matrix *multiply(Matrix *m1, Matrix *m2){
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
static inline double vector_multiply(double *col, double *row, int length){
	double sum;
	unsigned int i;
	sum = 0;
	for(i = 0; i < length; i++){
		sum += col[i] * row[i];
	}
	return sum;
}

/* m1 += m2  */
static inline int add(Matrix *m1, Matrix *m2){
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

static inline int subtract(Matrix *m1, Matrix *m2){
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
static inline Matrix *gram_schmidt(Matrix *m){
	Matrix *ortho;
	double *ortho_vector, *temp;
	unsigned int i, j;
	if(m == NULL)
		return NULL;
	if(m->rows == m->columns || zero_vector(m) != 1){
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
			free(temp);
		}
		return ortho;
	}
	return NULL;
}

static inline double *projection(Matrix *m, double *v, int length){
	unsigned int i, j;
	double *sum, *copy, *vector, factor, denom;
	if(m == NULL || v == NULL)
		return NULL;
	if(m->rows != length)
		return NULL;
	sum = calloc(sizeof(double), m->rows);
	copy = malloc(sizeof(double)*m->rows);
	for(i = 0; i < m->columns; i++){
		for(j = 0; j < m->rows; j++)
			copy[j] = m->numbers[i][j];
		vector = copy;
		denom = vector_multiply(vector, vector, m->rows);
		if(denom == 0)
			continue;
		factor = vector_multiply(v, vector, m->rows)/denom;
		scalar_vector_multiplication(factor, vector, m->rows);
		vector_addition(sum, vector, m->rows);
	}
	free(copy);
	return sum;
}

/* v1 *= v2  */
static inline void scalar_vector_multiplication(double factor, double *vector, int length){
	unsigned int i;
	for(i = 0; i < length; i++)
		vector[i] *= factor;
}

/* v1 += v2  */
static inline void vector_addition(double *v1, double *v2, int length){
	unsigned int i;
	for(i = 0; i < length; i++){
		v1[i] += v2[i];
	}
}

static inline void vector_subtraction(double *v1, double *v2, int length){
	unsigned int i;
	for(i = 0; i < length; i++){
		v1[i] -= v2[i];
	}
}

static inline double determinant(Matrix *m){
	Matrix *copy;
	unsigned int i, j, l;
	int sign;
	double det, factor;
	if(m == NULL)
		return NAN;
	if(m->columns != m->rows)
		return NAN;
	copy = clonemx(m);
	det = 1;
	sign = 1;

	/* reduce each of the rows to get a lower triangle */
	for(i = 0; i < copy->columns; i++){
		if(copy->numbers[i][i] == 0){
			for(l = i + 1; l < copy->rows; l++){
				if(copy->numbers[i][l] != 0){
					row_swap(copy, i, l);
					sign = -sign;
					break;
				}
			}
			if(copy->numbers[i][i] == 0){
				destroy_matrix(copy);
				return 0;
			}
		}
		for(j = i + 1; j < copy->rows; j++){
			factor = copy->numbers[i][j]/(copy->numbers[i][i]);
			reduce(copy, i, j, factor);
		}
	}
	for(i = 0; i < copy->columns; i++)
		det *= copy->numbers[i][i];
	destroy_matrix(copy);
	return det * sign;
}

static inline Matrix *orthonormal_basis(Matrix *m){
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

static inline Matrix *solved_aug_matrix(Matrix *m){
	Matrix *low;
	double factor;
	unsigned int i, l;
	int j;
	if(m == NULL)
		return NULL;
	low = clonemx(m);
	/* reduce each of the rows to get a lower triangle */
	for(i = 0; i < low->columns && i<low->rows; i++){
		for(j = i + 1; j < low->rows; j++){
			if(low->numbers[i][i] == 0){
				for(l = i+1; l < low->rows; l++){
					if(low->numbers[i][l]!=0){
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
			factor = low->numbers[i][j]/(low->numbers[i][i]);
			reduce(low, i, j, factor);
		}
	}
	/* scale everything to 1 — bounded by both dimensions since we index the diagonal */
	for(i = 0; i < low->columns && i < low->rows; i++){
		if(low->numbers[i][i]==0)
			continue;
		factor = 1/(low->numbers[i][i]);
		row_scalar_multiply(low, i, factor);
	}
	return low;
}

/* Return an array of all of the possible eigenvalues, via unshifted QR
   iteration: A_0 = m, A_{k+1} = R_k Q_k where A_k = Q_k R_k, converges
   (for matrices with real, distinct-magnitude eigenvalues) to a matrix
   whose diagonal holds the eigenvalues. Row reduction, used previously,
   does not preserve eigenvalues and was simply wrong. */
static inline double *eigenvalues(Matrix *m){
	double *values;
	Matrix *cur, *q, *qt, *r, *next;
	unsigned int i, j, iter;
	double norm, off_diag;
	if(m == NULL)
		return NULL;
	if(m->rows != m->columns)
		return NULL;
	values = malloc(sizeof(double)*m->rows);
	if(values == NULL)
		return NULL;
	cur = clonemx(m);
	if(cur == NULL){
		free(values);
		return NULL;
	}
	for(iter = 0; iter < 2000; iter++){
		q = gram_schmidt(cur);
		if(q == NULL)
			break;
		for(i = 0; i < q->columns; i++){
			norm = 0;
			for(j = 0; j < q->rows; j++)
				norm += q->numbers[i][j]*q->numbers[i][j];
			norm = sqrt(norm);
			if(norm < 1e-12)
				continue;
			for(j = 0; j < q->rows; j++)
				q->numbers[i][j] /= norm;
		}
		qt = transpose(q);
		r = multiply(qt, cur);
		next = multiply(r, q);
		destroy_matrix(q);
		destroy_matrix(qt);
		destroy_matrix(r);
		destroy_matrix(cur);
		cur = next;

		off_diag = 0;
		for(i = 0; i < cur->columns; i++)
			for(j = i + 1; j < cur->rows; j++)
				off_diag += cur->numbers[i][j]*cur->numbers[i][j];
		if(off_diag < 1e-18)
			break;
	}
	/* unshifted QR stalls on 2x2 blocks whose eigenvalues share a
	   magnitude (e.g. a real pair of opposite sign); solve any block
	   left with a nonzero subdiagonal directly via the quadratic
	   formula instead of reading off a diagonal that never converged */
	for(i = 0; i < cur->columns; ){
		if(i + 1 < cur->columns && fabs(cur->numbers[i][i + 1]) > 1e-6){
			double a11 = cur->numbers[i][i];
			double a12 = cur->numbers[i + 1][i];
			double a21 = cur->numbers[i][i + 1];
			double a22 = cur->numbers[i + 1][i + 1];
			double trace = a11 + a22;
			double det = a11*a22 - a12*a21;
			double disc = trace*trace - 4*det;
			if(disc >= 0){
				double root = sqrt(disc);
				values[i] = (trace + root)/2;
				values[i + 1] = (trace - root)/2;
			} else {
				/* genuinely complex-conjugate pair: this API has no
				   way to represent that, so report the real part */
				values[i] = values[i + 1] = trace/2;
			}
			i += 2;
		} else {
			values[i] = cur->numbers[i][i];
			i += 1;
		}
	}
	destroy_matrix(cur);
	return values;
}

/* make your own matrix */
static inline void manual_entry(Matrix **m){
	Matrix *temp;
	int i, rows, cols;
	double number;
	char buffer[64];
	printf("Rows | Columns\n");
	/* should only execute once but I need to do error detection */
	rows = -1; cols = -1;
	while(fgets(buffer, sizeof(buffer), stdin) != NULL){
		sscanf(buffer, "%d | %d", &rows, &cols);
		break;
	}
	if(rows == -1 || cols == -1)
		perror("bad input");
	temp = constructor(rows, cols);
	if(temp == NULL)
		return;
	i = 0;
	printf("start entering numbers from left to right, top to bottom\nand use either EOF to end input\n");
	while(i < rows*cols && fgets(buffer, sizeof(buffer), stdin) != NULL){
		number = atof(buffer);
		temp->numbers[i%cols][i/cols] = number;
		i++;
	}
	*m = temp;
}

#ifdef __cplusplus
}
#endif

#endif // yasML
