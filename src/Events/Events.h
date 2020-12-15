#pragma once

namespace Events
{
	class CrosshairRefManager :
		public RE::BSTEventSink<SKSE::CrosshairRefEvent>,
		public RE::BSTEventSink<RE::TESLockChangedEvent>
	{
	public:
		[[nodiscard]] static CrosshairRefManager* GetSingleton()
		{
			static CrosshairRefManager singleton;
			return std::addressof(singleton);
		}

		static void Register()
		{
			auto crosshair = SKSE::GetCrosshairRefEventSource();
			if (crosshair) {
				crosshair->AddEventSink(GetSingleton());
				logger::info("Registered {}"sv, typeid(SKSE::CrosshairRefEvent).name());
			}

			auto scripts = RE::ScriptEventSourceHolder::GetSingleton();
			if (scripts) {
				scripts->AddEventSink<RE::TESLockChangedEvent>(GetSingleton());
				logger::info("Registered {}"sv, typeid(RE::TESLockChangedEvent).name());
			}
		}

	protected:
		friend class LifeStateManager;

		using EventResult = RE::BSEventNotifyControl;

		EventResult ProcessEvent(const SKSE::CrosshairRefEvent* a_event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*) override
		{
			auto crosshairRef =
				a_event && a_event->crosshairRef ?
                    a_event->crosshairRef->CreateRefHandle() :
                    RE::ObjectRefHandle();
			if (_cachedRef == crosshairRef) {
				return EventResult::kContinue;
			}

			_cachedRef = crosshairRef;
			_cachedAshPile.reset();
			Evaluate(a_event->crosshairRef);

			return EventResult::kContinue;
		}

		EventResult ProcessEvent(const RE::TESLockChangedEvent* a_event, RE::BSTEventSource<RE::TESLockChangedEvent>*) override
		{
			if (a_event &&
				a_event->lockedObject &&
				a_event->lockedObject->GetHandle() == _cachedRef) {
				Evaluate(a_event->lockedObject);
			}

			return EventResult::kContinue;
		}

		void OnLifeStateChanged(RE::Actor& a_actor)
		{
			if (a_actor.GetHandle() == _cachedRef) {
				Evaluate(RE::TESObjectREFRPtr{ std::addressof(a_actor) });
			}
		}

	private:
		CrosshairRefManager() = default;
		CrosshairRefManager(const CrosshairRefManager&) = delete;
		CrosshairRefManager(CrosshairRefManager&&) = delete;

		~CrosshairRefManager() = default;

		CrosshairRefManager& operator=(const CrosshairRefManager&) = delete;
		CrosshairRefManager& operator=(CrosshairRefManager&&) = delete;

		void Evaluate(RE::TESObjectREFRPtr a_ref);

		[[nodiscard]] bool CanOpen(RE::TESObjectREFRPtr a_ref)
		{
			auto obj = a_ref ? a_ref->GetObjectReference() : nullptr;
			if (!a_ref || !obj) {
				return false;
			}

			if (obj->Is(RE::FormType::Activator)) {
				_cachedAshPile = a_ref->extraList.GetAshPileRef();
				return CanOpen(_cachedAshPile.get());
			}

			if (auto actor = a_ref->As<RE::Actor>(); actor) {
				if (!actor->IsDead() ||
					actor->IsSummoned()) {
					return false;
				}
			}

			return a_ref->HasContainer();
		}

		RE::ObjectRefHandle _cachedRef;
		RE::ObjectRefHandle _cachedAshPile;
	};

	class CombatManager :
		public RE::BSTEventSink<RE::TESCombatEvent>
	{
	public:
		static CombatManager* GetSingleton()
		{
			static CombatManager singleton;
			return std::addressof(singleton);
		}

		static void Register()
		{
			auto scripts = RE::ScriptEventSourceHolder::GetSingleton();
			if (scripts) {
				scripts->AddEventSink(GetSingleton());
				logger::info("Registered {}"sv, typeid(CombatManager).name());
			}
		}

	protected:
		using EventResult = RE::BSEventNotifyControl;

		EventResult ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>*) override
		{
			using CombatState = RE::ACTOR_COMBAT_STATE;

			const auto isPlayerRef = [](auto&& a_ref) {
				return a_ref && a_ref->IsPlayerRef();
			};

			if (a_event && (isPlayerRef(a_event->actor) || isPlayerRef(a_event->targetActor))) {
				switch (*a_event->newState) {
				case CombatState::kCombat:
				case CombatState::kSearching:
					Close();
					break;
				default:
					break;
				}
			}

			return EventResult::kContinue;
		}

	private:
		CombatManager() = default;
		CombatManager(const CombatManager&) = delete;
		CombatManager(CombatManager&&) = delete;

		~CombatManager() = default;

		CombatManager& operator=(const CombatManager&) = delete;
		CombatManager& operator=(CombatManager&&) = delete;

		void Close();
	};

	class LifeStateManager
	{
	public:
		static void Register()
		{
			struct Patch :
				Xbyak::CodeGenerator
			{
				explicit Patch(std::uintptr_t a_target)
				{
					mov(rcx, rsi);  // rsi == Actor* this

					pop(r15);
					pop(r14);
					pop(r12);
					pop(rdi);
					pop(rsi);
					pop(rbx);
					pop(rbp);

					mov(rax, a_target);
					jmp(rax);
				}
			};

			constexpr std::size_t begin = 0x503;
			constexpr std::size_t end = 0x50D;
			constexpr std::size_t size = end - begin;
			static_assert(size >= 6);

			REL::Relocation<std::uintptr_t> target{ REL::ID(36604), begin };
			REL::safe_fill(target.address(), REL::INT3, size);

			auto& trampoline = SKSE::GetTrampoline();
			Patch p{ reinterpret_cast<std::uintptr_t>(OnLifeStateChanged) };
			trampoline.write_branch<6>(
				target.address(),
				trampoline.allocate(p));

			logger::info("Registered {}"sv, typeid(LifeStateManager).name());
		}

	private:
		static void OnLifeStateChanged(RE::Actor* a_actor)
		{
			const auto manager = CrosshairRefManager::GetSingleton();
			manager->OnLifeStateChanged(*a_actor);
		}
	};

	void Register()
	{
		CrosshairRefManager::Register();
		LifeStateManager::Register();

		if (*Settings::closeInCombat) {
			CombatManager::Register();
		}

		logger::info("Registered all event handlers"sv);
	}
}
