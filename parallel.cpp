#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <mpi.h>

using namespace std;

const int MAXN=2048;
const int MACH_NUM = 3;
int N;                  /* graph size */
int E;                  /* edge size  */
bool Edge[MAXN][MAXN];  /* edge table */
int S[MAXN][35][MAXN];  /* set of selected vertices */
int Max_Buffer[MAXN];
int Max[MAXN];          /* max size of clique before verticle i */
int recv_buffer[MACH_NUM];
const float workload_partition[MACH_NUM + 1] = {0, 0.2, 0.5, 1.0};
#define DATA_PATH "/home/parallel_class/s1400012853/project/data/txt/125-34.txt"

int Ans;                /* best solution so far */
int Ans_buffer;

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

bool dfs(int cur, int tot, int world_rank, int proc)
{
    Ans = Ans_buffer;
    /* if current size + untouched vertices <= current best, don't bother searching it */
    if(cur == 0){
        if(tot > Ans){
            Ans = tot;
            Ans_buffer = Ans;
            Max_Buffer[proc] = Ans;
            return 1;
        }
        return 0;
    }
    /* try vertices with higher index */
    for(int i = 0; i < cur; ++i)
    {
        if(cur - i + tot <= Ans) return 0;
        int u = S[proc][tot][i];
        if(Max[u] > 0 and Max[u] + tot <= Ans) return 0;
        int nxt = 0;
        for(int j = i + 1; j < cur; j++)
            if(Edge[u][S[proc][tot][j]]) S[proc][(tot+1)][nxt++] = S[proc][tot][j];
        if(dfs(nxt, tot+1, world_rank, proc)) return 1;
    }
#pragma omp master
    {
        //printf("%d\n", Ans_buffer);
        //fflush(stdout);
        //MPI::COMM_WORLD.Bcast(&Ans_buffer, 1, MPI::INT, world_rank);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    /* init MPI */

    MPI::Init();

    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();
    
    /* parse data */
    ifstream ifs;
    ifs.open(argv[1], ifstream::in);
    ifs >> N;
    for(int i = 1; i <= N; ++i)
    {
        for(int j = 1; j <= N; ++j)
        {
            ifs >> Edge[i-1][j-1];
        }
    }
    /*
    int t1, t2;
    for(int i = 1; i <= E; ++i){
        ifs >> t1 >> t2;
        //printf("%d %d\n", t1, t2);
        //fflush(stdout);
        Edge[t1][t2] = Edge[t2][t1] = 1;
    }
    */
    ifs.close();

    int work_start, work_end;
    work_start = N * workload_partition[world_rank];
    work_end = N * workload_partition[world_rank + 1] - 1;
    printf("%d %d\n", work_start, work_end);
    vector<int> partial;
    Ans=0;
#pragma omp parallel
    {
#pragma omp single
        {
            for(int i = N-1; i >= 0; i--)
            {
#pragma omp task
                {
                    int cur = 0;
                    for(int j = i + 1; j < N; j++) if(Edge[i][j]) S[i][1][cur++] = j;
                    dfs(cur, 1, world_rank, i);
                    Max[i] = Max_Buffer[i];
                }
            }
        }
    }
    printf("%d\n", Ans);
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

