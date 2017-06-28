#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>

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

int main()
{
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

    vector<int> partial;
    Ans=0;
#pragma omp parallel
    {
#pragma omp single
        {
            dfs(partial, 1);
        }
    }
    printf("%d\n", Ans);
    return 0;
}
