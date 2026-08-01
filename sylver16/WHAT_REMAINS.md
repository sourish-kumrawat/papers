# What remains until opening 16 is solved

**Date:** 2026-08-01
**Companion to:** `AUDIT_AND_RESULTS_2026-08-01.md`

This document answers two questions precisely:

1. **What is left to prove?** — reduced to a finite, explicitly stated list.
2. **Can everything remaining be derived here?** — **No.** The residue is a genuinely open
   problem (Conway's $1000 question). But the reduction below is much sharper than the
   handoff's, and two further theorems are proved along the way.

---

## 1. Summary of the reduction

Writing every legal reply as `x = 2^e·q` with `q` odd and `e ∈ {0,1,2,3}`:

| family | position | status |
|---|---|---|
| `e = 0` | `⟨16,q⟩`, `q` odd | **SOLVED — all N.** Theorem 1; unconditional |
| `e = 1` | `⟨16,2q⟩ = 2⟨8,q⟩` | open — 4 rays (`q mod 8`) |
| `e = 2` | `⟨16,4q⟩ = 4⟨4,q⟩` | open — 2 rays (`q mod 4`) |
| `e = 3` | `⟨16,8q⟩ = 8⟨2,q⟩` | open — 1 ray |

and therefore

> **`{16}` is P ⟺ every one of `⟨16,2q⟩`, `⟨16,4q⟩`, `⟨16,8q⟩` (`q` odd) is N.**
> Equivalently, by ray descent (§4), **⟺ none of the ≤ 7 possible ray P-roots exists.**

The odd branch is now closed unconditionally, so the whole problem is the seven even rays.

---

## 2. Two new theorems

### 2.1 The Absorption Lemma

> **Lemma A.** In an impartial game, if a position `X` has a move to a child `Y` with
> `children(Y) ⊆ children(X)`, then `X` is an N-position.

*Proof.* Suppose `X` is P. Then every child of `X` is N; in particular `Y` is N. But
`children(Y) ⊆ children(X)`, so every child of `Y` is N, which makes `Y` a P-position.
Contradiction. ∎

This is the engine behind everything below. It is a strategy-stealing argument: it proves `X`
is N **without naming a uniform winning move**, which is exactly why the winning moves from
symmetric positions look erratic in the data and why a pairing proof was never found.

### 2.2 Theorem 1′ — the defect rule (proves the former Conjecture R2⁺)

For a numerical semigroup `S` put `δ(S) = 2·genus(S) − F(S) − 1` and
`D(S) = #{ {u,F−u} : 0 < u < F, u ≠ F−u, both gaps }`.

> **Theorem 1′.** If `F(S) > 1` and `δ(S) ≤ 1`, then `S` is an N-position.
> (`δ = 0` ⟺ symmetric, `δ = 1` ⟺ pseudo-symmetric.)

*Proof.*

**(a) `δ(S) = 2·D(S) + [F even]`.** Pair `{n, F−n}` for `0 ≤ n ≤ F`. Since `S` is a semigroup and
`F ∉ S`, `n` and `F−n` cannot both lie in `S` — so each pair contributes at least one gap, and
exactly two precisely when counted by `D`.
- `F` odd: `(F+1)/2` pairs, no fixed point, so `g = (F+1)/2 + D` and `δ = 2D`.
- `F` even: `F/2` proper pairs plus the fixed point `F/2`, which is always a gap (else
  `F = F/2 + F/2 ∈ S`), so `g = F/2 + D + 1` and `δ = 2D + 1`. ∎

Hence `δ ≤ 1 ⟺ D = 0`. *Verified 33,280/33,280 at genus ≤ 18.*

**(b) If `D(S) = 0` then `⟨S,u⟩ = ⟨S⁺,u⟩` for every gap `u`,** where `S⁺ = S ∪ {F}`.
If `u = F` this is trivial. If `u ≠ F/2` then `D = 0` forces `F−u ∈ S`, so
`F = u + (F−u) ∈ ⟨S,u⟩`. If `u = F/2` then `2u = F ∈ ⟨S,u⟩` automatically. Either way
`F ∈ ⟨S,u⟩`, so `S⁺ ⊆ ⟨S,u⟩` and the two generated semigroups coincide. ∎

*(This is the step that extends the symmetric case to the pseudo-symmetric one: the lone
exceptional gap `F/2` is harmless because it doubles to `F`.)*

**(c)** Since `gaps(S⁺) = gaps(S) ∖ {F}`, (b) gives
`children(S) = {S⁺} ∪ children(S⁺)`, so `children(S⁺) ⊆ children(S)` and Lemma A applies. ∎

*Verified 1,034/1,034 at genus ≤ 18.*

**Constructive form.** If `S⁺` is P, play `F`. Otherwise `S⁺` is N, so some `⟨S⁺,u⟩` is P, and
`⟨S,u⟩ = ⟨S⁺,u⟩` — play that same `u`.

### 2.3 Theorem 3 — gluing

> **Theorem 3.** Let `S` be a numerical semigroup, `d ≥ 2`, and `u ≥ 2` with `gcd(u,d) = 1` and
> `u ∈ S`. Put `T = ⟨dS, u⟩`. Then
> `F(T) = d·F(S) + (d−1)u`, and **`T` is symmetric ⟺ `S` is symmetric.**

*Proof.* Since `gcd(u,d) = 1`, define `k(n) ∈ {0,…,d−1}` by `k(n) ≡ n·u^{-1} (mod d)`, and
`a(n) = (n − k(n)u)/d ∈ ℤ`.

*Claim:* `n ∈ T ⟺ a(n) ≥ 0 and a(n) ∈ S`. (⇐) is clear. (⇒) if `n = da + ku` with `a ∈ S`,
`k ≥ 0`, then `k ≡ k(n) (mod d)`, so `k = k(n) + dm` with `m ≥ 0`, giving
`n = d(a + mu) + k(n)u`; and `a + mu ∈ S` because `u ∈ S`. Hence `a(n) = a + mu ∈ S`.

*Frobenius number:* `n ∉ T ⟺ a(n) ∉ S`. For fixed `k`, the largest such `n` is
`d·F(S) + ku`, maximised at `k = d−1`. So `F(T) = d·F(S) + (d−1)u`.

*Symmetry:* for any `n`, with `k = k(n)`, `a = a(n)`,
```
F(T) − n = d·F(S) + (d−1)u − da − ku = d(F(S) − a) + (d−1−k)u ,
```
and `0 ≤ d−1−k ≤ d−1`, so `k(F(T)−n) = d−1−k` and `a(F(T)−n) = F(S) − a`. Therefore
`n ∈ T ⟺ a ∈ S` and `F(T)−n ∈ T ⟺ F(S)−a ∈ S`. As `n` ranges over `ℤ`, so does `a`; hence
`[n ∈ T ⟺ F(T)−n ∉ T]` for all `n` ⟺ `[a ∈ S ⟺ F(S)−a ∉ S]` for all `a`. ∎

*Verified 12,867/12,867.* (This is the classical gluing/Delorme symmetry result; the new part is
the game-theoretic corollary below.)

> **Corollary 3.1 (deep-drop closure).** Let `(2^k, S)` be a position with `S` symmetric.
> For every odd `u ∈ S` and every `j < k`, the gcd-dropping move `2^j·u` lands on
> `(2^j, ⟨2^{k−j}S, u⟩)` whose **core is again symmetric**.
> In particular for `j = 0` the child is a symmetric numerical semigroup with `F > 1`, hence
> **N by Theorem 1′**.
>
> Only the finitely many odd `u ∉ S` (the odd gaps of `S`) need individual checking.

So the class *"external gcd `2^j`, symmetric core"* is closed under gcd-dropping moves, and the
entire drop-to-gcd-1 branch is discharged at every layer, at once.

---

## 3. The layered induction, and where it stops

Positions are `(d, S)`, `d = 2^k`, `k = 0..4`. Moves from `(2^k, S)`:

- **gcd-preserving:** `2^k·g` for `g` a gap of `S` → `(2^k, ⟨S,g⟩)`. *Finite*, same layer, strictly
  larger core — so this recursion always terminates.
- **gcd-dropping:** `2^j·u`, `u` odd, `j < k` → `(2^j, ⟨2^{k−j}S, u⟩)`. *Infinite in `u`.*

| layer | status |
|---|---|
| `k = 0` (numerical semigroups) | **DONE.** Finite game, exactly decidable; Theorem 1′ closes `δ ≤ 1` outright |
| `k = 1` (gcd 2) | children are gcd-2 (finite) + gcd-1 (done) ⟹ **self-contained given `k=0`** |
| `k = 2` (gcd 4) | children are gcd-4 (finite) + gcd-2 + gcd-1 ⟹ needs `k=1` |
| `k = 3` (gcd 8) | needs `k = 1,2` |
| `k = 4` (`{16}` itself) | needs `k = 1,2,3` |

**Consequence: the whole problem bottoms out at the gcd-2 layer.** Classify `out(2T)` for all
numerical semigroups `T` and the remaining layers follow by induction.

Within the gcd-2 layer, Theorem 2 (symmetric-core reduction, see the companion document) makes
`out(2T)` a **finite** computation whenever `T` is symmetric. The residual difficulty is exactly:

- (i) non-symmetric `T`, where the infinite odd family is not discharged; and
- (ii) **uniformity in the ray parameter** — each individual `T` being finitely decidable does not
  classify an infinite family `T(q)`.

---

## 4. Ray descent: at most seven P-roots

For the gcd-2 family, if `m < n` and `m ≡ n (mod 8)` then `2n = 2m + 16t`, so
`⟨16,2n,2m⟩ = ⟨16,2m⟩`; playing `2m` from `Q_n` reaches `Q_m`. Hence if `Q_m` is P then `Q_n` is N:
**each residue class mod 8 contains at most one P-root.** The same argument with `4p` (`p ≡ q mod 4`)
and `8p` (`p, q` both odd) gives:

| family | descent modulus | rays | max P-roots |
|---|---|---|---|
| `⟨16,2q⟩` | `q mod 8` | 4 | 4 |
| `⟨16,4q⟩` | `q mod 4` | 2 | 2 |
| `⟨16,8q⟩` | `q` odd | 1 | 1 |

> **`{16}` is P ⟺ all seven rays are P-root-free.**
> A single P-root anywhere makes `{16}` N, with that root as the winning reply.

---

## 5. The remaining obligations, explicitly

Everything below is what is left. Nothing else is needed.

### 5.0 Current state of the seven rays

```
 gcd-2 layer,  Q_n = <16,2n> = 2<8,n>,  n odd        (verified independently here)
   n = 1 mod 8 :  n=9  N  (odd move 5)        smallest undecided root: n=17
   n = 3 mod 8 :  n=3  N  (odd move 7)        smallest undecided root: n=11
   n = 5 mod 8 :  n=5  N  (odd move 9)        smallest undecided root: n=13
   n = 7 mod 8 :  n=7  N  (even move 8; rests on <8,14> P, i.e. on O3)
                                              smallest undecided root: n=15
 gcd-4 layer,  <16,4q> = 4<4,q>,  q odd       : no root decided by anyone
 gcd-8 layer,  <16,8q> = 8<2,q>,  q odd       : no root decided by anyone
```

Every entry above is an **N**, so no P-root has been found in any ray — but no ray has been
*cleared* either, and the search has reached only the first root of each. `<16,14,8> = <8,14>` and
`<16,6,7> = <6,7,16>` were checked directly.

> **See `O4_O5_ATTEMPT.md`** for three attacks on O4/O5 that were tried and failed (generalised
> absorption, ray-level absorption, and the computational route), plus the one positive result:
> the gcd-8 layer is a linear chain, and `⟨16,8⟩` is N via 14.

**O1 — the gcd-2 ray classification.** For each `r ∈ {1,3,5,7}`, decide `out(2⟨8,q⟩)` for all
`q ≡ r (mod 8)`: either exhibit the unique P-root or prove the ray has none.
*Status:* the bundle decided **no** root here (all its root runs crashed); `q = 3,5,9` are N
(reproduced independently), `q = 7` reduces to `⟨8,14⟩` (see O3).

**O2 — the gcd-4 and gcd-8 ray classifications.** Same for `4⟨4,q⟩` (`q mod 4`) and `8⟨2,q⟩`.
*Status:* untouched by the bundle. These layers depend on O1.

**O3 — the non-symmetric gcd-2 frontier.** Theorem 2 covers symmetric cores. What remains is the
set of gcd-2 positions `2T` with `T` non-symmetric that block a P-proof. For `⟨8,14⟩` this frontier
is a **single** position, `2⟨4,5,6,7⟩ = ⟨8,10,12,14⟩`, whose odd children obey the period-8 law
```
u ≡ 1 (mod 8) → winning move u+2 ;   otherwise → u − ((u−1) mod 8)
```
(N for all odd `u ≤ 61`). Proving that law settles `⟨8,14⟩ P`, hence `⟨16,14⟩ N`.

**O4 — ray uniformity.** A theorem making the outcome of `2⟨8,q⟩` eventually periodic (or otherwise
uniform) in `q`, with an *exact* certificate — the handoff's Route C, with its own warning that
matching root outcomes is not enough and complete affine-quotient equality is required.

**O5 — or a cutoff.** Alternatively (handoff's Route D): an explicit bound `B_r` such that any
P-root in ray `r` satisfies `root ≤ B_r`, reducing each ray to a finite search. Combined with §4's
at-most-one-root theorem this would finish the problem.

**O1–O5 are the whole of what remains.** O4/O5 are the hard mathematics; O3 is a concrete, small,
well-posed target; O1/O2 are largely *computation* now that Theorems 1′/2/3 remove the infinite
branches.

---

## 6. Why this cannot be finished here

O4 and O5 are the open problem itself. Concretely:

- The odd children of a **non-symmetric** core are not symmetric (`δ(D) = 2δ(B)`, verified
  734/734), so Theorem 1′ cannot discharge them — `δ = 2` is exactly where genuine P-positions
  start appearing (180 of them at genus ≤ 20).
- The period-8 law of O3 is visible in data but proving it needs induction on a family whose
  members are themselves P-positions requiring their own infinite-tail arguments — the same
  circularity the handoff records in §6.1 ("no fixed finite concrete quotient") and §6.3
  (deep moves collapsing states by `Θ(q)`).
- Nothing in the structure so far bounds where a P-root could sit, so §4's at-most-one-root
  theorem cannot yet be turned into a finite search.

Claiming a resolution would require exactly the missing ingredient the handoff's §11 names: an
outcome-complete bounded control signature with ranked affine counters. This work does not supply it.

---

## 7. What did move

- **Odd branch closed unconditionally**: `⟨16,q⟩` N for every odd `q > 1` (Theorem 1′), previously
  assumed.
- **R2, the bundle's hidden assumption, is now a theorem**, and strengthened from `δ = 0` to
  `δ ≤ 1`.
- **Drop-to-gcd-1 branches discharged at every layer** (Corollary 3.1) — previously handled, where
  handled at all, by numerical lasso search.
- **The problem is now known to bottom out at the gcd-2 layer**, which is self-contained.
- **Seven rays, at most seven P-roots**, and `{16}` is P iff there are none.
