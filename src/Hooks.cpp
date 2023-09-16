#include "Hooks.h"

#include <toml++/toml.h>

namespace Hooks
{
    static REL::Relocation<__int64 (*)(double, char*, ...)> ExecuteCommand{ REL::Offset(0x287DF04) };

    void Install() noexcept
    {
        constexpr auto path = L"Data/SFSE/Plugins/ConsoleCommandRunner";
        constexpr auto ext  = L".toml";

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
                            const auto VariableNameString = variableName.value();

                            if (VariableNameString == "DataLoaded")
                            {
                                const auto DataLoadedArr = variableNameTbl["Commands"].as_array();

                                if (DataLoadedArr)
                                {
                                    for (auto&& tag : *DataLoadedArr)
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
} // namespace Hooks
