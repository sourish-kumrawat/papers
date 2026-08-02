# Theorem 4: the exceptional-gap criterion

**Date:** 2026-08-01
**Status:** proved; verified 1,405/1,405 (P side) and 0 false certificates (N side) at genus ≤ 16.

This is the strongest N-criterion obtained in this project. It strictly generalises Theorem 1′
(`δ ≤ 1 ⟹ N`) and certifies **64.8%** of all N-positions, against 4.5% for the criteria available
before. It does **not** solve O4/O5.

---

## 1. Setup

`S` a numerical semigroup, `F = F(S) > 1`, `S⁺ = S ∪ {F} = ⟨S,F⟩`, so `gaps(S⁺) = gaps(S) ∖ {F}`.

> A gap `w` (`2 ≤ w < F`) is **exceptional** if `F ∉ ⟨S,w⟩`, i.e. there is no `k ≥ 1` with
> `F − kw ∈ S`.

For a **non**-exceptional `w` we have `F ∈ ⟨S,w⟩`, hence `S⁺ ⊆ ⟨S,w⟩` and

```
    ⟨S,w⟩ = ⟨S⁺,w⟩            (w non-exceptional)
```

Split the children accordingly:

```
    A = { ⟨S⁺,w⟩ : w non-exceptional }      ( = { ⟨S,w⟩ : w non-exceptional } )
    B = { ⟨S,w⟩  : w exceptional }
    C = { ⟨S⁺,w⟩ : w exceptional }

    children(S)  = {S⁺} ∪ A ∪ B
    children(S⁺) =        A ∪ C
```

`δ(S) ≤ 1` is exactly the case where **no** gap is exceptional, so `B = C = ∅` and
`children(S) = {S⁺} ∪ children(S⁺)` — that is Theorem 1′.

---

## 2. The theorem

> ### Theorem 4.
> If `S` is a **P**-position with `F(S) > 1`, then some **exceptional** gap `w` has `⟨S⁺,w⟩` a
> **P**-position.
>
> **Contrapositive.** If `⟨S⁺,w⟩` is N for every exceptional gap `w`, then `S` is **N**.

*Proof.* Suppose `S` is P. Then every child of `S` is N. In particular `S⁺` is N, so `S⁺` has some
P-child; that child lies in `A ∪ C`. But `A ⊆ children(S)`, so every element of `A` is N. Hence the
P-child lies in `C`, i.e. `⟨S⁺,w⟩` is P for some exceptional `w`. ∎

Note the proof consumes the hypothesis "`S` is P" twice: once to get `S⁺` N, and once to kill `A`.
That second use is what makes it stronger than the plain Absorption Lemma.

**Corollary 4.1.** A P-position with `F > 1` has at least one exceptional gap — i.e. `δ ≥ 2`.
(Recovers Theorem 1′.)

### Verification

```
=== Theorem 4 (numerical semigroups, genus<=16) ===
P-positions: 1405   N-positions: 10363
  every P has an exceptional gap w with <S+,w> P : 1405/1405   (failures 0)
  contrapositive certifies N                     : 6719 / 10363  = 64.8%   (false certs 0)
```

---

## 3. Decision procedure

Theorem 4 turns into an exact solver (`fast.cpp`):

```
solve(S):
    F <= 1                     -> P
    C  = { ⟨S⁺,w⟩ : w exceptional }
    every element of C is N    -> N                     [Theorem 4]
    otherwise S⁺ is N, and
      S is P  <=>  every element of A and of B is N
```

Differential-tested against the pruning-free solver over all 11,769 numerical semigroups of
genus ≤ 16: **0 disagreements**.

**Honest note on speed.** The node count is *not* reduced (831,615 vs 766,463 states on
`⟨16,27,28,46⟩`): the theorem terminates early but still has to solve the set `C`, which drags in
most of the same lattice. It is a structural result, not a large speedup. `⟨16,27,28,46⟩` P is
re-decided in 9.8 s.

---

## 4. The gcd-2 form

The same double-use argument runs at external gcd 2, and here it does **not** need a symmetric core.

Let `X = (2,B)`, `Y = (2,B⁺)` — a child of `X` via the even move `2·F(B)`. Even children of `Y` are
`(2,⟨B⁺,g⟩)`, which equal `(2,⟨B,g⟩) ∈ children(X)` for non-exceptional `g`. Odd children of `Y` are
`⟨2B⁺,u⟩`, and these are never children of `X` (they contain `2F(B)`, which needs a second move).

> ### Theorem 4′.
> If `X = (2,B)` is a P-position then **either** some exceptional gap `g` of `B` has `(2,⟨B⁺,g⟩)` P,
> **or** some odd `u` has `⟨2B⁺,u⟩` a P numerical semigroup.
>
> If `B` is symmetric the first alternative is vacuous, so: **`(2,B)` P ⟹ some odd `u` makes
> `⟨2B⁺,u⟩` a P-position.**

**Consistency check on `⟨8,14⟩ = 2⟨4,7⟩`** (`B⁺ = ⟨4,7,17⟩`). The theorem demands a witness; scanning
`⟨8,14,34,u⟩` finds `u = 27` and `u = 33` both P. Consistent with `⟨8,14⟩` P.

**Applied to ray roots.** For `Q_n = (2,⟨8,n⟩)` the core is symmetric, `B⁺ = ⟨8,n,7n−8⟩` (the
handoff's width-seven kernel `B_q`, with `δ = 6`). So `Q_n` is P only if some odd `u` makes
`⟨16,2n,14n−16,u⟩` a P-position. Scanning `u ≤ 25`:

```
 n=11 : <16,22,138,u>   all N   (u = 3..25)
 n=13 : <16,26,166,u>   all N   (u = 3..25)
 n=15 : <16,30,194,u>   all N   (u = 3..25)
```

**This is evidence that `Q_11`, `Q_13`, `Q_15` are all N, not a proof.** Theorem 4′ needs the
condition for *every* odd `u`, and the tail is not discharged: for odd `u ∈ B⁺` the child has
`δ = 2·δ(B⁺) = 12` (matching the handoff's `δ(H_{q,z}) = 12`), far outside the `δ ≤ 1` region where
Theorem 1′ bites. Extending the scan raises confidence but proves nothing.

---

## 4b. A bounded, uniform ray obligation

Combining Theorem 4′ with the handoff's §5.2 gives the sharpest form of the ray obligation I have.

For `Q_q = (2,⟨8,q⟩)`, `B_q := ⟨8,q⟩⁺ = ⟨8,q,7q−8⟩`. I verified §5.2 directly for all odd
`q ∈ [9,43]`:

```
F(B_q) = 7q − 16 ,   δ(B_q) = 6 ,
double-gap pairs = { (q−8, 6q−8), (2q−8, 5q−8), (3q−8, 4q−8) }      (exactly 3, all affine in q)
```

So `B_q` carries **exactly three** exceptional pairs, and their endpoints are affine in `q` —
a bounded amount of structure, *uniform in `q`*. By Theorem 4′,

> `Q_q` is P ⟹ some odd `u` makes `H_{q,u} := ⟨2B_q, u⟩` a P-position,

and `δ(H_{q,u}) = 2·δ(B_q) = 12` (matching the handoff's `δ(H_{q,z}) = 12`), so `H_{q,u}` has
**exactly six** exceptional pairs, again affine in `(q,u)`. Theorem 4 then says `H_{q,u}` is N as
soon as all six of its exceptional-gap completions `⟨H⁺,w⟩` are N.

**The branching factor is therefore bounded by 6, independently of `q` and `u`.** This is the shape
of the "bounded control signature" the handoff's §11 asks for, and it is genuinely uniform in the
ray parameter — which nothing else in this project achieved.

**It is not a solution.** The six sub-obligations are themselves large numerical semigroups whose
outcomes are not determined by the signature: resolving them recursively regenerates the same
problem one level down, with no proof that the recursion closes. A bounded branching factor is
necessary for a ray theorem, not sufficient. What is still missing is an *outcome-complete*
invariant — one whose value at `H_{q,u}` determines the six answers without recursion.

---

## 5. Why this still does not give O4/O5

Theorem 4 sharpens the *necessary condition* for a P-position from "`δ ≥ 2`" to "has an exceptional
gap `w` with `⟨S⁺,w⟩` P". That is a strictly better local test, and it is what a solver should use.
But it is still a **local** condition: it relates `S` to positions of smaller genus, so it gives a
better recursion, not a bound on where a P-root can sit along a ray.

For O5 one needs a quantity that is monotone *along a ray* (`q ↦ q+8`) and forces N past a
threshold. Theorem 4 says nothing about ray direction — the exceptional-gap structure of
`⟨8,q⟩⁺ = ⟨8,q,7q−8⟩` is essentially the same for every `q` (`δ = 6` throughout), which is precisely
why the rays look self-similar and why no cutoff drops out.

**Opening 16 remains open.**
