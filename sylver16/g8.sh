#!/bin/bash
# gcd-8 ray frontier: <16,40> = 8<2,5>  (q=5; q=1 and q=3 already N).
# Its gcd-2 children are <16,40,2m> = 2<8,20,m> for ODD m.
# Also <16,28> = 4<4,7>, the gcd-4 frontier: children 2<8,14,m> for odd m.
cd "$(dirname "$0")"
echo "=== <16,40> = 8<2,5>  : scanning 2<8,20,m>, m odd ==="
for m in 1 3 5 7 9 11 13 15 17 19 21 23 25; do
  r=$(timeout 200 ./vdc --solve 8 20 "$m" --kcap 30000 --progress 100000 2>/dev/null)
  case "$r" in
    *"all children N"*) echo "   *** P-CELL: $r   ==> <16,40> is N via move $((2*m))" ;;
    "")                 echo "   m=$m timeout" ;;
    *)                  echo "   m=$m N" ;;
  esac
done
echo "=== <16,28> = 4<4,7>  : scanning 2<8,14,m>, m odd ==="
for m in 1 3 5 9 11 13 15 17 19 21 23; do
  r=$(timeout 200 ./vdc --solve 8 14 "$m" --kcap 30000 --progress 100000 2>/dev/null)
  case "$r" in
    *"all children N"*) echo "   *** P-CELL: $r   ==> <16,28> is N via move $((2*m))" ;;
    "")                 echo "   m=$m timeout" ;;
    *)                  echo "   m=$m N" ;;
  esac
done
echo G8DONE
