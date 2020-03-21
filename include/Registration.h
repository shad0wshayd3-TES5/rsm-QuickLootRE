#pragma once

#include "SKSE/RegistrationSet.h"


class OnContainerOpenAnim : public SKSE::RegistrationSet<>
{
public:
	static OnContainerOpenAnim* GetSingleton();

private:
	using Base = SKSE::RegistrationSet<>;


	OnContainerOpenAnim();
	OnContainerOpenAnim(const OnContainerOpenAnim&) = delete;
	OnContainerOpenAnim(OnContainerOpenAnim&&) = delete;

	OnContainerOpenAnim& operator=(const OnContainerOpenAnim&) = delete;
	OnContainerOpenAnim& operator=(OnContainerOpenAnim&&) = delete;
};


class OnContainerCloseAnim : public SKSE::RegistrationSet<>
{
public:
	static OnContainerCloseAnim* GetSingleton();

private:
	using Base = SKSE::RegistrationSet<>;


	OnContainerCloseAnim();
	OnContainerCloseAnim(const OnContainerCloseAnim&) = delete;
	OnContainerCloseAnim(OnContainerCloseAnim&&) = delete;

	OnContainerCloseAnim& operator=(const OnContainerCloseAnim&) = delete;
	OnContainerCloseAnim& operator=(OnContainerCloseAnim&&) = delete;
};


namespace QuickLoot
{
	using Severity = RE::BSScript::Internal::VirtualMachine::Severity;
	using VM = RE::BSScript::Internal::VirtualMachine;


	bool RegisterFuncs(VM* a_vm);
}
