#!/bin/bash
# Targeted P-cell search: only m whose core <8,q,m> is SYMMETRIC.
# All five P-cells found so far have symmetric cores, so this is the right filter.
# usage: targ.sh <q> <m1> <m2> ...
cd "$(dirname "$0")"
q=$1; shift
echo "=== Q_$q = <16,$((2*q))>  candidates: $* ==="
for m in "$@"; do
  r=$(timeout 600 ./vdc --solve 8 "$m" "$q" --kcap 30000 --progress 100000 2>/dev/null)
  case "$r" in
    *"all children N"*) echo "   *** P-CELL: $r   ==> Q_$q is N via even move $((2*m))" ;;
    "")                 echo "   m=$m  timeout(600s)" ;;
    *)                  echo "   m=$m  N" ;;
  esac
done
echo "TARGDONE q=$q"
