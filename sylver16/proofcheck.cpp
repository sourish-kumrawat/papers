// Verify the structural identity behind the proof of R2:
//   S symmetric, F(S)>1  ==>  children(S) = { S+ }  U  children(S+),   S+ = S u {F}
// where children(X) = { <X,u> : u a gap of X, u >= 2 }.
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
static set<Key> children(const Key&S){ int F=keyF(S); set<Key> c;
    for(int u=2;u<=F;++u) if(tst(S,u)) c.insert(childGaps(S,u)); return c; }

int main(int argc,char**argv){
    int G=argc>1?atoi(argv[1]):18;
    int B=3*G+8;
    struct Node{ vector<char> mem; int F; };
    vector<Node> cur; { Node r; r.mem.assign(B+1,1); r.F=0; cur.push_back(r); }
    long long nsym=0, ok=0, bad=0, noExceptional=0;
    for(int gen=0; gen<G; ++gen){
        vector<Node> nxt;
        for(auto&nd:cur) for(int m=nd.F+1;m<=B;++m){
            if(!nd.mem[m]) continue; bool mini=true;
            for(int a=1;a<=m/2;++a) if(nd.mem[a]&&nd.mem[m-a]){ mini=false; break; }
            if(!mini) continue; Node c=nd; c.mem[m]=0; c.F=m; nxt.push_back(c);
        }
        for(auto&nd:nxt){
            Key S; for(int n=1;n<=nd.F;++n) if(!nd.mem[n]) st(S,n); trim(S);
            int F=keyF(S);
            if(F<=1||!isSym(S)) continue;
            ++nsym;
            // (i) symmetric => no exceptional gap (u and F-u both gaps)
            bool anyExc=false;
            for(int u=2;u<F;++u) if(tst(S,u)&&tst(S,F-u)) anyExc=true;
            if(!anyExc) ++noExceptional;
            // (ii) children(S) == {S+} U children(S+)
            Key Sp=childGaps(S,F);
            set<Key> lhs=children(S);
            set<Key> rhs=children(Sp); rhs.insert(Sp);
            if(lhs==rhs) ++ok; else { ++bad; if(bad<=5){ printf("MISMATCH F=%d gaps={",F);
                for(int x=1;x<=F;++x) if(tst(S,x)) printf("%d ",x); printf("}\n"); } }
        }
        cur.swap(nxt);
    }
    printf("symmetric semigroups with F>1, genus<=%d : %lld\n",G,nsym);
    printf("  (i)  no exceptional gap (u, F-u both gaps) : %lld/%lld\n",noExceptional,nsym);
    printf("  (ii) children(S) == {S+} U children(S+)    : %lld/%lld   (mismatches %lld)\n",ok,nsym,bad);
    return 0;
}
