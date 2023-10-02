#pragma once

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;
    }
};

static std::unordered_map<std::pair<std::string, int>, std::vector<std::string>, pair_hash> menuOpenCloseMap;
static std::vector<std::string>                                                             dataLoadedMap;

namespace RE
{
    void* BGSScaleFormManager();
    void  ExecuteCommand(void* a_scaleformManager, const char* a_command);
} // namespace RE

namespace Functions
{
    void StoreCommands();
    void RunCommands();
} // namespace Functions

template <class T>
class ISingleton
{
public:
    static T* GetSingleton()
    {
        static T singleton;
        return std::addressof(singleton);
    }

protected:
    ISingleton()  = default;
    ~ISingleton() = default;

    ISingleton(const ISingleton&)            = delete;
    ISingleton(ISingleton&&)                 = delete;
    ISingleton& operator=(const ISingleton&) = delete;
    ISingleton& operator=(ISingleton&&)      = delete;
};

namespace Events
{
    class EventHandler : public ISingleton<EventHandler>,
                         public RE::BSTEventSink<RE::MenuOpenCloseEvent>
    {
        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
    };
} // namespace Events
