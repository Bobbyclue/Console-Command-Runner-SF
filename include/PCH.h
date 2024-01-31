#pragma once

#pragma warning(push)
#include <RE/Starfield.h>
#include <REL/Relocation.h>
#include <SFSE/SFSE.h>

#include <spdlog/sinks/basic_file_sink.h>

#pragma warning(pop)

using namespace std::literals;

namespace logger = SFSE::log;

namespace util
{
	using SFSE::stl::report_and_fail;
}

#define DLLEXPORT __declspec(dllexport)

#include "Plugin.h"
