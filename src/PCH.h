#pragma once

#define WIN32_LEAN_AND_MEAN

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#pragma warning(push)
#include <spdlog/sinks/basic_file_sink.h>
#include <SimpleIni.h>
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = SKSE::log;

using namespace std::literals;
using RNG = SKSE::stl::RNG;

#include "Version.h"
