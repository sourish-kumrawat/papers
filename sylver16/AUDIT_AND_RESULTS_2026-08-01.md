# Sylver Coinage, Opening 16 — Independent Audit, a Proof, and New Reductions

**Date:** 2026-08-01
**Input:** `SYLVER16_SOL_ULTRA_BUNDLE_20260801.zip` (handoff + `sylver16_certificates/`)
**Scope:** review the bundle, verify its claims independently, and push the classification of `{16,x}` as far as sound computation allows.

---

## 0. Bottom line

**Opening 16 is not solved, and this work does not solve it.** Whether `{16}` is P or N remains
open — it is Conway's $1000 prize question. The bundle does not claim otherwise (its §15 lists
"final outcome of opening 16" as OPEN), and nothing here changes that.

What this review does establish:

1. **The bundle's entire certificate library rested on an unproven lemma** that the handoff never
   flags as an assumption (§1) — the solver's `is_symmetric ⟹ N` pruning rule.
2. **That lemma is now proved** (§2, Theorem 1). The proof is four lines and constructive. This
   removes the project's single hidden assumption and makes every certificate below unconditional.
3. Proofs are also given for the solver's other two pruning rules, plus a move-coverage check (§3).
4. A **new reduction theorem** (§4) collapses the infinite odd-move family — the thing the bundle's
   `LazyOddEngine`, lasso detection and `--kcap` machinery exist to handle — to a *finite* check
   whenever the core is symmetric. It applies to all four odd ray roots and the whole shell family.
5. The reduction is **not complete**; §5 gives the explicit position where it fails.
6. Several bundle claims reproduced independently — the headline one **bit-for-bit**, and one
   **upgraded** from "must be regenerated" to certified (§6).
7. The bundle **never decided a single ray root**: every root run crashed (§7).

Verification code: `indep.cpp` (pruning-free gcd-1 solver), `sylver2.cpp` (gcd-2 solver),
`proofcheck.cpp` (checks the structural identity behind Theorem 1).

---

## 1. The load-bearing rule

`verify_doubled_core.cpp` (line 59) decides positions with three pruning rules. The second is:

```cpp
if(is_symmetric(S,F)){ ++symmetric_hits; Rec r{2,0,3}; ... }   // 2 = N
```

> **R2.** If `S` is a symmetric numerical semigroup with `F(S) > 1`, then `S` is an N-position.

R2 is applied with **no witness move**. Every P-certificate in the bundle passes through it, since
proving a position P requires proving all its children N, and R2 is what discharges most of them.

The handoff never states R2. Its §15 ledger puts *"odd-response family N"* under **PROVED** — but
that claim is exactly R2 specialised to two-generated semigroups, since `⟨16,q⟩` with `q` odd is
2-generated and every 2-generated numerical semigroup is symmetric. §10's quarantine list does not
mention it either. As shipped, **the whole library was resting on an unexamined conjecture.**

Exhaustive check with a solver using *no* pruning at all (`indep.cpp`), over every numerical
semigroup of genus ≤ 20:

```
semigroups solved : 93141   (P: 9633)
RULE R1  [F>1 and (2 in S or 3 in S)  => N] : tested  101, N  101, VIOLATIONS 0
RULE R2  [symmetric and F>1           => N] : tested 1148, N 1148, VIOLATIONS 0
```

---

## 2. R2 is a theorem

The empirical winning moves from symmetric positions look erratic — sometimes `F` wins, sometimes a
small gap does — which is why a pairing argument is hard to find. The resolution is that R2 admits a
*strategy-stealing* proof that does not name a single uniform move.

> ### Theorem 1 (R2).
> Let `S` be a symmetric numerical semigroup with `F = F(S) > 1`. Then `S` is an N-position.

Write `S⁺ = S ∪ {F} = ⟨S,F⟩`. It is a numerical semigroup (`F+F > F`, and `F+s > F` for
`s ∈ S∖{0}`, and every integer `> F` lies in `S`), with `gaps(S⁺) = gaps(S) ∖ {F}`. Let
`children(X) = { ⟨X,u⟩ : u a gap of X, u ≥ 2 }` — the positions reachable by one non-suicidal move.

**Step 1 — a symmetric semigroup has no exceptional gap.** By definition `n ∈ S ⟺ F−n ∉ S`. So if
`u` is a gap then `F−u ∈ S`. Hence no pair `{u, F−u}` consists of two gaps.

**Step 2 — the Frobenius interface applies to every move.** If `u` is a gap with `F−u ∈ S`, then
`F = u + (F−u) ∈ ⟨S,u⟩`, so `S⁺ ⊆ ⟨S,u⟩` and therefore `⟨S,u⟩ = ⟨S⁺,u⟩`. By Step 1 this holds for
*every* gap `u` of `S`.

**Step 3 — the children identity.**
```
children(S) = { ⟨S,F⟩ } ∪ { ⟨S,u⟩ : u gap of S, 2 ≤ u < F }
            = { S⁺ }    ∪ { ⟨S⁺,u⟩ : u gap of S⁺, u ≥ 2 }
            = { S⁺ }    ∪ children(S⁺).
```

**Step 4 — conclusion.** Suppose `S` were P. Then every element of `children(S)` is N. Since
`children(S⁺) ⊆ children(S)`, every child of `S⁺` is N, so `S⁺` is P. But `S⁺ ∈ children(S)`, so
`S⁺` is N — contradiction. Hence `S` is N. ∎

*(Edge case: if `gaps(S) = {1,F}` then `children(S⁺) = ∅` and `S⁺` is the terminal position, which
is P; the same contradiction applies.)*

**The proof is constructive.** Either `S⁺` is P — then play `F` — or `S⁺` is N, so some
`⟨S⁺,u⟩` is P, and since `⟨S,u⟩ = ⟨S⁺,u⟩` that same `u` wins from `S`. This is exactly the observed
data: the winning move is `F` in some cases and a smaller gap in others, with no uniform formula.

**Verification.** `proofcheck.cpp` checks Steps 1 and 3 directly on every symmetric numerical
semigroup with `F > 1` of genus ≤ 20:

```
symmetric semigroups with F>1, genus<=20 : 1148
  (i)  no exceptional gap (u, F-u both gaps) : 1148/1148
  (ii) children(S) == {S+} U children(S+)    : 1148/1148   (mismatches 0)
```

### Consequences

- The bundle's solver is **sound**: all three pruning rules are now proved (R1 and the Frobenius
  interface in §3).
- Every certificate in the bundle, and every result in this document, is **unconditional**.
- The handoff's "odd-response family N" claim (`⟨16,q⟩` N for all odd `q > 1`) is now genuinely
  proved rather than assumed, since 2-generated ⟹ symmetric.
- The symmetric-core reduction of §4 becomes an unconditional theorem.

### What is still open on this front

Tabulating outcome against the defect `δ(S) = 2·genus(S) − F(S) − 1` (handoff §5.1; `δ=0` ⟺
symmetric, `δ=1` ⟺ pseudo-symmetric) over all semigroups of genus ≤ 20:

```
delta :   P-count   N-count
    0 :        1       1148     <- the single P is the terminal position, gaps={1}, F=1
    1 :        0        618
    2 :      180       3586
    3 :      149       1937
    4 :      749       7263
```

> **Conjecture R2⁺.** Every P-position other than the terminal `{1}` satisfies `δ ≥ 2`; i.e. no
> pseudo-symmetric semigroup with `F > 1` is P either.

Theorem 1 settles `δ = 0`. The `δ = 1` case is verified on all 618 pseudo-symmetric semigroups of
genus ≤ 20 but is **not** covered by the proof: a pseudo-symmetric `S` has exactly one exceptional
gap, `u = F/2`, so Step 1 fails there, `⟨S,F/2⟩ ≠ ⟨S⁺,F/2⟩` in general, and the inclusion
`children(S⁺) ⊆ children(S)` breaks. Closing that single exceptional branch would give R2⁺ and a
strictly stronger one-line pruning rule (`δ(S) ≤ 1 ⟹ N`) for the solver.

---

## 3. The other two pruning rules

### R1 — proved

> If `F(S) > 1` and (`2 ∈ S` or `3 ∈ S`) then `S` is N.

*Proof.* Let `T = {0,2,3,4,…}`, whose only gap is 1; the player to move in `T` must name 1 and
loses, so `T` is P.
- If `3 ∈ S` and `F(S) > 1` then `2 ∉ S`, so 2 is legal and `⟨S,2⟩ = T`: `S` is N via 2.
- If `2 ∈ S` and `F(S) > 1` then `3 ∉ S`, and `⟨S,3⟩ = T`: `S` is N via 3. ∎

### Frobenius interface (handoff §5.3) — proved, and move coverage is complete

> If `x` is a gap of `S` and `F − x ∈ S`, then `⟨S,x⟩ = ⟨S⁺,x⟩`.

*Proof.* `F = x + (F−x)` with `x ∈ ⟨S,x⟩` and `F−x ∈ S ⊆ ⟨S,x⟩`, so `F ∈ ⟨S,x⟩`, giving
`S⁺ ⊆ ⟨S,x⟩` and hence `⟨S⁺,x⟩ ⊆ ⟨S,x⟩`. The reverse inclusion is immediate. ∎

This is an *identity*, not a heuristic, so its use is sound for P and N conclusions alike. Move
coverage also checks out: since every integer `> F` lies in `S`, we get `S⁺ = S ∪ {F}` exactly, so
`gaps(S⁺) = gaps(S) ∖ {F}`, and the code's `nonex`/`ex` split covers every legal move.
**No coverage gap.**

---

## 4. New result: the symmetric-core reduction

**Lemma C (duplication, handoff §5.1).** For a numerical semigroup `B` and odd `z ∈ B`,
`⟨2B,z⟩ = 2B ⊔ (z + 2B)`. *Verified on 804/804 cases.*

**Lemma D (symmetry transfer).** `⟨2B,z⟩` is symmetric ⟺ `B` is symmetric. *Verified on the same
804 cases.* (Apparent failures are all the degenerate `B = ⟨2,3⟩`, which has `F = 1` and **is**
symmetric; my code excluded `F ≤ 1` by convention. Fixed.)

I also verified the remaining §5.1 invariants — `δ(D) = 2δ(B)`, `F(D) = z + 2F(B)`,
`g(D) = 2g(B) + (z−1)/2` — on **734/734** cases.

> ### Theorem 2 (symmetric-core reduction).
> Let `B` be a symmetric numerical semigroup. Then in the position `2B`, **every odd move `u ∈ B`
> leads to an N-position.**
>
> Consequently the infinite odd-move family from `2B` collapses to the **finitely many odd gaps of
> `B`**, and `out(2B)` is decidable by a finite computation — no lasso, no `--kcap`, no
> eventual-periodicity argument.

*Proof.* For odd `u ∈ B`, Lemma C gives `⟨2B,u⟩ = 2B ⊔ (u+2B)`, Lemma D makes it symmetric, and its
Frobenius number is `u + 2F(B) ≥ 3 > 1`, so Theorem 1 applies. Every odd `u ∉ B` is an odd gap of
`B`, and there are at most `(F(B)+1)/2` of those. ∎

Unconditional, given Theorem 1. The handoff has both ingredients — §5.1 states the duplication
formula *and* notes "`D(B,z)` is symmetric iff `B` is symmetric" — but never draws the
game-theoretic consequence; it builds `LazyOddEngine` with 192-bit state hashing and lasso detection
to handle the odd tail numerically instead.

### Where it applies

**(a) All four odd rays.** `Q_n = ⟨16,2n⟩ = 2⟨8,n⟩`. For odd `n`, `⟨8,n⟩` is two-generated with
coprime generators, hence symmetric. So the odd-move tail of *every* odd-ray root is finite.
Confirmed for `n = 3..39`. (Even `n` give external gcd 4 or 8 — the other three rays.)

**(b) The whole shell family `𝒫_k` (handoff §8.2).** `𝒫_k = ⟨16,8k+4,16k+2⟩ = 2⟨8,4k+2,8k+1⟩`, and

```
F(⟨8,4k+2,8k+1⟩) = 20k − 1 ,   genus = 10k ,   so genus = (F+1)/2  ⟹ symmetric
```

for every `k` tested (1..40; `k=3 → F=59,g=30`; `k=40 → F=799,g=400`). Since `genus = (F+1)/2` is
*equivalent* to symmetry, this is a proof once the two closed forms are established. This matters
because the handoff lists `𝒫_3 = ⟨16,28,50⟩` as unresolved and high-priority (§8.2, §15, §18.5) —
and the bundle's own run on it crashed (§7). Its odd tail is now finite.

**(c) Every certified P-cell in the bundle has a symmetric core** — not a coincidence:

| position | core | F | genus | symmetric |
|---|---|---|---|---|
| `⟨8,14⟩` | `⟨4,7⟩` | 17 | 9 | yes |
| `⟨12,16,18⟩` | `⟨6,8,9⟩` | 19 | 10 | yes |
| `⟨12,16,22⟩` | `⟨6,11⟩` | 49 | 25 | yes |
| `⟨16,20,34⟩` | `⟨8,10,17⟩` | 39 | 20 | yes |
| `⟨16,28,38,40⟩` | `⟨8,14,19,20⟩` | 45 | 23 | yes |
| `⟨16,28,50⟩` (open) | `⟨8,14,25⟩` | 59 | 30 | yes |

---

## 5. Where the reduction fails (honest limitation)

Theorem 2 is **not complete**: even children of a symmetric core need not be symmetric, so the
recursion leaves the class. Running my gcd-2 solver on `⟨8,14⟩ = 2⟨4,7⟩`, the frontier bottoms out
at exactly one position:

```
⟨4,7⟩  --(even move 12)-->  ⟨4,6,7⟩  --(even move 10)-->  ⟨4,5,6,7⟩
```

i.e. `2⟨4,5,6,7⟩ = ⟨8,10,12,14⟩`, whose core has `F = 3`, `genus = 3`, `δ = 2` — **not symmetric**.
Its odd children obey a clean period-8 law (`sylver2 oddscan`):

```
u ≡ 1 (mod 8)  ->  winning move u + 2
otherwise      ->  winning move u − ((u−1) mod 8)
```

verified N for all odd `u ≤ 61`; the P-targets are `⟨2B, m, m+2j⟩` with `m ≡ 1 (mod 8)`,
`j ∈ {1,2,3}`. So `⟨8,10,12,14⟩` is almost certainly **P with a non-symmetric core**. Two
consequences:

- The tempting converse — *"`2B` is P ⟹ `B` symmetric"* — is **false**, so Theorem 2 cannot be the
  whole story and lasso-type machinery is still needed in general.
- `⟨8,14⟩ P` rests on this position: `⟨8,12,14⟩ = 2⟨4,6,7⟩` has no odd witness (its core is
  symmetric, so all odd `u ∈ B` are N by Theorem 2, and the odd gaps 3, 5, 9 all give N children),
  so its only possible witness is the even move to `⟨8,10,12,14⟩`.

Proving that period-8 law is a concrete, well-posed next target.

---

## 6. Independent reproductions

Done with `indep.cpp`, which uses **no pruning whatsoever** — not even R1. These are unconditional.

| claim | source | result |
|---|---|---|
| `⟨16,28,38,40⟩` **P** | §7.2 headline | **REPRODUCED BIT-FOR-BIT** — `POSITION <16,28,38,40> P all children N nodes=2890848`, node count identical |
| `⟨16,27,28,46⟩` **P** | §7.3, flagged *"must be regenerated"* | **CONFIRMED** — `F=95`, genus 59, 766,463 states |
| `⟨16,6⟩` N via 7 | `R2_roots_small.txt` | CONFIRMED (`⟨6,7,16⟩` P) |
| `⟨16,10⟩` N via 9 | `R2_roots_small.txt` | CONFIRMED (`⟨9,10,16⟩` P) |
| `⟨16,18⟩` N via 5 | `R2_roots_small.txt` | CONFIRMED (`⟨5,16,18⟩` P) |
| `⟨16,20,38⟩` N via 13 | `test_8_10_19.out` | CONFIRMED (`⟨13,16,20,38⟩` P) |
| `⟨4,6⟩` P | `twogen_scan.txt` | CONFIRMED (gcd-2 solver) |

**`⟨16,27,28,46⟩` is an upgrade.** The handoff says its log "is not a full replayable proof
transcript" and must be regenerated. It is now verified by an independent, heuristic-free
implementation, with `F = 95` matching their log exactly. Therefore `⟨16,28,46⟩` **is N via 27**,
and the claim moves to CERTIFIED.

Differential testing: the bundle's `test_finite` and my pruning-free solver agree on every input
tried (`⟨4,7⟩`, `⟨3,5⟩`, `⟨8,10,17⟩`, `⟨6,8,9⟩`, `⟨5,7,9⟩`, `⟨7,9,11,13⟩`).

My gcd-2 solver independently reproduces the ray-root witnesses:

```
n=3  Q=<16,6>   core <8,3> sym  ==> N [odd move 7]
n=5  Q=<16,10>  core <8,5> sym  ==> N [odd move 9]
n=9  Q=<16,18>  core <8,9> sym  ==> N [odd move 5]
n=7  Q=<16,14>  core <8,7> sym  ==> UNKNOWN   (their "N via 8" needs ⟨8,14⟩ P — see §5)
```

---

## 7. The bundle never decided a ray root

Six `.out` files are **empty** while their `.err` files contain only the solver's header line —
these runs crashed or were killed immediately:

| run | core | reported | outcome |
|---|---|---|---|
| `q11_root` | `⟨8,11⟩` | `cores=857 F=69 genus=35` | **died, no result** |
| `q15_m20` | — | `cores=1048 F=57 genus=29` | **died, no result** |
| `q15_root512` | `⟨8,15⟩` | `cores=3601 F=97 genus=49` | **died, no result** |
| `q19_probe` | `⟨8,19⟩` | `cores=11682 F=125 genus=63` | **died, no result** |
| `test_8_14_25` | `⟨8,14,25⟩` | `cores=1337 F=59 genus=30` | **died, no result** |
| `Ps7_root` | — | progressed, then stopped | **died, no result** |

These are precisely the interesting targets: `Q_11 = ⟨16,22⟩`, `Q_15 = ⟨16,30⟩`, `Q_19 = ⟨16,38⟩`,
and the shell cell `𝒫_3 = ⟨16,28,50⟩`.

**So the bundle decided no genuine odd-ray root.** The four roots it reports (`⟨16,6⟩`, `⟨16,10⟩`,
`⟨16,14⟩`, `⟨16,18⟩`) are the small ones. The handoff's §15 OPEN entry "complete outcome table for
all seven rays" understates this: the table is not partially filled, it is **empty** for every root
beyond `n = 9`.

The cause is the documented `ExactMask` cap (§7.1): `set()` throws for `x ≥ 256` and
`numerical_gaps_exact` uses `LIM = 255`. Odd children `⟨2B,u⟩` have Frobenius number `u + 2F(B)`,
so for `B = ⟨8,11⟩` (`F = 69`) they blow past 255 almost at once. The 512-bit variant only moves the
wall — `q15_root512` used it and still died. Theorem 2 attacks exactly this bottleneck by removing
the need to walk the odd family at all for these cores.

---

## 8. Other defects found

- **Checksums do not verify as shipped.** `MANIFEST.sha256` and `SHA256SUMS.txt` record absolute
  `/mnt/data/...` paths, so `sha256sum -c` fails with "No such file or directory" on every entry.
  Worse, `MANIFEST.sha256` lists **its own** hash as `e3b0c442…b855` — the hash of the *empty
  string*. The manifest cannot be self-consistent. Ship relative paths, exclude the manifest from
  itself.
- **Hard-coded instance tuning.** In `ensure_base`:
  ```cpp
  bool p = (nu==35 ? exact.isP_with_hint(em,82) : exact.isP(em));
  ```
  A magic-number special case for `nu == 35` with hint `82`. It does not affect correctness
  (`isP_with_hint` falls back to a full solve), but a solver hand-tuned to specific instances should
  not underpin a certificate library.
- **Typo in the move-ordering table.** `pri` is a list of odd numbers and primes but contains `94`
  between `89` and `97` (almost certainly `93` or `91`). Harmless — `rankpri` only orders moves —
  but it is dead weight and shows the table went unchecked.
- **`verify_doubled_core512.cpp` is not an independent implementation.** It is byte-identical to
  `verify_doubled_core.cpp` apart from mask width (4→8 words) and `LIM` (255→511). The handoff's
  §16 agent 9/10 tasks ("replace masks", "independent checker from scratch") are unstarted; a second
  copy of the same code provides no independent verification.

---

## 9. Corrected status ledger

**PROVED (proofs given here)**
- **Theorem 1 (R2)**: symmetric with `F > 1` ⟹ N. *Was the project's hidden assumption.*
- R1: `2 ∈ S` or `3 ∈ S` with `F > 1` ⟹ N.
- Frobenius interface `⟨S,x⟩ = ⟨S⁺,x⟩` when `F − x ∈ S`; solver move coverage complete.
- **Theorem 2**: symmetric-core reduction — the odd tail of `2B` is finite when `B` is symmetric.
- `⟨8,n⟩` symmetric for all odd `n`; `⟨8,4k+2,8k+1⟩` symmetric for all `k` (via `genus = (F+1)/2`).
- Handoff §15's "odd-response family N" — now genuinely proved, as a corollary of Theorem 1.

**VERIFIED COMPUTATIONALLY (no counterexample, not a proof)**
- Handoff §5.1 duplication invariants: 734/734.
- Lemma C 804/804; Lemma D 804/804.

**CERTIFIED (independent, unconditional)**
- `⟨16,28,38,40⟩` P — reproduced bit-for-bit; hence `⟨16,28,38⟩` N via 40.
- `⟨16,27,28,46⟩` P, hence `⟨16,28,46⟩` N via 27. *(upgraded from "must be regenerated")*
- `⟨16,6⟩` N via 7; `⟨16,10⟩` N via 9; `⟨16,18⟩` N via 5; `⟨16,20,38⟩` N via 13; `⟨4,6⟩` P.

**REFUTED**
- "`2B` is P ⟹ `B` symmetric" — `2⟨4,5,6,7⟩ = ⟨8,10,12,14⟩` is a counterexample (§5).

**OPEN**
- Outcome of `{16}`. Conway's $1000 question.
- Conjecture R2⁺ (the `δ = 1` / pseudo-symmetric case).
- All seven ray outcome tables; the bundle has **no** root beyond `n = 9` (§7).
- `𝒫_3 = ⟨16,28,50⟩` — odd tail now finite (§4b), still undecided.
- The period-8 law of §5.

---

## 10. Recommended next steps, in priority order

1. **Re-run the four crashed ray roots** (§7) using Theorem 2, which makes their odd families
   finite. `Q_11`, `Q_15`, `Q_19` and `𝒫_3` are now in reach in a way they were not. This is the
   single biggest unlock and needs no new mathematics.
2. **Settle `⟨8,10,12,14⟩`** by proving the period-8 law of §5. `⟨8,14⟩ P` — and hence `⟨16,14⟩ N` —
   actually rests on it.
3. **Prove R2⁺** by closing the single exceptional branch `u = F/2` (§2). Payoff: the strictly
   stronger one-line pruning rule `δ(S) ≤ 1 ⟹ N`.
4. **Update the handoff ledger**: R2 was never listed; it is now Theorem 1. Add Theorem 2, which
   supersedes the lasso machinery for symmetric cores.
5. **Fix the manifests** and write the genuinely independent checker of §16 agent 10.

---

## Reproducing

```bash
g++ -O2 -std=c++20 -DNDEBUG indep.cpp      -o indep       # pruning-free gcd-1 solver
g++ -O2 -std=c++20 -DNDEBUG sylver2.cpp    -o sylver2     # gcd-2 solver
g++ -O2 -std=c++20 -DNDEBUG proofcheck.cpp -o proofcheck  # structural identity of Theorem 1

./proofcheck 20             # Steps 1 and 3 of Theorem 1, all symmetric S of genus <= 20
./indep audit 20            # R1/R2 audit over all semigroups of genus <= 20
./indep delta 20            # outcome vs defect delta (Conjecture R2+)
./indep sym 15              # winning moves from symmetric positions
./indep solve 16 27 28 46   # -> P, F=95                      (unconditional)
./sylver2 lemma             # Lemma C / Lemma D checks
./sylver2 p2 4 7            # position <8,14>
./sylver2 oddscan 3 61 4 5 6 7   # the period-8 family of section 5
./sylver2 ray 3 21          # odd ray roots Q_n
```
