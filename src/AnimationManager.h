#pragma once

class AnimationManager
{
public:
	static inline void Install()
	{
		auto handlers = RE::ResponseDictionary::GetSingleton();
		RE::BSSpinLockGuard locker(handlers->definitionLock);
		auto& definitions = handlers->objectDefinitions;
		auto it = definitions.find("PlayerCharacterResponse");
		if (it != definitions.end() && it->second) {
			constexpr std::array EVENTS = {
				std::string_view{ "StartAnimationDriven" }
			};

			auto animResponse = it->second;
			for (auto& event : EVENTS) {
				RE::BSFixedString anim{ event };
				auto original = animResponse->GetHandler(anim);
				animResponse->handlerMap.insert_or_assign(
					{ std::move(anim), RE::make_smart<AnimHandler>(std::move(original)) });
			}
		}

		_MESSAGE("Installed %s", typeid(AnimationManager).name());
	}

private:
	class AnimHandler :
		public RE::IHandlerFunctor<RE::Actor, RE::BSFixedString>
	{
	private:
		using super = RE::IHandlerFunctor<RE::Actor, RE::BSFixedString>;

	public:
		AnimHandler() = delete;

		AnimHandler(RE::BSTSmartPointer<super> a_original) :
			super(),
			_original(std::move(a_original))
		{}

		AnimHandler(const AnimHandler&) = default;
		AnimHandler(AnimHandler&&) = default;

		~AnimHandler() = default;

		AnimHandler& operator=(const AnimHandler&) = default;
		AnimHandler& operator=(AnimHandler&&) = default;

		bool ExecuteHandler(RE::Actor& a_handler, const RE::BSFixedString& a_parameter) override;

	private:
		RE::BSTSmartPointer<super> _original;
	};

	AnimationManager() = delete;
	AnimationManager(const AnimationManager&) = delete;
	AnimationManager(AnimationManager&&) = delete;

	~AnimationManager() = delete;

	AnimationManager& operator=(const AnimationManager&) = delete;
	AnimationManager& operator=(AnimationManager&&) = delete;
};
