#pragma once

#include "CLIK/Object.h"

namespace CLIK
{
	class Array :
		public Object
	{
	private:
		using super = Object;

	public:
		Array() = default;
		Array(const Array&) = default;
		Array(Array&&) = default;
		using super::super;

		inline Array(const super& a_rhs) :
			super(a_rhs)
		{}

		inline Array(super&& a_rhs) :
			super(std::move(a_rhs))
		{}

		inline Array(const RE::GFxValue& a_rhs) :
			super(a_rhs)
		{
			assert(IsArray());
		}

		inline Array(RE::GFxValue&& a_rhs) :
			super(std::move(a_rhs))
		{
			assert(IsArray());
		}

		inline Array(RE::GPtr<RE::GFxMovieView> a_rhs) :
			super()
		{
			CreateArray(std::move(a_rhs));
		}

		~Array() = default;

		Array& operator=(const Array&) = default;
		Array& operator=(Array&&) = default;
		using super::operator=;

		inline Array& operator=(const super& a_rhs)
		{
			super::operator=(a_rhs);
			return *this;
		}

		inline Array& operator=(super&& a_rhs)
		{
			super::operator=(std::move(a_rhs));
			return *this;
		}

		inline Array& operator=(const RE::GFxValue& a_rhs)
		{
			super::operator=(a_rhs);
			assert(IsArray());
			return *this;
		}

		inline Array& operator=(RE::GFxValue&& a_rhs)
		{
			super::operator=(std::move(a_rhs));
			assert(IsArray());
			return *this;
		}

		inline Array& operator=(RE::GPtr<RE::GFxMovieView> a_rhs)
		{
			CreateArray(std::move(a_rhs));
			return *this;
		}

		// properties
		static constexpr double CASEINSENSITIVE = 1;
		static constexpr double DESCENDING = 2;
		static constexpr double NUMERIC = 16;
		static constexpr double RETURNINDEXEDARRAY = 8;
		static constexpr double UNIQUESORT = 4;

		inline double Length() const { return GetNumber("length"); }
		inline void Length(double a_length) { SetNumber("length", a_length); }

		// methods
		inline Array Concat(std::optional<std::reference_wrapper<Object>> a_value)
		{
			RE::GFxValue arr;

			if (a_value) {
				enum
				{
					kValue,
					kNumArgs
				};

				std::array<RE::GFxValue, kNumArgs> args;

				args[kValue] = a_value->get().GetInstance();
				assert(args[kValue].IsObject());

				[[maybe_unused]] const auto success =
					Invoke("concat", std::addressof(arr), args.data(), args.size());
				assert(success);
			} else {
				[[maybe_unused]] const auto success =
					Invoke("concat", std::addressof(arr), nullptr, 0);
				assert(success);
			}

			return Array(arr);
		}

		inline std::string_view Join(std::optional<std::string_view> a_delimiter)
		{
			RE::GFxValue str;

			if (a_delimiter) {
				enum
				{
					kDelimiter,
					kNumArgs
				};

				std::array<RE::GFxValue, kNumArgs> args;

				args[kDelimiter] = *a_delimiter;
				assert(args[kDelimiter].IsString());

				[[maybe_unused]] const auto success =
					Invoke("join", std::addressof(str), args.data(), args.size());
				assert(success);
			} else {
				[[maybe_unused]] const auto success =
					Invoke("join", std::addressof(str), nullptr, 0);
				assert(success);
			}

			return str.GetString();
		}

		inline Object Pop()
		{
			RE::GFxValue object;
			[[maybe_unused]] const auto success =
				Invoke("pop", std::addressof(object));
			assert(success);
			return Object(object);
		}

		inline double Push(Object& a_value)
		{
			enum
			{
				kValue,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kValue] = a_value.GetInstance();

			RE::GFxValue number;
			[[maybe_unused]] const auto success =
				Invoke("push", std::addressof(number), args.data(), args.size());
			assert(success);

			return number.GetNumber();
		}

		inline void Reverse()
		{
			[[maybe_unused]] const auto success =
				Invoke("reverse");
			assert(success);
		}

		inline Object Shift()
		{
			RE::GFxValue object;
			[[maybe_unused]] const auto success =
				Invoke("shift", std::addressof(object));
			assert(success);
			return Object(object);
		}
	};
}
