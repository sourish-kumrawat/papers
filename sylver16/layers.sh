#!/bin/bash
# The gcd-4 and gcd-8 rays, which the bundle never touched.
#
#  gcd-4 family: <16,4q> = 4<4,q>, q odd.   Its gcd-2 children are
#                <16,4q,2m> = 2<8,2q,m>  for ODD m  (gcd drops to 2 exactly when m is odd).
#  gcd-8 family: <16,8q> = 8<2,q>, q odd.   Its gcd-2 children are
#                <16,8q,2m> = 2<8,4q,m>  for ODD m.
#
# If any such child is P, the root is N via the move 2m.
cd "$(dirname "$0")"
echo "############ gcd-4 ray:  <16,4q> = 4<4,q> ############"
for q in 1 3 5 7 9 11 13; do
  echo "=== <16,$((4*q))>  (q=$q, q mod 4 = $((q%4))) ==="
  hit=0
  for m in 1 3 5 7 9 11 13 15 17 19 21; do
    r=$(timeout 90 ./vdc --solve 8 $((2*q)) $m --kcap 30000 --progress 100000 2>/dev/null)
    case "$r" in
      *"all children N"*) echo "   *** $r  ==> <16,$((4*q))> is N via move $((2*m))"; hit=1; break;;
    esac
  done
  [ $hit = 0 ] && echo "   no P-cell found for m<=21"
done
echo "############ gcd-8 ray:  <16,8q> = 8<2,q> ############"
for q in 1 3 5 7 9 11 13; do
  echo "=== <16,$((8*q))>  (q=$q) ==="
  hit=0
  for m in 1 3 5 7 9 11 13 15 17 19 21; do
    r=$(timeout 90 ./vdc --solve 8 $((4*q)) $m --kcap 30000 --progress 100000 2>/dev/null)
    case "$r" in
      *"all children N"*) echo "   *** $r  ==> <16,$((8*q))> is N via move $((2*m))"; hit=1; break;;
    esac
  done
  [ $hit = 0 ] && echo "   no P-cell found for m<=21"
done
echo LAYERSDONE
