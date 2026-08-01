// Independent exact solver for Sylver-coinage positions of external gcd 2, i.e. 2B.
//
// Finite parts are computed unconditionally (no unproven pruning).
// The single infinite family (odd moves) is discharged by:
//
//   Lemma C (duplication):  B numerical semigroup, z odd, z in B
//                           =>  <2B,z> = 2B  disjoint-union  (z + 2B)
//   Lemma D:                <2B,z> is symmetric  <=>  B is symmetric
//   Rule  R2 (ASSUMED):     S symmetric with F(S)>1  =>  S is an N-position
//
// so when B is symmetric every odd move z in B is N, leaving only the finitely
// many odd gaps of B to check by hand.  R2 is the one unproven input; it is
// flagged in the output of every P-certificate.
#include <bits/stdc++.h>
using namespace std;

typedef vector<uint64_t> Key;
static inline int keyF(const Key&k){ for(int i=(int)k.size()-1;i>=0;--i) if(k[i]) return i*64+63-__builtin_clzll(k[i]); return 0; }
static inline void trim(Key&k){ while(!k.empty()&&k.back()==0) k.pop_back(); }
static inline bool tst(const Key&k,int x){ if(x<0) return false; size_t w=x>>6; return w<k.size() && ((k[w]>>(x&63))&1); }
static inline void st(Key&k,int x){ size_t w=x>>6; if(w>=k.size()) k.resize(w+1,0); k[w]|=1ULL<<(x&63); }
struct KeyHash{ size_t operator()(const Key&k) const noexcept { uint64_t h=1469598103934665603ULL; for(uint64_t x:k){h^=x;h*=1099511628211ULL;h^=h>>29;} return (size_t)h; } };
static inline bool inS(const Key&S,int n){ int F=keyF(S); return n==0 || n>F || !tst(S,n); }

static Key childGaps(const Key&S,int g){ int F=keyF(S); Key C; for(int x=1;x<=F;++x) if(tst(S,x)&&(x<g||tst(C,x-g))) st(C,x); trim(C); return C; }
static bool isSymmetric(const Key&S){ int F=keyF(S); if(F==0) return true; if((F&1)==0) return false; for(int n=0;n<=F;++n){ bool gap=(n>0&&tst(S,n)); if(gap!=inS(S,F-n)) return false; } return true; }
static int genusOf(const Key&S){ int F=keyF(S),c=0; for(int x=1;x<=F;++x) if(tst(S,x))++c; return c; }
static string gapstr(const Key&S){ int F=keyF(S); string s="{"; for(int x=1;x<=F;++x) if(tst(S,x)) s+=to_string(x)+" "; return s+"}"; }

// ---------------- exact gcd-1 solver (no unproven pruning) ----------------
static unordered_map<Key,char,KeyHash> memo1; static uint64_t nodes1=0;
static bool USE_R2=true; static int ODDEXTRA=80;
static char solve1(const Key&S){
    auto it=memo1.find(S); if(it!=memo1.end()) return it->second;
    ++nodes1; int F=keyF(S);
    if(F<=1){ memo1.emplace(S,(char)1); return 1; }
    // PROVED rule R1: if 2 in S or 3 in S (and F>1) then N.
    //   3 in S => <S,2> = {0,2,3,4,...} whose only gap is 1 => P.
    //   2 in S => 3 is a gap and <S,3> = {0,2,3,4,...} => P.
    if(!tst(S,2)||!tst(S,3)){ memo1.emplace(S,(char)2); return 2; }
    // R2 (ASSUMED, verified exhaustively to genus 20): symmetric with F>1 => N
    if(USE_R2 && isSymmetric(S)){ memo1.emplace(S,(char)2); return 2; }
    char res=1;
    for(int g=2;g<=F;++g) if(tst(S,g)){ if(solve1(childGaps(S,g))==1){ res=2; break; } }
    memo1.emplace(S,res); return res;
}
static int winMove1(const Key&S){ int F=keyF(S); for(int g=2;g<=F;++g) if(tst(S,g)&&solve1(childGaps(S,g))==1) return g; return -1; }

// ---------------- <2B,u> for odd u, built from membership ----------------
static Key oddChild(const Key&B,int u){
    int FB=keyF(B); int LIM=2*FB+2*u+128;
    vector<char> mem(LIM+1,0);
    for(int n=0;n<=LIM;++n){
        bool ok=false;
        for(int k=(n&1); k*u<=n; k+=2){ int r=n-k*u; if((r&1)==0 && inS(B,r/2)){ ok=true; break; } }
        mem[n]=ok;
    }
    int mult=LIM+1; for(int n=1;n<=LIM;++n) if(mem[n]){ mult=n; break; }
    int run=0,F=0; bool done=false;
    for(int n=1;n<=LIM;++n){ if(mem[n])++run; else {run=0;F=n;} if(run>=mult){done=true;break;} }
    if(!done){ fprintf(stderr,"oddChild: conductor overflow B=%s u=%d\n",gapstr(B).c_str(),u); exit(1); }
    Key K; for(int n=1;n<=F;++n) if(!mem[n]) st(K,n); trim(K); return K;
}

// ---------------- gcd-2 solver ----------------
// returns 1 = P, 2 = N, 0 = UNKNOWN (infinite odd tail not discharged)
struct Res2{ char v=0; string wit; };
static unordered_map<Key,Res2,KeyHash> memo2;
static bool VERBOSE=false;

static Res2 solve2(const Key&B){
    auto it=memo2.find(B); if(it!=memo2.end()) return it->second;
    memo2.emplace(B,Res2{0,"(computing)"});   // guard; even-children recursion is on strictly larger semigroups
    int FB=keyF(B);
    Res2 out;
    // --- look for a P child: that alone proves N ---
    // odd moves u (child is a numerical semigroup); scan far enough to cover every odd gap of B
    // symmetric core: every odd u in B is N (Lemma C+D+R2), so only odd gaps (u<=FB)
    // can be witnesses.  Non-symmetric core: large odd u may be a witness, so scan wider.
    int ODDCAP = isSymmetric(B) ? max(FB,8)+2 : max(FB,8)+2+ODDEXTRA;
    for(int u=3;u<=ODDCAP;u+=2){
        Key C=oddChild(B,u);
        if(solve1(C)==1){ out={2,"odd move "+to_string(u)}; memo2[B]=out; return out; }
    }
    // even moves 2g, g a gap of B
    for(int g=1;g<=FB;++g) if(tst(B,g)){
        Res2 r=solve2(childGaps(B,g));
        if(r.v==1){ out={2,"even move "+to_string(2*g)}; memo2[B]=out; return out; }
    }
    // --- no P child found; try to prove P ---
    if(!isSymmetric(B)){ out={0,"UNKNOWN: core not symmetric, infinite odd tail not discharged"}; memo2[B]=out; return out; }
    // every odd u in B gives a symmetric child (Lemma C + D) => N by R2.
    // every odd gap u of B was checked exactly in the scan above (u <= FB <= ODDCAP) and was N.
    // every even child must be N.
    for(int g=1;g<=FB;++g) if(tst(B,g)){
        Res2 r=solve2(childGaps(B,g));
        if(r.v!=2){ out={0,"UNKNOWN: even child 2<B,"+to_string(g)+"> is "+(r.v==1?"P":"UNKNOWN")}; memo2[B]=out; return out; }
    }
    out={1,"all children N (odd tail via symmetry+R2)"};
    memo2[B]=out; return out;
}

// sanity check of Lemma C and Lemma D
static void lemmaCheck(int maxF){
    // enumerate numerical semigroups by brute force over gap sets is costly; instead test
    // a family of explicit semigroups
    vector<vector<int>> gens={{2,3},{2,5},{3,4},{3,5},{4,7},{4,5},{5,6},{6,8,9},{8,10,17},{5,7,11},{4,9},{6,7,8},{8,14,19},{3,7},{5,8},{7,9,11}};
    long long okC=0,okD=0,tot=0;
    for(auto&gv:gens){
        // build B
        int LIM=2000; vector<char> mem(LIM+1,0); mem[0]=1;
        for(int n=0;n<=LIM;++n) if(mem[n]) for(int g:gv) if(n+g<=LIM) mem[n+g]=1;
        int m=*min_element(gv.begin(),gv.end()),run=0,F=0;
        for(int n=1;n<=LIM;++n){ if(mem[n])++run; else{run=0;F=n;} if(run>=m)break; }
        Key B; for(int n=1;n<=F;++n) if(!mem[n]) st(B,n); trim(B);
        if(keyF(B)>maxF) continue;
        bool bsym=isSymmetric(B);
        for(int z=1;z<=4*F+40;z+=2){
            if(!inS(B,z)) continue;              // need z in B
            Key D=oddChild(B,z);
            // Lemma C: D = 2B u (z+2B)
            int FD=keyF(D); bool cok=true;
            for(int n=0;n<=FD+8;++n){
                bool inD = inS(D,n);
                bool pred = ((n%2==0)&&inS(B,n/2)) || (n>=z && ((n-z)%2==0) && inS(B,(n-z)/2));
                if(inD!=pred){ cok=false; break; }
            }
            bool dok = (isSymmetric(D)==bsym);
            ++tot; okC+=cok; okD+=dok;
            if(!cok) printf("  LEMMA C FAIL B=%s z=%d\n",gapstr(B).c_str(),z);
            if(!dok) printf("  LEMMA D FAIL B=%s z=%d (B sym=%d, D sym=%d)\n",gapstr(B).c_str(),z,(int)bsym,(int)isSymmetric(D));
        }
    }
    printf("Lemma C (duplication)      : %lld/%lld cases OK\n",okC,tot);
    printf("Lemma D (symmetry transfer): %lld/%lld cases OK\n",okD,tot);
}

static Key buildSG(const vector<int>&gv){
    int LIM=8000; vector<char> mem(LIM+1,0); mem[0]=1;
    for(int n=0;n<=LIM;++n) if(mem[n]) for(int g:gv) if(g>0&&n+g<=LIM) mem[n+g]=1;
    int m=*min_element(gv.begin(),gv.end()),run=0,F=0; bool done=false;
    for(int n=1;n<=LIM;++n){ if(mem[n])++run; else{run=0;F=n;} if(run>=m){done=true;break;} }
    if(!done){ fprintf(stderr,"buildSG overflow\n"); exit(1); }
    Key B; for(int n=1;n<=F;++n) if(!mem[n]) st(B,n); trim(B); return B;
}

int main(int argc,char**argv){
    string mode=argc>1?argv[1]:"lemma";
    if(const char*e=getenv("ODDEXTRA")) ODDEXTRA=atoi(e);
    if(const char*e=getenv("NO_R2")) USE_R2=(atoi(e)==0);
    if(mode=="lemma"){ lemmaCheck(60); return 0; }
    if(mode=="p2"){
        vector<int> gv; for(int i=2;i<argc;++i) gv.push_back(atoi(argv[i]));
        Key B=buildSG(gv);
        printf("core B=<"); for(size_t i=0;i<gv.size();++i) printf("%s%d",i?",":"",gv[i]);
        printf(">  F=%d genus=%d symmetric=%s gaps=%s\n",keyF(B),genusOf(B),isSymmetric(B)?"YES":"no",gapstr(B).c_str());
        Res2 r=solve2(B);
        printf("POSITION 2B = <"); for(size_t i=0;i<gv.size();++i) printf("%s%d",i?",":"",2*gv[i]);
        printf(">  ==>  %s   [%s]\n", r.v==1?"P":(r.v==2?"N":"UNKNOWN"), r.wit.c_str());
        printf("   gcd-1 states=%zu nodes=%llu ; gcd-2 states=%zu\n",memo1.size(),(unsigned long long)nodes1,memo2.size());
        if(getenv("TRACE")){
            printf("   --- gcd-2 frontier ---\n");
            for(auto&kv:memo2){ if(kv.second.v!=0) continue;
                printf("   UNKNOWN core F=%d genus=%d sym=%d gaps=%s  :: %s\n",
                    keyF(kv.first),genusOf(kv.first),(int)isSymmetric(kv.first),gapstr(kv.first).c_str(),kv.second.wit.c_str()); }
            printf("   --- P cores found ---\n");
            for(auto&kv:memo2){ if(kv.second.v!=1) continue;
                printf("   P core F=%d genus=%d sym=%d gaps=%s\n",
                    keyF(kv.first),genusOf(kv.first),(int)isSymmetric(kv.first),gapstr(kv.first).c_str()); }
        }
        return 0;
    }
    if(mode=="oddscan"){
        // for core B (given by generators after the cap), report outcome of <2B,u> for odd u
        int lo=atoi(argv[2]), hi=atoi(argv[3]);
        vector<int> gv; for(int i=4;i<argc;++i) gv.push_back(atoi(argv[i]));
        Key B=buildSG(gv);
        printf("core B F=%d genus=%d sym=%d gaps=%s\n",keyF(B),genusOf(B),(int)isSymmetric(B),gapstr(B).c_str());
        for(int u=lo;u<=hi;u+=2){
            Key C=oddChild(B,u);
            char r=solve1(C);
            int w = (r==2)? winMove1(C) : -1;
            printf("u=%-4d  <2B,u> F=%-4d genus=%-4d sym=%d  %s  win=%d\n",
                   u,keyF(C),genusOf(C),(int)isSymmetric(C), r==1?"P":"N", w);
            fflush(stdout);
        }
        return 0;
    }
    if(mode=="ray"){
        // classify Q_n = <16,2n> = 2<8,n> for n in [lo,hi]
        int lo=atoi(argv[2]),hi=atoi(argv[3]);
        for(int n=lo;n<=hi;++n){
            if(n%8==0) continue;
            int d=__gcd(8,n);
            vector<int> gv={8/d,n/d};
            Key B=buildSG(gv);
            // 2<8,n> has external gcd 2d ; only d==1 is a genuine gcd-2 position
            if(d!=1){ printf("n=%-3d  Q_n=<16,%d>  external gcd %d  -- outside gcd-2 layer, skipped\n",n,2*n,2*d); continue; }
            Res2 r=solve2(B);
            printf("n=%-3d (n mod 8 = %d)  Q_n=<16,%d>  core <8,%d> sym=%d  ==> %-7s [%s]\n",
                   n,n%8,2*n,n,(int)isSymmetric(B), r.v==1?"P":(r.v==2?"N":"UNKNOWN"), r.wit.c_str());
            fflush(stdout);
        }
        return 0;
    }
    return 0;
}
