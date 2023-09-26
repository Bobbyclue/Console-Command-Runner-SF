#include "Hooks.h"

#include <toml++/toml.h>

namespace Functions
{
    RE::TESForm* LookupByID(std::uint32_t a_formID)
    {
        using func_t = decltype(&Functions::LookupByID);
        REL::Relocation<func_t> func{ REL::ID(86125) };
        return func(a_formID);
    }

    // static REL::Relocation<__int64 (*)(double, char*, ...)> ExecuteCommand{ REL::Offset(0x287DF04) }; // 1.7.29
    // static REL::Relocation<__int64 (*)(double, char*, ...)> ExecuteCommand{ REL::Offset(0x2881064) }; // 1.7.33
    static REL::Relocation<__int64 (*)(double, char*, ...)> ExecuteCommand{ REL::ID(166307) }; // Address Library ID

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

                                            logger::info("{} execute {}", a_Event, commandCString);
                                            ExecuteCommand(0, commandCString);
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
}
