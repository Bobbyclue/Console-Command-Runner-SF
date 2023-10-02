#include "Hooks.h"

#include <toml++/toml.h>

namespace RE
{
    void* BGSScaleFormManager()
    {
        static REL::Relocation<void**> singleton{ REL::ID(879512) };
        return *singleton;
    }

    void ExecuteCommand(void* a_scaleformManager, const char* a_command)
    {
        using func_t = decltype(&ExecuteCommand);
        REL::Relocation<func_t> func{ REL::ID(166307) };
        func(a_scaleformManager, a_command);
    }
} // namespace RE

namespace Functions
{
    void StoreCommands()
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

                                if (variableNameString == "OnMenuOpenCloseEvent")
                                {
                                    std::string hasMenuName  = "";
                                    int         hasIsOpening = -1;

                                    const auto menuNameCond  = variableNameTbl["asMenuName"].as_string();
                                    const auto isOpeningCond = variableNameTbl["abOpening"].as_boolean();

                                    if (menuNameCond)
                                    {
                                        hasMenuName = menuNameCond->get();
                                    }

                                    if (isOpeningCond)
                                    {
                                        hasIsOpening = isOpeningCond->get();
                                    }

                                    const auto menuPair = std::make_pair(hasMenuName, hasIsOpening);

                                    const auto dataLoadedArr = variableNameTbl["Commands"].as_array();

                                    if (dataLoadedArr)
                                    {
                                        for (auto&& tag : *dataLoadedArr)
                                        {
                                            const auto tagStr        = tag.as_string();
                                            const auto commandString = tagStr->get();

                                            logger::info("OnMenuOpenCloseEvent store command {}", commandString);

                                            if (menuOpenCloseMap.count(menuPair))
                                            {
                                                auto& oldVector = menuOpenCloseMap.at(menuPair);

                                                oldVector.push_back(commandString);
                                            }
                                            else
                                            {
                                                menuOpenCloseMap.insert_or_assign(menuPair, std::vector(1, commandString));
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    const auto dataLoadedArr = variableNameTbl["Commands"].as_array();

                                    if (dataLoadedArr)
                                    {
                                        for (auto&& tag : *dataLoadedArr)
                                        {
                                            const auto& tagStr        = *tag.as_string();
                                            const auto  commandString = tagStr.as_string()->get();

                                            logger::info("Data Loaded store command {}", commandString);
                                            dataLoadedMap.push_back(commandString);
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

    void RunCommands()
    {
        for (const auto commandString : dataLoadedMap)
        {
            const auto command = commandString.data();

            if (command)
            {
                logger::info("{} execute {}", "DataLoaded", commandString);
                RE::ExecuteCommand(RE::BGSScaleFormManager(), command);
            }
        }
    }
} // namespace Functions

namespace Events
{
    RE::BSEventNotifyControl EventHandler::ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
    {
        for (const auto commandMap : menuOpenCloseMap)
        {
            const auto commandPair = commandMap.first;

            if ((commandPair.first == a_event.menuName || commandPair.first == "") && (commandPair.second == a_event.opening || commandPair.second == -1))
            {
                const auto commandList = menuOpenCloseMap.at(commandPair);

                for (const auto commandString : commandList)
                {
                    const auto command = commandString.data();

                    if (command)
                    {
                        logger::info("{} with params {} {} execute {}", "OnMenuOpenCloseEvent", a_event.menuName.data(), a_event.opening, commandString);
                        RE::ExecuteCommand(RE::BGSScaleFormManager(), command);
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
} // namespace Events
