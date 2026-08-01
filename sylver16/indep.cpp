// Independent exact Sylver-coinage solver for numerical semigroups (gcd 1).
// NO heuristics, NO pruning rules: pure memoised minimax on the oversemigroup lattice.
// Used to audit the pruning rules of verify_doubled_core.cpp.
//
// State: a numerical semigroup S (1 is always a gap) stored as the bitmask of its
// gaps in [1..F].  Moves: any gap g >= 2 (naming 1 loses immediately, so a player
// only names 1 when forced).  Child = <S,g>.  Position with gap set {1} is P.
#include <bits/stdc++.h>
using namespace std;

typedef vector<uint64_t> Key;

static inline int keyF(const Key&k){
    for(int i=(int)k.size()-1;i>=0;--i) if(k[i]) return i*64+63-__builtin_clzll(k[i]);
    return 0;
}
static inline void trim(Key&k){ while(!k.empty()&&k.back()==0) k.pop_back(); }
static inline bool tst(const Key&k,int x){ if(x<0) return false; size_t w=x>>6; return w<k.size() && ((k[w]>>(x&63))&1); }
static inline void st(Key&k,int x){ size_t w=x>>6; if(w>=k.size()) k.resize(w+1,0); k[w]|=1ULL<<(x&63); }

struct KeyHash{ size_t operator()(const Key&k) const noexcept {
    uint64_t h=1469598103934665603ULL;
    for(uint64_t x:k){ h^=x; h*=1099511628211ULL; h^=h>>29; }
    return (size_t)h; } };

// gaps of <S,g> given gaps of S
static Key childGaps(const Key&S,int g){
    int F=keyF(S); Key C;
    for(int x=1;x<=F;++x) if(tst(S,x) && (x<g || tst(C,x-g))) st(C,x);
    trim(C); return C;
}

// membership in S: n==0 || n>F || !gap(n)
static inline bool inS(const Key&S,int F,int n){ return n==0 || n>F || !tst(S,n); }

static unordered_map<Key,char,KeyHash> memo;   // 1 = P, 2 = N
static uint64_t nodes=0;

static char solve(const Key&S){
    auto it=memo.find(S); if(it!=memo.end()) return it->second;
    ++nodes;
    int F=keyF(S);
    if(F<=1){ memo.emplace(S,(char)1); return 1; }          // gaps == {1}: must name 1
    char res=1;
    for(int g=2;g<=F;++g) if(tst(S,g)){
        if(solve(childGaps(S,g))==1){ res=2; break; }
    }
    memo.emplace(S,res); return res;
}

static Key gapsOf(const vector<int>&gens,int LIM){
    vector<char> mem(LIM+1,0); mem[0]=1;
    for(int n=0;n<=LIM;++n) if(mem[n]) for(int g:gens) if(g>0&&n+g<=LIM) mem[n+g]=1;
    int m=*min_element(gens.begin(),gens.end()), run=0, F=0;
    for(int n=1;n<=LIM;++n){ if(mem[n]) ++run; else { run=0; F=n; } if(run>=m) break; }
    if(run<m){ fprintf(stderr,"conductor overflow\n"); exit(1); }
    Key K; for(int n=1;n<=F;++n) if(!mem[n]) st(K,n); trim(K); return K;
}

static bool isSymmetric(const Key&S){
    int F=keyF(S);
    if(F<=1||(F&1)==0) return false;
    for(int n=0;n<=F;++n){ bool gap=(n>0&&tst(S,n)); if(gap != inS(S,F,F-n)) return false; }
    return true;
}

// ---- enumerate all numerical semigroups of genus <= G (tree of gap-removal) ----
static void enumerate(int G, vector<Key>&out){
    int B=3*G+8;
    // state: membership vector
    struct Node{ vector<char> mem; int F; int g; };
    vector<Node> cur;
    { Node r; r.mem.assign(B+1,1); r.F=0; r.g=0; cur.push_back(r); }
    for(int gen=0; gen<G; ++gen){
        vector<Node> nxt;
        for(auto&nd:cur){
            // minimal generators m > F
            for(int m=nd.F+1;m<=B;++m){
                if(!nd.mem[m]) continue;
                bool minimal=true;
                for(int a=1;a<=m/2;++a) if(nd.mem[a]&&nd.mem[m-a]){ minimal=false; break; }
                if(!minimal) continue;
                Node c=nd; c.mem[m]=0; c.F=m; c.g=nd.g+1;
                nxt.push_back(c);
            }
        }
        for(auto&nd:nxt){
            Key K; for(int n=1;n<=nd.F;++n) if(!nd.mem[n]) st(K,n); trim(K);
            out.push_back(K);
        }
        cur.swap(nxt);
        fprintf(stderr,"genus %d : %zu semigroups\n",gen+1,cur.size());
    }
}

int main(int argc,char**argv){
    string mode = argc>1?argv[1]:"audit";
    if(mode=="audit"){
        int G = argc>2?atoi(argv[2]):16;
        vector<Key> all; enumerate(G,all);
        fprintf(stderr,"total %zu semigroups, solving...\n",all.size());
        long long nSym=0,symN=0,symP=0, n23=0,r1N=0,r1P=0, nP=0;
        vector<Key> symPex, r1Pex;
        for(auto&K:all){
            char r=solve(K);
            if(r==1) ++nP;
            int F=keyF(K);
            if(isSymmetric(K)){ ++nSym; if(r==2)++symN; else {++symP; if(symPex.size()<10)symPex.push_back(K);} }
            if(F>1 && (!tst(K,2)||!tst(K,3))){ ++n23; if(r==2)++r1N; else {++r1P; if(r1Pex.size()<10)r1Pex.push_back(K);} }
        }
        printf("semigroups solved : %zu   (P: %lld)\n",all.size(),nP);
        printf("RULE R1  [F>1 and (2 in S or 3 in S)  => N] : tested %lld, N %lld, VIOLATIONS %lld\n",n23,r1N,r1P);
        printf("RULE R2  [symmetric and F>1           => N] : tested %lld, N %lld, VIOLATIONS %lld\n",nSym,symN,symP);
        auto dump=[&](const char*tag, vector<Key>&v){
            for(auto&K:v){ printf("  %s counterexample gaps={",tag); int F=keyF(K);
                for(int x=1;x<=F;++x) if(tst(K,x)) printf("%d,",x); printf("}\n"); } };
        dump("R1",r1Pex); dump("R2",symPex);
        printf("memo states=%zu nodes=%llu\n",memo.size(),(unsigned long long)nodes);
    } else if(mode=="delta"){
        int G = argc>2?atoi(argv[2]):16;
        vector<Key> all; enumerate(G,all);
        for(auto&K:all) solve(K);
        map<int,pair<long long,long long>> hist;   // delta -> (P count, N count)
        int minDeltaP=1<<30;
        for(auto&K:all){
            int F=keyF(K), g=0; for(int x=1;x<=F;++x) if(tst(K,x))++g;
            int d=2*g-F-1;
            if(solve(K)==1){ hist[d].first++; if(d<minDeltaP) minDeltaP=d; }
            else hist[d].second++;
        }
        printf("delta=2*genus-F-1 :   P-count   N-count   (delta=0 <=> symmetric)\n");
        for(auto&kv:hist) printf("  delta=%-4d %10lld %10lld\n",kv.first,kv.second.first,kv.second.second);
        printf("minimum delta over all P-positions = %d\n",minDeltaP);
    } else if(mode=="sym"){
        // structure of winning moves from symmetric positions
        int G = argc>2?atoi(argv[2]):14;
        vector<Key> all; enumerate(G,all);
        for(auto&K:all) solve(K);
        printf("F genus  gaps | winning moves (child P)\n");
        long long bad=0, Fwins=0, tot=0;
        for(auto&K:all){
            if(!isSymmetric(K)) continue;
            int F=keyF(K); ++tot;
            if(solve(K)!=2){ ++bad; printf("VIOLATION F=%d\n",F); continue; }
            vector<int> wins;
            for(int g=2;g<=F;++g) if(tst(K,g)&&solve(childGaps(K,g))==1) wins.push_back(g);
            bool hasF = find(wins.begin(),wins.end(),F)!=wins.end();
            if(hasF) ++Fwins;
            if(F<=25){
                int genus=0; for(int x=1;x<=F;++x) if(tst(K,x))++genus;
                printf("%2d %2d  {",F,genus);
                for(int x=1;x<=F;++x) if(tst(K,x)) printf("%d ",x);
                printf("} |");
                for(int w:wins) printf(" %d",w);
                printf("%s\n", hasF?"   [F wins]":"   *** F LOSES ***");
            }
        }
        printf("\nsymmetric positions: %lld, violations of R2: %lld, where move F wins: %lld\n",tot,bad,Fwins);
    } else if(mode=="solve"){
        vector<int> gens; for(int i=2;i<argc;++i) gens.push_back(atoi(argv[i]));
        Key K=gapsOf(gens,4000);
        int F=keyF(K); int genus=0; for(int x=1;x<=F;++x) if(tst(K,x))++genus;
        char r=solve(K);
        printf("<"); for(size_t i=0;i<gens.size();++i) printf("%s%d",i?",":"",gens[i]); printf(">");
        printf(" F=%d genus=%d  %s   states=%zu nodes=%llu\n",F,genus,r==1?"P":"N",memo.size(),(unsigned long long)nodes);
        if(r==2){ for(int g=2;g<=F;++g) if(tst(K,g)&&solve(childGaps(K,g))==1){ printf("   winning move: %d\n",g); break; } }
    }
    return 0;
}
