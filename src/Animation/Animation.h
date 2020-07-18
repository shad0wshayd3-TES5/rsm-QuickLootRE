#pragma once

namespace Animation
{
	enum class Type
	{
		kKillMoveStart,
		kKillMoveEnd,
		kTotal
	};

	class IEventSink
	{
	public:
		virtual ~IEventSink() = default;

		inline void operator()(Type a_type) { ProcessEvent(a_type); }

	protected:
		virtual void ProcessEvent(Type a_type) = 0;
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

	protected:
		class AnimHandler :
			public RE::IHandlerFunctor<RE::Actor, RE::BSFixedString>
		{
		private:
			using super = RE::IHandlerFunctor<RE::Actor, RE::BSFixedString>;

		public:
			AnimHandler() = delete;

			AnimHandler(RE::BSTSmartPointer<super> a_original, Type a_type) :
				super(),
				_original(std::move(a_original)),
				_type(a_type)
			{}

			bool ExecuteHandler(RE::Actor& a_handler, const RE::BSFixedString& a_parameter) override
			{
				auto manager = AnimationManager::GetSingleton();
				manager->SendEvent(_type);

				return _original ? (*_original)(a_handler, a_parameter) : true;
			}

		private:
			RE::BSTSmartPointer<super> _original;
			const Type _type;
		};

		void SendEvent(Type a_type)
		{
			if (_sink) {
				(*_sink)(a_type);
			}
		}

	private:
		struct CICompare
		{
			using value_type = std::string_view;

			[[nodiscard]] inline bool operator()(const value_type& a_lhs, const value_type& a_rhs) const
			{
				return _stricmp(a_lhs.data(), a_rhs.data()) < 0;
			}
		};

		AnimationManager() = default;
		AnimationManager(const AnimationManager&) = delete;
		AnimationManager(AnimationManager&&) = delete;

		~AnimationManager() = default;

		AnimationManager& operator=(const AnimationManager&) = delete;
		AnimationManager& operator=(AnimationManager&&) = delete;

		inline void DoInstall()
		{
			assert(EVENTS.size() == static_cast<std::size_t>(to_underlying(Type::kTotal)));

			auto handlers = RE::ResponseDictionary::GetSingleton();
			RE::BSSpinLockGuard locker(handlers->definitionLock);
			auto& definitions = handlers->objectDefinitions;

			auto it = definitions.find("PlayerCharacterResponse"sv);
			if (it != definitions.end() && it->second) {
				auto animResponse = it->second;
				for (auto i = static_cast<Type>(0); i < Type::kTotal; ++i) {
					RE::BSFixedString anim{ EVENTS[static_cast<std::size_t>(to_underlying(i))] };
					auto original = animResponse->GetHandler(anim);
					animResponse->handlerMap.insert_or_assign(
						{ std::move(anim),
							RE::make_smart<AnimHandler>(
								std::move(original), i) });
				}
			} else {
				assert(false);
			}

			logger::info("Installed {}"sv, typeid(decltype(*this)).name());
		}

		static constexpr std::array EVENTS{
			"KillMoveStart"sv,
			"KillMoveEnd"sv,
		};

		observer<IEventSink*> _sink{ nullptr };
	};
}
