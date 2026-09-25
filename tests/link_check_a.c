/* second translation unit including yasML.h — this must link cleanly
   against link_check_main.c without "duplicate symbol" errors, which
   requires every function in the header to be static (or inline) */
#include "../yasML.h"

double link_check_determinant(Matrix *m){
    return determinant(m);
}
