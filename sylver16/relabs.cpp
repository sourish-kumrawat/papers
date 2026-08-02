// Lemma A' (relative absorption) and its consequences.
//
// Setup: S numerical semigroup, F=F(S)>1, S+ = S u {F}.
// Call a gap w (2<=w<F) EXCEPTIONAL if F not in <S,w>  (equivalently no k>=1 with F-kw in S).
// For non-exceptional w we proved <S,w> = <S+,w>.  Write
//     A = { <S+,w> : w non-exceptional },   C = { <S+,w> : w exceptional }
// so children(S)  = {S+} u A u { <S,w> : w exceptional }
//    children(S+) = A u C.
//
// If S is P then every child of S is N; in particular S+ is N, so S+ has a P-child,
// which must lie in A u C; but A is contained in children(S) hence all N.  Therefore:
//
//   THEOREM 4.  If S is a P-position (F>1) then some EXCEPTIONAL gap w has <S+,w> a P-position.
//   Contrapositive: if <S+,w> is N for every exceptional gap w, then S is N.
//
// delta<=1 <=> no exceptional gaps, so Theorem 1' is the special case.
// This program (i) verifies Theorem 4, (ii) measures how many N-positions the
// contrapositive certifies, (iii) tests the gcd-2 analogue on <8,14>.
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
static unordered_map<Key,char,KeyHash> memo;
static char solve(const Key&S){
    auto it=memo.find(S); if(it!=memo.end()) return it->second;
    int F=keyF(S);
    if(F<=1){ memo.emplace(S,(char)1); return 1; }
    char res=1;
    for(int g=2;g<=F;++g) if(tst(S,g)){ if(solve(childGaps(S,g))==1){ res=2; break; } }
    memo.emplace(S,res); return res;
}
// exceptional: no k>=1 with F-kw in S
static bool exceptional(const Key&S,int w){ int F=keyF(S);
    for(int k=1;(long long)k*w<=F;++k) if(inS(S,F-k*w)) return false;
    return true; }

static Key buildSG(const vector<int>&gv,int LIM=4000){
    vector<char> mem(LIM+1,0); mem[0]=1;
    for(int n=0;n<=LIM;++n) if(mem[n]) for(int g:gv) if(g>0&&n+g<=LIM) mem[n+g]=1;
    int m=*min_element(gv.begin(),gv.end()),run=0,F=0; bool done=false;
    for(int n=1;n<=LIM;++n){ if(mem[n])++run; else{run=0;F=n;} if(run>=m){done=true;break;} }
    if(!done){ fprintf(stderr,"overflow\n"); exit(1); }
    Key B; for(int n=1;n<=F;++n) if(!mem[n]) st(B,n); trim(B); return B;
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
    long long nP=0,nN=0, thm4ok=0,thm4bad=0, certN=0, certTested=0, certBad=0;
    for(auto&S:all){
        int F=keyF(S); if(F<=1) continue;
        char r=solve(S);
        vector<int> exc;
        for(int w=2;w<F;++w) if(tst(S,w)&&exceptional(S,w)) exc.push_back(w);
        Key Sp=childGaps(S,F);
        bool someP=false;
        for(int w:exc) if(solve(childGaps(Sp,w))==1){ someP=true; break; }
        if(r==1){ ++nP; if(someP) ++thm4ok; else { ++thm4bad; if(thm4bad<=5){ printf("  THM4 FAIL F=%d gaps={",F);
              for(int x=1;x<=F;++x) if(tst(S,x)) printf("%d ",x); printf("} exc=%zu\n",exc.size()); } } }
        else {
            ++nN;
            if(!someP){ ++certTested; ++certN; }   // contrapositive certifies N
        }
        // sanity: contrapositive must never certify a P
        if(!someP && r==1) ++certBad;
    }
    printf("\n=== Theorem 4 (numerical semigroups, genus<=%d) ===\n",G);
    printf("P-positions: %lld   N-positions: %lld\n",nP,nN);
    printf("  every P has an exceptional gap w with <S+,w> P : %lld/%lld   (failures %lld)\n",thm4ok,nP,thm4bad);
    printf("  contrapositive certifies N                     : %lld / %lld  = %.1f%%   (false certs %lld)\n",
           certN,nN,100.0*certN/nN,certBad);

    // ---- gcd-2 analogue on <8,14> = 2<4,7> ----
    // If (2,B) is P with B symmetric, then some odd u makes <2B+,u> a P numerical semigroup.
    printf("\n=== gcd-2 analogue: <8,14> = 2<4,7>, B+ = <4,7,17> ===\n");
    printf("If (2,B) is P and B symmetric, some odd u must make <2B+,u> = <8,14,34,u> a P-position.\n");
    int found=0;
    for(int u=3;u<=81;u+=2){
        Key T=buildSG({8,14,34,u},6000);
        char r=solve(T);
        if(r==1){ printf("   u=%-3d  <8,14,34,%d>  F=%d  P   <-- witness\n",u,u,keyF(T)); if(++found>=6) break; }
    }
    if(!found) printf("   NO odd u<=81 gives a P-position  ==> necessary condition FAILS in that range\n");
    return 0;
}
