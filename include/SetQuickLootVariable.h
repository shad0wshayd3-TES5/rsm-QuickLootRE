#pragma once

#include "RE/Skyrim.h"


class SetQuickLootVariable
{
public:
	static bool Exec(const RE::SCRIPT_PARAMETER* a_paramInfo, RE::SCRIPT_FUNCTION::ScriptData* a_scriptData, RE::TESObjectREFR* a_thisObj, RE::TESObjectREFR* a_containingObj, RE::Script* a_scriptObj, RE::ScriptLocals* a_locals, double& a_result, UInt32& a_opcodeOffsetPtr);
	static void Register();

private:
	SetQuickLootVariable() = delete;
	SetQuickLootVariable(const SetQuickLootVariable&) = delete;
	SetQuickLootVariable(SetQuickLootVariable&&) = delete;
	~SetQuickLootVariable() = delete;

	SetQuickLootVariable& operator=(const SetQuickLootVariable&) = delete;
	SetQuickLootVariable& operator=(SetQuickLootVariable&&) = delete;

	static void CPrint(const char* a_fmt, ...);
	static const char* HelpStr();


	static constexpr char LONG_NAME[] = "SetQuickLootVariable";
	static constexpr char SHORT_NAME[] = "SQLV";
};
