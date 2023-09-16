#include "Hooks.h"

static DWORD ThreadProc_OnDelayLoad(void* unused)
{
    (void)unused;
    while (!RE::TESForm::LookupByID(0x14)) // Wait until forms are loaded, then run startup commands
    {
        Sleep(1000);
    }
    Hooks::RunCommands("DataLoaded");
    return 0;
}

// SFSE message listener, use this to do stuff at specific moments during runtime
void Listener(SFSE::MessagingInterface::Message* message) noexcept
{
    if (message->type == SFSE::MessagingInterface::kPostLoad)
    {
        // Hooks::Install();
        CreateThread(NULL, 4096, ThreadProc_OnDelayLoad, NULL, 0, NULL);
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
