#!/bin/bash
# The general P-cell table  P_{a,b} = 2<8,a,b>  (handoff section 8.3).
# Every P entry is a ray-root witness by Proposition R:
#     Q_b = <16,2b> is N via move 2a,   and   <16,2a> is N via move 2b
# (whichever of a,b makes the root's external gcd right).
# Restrict to cores <8,a,b> that are SYMMETRIC -- every P-cell found so far is.
cd "$(dirname "$0")"
python3 - > table_cand.txt <<'PY'
def info(gens,LIM=3000):
    m=bytearray(LIM+1); m[0]=1
    for n in range(LIM+1):
        if m[n]:
            for g in gens:
                if n+g<=LIM: m[n+g]=1
    mult=min(gens); run=0; F=0
    for n in range(1,LIM+1):
        if m[n]: run+=1
        else: run=0; F=n
        if run>=mult: break
    sym=all(((not m[n])==bool(m[F-n])) for n in range(0,F+1))
    return F,sym,m
from math import gcd
out=[]
for a in range(2,26):
    for b in range(a+1,34):
        if gcd(gcd(8,a),b)!=1: continue
        F,sym,_=info([8,a,b])
        if not sym: continue
        if F>60: continue          # keep runs tractable
        out.append((a,b,F))
for a,b,F in out: print(a,b,F)
PY
n=$(wc -l < table_cand.txt)
echo "symmetric-core candidates with F<=60: $n"
while read -r a b F; do
  r=$(timeout 240 ./vdc --solve 8 "$a" "$b" --kcap 30000 --progress 100000 2>/dev/null)
  case "$r" in
    *"all children N"*) echo "P-CELL  2<8,$a,$b>  F=$F   $r" ;;
    "")                 echo "timeout 2<8,$a,$b>  F=$F" ;;
  esac
done < table_cand.txt
echo TABLEDONE
