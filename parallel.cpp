#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
//#include <mpi.h>

using namespace std;

const int MAXN=512;
int N;                  /* graph size */
bool Edge[MAXN][MAXN];  /* edge table */

int Ans;                /* best solution so far */

/* determine whether current_set + to_add is a clique */
bool is_clique(const vector<int> partial, const int to_add)
{
    for(vector<int>::const_iterator it = partial.begin(); it != partial.end(); ++it)
    {
        if(!Edge[*it][to_add])
        {
            return false;
        }
    }
    return true;
}

void dfs(vector<int> partial, int try_start)
{
    /* if current size + untouched vertices <= current best, don't bother searching it */
    if(partial.size() + N - try_start + 1 <= Ans)
    {
        return;
    }
    /* try vertices with higher index */
    for(int i = try_start; i <= N; ++i)
    {
        if(is_clique(partial, i))
        {
            vector<int> new_set = partial;
            new_set.push_back(i);
#pragma omp task
            dfs(new_set, i + 1);
        }
    }
    if(partial.size() > Ans)
    {
        Ans=partial.size();
    }
}

int main(int argc, char* argv[])
{
    /* init MPI */
    /*
    MPI_Init(&argc, &argv);

    int world_size;
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    */
    /* parse data */
    ifstream ifs;
    ifs.open ("data", ifstream::in);
    ifs >> N;
    for(int i = 1; i <= N; ++i)
    {
        for(int j = 1; j <= N; ++j)
        {
            ifs >> Edge[i][j];
        }
    }
    ifs.close();

    int work_start = 1;
    int work_end = N;

    vector<int> partial;
    Ans=0;
#pragma omp parallel
    {
#pragma omp single
        {
            for (int i = work_start; i <= work_end; i++)
            {
                vector<int> partial;
                partial.push_back(i);
#pragma omp task
                dfs(partial, i + 1);
            }
        }
    }
    printf("%d\n", Ans);
    /*
    MPI_Finalize();
    */
    return 0;
}

