# O4 and O5: attempt, results, and why they are not solved

**Date:** 2026-08-01
**Companion to:** `WHAT_REMAINS.md`

## Outcome

**O4 and O5 are not solved here.** They are the open problem: O5 (an explicit cutoff `B_r` bounding
where a P-root can sit in ray `r`) combined with the at-most-one-root theorem would reduce each ray
to a finite search and *resolve opening 16*; O4 (certified ray uniformity) would do the same. A
correct proof of either is a resolution of Conway's $1000 question.

This document records three attacks I ran, all of which failed, with the evidence — so the next
attempt does not repeat them. One positive structural result came out of it (§4).

---

## 1. Attack A — generalising the Absorption Lemma. **Collapses.**

Theorem 1′ requires `F − w ∈ S` for each gap `w` (i.e. `δ ≤ 1`). But the *actual* requirement for
`⟨S,w⟩ = ⟨S⁺,w⟩` is only `F ∈ ⟨S,w⟩`, i.e.

> a gap `w` is **absorbing** if `∃k ≥ 1` with `F − kw ∈ S`.

`δ ≤ 1` only ever uses `k = 1` (plus `k = 2` for `w = F/2`), so "every gap absorbing" is a formally
**weaker hypothesis**, hence a formally **stronger N-criterion**. If it were strictly stronger it
would prune more and might reach the ray roots.

It does not. Over all 19,813 numerical semigroups with `F > 1` of genus ≤ 17:

```
  delta<=1                          : 754
  all gaps absorbing                : 754   (of these, NOT delta<=1: 0)
     -> N 754 , P 0
     children identity holds        : 754 ok, 0 bad
  fraction of N-positions certified : 754 / 16788  = 4.5%
```

**The two conditions are exactly equivalent in this range** — the `k ≥ 2` freedom never buys a
single extra semigroup. And the criterion certifies only 4.5% of N-positions, so it was never going
to carry a ray argument regardless.

*(The equivalence `all-absorbing ⟺ δ ≤ 1` is itself an unproved empirical observation, verified to
genus 17. It is not needed for anything above; it is recorded because it explains why the
generalisation is a dead end.)*

## 2. Attack B — absorption at the ray level. **Fails, and worsens with `n`.**

The direct route to O5: if `children(Q_{n−8}) ⊆ children(Q_n)`, the Absorption Lemma would make
`Q_n` N for every `n` above some point, clearing the ray outright and giving a cutoff for free.

It fails. Counting even children of `Q_{n−8}` that are *not* children of `Q_n = ⟨16,2n⟩`:

```
  n=11:  6/39 not children of Q_n
  n=13: 13/39 not children of Q_n
  n=19: 25/39 not children of Q_n
```

The obstruction is structural, not incidental: a child of `Q_{n−8}` is `2⟨8,n−8,m⟩`, and reaching it
from `Q_n` needs `⟨8,n,n−8,m⟩` — **two** moves (`2(n−8)` then `2m`), not one. The inclusion fails
worse as `n` grows, so no large-`n` version rescues it.

This is the same wall the handoff records in §6.1 ("no fixed finite concrete quotient... the number
of exact transition-bisimulation classes grows at least linearly with `q`").

## 3. Attack C — get root data first, then conjecture a bound. **Blocked by compute.**

O5 needs data: with no decided root beyond `n = 9` there is nothing to fit a bound to. I rebuilt the
gcd-2 solver around Theorems 1′/2/3 (which remove the infinite branches) and ran it at
`Q_11 = ⟨16,22⟩` — the first genuinely undecided root, and one the bundle's own solver crashed on.

**It did not finish**: 17+ minutes, 3.6 GB resident and still growing, no decision. Killed.

The cost is intrinsic, not an implementation defect. Deciding `Q_11` means walking the 857-element
oversemigroup lattice of `⟨8,11⟩`; for each *non-symmetric* core in it, Theorem 2 does not apply, so
its odd children `D(B,u)` have `δ = 2δ(B) ≥ 2` — exactly where Theorem 1′ stops pruning — and each
is a numerical semigroup of genus around 110 requiring a full search.

So the theorems proved in this project remove the infinite branches but **do not make the finite
residue small**. That gap is precisely O3/O4.

## 4. What did come out: the gcd-8 layer is a chain

For odd `g < q`, `⟨2,q,g⟩ = ⟨2,g⟩` (since `q − g` is even, so `q ∈ ⟨2,g⟩`). **Verified 190/190.**

So under gcd-preserving moves the entire gcd-8 layer is a **linear chain**

```
(8, N) ← (8,⟨2,3⟩) ← (8,⟨2,5⟩) ← (8,⟨2,7⟩) ← ⋯
```

with `(8,⟨2,q⟩) = ⟨16,8q⟩`, and every gcd-preserving move from `⟨16,8q⟩` reaches *some smaller
member of the same chain*. This is much simpler than the gcd-2 and gcd-4 layers, whose
gcd-preserving moves range over a full oversemigroup lattice.

It also settles the `q = 1` root of that ray: `⟨16,8⟩ = ⟨8⟩` (verified), the move `14` is legal from
`⟨8⟩`, and its child is the certified P-position `⟨8,14⟩`. Hence

> **`⟨16,8⟩` is N via 14.**

(This inherits the one dependency of `⟨8,14⟩ P` — obligation O3.)

The chain structure does **not** by itself decide the ray: the gcd-8 layer's *gcd-dropping* children
are gcd-4 and gcd-2 positions, so it still sits on top of O1.

---

## 5. Why O4/O5 resist — the precise obstruction

All three attacks fail at the same place, and it is the place the handoff already identified in §11:

- The N-criteria available (`δ ≤ 1`, and its absorbing generalisation) are **strategy-stealing**
  arguments. They work exactly when a position's children collapse onto its Frobenius-completion's
  children. That collapse happens iff `δ ≤ 1`, and genuine P-positions begin at `δ = 2` — 180 of
  them at genus ≤ 20. So no refinement of this technique reaches the P/N boundary.
- Ray descent gives *at most one* P-root per ray but is a **one-directional** implication: `Q_m` P
  ⟹ `Q_n` N for `n > m`. It yields no bound on where an unfound root could be, because nothing
  rules out the root sitting arbitrarily far out.
- Duplication multiplies the defect (`δ(D) = 2δ(B)`, verified 734/734), so descending into the ray
  drives states *away* from the tractable `δ ≤ 1` region rather than towards it.

A cutoff therefore needs a genuinely new ingredient: a monotone quantity that is bounded along a
ray and forces N once it exceeds a threshold. Nothing in the current calculus supplies one. This is
the handoff's §11 "outcome-complete bounded control signature with ranked affine counters", and it
remains missing.

---

## 6. Honest status

| obligation | status |
|---|---|
| O1 gcd-2 ray classification | open; no root decided beyond `n=9` (compute wall, §3) |
| O2 gcd-4 / gcd-8 rays | open; gcd-8 layer structure clarified (§4), `⟨16,8⟩` N via 14 |
| O3 the `⟨8,10,12,14⟩` period-8 law | open |
| **O4 ray uniformity** | **open — not solved here** |
| **O5 P-root cutoff** | **open — not solved here** |

Three attacks closed off (§1, §2, §3). One ray root added (`⟨16,8⟩` N). Opening 16 remains open.
