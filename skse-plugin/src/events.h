#pragma once

#include <nlohmann/json.hpp>

namespace companion::events
{
    // Registers the game event sinks. Call once after kDataLoaded.
    void Install();

    // A save was loaded or a new game started: forget per-game state such as bounty baselines.
    void OnGameLoaded();

    // Cache invalidation for the throttled snapshot rebuilds in snapshot.cpp. Journal
    // (quests + bounties) goes stale after a save load; world-map places go stale after
    // a load and whenever a location is discovered.
    void MarkJournalDirty();
    bool ConsumeJournalDirty();
    void MarkPlacesDirty();
    bool ConsumePlacesDirty();

    // Records new bounties since the last call. Must run on the game thread.
    void PollCrime(RE::TESDataHandler* data);

    // Plugin start time in ms; with `seq` it lets the app recognise events it already saved.
    std::uint64_t Session();

    // The most recent events, oldest first (see GameEvent in crates/protocol).
    nlohmann::json Recent();
}
