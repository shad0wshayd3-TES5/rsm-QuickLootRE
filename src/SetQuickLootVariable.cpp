#include "SetQuickLootVariable.h"

#include <cstdarg>

#include "LootMenu.h"
#include "Settings.h"


bool SetQuickLootVariable::Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::CommandInfo::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
{
	auto strChunk = static_cast<RE::CommandInfo::StringChunk*>(a_scriptData->GetChunk());
	auto name = strChunk->GetString();

	auto intChunk = static_cast<RE::CommandInfo::IntegerChunk*>(strChunk->GetNext());
	auto val = intChunk->GetInteger();

	auto setting = Settings::set(name, val);
	if (setting) {
		Dispatch<SetupDelegate>();
		CPrint("> [LootMenu] Set \"%s\" = %s", name.c_str(), setting->getValueAsString().c_str());
	} else {
		CPrint("> [LootMenu] ERROR: Variable \"%s\" not found.", name.c_str());
	}

	return true;
}


void SetQuickLootVariable::Register()
{
	using Type = RE::SCRIPT_PARAMETER::Type;

	auto info = RE::CommandInfo::LocateConsoleCommand("TestSeenData");  // Unused
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "Name", Type::kString, 0 },
			{ "Value", Type::kInteger, 0 }
		};
		info->longName = "SetQuickLootVariable";
		info->shortName = "sqlv";
		info->helpText = "<SetQuickLootVariable|sqlv> <variable-name> <new-value>";
		info->isRefRequired = false;
		info->SetParameters(params);
		info->execute = &Exec;
		info->eval = 0;

		_DMESSAGE("Registered console command: %s (%s)", info->longName, info->shortName);
	} else {
		_ERROR("Failed to register console command!\n");
	}
}


void SetQuickLootVariable::CPrint(const char* a_fmt, ...)
{
	auto console = RE::ConsoleManager::GetSingleton();
	if (console && console->IsConsoleMode()) {
		std::va_list args;
		va_start(args, a_fmt);
		console->VPrint(a_fmt, args);
		va_end(args);
	}
}
