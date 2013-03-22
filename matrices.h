/* current representation of a matrix in my mind  */
typedef struct Matrix{
	int rows;
	int columns;
	double **numbers;
} Matrix;

/* return identity matrix for any given length  */
Matrix *identity(int length);
/* use Gauss-Jordan method to invert a given matrix  */
Matrix *inversion(Matrix *m);
/* create an empty matrix with given rows and columns  */
Matrix *constructor(int r, int c);
/* free memory in matrix  */
int destroy_matrix(Matrix *m);
/* print matrix to stdout  */
int print(Matrix *m);
/* swap to rows in a matrix  */
int row_swap(Matrix *m, int a, int b);
/* scalar multiplication  */
int scalar_multiply(Matrix *m, float f);
/* elementary row reduction  */
int reduce(Matrix *m, int a, int b, float factor);
/* check for equality  */
int equals(Matrix *m1, Matrix *m2);
/* scalar multiply a row  */
int row_scalar_multiply(Matrix *m, int row, float factor);
/* create a copy of the matrix  */
Matrix *clone(Matrix *m);
/* compute the transpose  */
Matrix *transpose(Matrix *m);
/* Create a random matrix  */
Matrix *rand_matrix(int rows, int columns, int modulo);
Matrix *multiply(Matrix *m1, Matrix *m2);
int add(Matrix *m1, Matrix *m2);
int subtract(Matrix *, Matrix *);
Matrix *gram_schmidt(Matrix *);
double *get_vector(Matrix *, int);
Matrix *projection(Matrix *, double *, int length);
