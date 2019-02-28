#pragma once

#include <string>  // string

#include "RE/TESObjectREFR.h"  // TESObjectREFR


std::string boolToString(bool a_bool);
bool		IsValidPickPocketTarget(RE::TESObjectREFR* a_refr, bool a_isSneaking);
