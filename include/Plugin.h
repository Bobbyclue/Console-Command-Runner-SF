#pragma once

namespace Plugin
{
    using namespace std::string_view_literals;

    static constexpr auto Name{ "ConsoleCommandsExtender"sv };
    static constexpr auto Author{ "Bobbyclue"sv };
    static constexpr auto Version{
        REL::Version{0, 1, 3, 1}
    };
} // namespace Plugin

namespace SFSE
{
    constexpr REL::Version RUNTIME_SF_1_8_88(1, 8, 88, 0); // this was the version missing from CommonLib
}

extern "C"
{
    __declspec(dllexport) SFSE::PluginVersionData SFSEPlugin_Version = {
        SFSE::PluginVersionData::kVersion,

        1.31, // version 1.3.1
        "Console Commands Extender",
        "Bobbyclue",

        1, // yes address independent
        0, // not structure independent
        {SFSE::RUNTIME_SF_1_8_88.pack(), 0},

        0, // works with any version of the script extender. you probably do not need to put anything here
        0,
        0, // set these reserved fields to 0
    };
};
