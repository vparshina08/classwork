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
{
    int ** mtx = new int * [rows];
    size_t created = 0;
    size_t k = 0;

    try
    {
        for (created = 0; created < rows; ++created)
        {
            mtx[created] = new int [lns[created]];

            for (size_t j = 0; j < lns[created]; ++j)
            {
                mtx[created][j] = t[k];
                ++k;
            }
        }
    }
    catch (const std::bad_alloc & e)
    {
        rmMtx(mtx, created);
        throw;
    }

    return mtx;
}