#pragma once

namespace Functions
{
    RE::TESForm* LookupByID(std::uint32_t a_formID);
    void RunCommands(const char* a_Event) noexcept;
}
