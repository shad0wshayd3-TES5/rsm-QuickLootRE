#pragma once

namespace Animation
{
	class IEventSink
	{
	public:
		virtual ~IEventSink() = default;

		inline void operator()(bool a_animating) { OnAnimationChange(a_animating); }

	protected:
		virtual void OnAnimationChange(bool a_animating) = 0;
	};

	class AnimationManager
	{
	public:
		static inline AnimationManager* GetSingleton()
		{
			static AnimationManager singleton;
			return std::addressof(singleton);
		}

		static inline void Install()
		{
			auto singleton = GetSingleton();
			singleton->DoInstall();
		}

		constexpr void SetEventSink(observer<IEventSink*> a_sink) noexcept { _sink = a_sink; }

		[[nodiscard]] inline bool IsAnimating() const noexcept { return _activeAnimations.any(); }

	protected:
		class AnimHandler :
			public RE::IHandlerFunctor<RE::Actor, RE::BSFixedString>
		{
		private:
			using super = RE::IHandlerFunctor<RE::Actor, RE::BSFixedString>;

		public:
			AnimHandler() = delete;

			inline AnimHandler(RE::BSTSmartPointer<super> a_original, std::size_t a_idx, bool a_active) :
				super(),
				_original(std::move(a_original)),
				_idx(a_idx),
				_active(a_active)
			{}

			inline bool ExecuteHandler(RE::Actor& a_handler, const RE::BSFixedString& a_parameter) override
			{
				auto manager = AnimationManager::GetSingleton();
				manager->OnAnimationChange(_idx, _active);

				return _original ? (*_original)(a_handler, a_parameter) : true;
			}

		private:
			RE::BSTSmartPointer<super> _original;
			const std::size_t _idx;
			const bool _active;
		};

		inline void OnAnimationChange(std::size_t a_idx, bool a_active)
		{
			_activeAnimations.set(a_idx, a_active);
			if (_sink) {
				(*_sink)(_activeAnimations.any());
			}
		}

	private:
		enum : std::size_t
		{
			kKillMove,
			kAnimatedCamera,
			kTotal
		};

		AnimationManager() = default;
		AnimationManager(const AnimationManager&) = delete;
		AnimationManager(AnimationManager&&) = delete;

		~AnimationManager() = default;

		AnimationManager& operator=(const AnimationManager&) = delete;
		AnimationManager& operator=(AnimationManager&&) = delete;

		inline void DoInstall()
		{
			assert(EVENTS.size() == kTotal);

			auto handlers = RE::ResponseDictionary::GetSingleton();
			RE::BSSpinLockGuard locker(handlers->definitionLock);
			auto& definitions = handlers->objectDefinitions;

			auto it = definitions.find("PlayerCharacterResponse"sv);
			if (it != definitions.end() && it->second) {
				auto animResponse = it->second;
				for (std::size_t i = 0; i < kTotal; ++i) {
					InjectHandler(*animResponse, EVENTS[i].first, i, true);
					InjectHandler(*animResponse, EVENTS[i].second, i, false);
				}
			} else {
				assert(false);
			}

			logger::info("Installed {}"sv, typeid(decltype(*this)).name());
		}

		inline void InjectHandler(RE::AnimResponse& a_response, std::string_view a_animation, std::size_t a_idx, bool a_active)
		{
			const RE::BSFixedString anim(a_animation);
			auto original = a_response.GetHandler(anim);
			a_response.handlerMap.insert_or_assign(
				{ std::move(anim),
					RE::make_smart<AnimHandler>(
						std::move(original), a_idx, a_active) });
		}

		static constexpr std::array EVENTS{
			std::make_pair("KillMoveStart"sv, "KillMoveEnd"sv),
			std::make_pair("StartAnimatedCamera"sv, "EndAnimatedCamera"sv)
		};

		std::bitset<kTotal> _activeAnimations;
		observer<IEventSink*> _sink{ nullptr };
	};
}
