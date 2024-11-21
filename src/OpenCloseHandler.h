#pragma once

class OpenCloseHandler
{
public:
	OpenCloseHandler() = default;
	OpenCloseHandler(const OpenCloseHandler&) = default;
	OpenCloseHandler(OpenCloseHandler&&) = default;

	OpenCloseHandler(RE::ActorHandle a_dst) :
		OpenCloseHandler(a_dst, {})
	{}

	OpenCloseHandler(RE::ActorHandle a_dst, RE::ObjectRefHandle a_src) :
		_dst(a_dst),
		_src(a_src)
	{}

	~OpenCloseHandler() { Close(); }

	OpenCloseHandler& operator=(const OpenCloseHandler&) = default;
	OpenCloseHandler& operator=(OpenCloseHandler&&) = default;

	void SetSource(RE::ObjectRefHandle a_src)
	{
		Close();
		_src = a_src;
		_doOneShotOpen = true;
		_doOneShotClose = true;
	}

	void Open()
	{
		auto src = _src.get();
		if (src)
		{
			OneShotOpen(src);
			const auto state = RE::BGSOpenCloseForm::GetOpenState(src.get());
			switch (state)
			{
			case State::kClosed:
				RE::BGSOpenCloseForm::SetOpenState(src.get(), true, false);
				break;
			case State::kClosing:
				AsyncPlayAnimation(*src, true);
				break;
			default:
				break;
			}
		}
	}

	void Close()
	{
		auto src = _src.get();
		if (src)
		{
			OneShotClose(src);
			const auto state = RE::BGSOpenCloseForm::GetOpenState(src.get());
			switch (state)
			{
			case State::kOpen:
				RE::BGSOpenCloseForm::SetOpenState(src.get(), false, false);
				break;
			case State::kOpening:
				AsyncPlayAnimation(*src, false);
				break;
			default:
				break;
			}
		}
	}

private:
	using State = RE::BGSOpenCloseForm::OPEN_STATE;

	static void AsyncCallback(long double a_wait, RE::ObjectRefHandle a_src, bool a_open)
	{
		std::this_thread::sleep_for(std::chrono::duration<long double>(a_wait));
		auto task = SKSE::GetTaskInterface();
		task->AddTask([=]()
		              {
			const auto src = a_src.get();
			if (src) {
				const auto sequence =
					a_open ?
                        src->GetSequence("Open"sv) :
                        src->GetSequence("Close"sv);

				if (sequence && !sequence->Animating()) {
					RE::BGSOpenCloseForm::SetOpenState(src.get(), a_open, false);
				}
			} });
	}

	void OneShotOpen(const RE::TESObjectREFRPtr& a_src)
	{
		if (_doOneShotOpen)
		{
			auto dst = _dst.get();
			if (dst)
			{
				a_src->InitChildActivates(dst.get());

				auto events = RE::ScriptEventSourceHolder::GetSingleton();
				if (events)
				{
					events->SendActivateEvent(a_src, dst);
					events->SendOpenCloseEvent(a_src, dst, true);
				}

				RE::ChestsLooted::SendEvent();
			}

			_doOneShotOpen = false;
		}
	}

	void OneShotClose(const RE::TESObjectREFRPtr& a_src)
	{
		if (_doOneShotClose)
		{
			auto events = RE::ScriptEventSourceHolder::GetSingleton();
			auto dst = _dst.get();
			if (events && dst)
			{
				events->SendOpenCloseEvent(a_src, dst, false);
			}

			_doOneShotClose = false;
		}
	}

	void AsyncPlayAnimation(const RE::TESObjectREFR& a_src, bool a_open)
	{
		const auto sequence =
			!a_open ? a_src.GetSequence("Open"sv) : a_src.GetSequence("Close"sv);
		const long double duration = sequence ? sequence->endKeyTime : 0.5;
		std::thread t(AsyncCallback, duration + 0.1, _src, a_open);
		t.detach();
	}

	RE::ActorHandle _dst;
	RE::ObjectRefHandle _src;
	bool _doOneShotOpen{ true };
	bool _doOneShotClose{ true };
};
