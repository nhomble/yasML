#include "../yasML.h"
#include <stdio.h>
#include <stdlib.h>

double link_check_determinant(Matrix *m);

int main(void){
    Matrix *m = identity(3);
    double a = determinant(m);
    double b = link_check_determinant(m);
    destroy_matrix(m);
    if(a != b){
        fprintf(stderr, "link_check: mismatch %f vs %f\n", a, b);
        return 1;
    }
    printf("link_check: OK (yasML.h links across multiple translation units)\n");
    return 0;
}
