#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;
const int N = 4000;
bool G[N][N];
int n, Max[N], Alt[N][N], ans;
struct MAX_CLIQUE {


    bool DFS(int cur, int tot) {
        if(cur==0) {
            if(tot>ans) {
                ans=tot;
                return 1;
            }
            return 0;
        }
        for(int i=0; i<cur; i++) {
            if(cur-i+tot<=ans) return 0;
            int u=Alt[tot][i];
            if(Max[u]+tot<=ans) return 0;
            int nxt=0;
            for(int j=i+1; j<cur; j++)
                if(G[u][Alt[tot][j]]) Alt[tot+1][nxt++]=Alt[tot][j];
            if(DFS(nxt, tot+1)) return 1;
        }
        return 0;
    }

    int MaxClique() {
        ans=0, memset(Max, 0, sizeof Max);
        for(int i=n-1; i>=0; i--) {
            int cur=0;
            memset(Alt, 0, sizeof(Alt));
            for(int j=i+1; j<n; j++) if(G[i][j]) Alt[1][cur++]=j;
            DFS(cur, 1);
            Max[i]=ans;
        }
        return ans;
    }
};

MAX_CLIQUE a;
int e;

int main(int argc, char * argv[]) {
    FILE * fp;
    fp = fopen(argv[1], "r");
    fscanf(fp, "%d", &n);
    int i, j, k, l;
    int t1, t2;
    memset(G, sizeof(G), 0);
    for(i = 1; i <= n; i++){
        for(j = 1; j <= n; j++)fscanf(fp, "%d ", &G[i-1][j-1]);
    }
    printf("%d %d\n", n, e);
    fflush(stdout);
    printf("%d\n", a.MaxClique());
    return 0;
}
