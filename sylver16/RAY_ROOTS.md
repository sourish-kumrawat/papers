# Ray roots decided

**Date:** 2026-08-01
**Companion to:** `WHAT_REMAINS.md`, `THEOREM4.md`

The bundle decided **no** odd-ray root beyond `n = 9` — every one of its root runs crashed (see
`AUDIT_AND_RESULTS_2026-08-01.md` §7). This file records the roots decided here, the method, and
the current frontier.

**Opening 16 is still open.** Every root below is an **N**, so no ray has been cleared and no
P-root has been found; the rays remain infinite and unclassified.

---

## 1. Method

The witnesses come from the even branch, not the odd one.

> **Proposition R.** Let `q` be odd and `m ∉ ⟨8,q⟩`. Then `2m` is a legal move from
> `Q_q = ⟨16,2q⟩`, and
> `⟨16, 2q, 2m⟩ = 2⟨8,q,m⟩`.
> Hence **if `2⟨8,q,m⟩` is P then `Q_q` is N via the move `2m`.**

So the "general P-cell table" `P_{m,q} = 2⟨8,m,q⟩` of the handoff's §8.3 is not just a curiosity —
**every P entry in it is a ray-root witness.** That is the tool that decides the roots below.

Two things made this usable:

1. **Theorem 1 makes the bundle's solver trustworthy.** Its `is_symmetric ⟹ N` pruning rule was
   an unexamined conjecture; now that it is proved (together with the other two rules), the solver's
   output can be used as an oracle where my own gcd-2 solver is incomplete. The audit paid for
   itself here.
2. **Theorem 2 explains the solver's shape.** For a symmetric core only the *odd gaps* need checking,
   and the bundle's own `tmp_6_11.out` lists exactly `c = 3,5,7,9,13,15,21` — precisely the odd gaps
   of `⟨6,8,11⟩`. Independent confirmation of the reduction.

---

## 2. Roots decided

All P-cells below were re-derived here, and every legality claim machine-checked.

| root | P-cell witness | nodes | move | status |
|---|---|---|---|---|
| `Q_9  = ⟨16,18⟩` | `⟨12,16,18⟩ = 2⟨6,8,9⟩` P | 2,470 | **12** | N *(bundle had odd 5; 12 is a second witness)* |
| `Q_11 = ⟨16,22⟩` | `⟨12,16,22⟩ = 2⟨6,8,11⟩` P | 4,422 | **12** | **N — NEW** |
| `Q_17 = ⟨16,34⟩` | `⟨16,20,34⟩ = 2⟨8,10,17⟩` P | 514,391 | **20** | **N — NEW** |
| `Q_7  = ⟨16,14⟩` | `⟨8,14⟩ = 2⟨4,7⟩` P | 718 | **8** | N — now **unconditional** (O3 closed) |

Legality and identity checks (all `True`):

```
Q_9 =<16,18>: move 12 legal, <16,18,12> == <12,16,18>
Q_11=<16,22>: move 12 legal, <16,22,12> == <12,16,22>
Q_17=<16,34>: move 20 legal, <16,34,20> == <16,20,34>
```

`Q_11` is the significant one: the bundle's `q11_root` run printed `cores=857 F=69 genus=35` and
then died with no result. It is now decided.

### `Q_11`'s odd branch is also completely closed

Independently of the witness above, **every** odd child of `Q_11` is an N-position:

- the core `⟨8,11⟩` is symmetric, so by **Theorem 2** every odd `u ∈ ⟨8,11⟩` gives a symmetric
  child, hence N by **Theorem 1′** — infinitely many moves, discharged by proof;
- that leaves exactly the 19 odd **gaps** of `⟨8,11⟩`, each checked explicitly (largest:
  `⟨16,22,69⟩`, `F=191`, genus 102, 11.4M states):

```
u = 3,5,7,9,13,15,17,21,23,25,29,31,37,39,45,47,53,61,69   all N   (complete)
```

This is the first complete odd-branch classification of any ray root, and it is exactly the
finiteness that Theorem 2 buys: an infinite family reduced to 19 checks.

---

## 2b. The gcd-4 and gcd-8 layers

The bundle never touched these two layers at all. Proposition R applies there too: from
`⟨16,4q⟩` the gcd-2 children are `⟨16,4q,2m⟩ = 2⟨8,2q,m⟩` with `m` **odd** (the gcd drops to 2
exactly then), and from `⟨16,8q⟩` they are `2⟨8,4q,m⟩`.

| root | layer | P-cell witness | move | status |
|---|---|---|---|---|
| `⟨16,4⟩ = 4⟨4,1⟩` | gcd 4 | `⟨4,6⟩` P | **6** | N |
| `⟨16,12⟩ = 4⟨4,3⟩` | gcd 4 | `⟨12,14,16⟩ = 2⟨6,7,8⟩` P, 966 nodes | **14** | **N — NEW** |
| `⟨16,20⟩ = 4⟨4,5⟩` | gcd 4 | `⟨16,20,34⟩ = 2⟨8,10,17⟩` P, 514,391 nodes | **34** | **N — NEW** |
| `⟨16,8⟩ = 8⟨2,1⟩` | gcd 8 | `⟨8,14⟩ = 2⟨4,7⟩` P, 718 nodes | **14** | N (now unconditional) |
| `⟨16,24⟩ = 8⟨2,3⟩` | gcd 8 | `⟨16,24,10⟩ = 2⟨8,12,5⟩` P, 1,392 nodes | **10** | **N — NEW** |

Legality and identity machine-checked for all five; external gcds confirmed (`4,4,4,8,8`).

Note `⟨16,20,34⟩` does double duty: it witnesses both `Q_17 = ⟨16,34⟩` (gcd-2 layer) and
`⟨16,20⟩` (gcd-4 layer).

---

## 3. Current frontier

Ray descent (§4 of `WHAT_REMAINS.md`) allows at most one P-root per residue class, so a ray is only
cleared by classifying *all* of it. Known values, by ray:

```
 q = 1 mod 8 :  q=9  N,  q=17 N        smallest undecided root: q=25
 q = 3 mod 8 :  q=3  N,  q=11 N        smallest undecided root: q=19
 q = 5 mod 8 :  q=5  N                 smallest undecided root: q=13
 q = 7 mod 8 :  q=7  N (unconditional) smallest undecided root: q=15
 gcd-4, q = 1 mod 4 :  q=1 N, q=5 N     smallest undecided root: q=9   (<16,36>)
 gcd-4, q = 3 mod 4 :  q=3 N             smallest undecided root: q=7   (<16,28>)
 gcd-8 single ray   :  q=1 N, q=3 N      smallest undecided root: q=5   (<16,40>)
```

Every root decided so far is **N**. That is consistent both with `{16}` being P (all seven rays
P-root-free) and with a P-root sitting further out; nothing here distinguishes the two.

---

## 4. What this does and does not settle

**Does:** three ray roots (one of them previously crashed-on), a complete odd-branch classification
for `Q_11`, and a general mechanism — Proposition R — turning the P-cell table into root witnesses.

**Does not:** clear a single ray. Each ray is an infinite family and the roots decided are the first
few members. The obligations O1–O5 stand; O4 (ray uniformity) and O5 (a P-root cutoff) are
untouched, and remain the open problem.


---

## 5. O3 is closed

The obligation O3 — the position the whole `⟨8,14⟩ P` claim rested on — is now decided:

```
<8,10,12,14> = 2<4,5,6,7>   P   all children N   (nodes=7)
<8,12,14>    = 2<4,6,7>     N   via even move 10
<8,14>       = 2<4,7>       P   all children N   (nodes=718)
```

This confirms the prediction made from the period-8 law: `⟨8,12,14⟩`'s only possible witness was
the even move to `⟨8,10,12,14⟩`, and that position is indeed P.

Two consequences.

**(a) `Q_7 = ⟨16,14⟩` is N via 8, unconditionally.** Every claim previously carried with an
"inherits O3" caveat — including `⟨16,8⟩` N via 14 — is now unconditional.

**(b) The refutation of "`2B` P ⟹ `B` symmetric" now has a certificate.** `⟨4,5,6,7⟩` has
`δ = 2` and is *not* symmetric, yet `2⟨4,5,6,7⟩` is P. So Theorem 2's symmetric-core hypothesis is
genuinely necessary, and the class of P-positions at gcd 2 is strictly larger than the
symmetric-core ones. This was stated earlier as a near-certainty from a bounded scan; it is now
settled.

It also independently corroborates the period-8 law: every odd child of `⟨8,10,12,14⟩` had to be N
for the position to be P, and the scan to `u ≤ 61` had found exactly that.
