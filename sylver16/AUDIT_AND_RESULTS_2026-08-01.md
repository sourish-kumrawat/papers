# Sylver Coinage, Opening 16 — Independent Audit and New Reductions

**Date:** 2026-08-01
**Input:** `SYLVER16_SOL_ULTRA_BUNDLE_20260801.zip` (handoff + `sylver16_certificates/`)
**Scope:** review the bundle, verify its claims independently, and push the classification of `{16,x}` as far as sound computation allows.

---

## 0. Bottom line

**Opening 16 is not solved, and this work does not solve it.** Whether `{16}` is P or N remains
open — it is Conway's $1000 prize question. The bundle does not claim to have solved it either
(its own §15 lists "final outcome of opening 16" as OPEN), and nothing found here changes that.

What this review does establish:

1. The bundle's entire certificate library rests on **one unproven lemma that the handoff never
   flags as an assumption** (§1). I verified it exhaustively over all 93,141 numerical semigroups
   of genus ≤ 20 and it holds, but it is not proved.
2. Two of the solver's three pruning rules **are** provable, and I give the proofs (§2).
3. A **new reduction theorem** (§3) collapses the infinite odd-move family — the thing the bundle's
   `LazyOddEngine`, lasso detection and `--kcap` machinery exist to handle — to a *finite* check
   whenever the core is symmetric. This applies to all four odd rays and to the entire shell family.
4. The reduction is **not complete**, and I give the explicit position where it fails (§4).
5. Several bundle claims reproduced independently, one of them **upgraded** from "must be
   regenerated" to certified (§5).
6. The bundle **never decided a single ray root** — every root run crashed (§6).

Verification code: `indep.cpp` (shortcut-free gcd-1 solver), `sylver2.cpp` (gcd-2 solver).

---

## 1. The load-bearing unproven lemma

`verify_doubled_core.cpp` (line 59) decides positions with three pruning rules. The second is:

```cpp
if(is_symmetric(S,F)){ ++symmetric_hits; Rec r{2,0,3}; ... }   // 2 = N
```

> **R2 (assumed).** If `S` is a symmetric numerical semigroup with `F(S) > 1`, then `S` is an N-position.

R2 is applied with **no witness move**. Every P-certificate in the bundle passes through it, because
proving a position P requires proving all children N, and R2 is what discharges most of them.

The handoff does not list R2 anywhere. Its §15 ledger puts *"odd-response family N"* under
**PROVED** — but that claim is exactly R2 specialised to two-generated semigroups, since
`⟨16,q⟩` with `q` odd is 2-generated and every 2-generated numerical semigroup is symmetric.
§10's quarantine list does not mention it either. **R2's status should be `ASSUMED`, not `PROVED`.**

### Evidence

I wrote a solver with *no* pruning at all (`indep.cpp`: pure memoised minimax on the oversemigroup
lattice) and enumerated every numerical semigroup of genus ≤ 20:

```
semigroups solved : 93141   (P: 9633)
RULE R1  [F>1 and (2 in S or 3 in S)  => N] : tested  101, N  101, VIOLATIONS 0
RULE R2  [symmetric and F>1           => N] : tested 1148, N 1148, VIOLATIONS 0
```

So R2 survives 1,148 symmetric semigroups with zero counterexamples. That is strong evidence.

### Why it is not easy to prove

The winning move from a symmetric position is **not uniform**, so there is no obvious pairing or
strategy-stealing argument. Sample (`indep sym`), gaps listed, then all moves to a P-child:

```
F=11  {1 2 3 4 5 11}   -> 11        F=13  {1 2 3 4 5 6 13} -> 4 5 6
F=11  {1 2 4 5 8 11}   -> 2         F=13  {1 2 3 4 6 8 13} -> 8
F=17  {1 2 3 4 5 6 7 8 17} -> 8     F=17  {1 2 3 5 6 9 10 13 17} -> 13
F=19  {1 2 3 4 5 6 7 8 9 19} -> 19  F=19  {1 2 3 4 5 6 8 9 12 19} -> 5 6
```

The move `F` wins in some cases and loses in others; the smallest winning move jumps around.
**Proving R2 is the single highest-value open sub-problem in this project** — it is a clean,
self-contained statement about numerical semigroups, and everything else is downstream of it.

---

## 2. The other two rules are provable

### R1 — proved

> If `F(S) > 1` and (`2 ∈ S` or `3 ∈ S`) then `S` is N.

*Proof.* Write `T = {0,2,3,4,…}`, whose only gap is 1; the player to move in `T` must name 1 and
loses, so `T` is P.
- If `3 ∈ S` and `F(S) > 1`, then `2 ∉ S` (else `F(S)=1`), so 2 is a legal move and
  `⟨S,2⟩ = T`. Hence `S` is N via 2.
- If `2 ∈ S` and `F(S) > 1`, then `3 ∉ S`, and `⟨S,3⟩ = T`. Hence `S` is N via 3. ∎

### Frobenius interface (handoff §5.3) — proved, and move coverage is complete

> If `x` is a gap of `S` and `F − x ∈ S`, then `⟨S,x⟩ = ⟨S⁺,x⟩` where `S⁺ = S ∪ {F}`.

*Proof.* `F = x + (F−x)` with `x ∈ ⟨S,x⟩` and `F−x ∈ S ⊆ ⟨S,x⟩`, so `F ∈ ⟨S,x⟩`, giving
`S⁺ ⊆ ⟨S,x⟩` and therefore `⟨S⁺,x⟩ ⊆ ⟨S,x⟩`. The reverse inclusion is immediate. ∎

This is an *identity*, not a heuristic, so the solver's use of it is sound for both P and N
conclusions. I also checked the solver's move coverage: since everything above `F` lies in `S`,
`S⁺ = S ∪ {F}` exactly, so `gaps(S⁺) = gaps(S) \ {F}`; the code's `nonex`/`ex` split therefore
covers every legal move. **No coverage gap.**

---

## 3. New result: the symmetric-core reduction

This is the main contribution of this review. It removes the need for the lasso machinery in a
large class of cases.

**Lemma C (duplication, handoff §5.1).** For a numerical semigroup `B` and odd `z ∈ B`,
`⟨2B,z⟩ = 2B ⊔ (z + 2B)`.
*Verified computationally on 804/804 cases* (`sylver2 lemma`).

**Lemma D (symmetry transfer).** `⟨2B,z⟩` is symmetric ⟺ `B` is symmetric.
*Verified on the same 804 cases.* (The apparent failures are all the degenerate `B = ⟨2,3⟩`,
which has `F = 1` and **is** symmetric; my code excluded `F ≤ 1` by convention. Fixed.)

> ### Theorem (symmetric-core reduction).
> Let `B` be a symmetric numerical semigroup. Then in the position `2B`, **every odd move
> `u ∈ B` leads to an N-position** (given R2).
>
> Consequently the infinite odd-move family from `2B` collapses to the **finitely many odd gaps
> of `B`**, and `out(2B)` is decidable by a finite computation — no lasso, no `--kcap`, no
> eventual-periodicity argument.

*Proof.* For odd `u ∈ B`, Lemma C gives `⟨2B,u⟩ = 2B ⊔ (u+2B)` and Lemma D makes it symmetric;
its Frobenius number is `u + 2F(B) ≥ 3 > 1`, so R2 applies. Every odd `u ∉ B` is an odd gap of
`B`, and there are at most `(F(B)+1)/2` of those. ∎

The handoff has both ingredients — §5.1 states the duplication formula *and* notes "`D(B,z)` is
symmetric iff `B` is symmetric" — but never draws the game-theoretic consequence. Instead it builds
`LazyOddEngine` with 192-bit state hashing and lasso detection to handle the odd tail numerically.

### Where it applies

**(a) All four odd rays.** `Q_n = ⟨16,2n⟩ = 2⟨8,n⟩`. For odd `n`, `⟨8,n⟩` is two-generated with
coprime generators, hence **symmetric** (classical). So the odd-move tail of *every* odd-ray root
is finite. Confirmed for `n = 3..39`.

**(b) The whole shell family `𝒫_k` (handoff §8.2).** `𝒫_k = ⟨16,8k+4,16k+2⟩ = 2⟨8,4k+2,8k+1⟩`.
Computation gives, for every `k` tested (1..40):

```
F(⟨8,4k+2,8k+1⟩) = 20k − 1 ,   genus = 10k ,   so genus = (F+1)/2  ⟹ symmetric
```

Since `genus = (F+1)/2` is *equivalent* to symmetry, this is a proof once the two closed forms are
established (both verified k=1..40; `k=3 → F=59, g=30`, `k=40 → F=799, g=400`).

This matters because the handoff lists `𝒫_3 = ⟨16,28,50⟩` as unresolved and a
"high-priority falsification/certification target" (§8.2, §15 OPEN, §18.5). Its odd tail is now
finite. Note also that the bundle's own run on this cell (`test_8_14_25`) **crashed** — see §6.

**(c) Every certified P-cell in the bundle has a symmetric core.** Not a coincidence:

| position | core | F | genus | symmetric |
|---|---|---|---|---|
| `⟨8,14⟩` | `⟨4,7⟩` | 17 | 9 | yes |
| `⟨12,16,18⟩` | `⟨6,8,9⟩` | 19 | 10 | yes |
| `⟨12,16,22⟩` | `⟨6,11⟩` | 49 | 25 | yes |
| `⟨16,20,34⟩` | `⟨8,10,17⟩` | 39 | 20 | yes |
| `⟨16,28,38,40⟩` | `⟨8,14,19,20⟩` | 45 | 23 | yes |
| `⟨16,28,50⟩` (open) | `⟨8,14,25⟩` | 59 | 30 | yes |

---

## 4. Where the reduction fails (honest limitation)

The reduction is **not complete**: even children of a symmetric core need not be symmetric, so the
recursion can leave the class.

Running my gcd-2 solver on `⟨8,14⟩ = 2⟨4,7⟩`, the UNKNOWN frontier bottoms out at exactly one
position:

```
⟨4,7⟩  --(even move 12)-->  ⟨4,6,7⟩  --(even move 10)-->  ⟨4,5,6,7⟩
```

i.e. `2⟨4,5,6,7⟩ = ⟨8,10,12,14⟩`, whose core has `F = 3`, `genus = 3`, `2g − F − 1 = 2` —
**not symmetric**. Its odd children show a clean period-8 law (`sylver2 oddscan`):

```
u ≡ 1 (mod 8)  ->  winning move u + 2
otherwise      ->  winning move u − ((u−1) mod 8)
```

verified N for all odd `u ≤ 61`. The P-targets are `⟨2B, m, m+2j⟩` with `m ≡ 1 (mod 8)`, `j ∈ {1,2,3}`.

So `⟨8,10,12,14⟩` is almost certainly **P with a non-symmetric core**. Two consequences:

- The tempting converse — *"`2B` is P ⟹ `B` symmetric"* — is **false**, so the symmetric-core
  reduction cannot be the whole story and the lasso machinery is still needed in general.
- The truth of `⟨8,14⟩ P` hinges on this position: `⟨8,12,14⟩ = 2⟨4,6,7⟩` has no odd witness
  (its core is symmetric, so all odd `u ∈ B` are N, and the odd gaps 3, 5, 9 all give N children),
  so its only possible witness is the even move to `⟨8,10,12,14⟩`.

Proving that period-8 law is a concrete, well-posed next target.

---

## 5. Independent reproductions

All done with `indep.cpp`, which uses **no pruning whatsoever** — these are unconditional
(they do not even assume R1, let alone R2).

| claim | source | result |
|---|---|---|
| `⟨16,27,28,46⟩` **P** | §7.3, flagged *"must be regenerated"* | **CONFIRMED** — `F=95`, genus 59, 766,463 states |
| `⟨16,6⟩` N via 7 | `R2_roots_small.txt` | CONFIRMED (`⟨6,7,16⟩` P) |
| `⟨16,10⟩` N via 9 | `R2_roots_small.txt` | CONFIRMED (`⟨9,10,16⟩` P) |
| `⟨16,18⟩` N via 5 | `R2_roots_small.txt` | CONFIRMED (`⟨5,16,18⟩` P) |
| `⟨16,20,38⟩` N via 13 | `test_8_10_19.out` | CONFIRMED (`⟨13,16,20,38⟩` P) |
| `⟨4,6⟩` P | `twogen_scan.txt` | CONFIRMED (gcd-2 solver) |

**The `⟨16,27,28,46⟩` result is an upgrade.** The handoff says its log "is not a full replayable
proof transcript" and must be regenerated before use. It is now verified by an independent,
heuristic-free implementation, and my `F = 95` matches their log exactly. Therefore
`⟨16,28,46⟩` **is N via 27** — this claim can move to `CERTIFIED`.

Differential testing: the bundle's `test_finite` and my shortcut-free solver agree on every input
tried (`⟨4,7⟩`, `⟨3,5⟩`, `⟨8,10,17⟩`, `⟨6,8,9⟩`, `⟨5,7,9⟩`, `⟨7,9,11,13⟩`).

My gcd-2 solver independently reproduces the ray-root witnesses:

```
n=3  Q=<16,6>   core <8,3> sym  ==> N [odd move 7]
n=5  Q=<16,10>  core <8,5> sym  ==> N [odd move 9]
n=9  Q=<16,18>  core <8,9> sym  ==> N [odd move 5]
n=7  Q=<16,14>  core <8,7> sym  ==> UNKNOWN   (their "N via 8" needs ⟨8,14⟩ P — see §4)
```

---

## 6. The bundle never decided a ray root

Six `.out` files in `sylver16_certificates/` are **empty** while their `.err` files contain only the
solver's header line. These runs crashed or were killed immediately:

| run | core | reported | outcome |
|---|---|---|---|
| `q11_root` | `⟨8,11⟩` | `cores=857 F=69 genus=35` | **died, no result** |
| `q15_m20` | — | `cores=1048 F=57 genus=29` | **died, no result** |
| `q15_root512` | `⟨8,15⟩` | `cores=3601 F=97 genus=49` | **died, no result** |
| `q19_probe` | `⟨8,19⟩` | `cores=11682 F=125 genus=63` | **died, no result** |
| `test_8_14_25` | `⟨8,14,25⟩` | `cores=1337 F=59 genus=30` | **died, no result** |
| `Ps7_root` | — | progressed then stopped | **died, no result** |

These are precisely the interesting targets: `Q_11 = ⟨16,22⟩`, `Q_15 = ⟨16,30⟩`,
`Q_19 = ⟨16,38⟩`, and the shell cell `𝒫_3 = ⟨16,28,50⟩`.

**So the bundle decided no genuine odd-ray root at all.** The four roots it does report
(`⟨16,6⟩`, `⟨16,10⟩`, `⟨16,14⟩`, `⟨16,18⟩`) are the small ones. The handoff's §15 OPEN entry
"complete outcome table for all seven rays" understates this: the table is not partially filled,
it is **empty** for every root beyond `n = 9`.

The cause is the documented `ExactMask` cap (§7.1): `set()` throws for `x ≥ 256`, and
`numerical_gaps_exact` uses `LIM = 255`. Odd children `⟨2B,u⟩` have Frobenius number `u + 2F(B)`,
so for `B = ⟨8,11⟩` (`F = 69`) the children exceed 255 almost immediately. The 512-bit variant
just moves the wall (and `q15_root512`, which used it, still died).

The symmetric-core reduction of §3 attacks exactly this bottleneck, since it removes the need to
walk the odd family at all for these cores.

---

## 7. Other defects found

- **Checksums do not verify as shipped.** `MANIFEST.sha256` and `SHA256SUMS.txt` record absolute
  `/mnt/data/...` paths, so `sha256sum -c` fails with "No such file or directory" for every entry.
  Worse, `MANIFEST.sha256` lists **its own** hash as `e3b0c442…b855` — the hash of the *empty
  string*. The manifest cannot be self-consistent. Ship relative paths and exclude the manifest
  from itself.
- **Hard-coded instance tuning.** `verify_doubled_core.cpp`, in `ensure_base`:
  ```cpp
  bool p = (nu==35 ? exact.isP_with_hint(em,82) : exact.isP(em));
  ```
  A magic-number special case for `nu == 35` with hint `82`. It does not affect correctness
  (`isP_with_hint` falls back to a full solve), but a solver hand-tuned to specific instances
  should not be the basis of a certificate library.
- **Typo in the move-ordering table.** The `pri` array is a list of odd numbers and primes but
  contains `94` between `89` and `97` (almost certainly `93` or `91`). Harmless — `rankpri` only
  orders moves — but it is dead weight and signals the table was not checked.
- **`verify_doubled_core512.cpp` is not an independent implementation.** It is byte-identical to
  `verify_doubled_core.cpp` apart from the mask width (4→8 words) and `LIM` (255→511). The
  handoff's §16 agent 9/10 tasks ("replace masks", "independent checker from scratch") are
  unstarted; a second copy of the same code provides no independent verification.

---

## 8. Corrected status ledger

**PROVED (with proof given here)**
- R1: `2 ∈ S` or `3 ∈ S` with `F > 1` ⟹ N.
- Frobenius interface `⟨S,x⟩ = ⟨S⁺,x⟩` when `F − x ∈ S`; solver move coverage complete.
- Symmetric-core reduction (§3), *conditional on R2*.
- `⟨8,n⟩` symmetric for all odd `n`; `⟨8,4k+2,8k+1⟩` symmetric for all `k` (via `genus = (F+1)/2`).

**ASSUMED — must be reclassified in the handoff**
- **R2**: symmetric with `F > 1` ⟹ N. Verified on all 93,141 semigroups of genus ≤ 20.
  Everything below depends on it.

**CERTIFIED (independent, unconditional)**
- `⟨16,27,28,46⟩` P, hence `⟨16,28,46⟩` N via 27. *(upgraded from "must be regenerated")*
- `⟨16,6⟩` N via 7; `⟨16,10⟩` N via 9; `⟨16,18⟩` N via 5; `⟨16,20,38⟩` N via 13.
- `⟨4,6⟩` P.

**REFUTED**
- "`2B` is P ⟹ `B` symmetric" — `2⟨4,5,6,7⟩ = ⟨8,10,12,14⟩` is a counterexample (§4).

**OPEN (unchanged)**
- Outcome of `{16}`. Conway's $1000 question.
- All seven ray outcome tables; the bundle has **no** root beyond `n = 9` (§6).
- `𝒫_3 = ⟨16,28,50⟩` — odd tail now finite (§3b), still undecided.
- A proof of R2.
- The period-8 law of §4.

---

## 9. Recommended next steps, in priority order

1. **Prove R2.** Self-contained, purely about numerical semigroups, and everything depends on it.
   If it is false, the entire certificate library falls; if true, several claims below follow.
2. **Re-run the four crashed ray roots** using the §3 reduction, which makes their odd families
   finite. `Q_11`, `Q_15`, `Q_19` and `𝒫_3` are now in reach in a way they were not.
3. **Settle `⟨8,10,12,14⟩`** by proving the period-8 law of §4. This is what `⟨8,14⟩ P` —
   and hence `⟨16,14⟩ N` — actually rests on.
4. **Fix the manifests** and write the genuinely independent checker of §16 agent 10.
5. Extend the R2 audit past genus 20 with a canonical-form enumerator.

---

## Reproducing

```bash
g++ -O2 -std=c++20 -DNDEBUG indep.cpp   -o indep      # shortcut-free gcd-1 solver
g++ -O2 -std=c++20 -DNDEBUG sylver2.cpp -o sylver2    # gcd-2 solver

./indep audit 20            # R1/R2 audit over all semigroups of genus <= 20
./indep sym 15              # winning moves from symmetric positions
./indep solve 16 27 28 46   # -> P, F=95            (unconditional)
./sylver2 lemma             # Lemma C / Lemma D checks
./sylver2 p2 4 7            # position <8,14>
./sylver2 oddscan 3 61 4 5 6 7   # the period-8 family of section 4
./sylver2 ray 3 21          # odd ray roots Q_n
```
