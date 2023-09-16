#include "Hooks.h"

static DWORD ThreadProc_OnDelayLoad(void* unused)
{
    (void)unused;
    Sleep(8000); // a reasonable amount of time for the game to initialize
    Hooks::Install();
    return 0;
}

// SFSE message listener, use this to do stuff at specific moments during runtime
void Listener(SFSE::MessagingInterface::Message* message) noexcept
{
    if (message->type == SFSE::MessagingInterface::kPostLoad)
    {
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
