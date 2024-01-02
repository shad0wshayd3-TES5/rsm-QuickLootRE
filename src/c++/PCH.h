#pragma once
#pragma warning(disable : 4702)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <boost/regex.hpp>
#include <frozen/map.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "AutoTOML.hpp"

#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;

namespace logger = SKSE::log;
namespace WinAPI = SKSE::WinAPI;

namespace stl
{
	using namespace SKSE::stl;

	constexpr std::string_view safe_string(const char* a_str) { return a_str ? a_str : ""sv; }
}

#ifdef SKYRIM_AE
# define OFFSET(se, ae) ae
#else
# define OFFSET(se, ae) se
#endif

#include "Version.h"

#include "Settings.h"
