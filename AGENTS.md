# Gran Turismo

Resolve `[XPORT_ROOT]` from `xport-project.json` and read `[XPORT_ROOT]/AGENTS.md`. Keep only stable game-specific facts and task-routing links here; put detailed evidence under `status`. Never create, edit, move, delete or regenerate `README.md`.

## Project facts

- Native short name: `GT`; language: C
- Solution: `src/platform/win/GT.sln`; Debug executable: `bin/GT_debug.exe`; Release executable: `bin/GT.exe`; working directory: `bin`; intermediates: `_build`
- Runtime data: `bin/DATA`; Red Book output when applicable: `bin/MUSIC`
- Internal native tests run on a hidden desktop and skip intros with Start or Cross; launcher and native input scenario: `status/native-startup-fix/hidden-arcade.cdb`
- Before relying on them, record reviewed image identities, language decision, dummy scope, hooks/layouts, adapter contract and evidence links here

## Original image evidence

- Supplied disc: Gran Turismo (USA) v1.0; BIN SHA-256 `816beabc9e8f694510fb3bbb6ea80a68546dc1273fc5d9f3da6f7600d66c2206`; CUE SHA-256 `dbcdae4dbe93bd1ad137090c067f14ec9f4a9fce5a7e285013aaf06f63fb4bd8`
- Boot image: `SCUS_941.94`, confirmed by immutable `orig/SYSTEM.CNF`; configured primary gameplay image: `GTMAIN.EXE`
- All five original PS-X EXE files use PSLZ; original files under `orig` remain immutable
- IDA loads verified decoded payloads under `status/pslz-review` at `0x80010000`, offset 0; decoded entry is `0x80010000` for each image
- Original and decoded hashes, lengths and GP candidates: `status/pslz-review/report.json`
- Original unpacker MIPS execution and byte-for-byte decoder comparison: `status/pslz-review/mips-verification.json`
- BIOS Exec supplies GP from the original EXE header; exact BIOS hash and transfer instructions: `status/pslz-review/bios-gp-evidence.json`
- PSLZ inspection entrypoints in `tools` are thin shims to the configured shared xport toolset; never add game function implementations to them

## Startup classification scope

- Reviewed SDK classification: `status/ghidra/classification.json`; per-item hash/chunk/MIPS checks and pending decisions: `status/ghidra/startup-review.json`
- 905 image-qualified SDK entries define planned replacement boundaries; 74 ambiguous/weak candidates remain TODO
- Wrapper ABI and runtime equivalence remain unverified; no dummy game bodies are permitted during startup
- Native target language is C; original source language is not established by the startup evidence
- `xport_gpu_graph_type_address` remains unresolved until original-image storage evidence is audited
- Reserved GDB roles: user 2434, lockstep 2435, audit 2436, trace 2437; each role has its own configured data directory

## Startup entrypoint boundary

- `GTMAIN.EXE:80010000` maps to `gtmain_startup` in `src/game_main.c`; the same original owner's secondary entry `8001004C` maps to `gtmain_restore_loader_context`
- `GTMainStartupState` in `src/game_startup.h` carries explicit 32-bit guest arguments, results, at, SP, RA and s0..s7; it is a native translation boundary, not an inferred original structure
- Complete static owner mapping: `status/audits/GTMAIN.EXE-80010000.md`; independent raw-MIPS/C fixture comparison: `status/startup-owner-verification/receipt.json`
- The entrypoint is WIP; declarations for its three callees and reviewed loader-context adapters remain unresolved link dependencies for coverage, with no production stubs or fabricated initial state
- GPU graph binding, real callee ABI/body behavior and recorded-game runtime equivalence remain unverified

## Native coverage routing

- `coverage first2` selected owners, agreed C signatures, source inventory and current native blocker: `status/function-coverage/first2/native-delivery.json`; static entry uniqueness selects GTMAIN owners and does not establish runtime overlay ownership
- Project adapter contracts and reviewed shared identities: `status/function-coverage/first2/native-disc-toc-adapter.json`, `native-cd-async-adapter.json`, `native-timer1-binding.json` and `gte-lz-native-binding.json` under the same status directory; fail-fast missing BIOS/SDK/adapter boundaries remain authorized and unverified
- Native race context and actual DRAM ordering bindings: `status/function-coverage/first2/native-race-context-adapter.json` and `native-ordering-dram-adapter.json`; native arrays outside DRAM and remaining frame callback adapters are unresolved
- Native TIM workspaces and retained static-image source spans: `status/function-coverage/first2/native-tim-workspace-adapter.json`; nonstatic compressed resource containers remain unresolved

## Native audio adapter

- Translated SPU register accesses use project `src/game_spu_access.h` and `src/game_draft_spu_access.c`, which route to existing native PsyQ/SPU APIs; ordinary guest memory and shared IRQ binding retain the existing contract
- Adapter scope, explicit unsupported operations, build logs and Arcade model detail observations: `status/function-coverage/first2/audio-lod-repair.json`
- Cold native launch restores reviewed SCUS sound/controller defaults once; GTMAIN SDK shutdown retains SPU sample RAM so GAMESTATUS survives EXE transitions; garage initialization remains TODO in `src/game_draft_boot_snapshot.c`
