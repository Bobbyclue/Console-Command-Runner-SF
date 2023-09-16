#include "Hooks.h"

#include <toml++/toml.h>

namespace Hooks
{
    static REL::Relocation<__int64 (*)(double, char*, ...)> ExecuteCommand{ REL::Offset(0x287DF04) };

    void RunCommands(const char* a_Event) noexcept
    {
        constexpr auto path = L"Data/SFSE/Plugins/ConsoleCommandRunner";

        if (std::filesystem::exists(path))
        {
            constexpr auto ext = L".toml";

            for (const auto& file : std::filesystem::directory_iterator(path))
            {
                if (std::filesystem::is_regular_file(file) && file.path().extension() == ext)
                {
                    const auto filePath = file.path();
                    const auto tbl      = toml::parse_file(filePath.c_str());
                    const auto arr      = tbl.get_as<toml::array>("Event");

                    if (arr)
                    {
                        for (auto&& elem : *arr)
                        {
                            const auto& variableNameTbl = *elem.as_table();
                            const auto  variableName    = variableNameTbl["EventType"].value<std::string>();

                            if (variableName)
                            {
                                const auto variableNameString = variableName.value();

                                if (variableNameString == a_Event)
                                {
                                    const auto dataLoadedArr = variableNameTbl["Commands"].as_array();

                                    if (dataLoadedArr)
                                    {
                                        for (auto&& tag : *dataLoadedArr)
                                        {
                                            const auto& tagStr         = *tag.as_string();
                                            auto        commandString  = tagStr.as_string()->get();
                                            auto        commandCString = commandString.data();

                                            ExecuteCommand(0, commandCString);
                                            logger::info("DataLoaded execute {}", commandCString);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /* // For testing only, should run commands when the game loads. Need to find a better hook.
    static bool GameLoaded;

    struct ActorUpdate
    {
        static void Thunk(float a_delta)
        {
            func(a_delta);
            if (!GameLoaded)
            {
                GameLoaded = true;
                RunCommands("OnPlayerLoadGame");
            }
        }
        [[maybe_unused]] static inline REL::Relocation<decltype(Thunk)> func;

        static inline std::size_t idx = 0x13F;
    };

    void Install() noexcept
    {
        SFSE::stl::write_vfunc<RE::Actor, ActorUpdate>();

        logger::info("Hooked Actor Update");
    }
    */
} // namespace Hooks
