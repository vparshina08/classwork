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

int main()
{
    size_t rows = 0;
    std::cin >> rows;

    if (!std::cin || rows == 0)
    {
        return 1;
    }

    size_t * lns = nullptr;

    try
    {
        lns = new size_t [rows];
    }
    catch (const std::bad_alloc & e)
    {
        return 2;
    }

    size_t n = 0;

    for (size_t i = 0; i < rows; ++i)
    {
        std::cin >> lns[i];

        if (!std::cin)
        {
            delete [] lns;
            return 1;
        }

        n += lns[i];
    }

    int * t = nullptr;

    try
    {
        t = new int [n];
    }
    catch (const std::bad_alloc & e)
    {
        delete [] lns;
        return 2;
    }

    for (size_t i = 0; i < n; ++i)
    {
        std::cin >> t[i];

        if (!std::cin)
        {
            delete [] t;
            delete [] lns;
            return 1;
        }
    }

    int ** mtx = nullptr;

    try
    {
        mtx = convert(t, n, lns, rows);
    }
    catch (const std::bad_alloc & e)
    {
        delete [] t;
        delete [] lns;
        return 2;
    }

    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < lns[i]; ++j)
        {
            std::cout << mtx[i][j];

            if (j + 1 < lns[i])
            {
                std::cout << ' ';
            }
        }

        std::cout << '\n';
    }

    rmMtx(mtx, rows);
    delete [] t;
    delete [] lns;

    return 0;
}