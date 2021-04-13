yet another simple Matrix Library - (yasML)
===========================================
![C CI](https://github.com/nhomble/yasML/workflows/C/C++%20CI/badge.svg?branch=master)

# Usage
```c
#include "yasML.h"

int main(void){
  Matrix *i1, *m1, *res;

  i1 = identity(2);
  m1 = constructor(2, 2);
  m1->numbers[0][0] = 1;
  m1->numbers[0][1] = 2;
  m1->numbers[1][0] = 3;
  m1->numbers[1][1] = 4;

  res = multiply(i1, m1);
  print(res);

  return 0;
}
```
# Build
`yasML` needs `math.h` so remember to include `-lm`

# Checkout
Don't forget
```bash
$ git submodule update --init
```
