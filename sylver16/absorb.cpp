// Generalised absorption criterion.
//
// For a gap w of S, <S,w> = <S+,w>  iff  F in <S,w>  iff  exists k>=1 with F-kw in S.
// Call such a gap ABSORBING.  Theorem 1' only used k=1 (plus w=F/2 with k=2).
// If EVERY gap w in [2,F) is absorbing then children(S) = {S+} u children(S+),
// so the Absorption Lemma gives S is N.
//
// This is strictly weaker as a hypothesis than delta(S)<=1, hence a strictly
// stronger N-criterion.  Measure how much stronger.
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
static set<Key> children(const Key&S){ int F=keyF(S); set<Key> c; for(int u=2;u<=F;++u) if(tst(S,u)) c.insert(childGaps(S,u)); return c; }

// is gap w absorbing:  exists k>=1 with F-kw in S
static bool absorbing(const Key&S,int w){ int F=keyF(S);
    for(int k=1;(long long)k*w<=F;++k) if(inS(S,F-k*w)) return true;
    return false; }
static bool allAbsorbing(const Key&S){ int F=keyF(S);
    for(int w=2;w<F;++w) if(tst(S,w)&&!absorbing(S,w)) return false;
    return true; }

static unordered_map<Key,char,KeyHash> memo; static uint64_t nodes=0;
static char solve(const Key&S){
    auto it=memo.find(S); if(it!=memo.end()) return it->second;
    ++nodes; int F=keyF(S);
    if(F<=1){ memo.emplace(S,(char)1); return 1; }
    char res=1;
    for(int g=2;g<=F;++g) if(tst(S,g)){ if(solve(childGaps(S,g))==1){ res=2; break; } }
    memo.emplace(S,res); return res;
}

int main(int argc,char**argv){
    int G=argc>1?atoi(argv[1]):17;
    int B=3*G+8;
    struct Node{ vector<char> mem; int F; };
    vector<Node> cur; { Node r; r.mem.assign(B+1,1); r.F=0; cur.push_back(r); }
    vector<Key> all;
    for(int gen=0; gen<G; ++gen){
        vector<Node> nxt;
        for(auto&nd:cur) for(int m=nd.F+1;m<=B;++m){
            if(!nd.mem[m]) continue; bool mini=true;
            for(int a=1;a<=m/2;++a) if(nd.mem[a]&&nd.mem[m-a]){ mini=false; break; }
            if(!mini) continue; Node c=nd; c.mem[m]=0; c.F=m; nxt.push_back(c);
        }
        for(auto&nd:nxt){ Key S; for(int n=1;n<=nd.F;++n) if(!nd.mem[n]) st(S,n); trim(S); all.push_back(S); }
        cur.swap(nxt);
    }
    for(auto&S:all) solve(S);
    long long tot=0,nP=0,nN=0, absN=0,absP=0, d1=0, absNotD1=0, idOK=0,idBad=0;
    long long Ncaught=0;
    for(auto&S:all){
        int F=keyF(S); if(F<=1) continue;
        ++tot; char r=solve(S); if(r==1)++nP; else ++nN;
        int g=0; for(int x=1;x<=F;++x) if(tst(S,x))++g;
        bool dd = (2*g-F-1<=1);
        bool ab = allAbsorbing(S);
        if(dd) ++d1;
        if(ab){
            if(r==2) ++absN; else { ++absP; if(absP<=5){ printf("  *** ABSORBING BUT P *** F=%d gaps={",F);
                for(int x=1;x<=F;++x) if(tst(S,x)) printf("%d ",x); printf("}\n"); } }
            if(!dd) ++absNotD1;
            // verify the children identity
            Key Sp=childGaps(S,F); set<Key> l=children(S), rr=children(Sp); rr.insert(Sp);
            if(l==rr) ++idOK; else ++idBad;
        }
        if(r==2 && ab) ++Ncaught;
    }
    printf("\nsemigroups F>1, genus<=%d : %lld   (P %lld, N %lld)\n",G,tot,nP,nN);
    printf("  delta<=1                          : %lld\n",d1);
    printf("  all gaps absorbing                : %lld   (of these, NOT delta<=1: %lld)\n",absN+absP,absNotD1);
    printf("     -> N %lld , P %lld  (P must be 0)\n",absN,absP);
    printf("     children identity holds        : %lld ok, %lld bad\n",idOK,idBad);
    printf("  fraction of N-positions certified : %lld / %lld  = %.1f%%\n",Ncaught,nN,100.0*Ncaught/nN);
    return 0;
}
