#include <iostream>
#include <new>

void rmMtx(int ** mtx, size_t m);

int ** makeMtx(size_t m, size_t n)
{
    int ** mtxR = new int * [m];
    size_t created = 0;

    try
    {
        for (created = 0; created < m; ++created)
        {
            mtxR[created] = new int [n];
        }
    }
    catch (const std::bad_alloc & e)
    {
        rmMtx(mtxR, created);
        throw;
    }

    return mtxR;
}


int ** transpose(int ** mtx, size_t m, size_t n)
{
    int ** res = makeMtx(n, m);

    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            res[j][i] = mtx[i][j];
        }
    }

    return res;
}


void rmMtx(int ** mtx, size_t m)
{
    for (size_t i = 0; i < m; ++i)
    {
        delete [] mtx[i];
    }

    delete [] mtx;
}


void printMtx(int ** mtx, size_t m, size_t n)
{
    std::cout << mtx[0][0];
    for (size_t j = 1; j < n; ++j)
    {
        std::cout << ' ' << mtx[0][j];
    }

    for (size_t i = 1; i < m; ++i)
    {
        std::cout << '\n' << mtx[i][0];

        for (size_t j = 1; j < n; ++j)
        {
            std::cout << ' ' << mtx[i][j];
        }
    }
}


int main()
{
    size_t m = 0;
    size_t n = 0;
    std::cin >> m >> n;
    if (!std::cin || m == 0 || n == 0)
    {
        return 1;
    }

    int ** mtx = nullptr;

    try
    {
        mtx = makeMtx(m, n);
    }
    catch (const std::bad_alloc & e)
    {
        return 2;
    }

    for (size_t i = 0; i < m * n; ++i)
    {
        std::cin >> mtx[i / n][i % n];
    }

    if (std::cin.fail())
    {
        rmMtx(mtx, m);
        return 1;
    }

    int ** tr = nullptr;

    try
    {
        tr = transpose(mtx, m, n);
    }
    catch (const std::bad_alloc & e)
    {
        rmMtx(mtx, m);
        return 2;
    }

    rmMtx(mtx, m);

    printMtx(tr, n, m);
    std::cout << '\n';

    rmMtx(tr, n);

    return 0;
}