// Solver built on Theorem 4.
//
// children(S)  = {S+} u A u B,   children(S+) = A u C,  where
//    A = { <S+,w> : w non-exceptional gap },  B = { <S,w> : w exceptional },
//    C = { <S+,w> : w exceptional }.
// (w exceptional <=> F not in <S,w>; for non-exceptional w, <S,w> = <S+,w>.)
//
//   - if every element of C is N        -> S is N            [Theorem 4]
//   - else S+ is N, and S is P  <=>  every element of A and of B is N.
//
// |C| = number of exceptional gaps, usually small, and the first branch fires
// for ~65% of N-positions, so this is much cheaper than scanning every gap.
#include <bits/stdc++.h>
using namespace std;
typedef vector<uint64_t> Key;
static inline int keyF(const Key&k){ for(int i=(int)k.size()-1;i>=0;--i) if(k[i]) return i*64+63-__builtin_clzll(k[i]); return 0; }
static inline void trim(Key&k){ while(!k.empty()&&k.back()==0) k.pop_back(); }
static inline bool tst(const Key&k,int x){ if(x<0) return false; size_t w=x>>6; return w<k.size()&&((k[w]>>(x&63))&1); }
static inline void st(Key&k,int x){ size_t w=x>>6; if(w>=k.size()) k.resize(w+1,0); k[w]|=1ULL<<(x&63); }
static inline bool inS(const Key&S,int n){ int F=keyF(S); return n==0||n>F||!tst(S,n); }
static Key childGaps(const Key&S,int g){ int F=keyF(S); Key C; for(int x=1;x<=F;++x) if(tst(S,x)&&(x<g||tst(C,x-g))) st(C,x); trim(C); return C; }
struct KeyHash{ size_t operator()(const Key&k)const noexcept{ uint64_t h=1469598103934665603ULL; for(uint64_t x:k){h^=x;h*=1099511628211ULL;h^=h>>29;} return (size_t)h;} };
static unordered_map<Key,char,KeyHash> memo; static uint64_t nodes=0;

static char solveFast(const Key&S){
    auto it=memo.find(S); if(it!=memo.end()) return it->second;
    ++nodes;
    int F=keyF(S);
    if(F<=1){ memo.emplace(S,(char)1); return 1; }
    // exceptional gaps: no k>=1 with F-kw in S
    vector<int> exc;
    for(int w=2;w<F;++w) if(tst(S,w)){
        bool ab=false;
        for(int k=1;(long long)k*w<=F;++k) if(inS(S,F-k*w)){ ab=true; break; }
        if(!ab) exc.push_back(w);
    }
    Key Sp=childGaps(S,F);
    // Theorem 4: all of C is N  =>  S is N
    bool someCP=false;
    for(int w:exc) if(solveFast(childGaps(Sp,w))==1){ someCP=true; break; }
    if(!someCP){ memo.emplace(S,(char)2); return 2; }
    // S+ is N.  S is P iff every element of A and of B is N.
    for(int w=2;w<F;++w) if(tst(S,w)){
        bool isexc = find(exc.begin(),exc.end(),w)!=exc.end();
        const Key ch = isexc ? childGaps(S,w) : childGaps(Sp,w);
        if(solveFast(ch)==1){ memo.emplace(S,(char)2); return 2; }
    }
    memo.emplace(S,(char)1); return 1;
}
// reference: naive full scan
static unordered_map<Key,char,KeyHash> memoN; static uint64_t nodesN=0;
static char solveNaive(const Key&S){
    auto it=memoN.find(S); if(it!=memoN.end()) return it->second;
    ++nodesN; int F=keyF(S);
    if(F<=1){ memoN.emplace(S,(char)1); return 1; }
    char res=1;
    for(int g=2;g<=F;++g) if(tst(S,g)){ if(solveNaive(childGaps(S,g))==1){ res=2; break; } }
    memoN.emplace(S,res); return res;
}
static Key buildSG(const vector<int>&gv,int LIM=20000){
    vector<char> mem(LIM+1,0); mem[0]=1;
    for(int n=0;n<=LIM;++n) if(mem[n]) for(int g:gv) if(g>0&&n+g<=LIM) mem[n+g]=1;
    int m=*min_element(gv.begin(),gv.end()),run=0,F=0; bool done=false;
    for(int n=1;n<=LIM;++n){ if(mem[n])++run; else{run=0;F=n;} if(run>=m){done=true;break;} }
    if(!done){ fprintf(stderr,"conductor overflow\n"); exit(1); }
    Key B; for(int n=1;n<=F;++n) if(!mem[n]) st(B,n); trim(B); return B;
}
int main(int argc,char**argv){
    string mode=argv[1];
    if(mode=="check"){   // differential test fast vs naive over all semigroups of genus<=G
        int G=atoi(argv[2]); int B=3*G+8;
        struct Node{ vector<char> mem; int F; };
        vector<Node> cur; { Node r; r.mem.assign(B+1,1); r.F=0; cur.push_back(r); }
        long long tot=0,dis=0;
        for(int gen=0; gen<G; ++gen){
            vector<Node> nxt;
            for(auto&nd:cur) for(int m=nd.F+1;m<=B;++m){
                if(!nd.mem[m]) continue; bool mini=true;
                for(int a=1;a<=m/2;++a) if(nd.mem[a]&&nd.mem[m-a]){ mini=false; break; }
                if(!mini) continue; Node c=nd; c.mem[m]=0; c.F=m; nxt.push_back(c);
            }
            for(auto&nd:nxt){ Key S; for(int n=1;n<=nd.F;++n) if(!nd.mem[n]) st(S,n); trim(S);
                ++tot; if(solveFast(S)!=solveNaive(S)){ ++dis; if(dis<=3) printf("  DISAGREE F=%d\n",keyF(S)); } }
            cur.swap(nxt);
        }
        printf("differential test genus<=%d : %lld semigroups, %lld disagreements\n",G,tot,dis);
        printf("  fast nodes=%llu   naive nodes=%llu   ratio %.2fx\n",
               (unsigned long long)nodes,(unsigned long long)nodesN,(double)nodesN/max<uint64_t>(nodes,1));
    } else if(mode=="solve"){
        vector<int> gv; for(int i=2;i<argc;++i) gv.push_back(atoi(argv[i]));
        Key S=buildSG(gv);
        int F=keyF(S),g=0; for(int x=1;x<=F;++x) if(tst(S,x))++g;
        auto t0=chrono::steady_clock::now();
        char r=solveFast(S);
        double dt=chrono::duration<double>(chrono::steady_clock::now()-t0).count();
        printf("<"); for(size_t i=0;i<gv.size();++i) printf("%s%d",i?",":"",gv[i]);
        printf(">  F=%d genus=%d delta=%d  %s   nodes=%llu  %.2fs\n",F,g,2*g-F-1,r==1?"P":"N",
               (unsigned long long)nodes,dt);
    }
    return 0;
}
