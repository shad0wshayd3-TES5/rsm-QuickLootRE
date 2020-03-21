#include "SetQuickLootVariable.h"

#include <cstdarg>
#include <string>

#include "LootMenu.h"
#include "Settings.h"


bool SetQuickLootVariable::Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr)
{
	auto strChunk = a_scriptData->GetStringChunk();
	auto name = strChunk->GetString();

	auto intChunk = strChunk->GetNext()->AsInteger();
	auto val = intChunk->GetInteger();

	auto setting = Settings::set(name, val);
	if (setting) {
		Dispatch<SetupDelegate>();
		CPrint("> [%s] Set \"%s\" = %s", LONG_NAME, name.c_str(), setting->getValueAsString().c_str());
	} else {
		CPrint("> [%s] ERROR: Variable \"%s\" not found.", LONG_NAME, name.c_str());
	}

	return true;
}


void SetQuickLootVariable::Register()
{
	using Type = RE::SCRIPT_PARAM_TYPE;

	auto info = RE::SCRIPT_FUNCTION::LocateConsoleCommand("TestSeenData");  // Unused
	if (info) {
		static RE::SCRIPT_PARAMETER params[] = {
			{ "String", Type::kChar, 0 },
			{ "Integer", Type::kInt, 0 }
		};

		info->functionName = LONG_NAME;
		info->shortName = SHORT_NAME;
		info->helpString = HelpStr();
		info->referenceFunction = false;
		info->SetParameters(params);
		info->executeFunction = &Exec;
		info->conditionFunction = 0;

		_MESSAGE("Registered console command: %s (%s)", LONG_NAME, SHORT_NAME);
	} else {
		_ERROR("Failed to register console command!\n");
	}
}


void SetQuickLootVariable::CPrint(const char* a_fmt, ...)
{
	auto log = RE::ConsoleLog::GetSingleton();
	if (log && log->IsConsoleMode()) {
		std::va_list args;
		va_start(args, a_fmt);
		log->VPrint(a_fmt, args);
		va_end(args);
	}
}


const char* SetQuickLootVariable::HelpStr()
{
	static std::string help;
	if (help.empty()) {
		help += "<setquicklootvariable> \" \" <variablename> \" \" <newvalue>";
		help += "\m\t<setquicklootvariable> ::= \"SetQuickLootVariable\" | \"SQLV\"";
		help += "\m\t<variablename> ::= <string> ; The variable to set";
		help += "\m\t<newvalue> ::= <integer> ; The new value of the variable to set";
	}
	return help.c_str();
}
