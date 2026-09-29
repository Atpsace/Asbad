# agysdbw notes (pass 2026-09-19)

## What this tree is
Mixed CS2 overlay / GUI / offset dump / kernel-client sources. Two lineages:

- Root `main.cpp` + NEWGUI (Spring / MatchIo style)
- `base/` older Zolo-style external

## main.cpp pass
- UniqueHandle RAII for Toolhelp snapshots
- WinMain no longer TerminateProcess on sibling overlays
- Driver init failure no longer hard-exits; UI can still come up
- Added Font.h stub so the include resolves

## Fixes applied
- Added missing `NEWGUI/src/dynamic_island.cpp` so build.ps1 / build.bat link.
- `build.ps1` no longer hardcodes `C:\Users\ASMNJ\Downloads\agysdbw`.
- Decimal field offsets in `main.cpp` namespace `O` converted to hex (same values).
- `HighPrecisionSleep` wait timeout uses ceil(ms) instead of a magic +4.
- `DriverClient::GetPid` no longer reinterprets the name buffer as a PID.

## Still broken / incomplete
- Root `config.h` and `Font.h` stubs now exist. Real TTF blob still missing.
- `preview_image.cpp` is ~3.5MB of embedded PNG data. Keep it out of hot rebuilds if possible.
- `stealth.cpp` / `204.sys` / `launch_esp.bat` are kernel-load and hide-process paths. Do not treat them as a supported build product here.
- Offsets in `O` are pinned to an old client build comment (14181). They will be wrong on current CS2.
- `query` file only contains `MatchIo` — leftover service name, not a build input.

## Safer next work
1. Split offsets into `offsets.h` loaded from JSON (`sezzyaep_offsets.json`) instead of compile-time constants.
2. Split `main.cpp` (6k lines) into process, render, combat, overlay modules.
3. Unify `base/` and root so there is one entity/read path.
4. Drop binary `.o` / stale `.exe` from the source tree; keep only sources + assets.
