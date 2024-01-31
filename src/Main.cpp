#include "Hooks.h"

extern "C" DLLEXPORT constinit auto SFSEPlugin_Version = []() noexcept {
	SFSE::PluginVersionData data{};

	data.PluginVersion(Plugin::VERSION);
	data.PluginName(Plugin::NAME);
	data.UsesSigScanning(false);
	data.UsesAddressLibrary(true);
	data.HasNoStructUse(false);
	data.IsLayoutDependent(true);
	data.CompatibleVersions({ SFSE::RUNTIME_LATEST });

	return data;
}();

// SFSE message listener, use this to do stuff at specific moments during runtime
void Listener(SFSE::MessagingInterface::Message* message) noexcept
{
    if (message->type == SFSE::MessagingInterface::kPostLoad)
    {
        Functions::StoreCommands();
    }
    else if (message->type == SFSE::MessagingInterface::kPostDataLoad) {
        RE::UI::GetSingleton()->RegisterSink(Events::EventHandler::GetSingleton());
        Functions::RunCommands();
    }
}

// Main SFSE plugin entry point, initialize everything here
extern "C" DLLEXPORT bool SFSEPluginLoad(const SFSE::LoadInterface* sfse)
{
    Init(sfse);

    logger::info("{} {} is loading...", Plugin::NAME, Plugin::VERSION.string("."sv));

    if (const auto messaging{ SFSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    logger::info("{} has finished loading.", Plugin::NAME);

    return true;
}
