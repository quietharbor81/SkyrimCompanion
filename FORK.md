# Fork notes — quietharbor81/SkyrimCompanion

Forked from itoleck/SkyrimCompanion v0.1.3 (2026-10-29). This fork exists for one
reason: **cut the plugin's game-thread cost on heavily modded installs.** The
upstream plugin rebuilds the quest journal, bounty list, and world-map places on
a 250 ms cadence; on a list with thousands of plugins that sweep is the main
frame-time cost. Nothing about the wire protocol changes — the companion app
from upstream releases works with this dll unchanged.

## Changes (0.1.4)

- **Quest journal cached** (`snapshot.cpp Quests()`): the full quest sweep —
  every quest in the load order plus objective-text resolution — rebuilds every
  **3 s** instead of every snapshot. Served from cache between rebuilds.
- **Bounties cached, double pass merged** (`Crime()`): the faction array was
  walked twice per snapshot (once for the display list, once for the crime-event
  diff). Now one pass every **3 s**; `PollCrime` runs inside the rebuild, so
  crime *events* still reach the app every snapshot via the event ring.
- **World-map places event-driven** (`Places()`): the Tamriel persistent-cell
  walk (a multi-millisecond frame spike) now runs only when a location is
  discovered, after a save load, or as a 30 s fallback — previously every 5 s.
- **Cache invalidation plumbing** (`events.h/cpp`): `MarkJournalDirty` /
  `MarkPlacesDirty` set on save load; places also on `LocationDiscovery`.
- **CI**: `.github/workflows/build.yml` builds the SKSE plugin on a Windows
  runner and uploads the dll as an artifact (upstream has no CI).

## Cadence constants

`skse-plugin/src/snapshot.cpp`: `kQuestsInterval` (3 s) and `kPlacesInterval`
(30 s). Compile-time on purpose — edit and rebuild if you want different pacing.

## Build

The CI workflow is the sanctioned path (needs no local toolchain). Locally:
VS 2022 + CMake ≥ 3.21 + Ninja + vcpkg (`VCPKG_ROOT`), then
`cmake --preset release` / `cmake --build --preset release` in `skse-plugin/`.

## Versioning

`version.txt` bumped to 0.1.4 for this fork; `COMPANION_VERSION` in the dll
follows it. Keep upstream's protocol untouched so app and plugin stay
interchangeable.
