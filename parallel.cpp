#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <mpi.h>

using namespace std;

const int MAXN=512;
const int MACH_NUM = 4;
int N;                  /* graph size */
bool Edge[MAXN][MAXN];  /* edge table */
const float workload_partition[MACH_NUM + 1] = {0, 0.1, 0.3, 0.6, 1.0};

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
    if((int)partial.size() + N - try_start + 1 <= Ans)
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
    if((int)partial.size() > Ans)
    {
        Ans=partial.size();
    }
}

int main(int argc, char* argv[])
{
    /* init MPI */

    MPI::Init();

    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();
    
    if (world_size != MACH_NUM)
    {
        printf("wrong MPI process number, please start %d processes\n", MACH_NUM);
        MPI::Finalize();
        exit(1);
    }
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

    int work_start, work_end;
    work_start = N * workload_partition[world_rank] + 1;
    work_end = N * workload_partition[world_rank + 1];

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
    int recv_buffer[MACH_NUM];
    MPI::COMM_WORLD.Gather(&Ans, 1, MPI::INT, recv_buffer, 1, MPI::INT, 0);

    if (world_rank == 0)
    {
        int final_ans = 0;
        for (int i = 0; i < MACH_NUM; i++)
        {
            if (recv_buffer[i] > final_ans)
            {
                final_ans = recv_buffer[i];
            }
        }
        printf("%d\n", final_ans);
    }

    MPI::Finalize();

    return 0;
}

