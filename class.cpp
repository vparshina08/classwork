#include <iostream>

void rmMtx(int **mtx, size_t rows)
{
    for (size_t i = 0; i < rows; ++i){
        delete [] mtx[i];
    }
    delete [] mtx;
}
//t массив, n количество эл-в в массиве, указатель на длину, кол-во стр
int ** convert(const int * t, size_t n, const size_t * lns, size_t rows)



int main(){


}


