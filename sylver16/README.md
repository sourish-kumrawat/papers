# Sylver Coinage, Opening 16

Independent audit of `SYLVER16_SOL_ULTRA_BUNDLE_20260801.zip`, four new theorems, and three ray
roots decided.

> **Opening 16 is not solved.** Whether `{16}` is P or N remains open — it is Conway's $1000
> prize question, and nothing here settles it. Every claim below is either proved or explicitly
> marked as computational evidence.

## Documents

| file | contents |
|---|---|
| `AUDIT_AND_RESULTS_2026-08-01.md` | the audit: what the bundle got right, wrong, and left unstated |
| `WHAT_REMAINS.md` | the reduction, and the remaining obligations O1–O5 |
| `THEOREM4.md` | the exceptional-gap criterion, the strongest N-test obtained |
| `RAY_ROOTS.md` | ray roots decided, method, and the current frontier |
| `O4_O5_ATTEMPT.md` | three failed attacks on the open part, recorded so they aren't repeated |

## The audit finding

The bundle's solver prunes with `is_symmetric ⟹ N`, applied with **no witness move**. Every
certificate in the library passes through it, and the handoff never states it as an assumption —
its ledger even files a consequence of it under *PROVED*. As shipped, the whole library rested on
an unexamined conjecture.

It is now a theorem (Theorem 1), so the library is sound and unconditional.

## Theorems proved here

- **Lemma A (Absorption).** If a position `X` has a child `Y` with `children(Y) ⊆ children(X)`,
  then `X` is N. A strategy-stealing argument — it names no uniform winning move, which is why the
  winning moves from symmetric positions look erratic and why a pairing proof was never found.
- **Theorem 1 / 1′ (defect rule).** If `F(S) > 1` and `δ(S) = 2·genus − F − 1 ≤ 1`, then `S` is N.
  Covers the symmetric *and* pseudo-symmetric cases. Key identity: `δ = 2D + [F even]`, where `D`
  counts gap pairs `{u, F−u}`.
- **Theorem 2 (symmetric-core reduction).** If `B` is symmetric, every odd move `u ∈ B` from `2B`
  is N — collapsing the infinite odd family to the finitely many odd gaps of `B`. This replaces the
  bundle's `LazyOddEngine` / lasso machinery for symmetric cores.
- **Theorem 3 (gluing).** `S` symmetric, `gcd(u,d) = 1`, `u ∈ S` ⟹ `⟨dS,u⟩` symmetric with
  `F = d·F(S) + (d−1)u`. Discharges every drop-to-gcd-1 branch at every layer.
- **Theorem 4 (exceptional-gap criterion).** If `S` is P then some *exceptional* gap `w` has
  `⟨S⁺,w⟩` P. Certifies **64.8%** of N-positions, against 4.5% for anything available before.

## Results

**Ray roots decided** (the bundle decided none beyond `n = 9`; all its root runs crashed):

| root | layer | move | witness |
|---|---|---|---|
| `Q_11 = ⟨16,22⟩` | gcd 2 | **12** | `⟨12,16,22⟩` P — its `q11_root` run died with no result |
| `Q_17 = ⟨16,34⟩` | gcd 2 | **20** | `⟨16,20,34⟩` P |
| `Q_9 = ⟨16,18⟩` | gcd 2 | 12 | `⟨12,16,18⟩` P (second witness) |
| `Q_7 = ⟨16,14⟩` | gcd 2 | 8 | `⟨8,14⟩` P — unconditional once O3 closed |
| `⟨16,12⟩` | gcd 4 | **14** | `⟨12,14,16⟩` P |
| `⟨16,20⟩` | gcd 4 | **34** | `⟨16,20,34⟩` P |
| `⟨16,4⟩` | gcd 4 | 6 | `⟨4,6⟩` P |
| `⟨16,24⟩` | gcd 8 | **10** | `⟨10,16,24⟩` P |
| `⟨16,8⟩` | gcd 8 | 14 | `⟨8,14⟩` P |

The gcd-4 and gcd-8 layers were untouched by the bundle. Every witness's **odd branch** is
independently verified (Theorem 2 reduces each to a handful of odd-gap checks); the even branches
rest on the bundle's solver, whose pruning rules are proved sound here.

**O3 closed.** `⟨8,10,12,14⟩ = 2⟨4,5,6,7⟩` is P, so `⟨8,12,14⟩` is N via 10 and `⟨8,14⟩` is P.
This also certifies the refutation of *"`2B` P ⟹ `B` symmetric"*.

`Q_11`'s **odd branch is completely classified**: Theorem 2 discharges infinitely many moves by
proof, leaving 19 odd gaps, all checked N. The same computation for `Q_13` is in progress.

**Every root decided is N.** A single P-root anywhere would make `{16}` N and refute the opening;
all seven rays being P-root-free would make `{16}` P. Nothing here distinguishes the two — the rays
are infinite and only their first few members are known.

**Reproductions.** The headline `⟨16,28,38,40⟩` P reproduced bit-for-bit (`nodes=2890848`);
`⟨16,27,28,46⟩` P confirmed unconditionally, upgrading the certificate the handoff flagged as
unreliable.

## Building

```bash
g++ -O2 -std=c++20 -DNDEBUG indep.cpp      -o indep       # pruning-free reference solver
g++ -O2 -std=c++20 -DNDEBUG sylver2.cpp    -o sylver2     # gcd-2 solver
g++ -O2 -std=c++20 -DNDEBUG proofcheck.cpp -o proofcheck  # structural identity, Theorem 1
g++ -O2 -std=c++20 -DNDEBUG thm3.cpp       -o thm3        # Theorems 1' and 3
g++ -O2 -std=c++20 -DNDEBUG relabs.cpp     -o relabs      # Theorem 4
g++ -O2 -std=c++20 -DNDEBUG fast.cpp       -o fast        # Theorem 4 as a decision procedure
g++ -O2 -std=c++20 -DNDEBUG absorb.cpp     -o absorb      # the failed generalisation (negative result)
```

Every criterion is differential-tested against `indep`, which uses no pruning at all.
