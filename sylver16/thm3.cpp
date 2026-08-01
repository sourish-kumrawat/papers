// Two claims to test:
//
// (A) R2+ :  delta(S) = 2*genus - F - 1 <= 1  and F>1  =>  S is N.
//     Proof idea: delta = 2D (F odd) or 2D+1 (F even), where D = number of pairs
//     {u,F-u} with u != F-u and BOTH gaps.  delta<=1 <=> D=0.  When D=0 the
//     Frobenius-interface identity <S,u> = <S+,u> holds for EVERY gap u:
//       - u != F/2 : F-u in S, so F = u+(F-u) in <S,u>.
//       - u == F/2 : 2u = F in <S,u> automatically.
//     Hence children(S) = {S+} u children(S+) and the Absorption Lemma applies.
//     TEST: delta<=1 <=> D=0, and the children identity, over all S of genus<=G.
//
// (B) Gluing:  S symmetric, d>=2, u odd, gcd(u,d)=1, u in S, u>=2
//              =>  <dS,u> is symmetric,  with F(<dS,u>) = d*F(S) + (d-1)*u.
//     If true, every gcd-dropping move to external gcd 1 from a symmetric core
//     lands on a symmetric numerical semigroup, hence on an N-position by (A).
#include <bits/stdc++.h>
using namespace std;
typedef vector<uint64_t> Key;
static inline int keyF(const Key&k){ for(int i=(int)k.size()-1;i>=0;--i) if(k[i]) return i*64+63-__builtin_clzll(k[i]); return 0; }
static inline void trim(Key&k){ while(!k.empty()&&k.back()==0) k.pop_back(); }
static inline bool tst(const Key&k,int x){ if(x<0) return false; size_t w=x>>6; return w<k.size()&&((k[w]>>(x&63))&1); }
static inline void st(Key&k,int x){ size_t w=x>>6; if(w>=k.size()) k.resize(w+1,0); k[w]|=1ULL<<(x&63); }
static inline bool inS(const Key&S,int n){ int F=keyF(S); return n==0||n>F||!tst(S,n); }
static Key childGaps(const Key&S,int g){ int F=keyF(S); Key C; for(int x=1;x<=F;++x) if(tst(S,x)&&(x<g||tst(C,x-g))) st(C,x); trim(C); return C; }
static bool isSym(const Key&S){ int F=keyF(S); if(F==0) return true; if((F&1)==0) return false;
    for(int n=0;n<=F;++n){ bool gap=(n>0&&tst(S,n)); if(gap!=inS(S,F-n)) return false; } return true; }
static set<Key> children(const Key&S){ int F=keyF(S); set<Key> c; for(int u=2;u<=F;++u) if(tst(S,u)) c.insert(childGaps(S,u)); return c; }

int main(int argc,char**argv){
    int G=argc>1?atoi(argv[1]):18;
    int B=3*G+8;
    struct Node{ vector<char> mem; int F; };
    vector<Node> cur; { Node r; r.mem.assign(B+1,1); r.F=0; cur.push_back(r); }
    long long tot=0, deltaMatch=0, idOK=0, idTested=0, idBad=0;
    long long glueTot=0, glueSymOK=0, glueFOK=0;
    for(int gen=0; gen<G; ++gen){
        vector<Node> nxt;
        for(auto&nd:cur) for(int m=nd.F+1;m<=B;++m){
            if(!nd.mem[m]) continue; bool mini=true;
            for(int a=1;a<=m/2;++a) if(nd.mem[a]&&nd.mem[m-a]){ mini=false; break; }
            if(!mini) continue; Node c=nd; c.mem[m]=0; c.F=m; nxt.push_back(c);
        }
        for(auto&nd:nxt){
            Key S; for(int n=1;n<=nd.F;++n) if(!nd.mem[n]) st(S,n); trim(S);
            int F=keyF(S); if(F<=1) continue;
            int g=0; for(int x=1;x<=F;++x) if(tst(S,x))++g;
            int delta=2*g-F-1;
            // D = number of pairs {u,F-u}, u != F-u, both gaps
            int D=0; for(int u=1;u*2<F;++u) if(tst(S,u)&&tst(S,F-u)) ++D;
            int pred = ((F&1)? 2*D : 2*D+1);
            ++tot; if(pred==delta) ++deltaMatch;
            // (A) children identity when delta <= 1
            if(delta<=1){
                ++idTested;
                Key Sp=childGaps(S,F);
                set<Key> lhs=children(S), rhs=children(Sp); rhs.insert(Sp);
                if(lhs==rhs) ++idOK; else { ++idBad; if(idBad<=5){ printf("  ID MISMATCH delta=%d F=%d gaps={",delta,F);
                    for(int x=1;x<=F;++x) if(tst(S,x)) printf("%d ",x); printf("}\n"); } }
            }
            // (B) gluing, only for symmetric S and small cases
            if(isSym(S) && F<=25){
                for(int d=2;d<=8;d*=2) for(int u=3;u<=2*F+25;u+=2){
                    if(__gcd(u,d)!=1) continue;
                    if(!inS(S,u)) continue;
                    // build T = <dS, u> by membership
                    int LIM=d*F+(d)*u+200;
                    vector<char> mem(LIM+1,0);
                    for(int n=0;n<=LIM;++n){ bool ok=false;
                        for(int k=0;(long long)k*u<=n;++k){ int r=n-k*u; if(r%d==0 && inS(S,r/d)){ ok=true; break; } }
                        mem[n]=ok; }
                    int mult=LIM+1; for(int n=1;n<=LIM;++n) if(mem[n]){mult=n;break;}
                    int run=0,FT=0; bool done=false;
                    for(int n=1;n<=LIM;++n){ if(mem[n])++run; else{run=0;FT=n;} if(run>=mult){done=true;break;} }
                    if(!done) continue;
                    Key T; for(int n=1;n<=FT;++n) if(!mem[n]) st(T,n); trim(T);
                    ++glueTot;
                    if(isSym(T)) ++glueSymOK;
                    else if(glueTot-glueSymOK<=4) printf("  GLUE NOT SYM: S gaps F=%d, d=%d u=%d -> F(T)=%d\n",F,d,u,FT);
                    if(FT == d*F + (d-1)*u) ++glueFOK;
                    else if(glueTot-glueFOK<=4) printf("  GLUE F FORMULA: d=%d u=%d F(S)=%d  got %d want %d\n",d,u,F,FT,d*F+(d-1)*u);
                }
            }
        }
        cur.swap(nxt);
    }
    printf("\nsemigroups with F>1, genus<=%d : %lld\n",G,tot);
    printf("(A1) delta == 2D + [F even]                 : %lld/%lld\n",deltaMatch,tot);
    printf("(A2) delta<=1 => children(S)={S+}Uchildren(S+): %lld/%lld  (mismatches %lld)\n",idOK,idTested,idBad);
    printf("(B1) S symmetric => <dS,u> symmetric         : %lld/%lld\n",glueSymOK,glueTot);
    printf("(B2) F(<dS,u>) = d*F(S)+(d-1)*u              : %lld/%lld\n",glueFOK,glueTot);
    return 0;
}
