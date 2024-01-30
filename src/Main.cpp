#include "Hooks.h"
#include "Hooks.cpp"

// SFSE message listener, use this to do stuff at specific moments during runtime
void Listener(SFSE::MessagingInterface::Message* message) noexcept
{
    if (message->type == SFSE::MessagingInterface::kPostLoad)
    {
        Functions::StoreCommands();
        RE::UI::GetSingleton()->RegisterSink(Events::EventHandler::GetSingleton());
    }
    else if (message->type == SFSE::MessagingInterface::kPostDataLoad) {
        Functions::RunCommands();
    }
}

// Main SFSE plugin entry point, initialize everything here
SFSEPluginLoad(const SFSE::LoadInterface* sfse)
{
    Init(sfse);

    logger::info("{} {} is loading...", Plugin::Name, Plugin::Version.string("."sv));

    if (const auto messaging{ SFSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    logger::info("{} has finished loading.", Plugin::Name);

    return true;
}
