#include "CLIK/Array.h"

#include <memory>


namespace CLIK
{
	Array::Array() :
		super()
	{}


	Array::Array(const Array& a_rhs) :
		super(a_rhs)
	{}


	Array::Array(Array&& a_rhs) :
		super(std::move(a_rhs))
	{}


	Array::Array(const Object& a_rhs) :
		super(a_rhs)
	{}


	Array::Array(Object&& a_rhs) :
		super(std::move(a_rhs))
	{}


	Array::Array(RE::GPtr<RE::GFxMovieView>& a_rhs) :
		super()
	{
		CreateArray(a_rhs);
	}


	Array::Array(RE::GFxMovieView* a_rhs) :
		super()
	{
		CreateArray(a_rhs);
	}


	Array::Array(const RE::GFxValue& a_rhs) :
		super(a_rhs)
	{
		assert(IsArray());
	}


	Array::Array(RE::GFxValue&& a_rhs) :
		super(std::move(a_rhs))
	{
		assert(IsArray());
	}


	Array::~Array()
	{}


	Array& Array::operator=(const Array& a_rhs)
	{
		super::operator=(a_rhs);
		return *this;
	}


	Array& Array::operator=(Array&& a_rhs)
	{
		super::operator=(std::move(a_rhs));
		return *this;
	}


	Array& Array::operator=(const Object& a_rhs)
	{
		super::operator=(a_rhs);
		return *this;
	}


	Array& Array::operator=(Object&& a_rhs)
	{
		super::operator=(std::move(a_rhs));
		return *this;
	}


	Array& Array::operator=(RE::GPtr<RE::GFxMovieView>& a_rhs)
	{
		CreateArray(a_rhs);
		return *this;
	}


	Array& Array::operator=(RE::GFxMovieView* a_rhs)
	{
		CreateArray(a_rhs);
		return *this;
	}


	Array& Array::operator=(const RE::GFxValue& a_rhs)
	{
		super::operator=(a_rhs);
		assert(IsArray());
		return *this;
	}


	Array& Array::operator=(RE::GFxValue&& a_rhs)
	{
		super::operator=(std::move(a_rhs));
		assert(IsArray());
		return *this;
	}


	double Array::Length() const
	{
		return GetNumber("length");
	}


	void Array::Length(double a_length)
	{
		SetNumber("length", a_length);
	}


	Array Array::Concat(std::optional<std::reference_wrapper<Object>> a_value)
	{
		RE::GFxValue arr;

		if (a_value) {
			enum
			{
				kValue,
				kNumArgs
			};

			RE::GFxValue args[kNumArgs];

			args[kValue] = a_value->get().GetInstance();
			assert(args[kValue].IsObject());

			[[maybe_unused]] auto success = Invoke("concat", std::addressof(arr), args, kNumArgs);
			assert(success);
		} else {
			[[maybe_unused]] auto success = Invoke("concat", std::addressof(arr), nullptr, 0);
			assert(success);
		}

		return Array(arr);
	}


	std::string_view Array::Join(std::optional<std::string_view> a_delimiter)
	{
		RE::GFxValue str;

		if (a_delimiter) {
			enum
			{
				kDelimiter,
				kNumArgs
			};

			RE::GFxValue args[kNumArgs];

			args[kDelimiter] = *a_delimiter;
			assert(args[kDelimiter].IsString());

			[[maybe_unused]] auto success = Invoke("join", std::addressof(str), args, kNumArgs);
			assert(success);
		} else {
			[[maybe_unused]] auto success = Invoke("join", std::addressof(str), nullptr, 0);
			assert(success);
		}

		return str.GetString();
	}


	Object Array::Pop()
	{
		RE::GFxValue object;
		[[maybe_unused]] auto success = Invoke("pop", std::addressof(object));
		assert(success);
		return Object(object);
	}


	double Array::Push(Object& a_value)
	{
		enum
		{
			kValue,
			kNumArgs
		};

		RE::GFxValue args[kNumArgs];

		args[kValue] = a_value.GetInstance();

		RE::GFxValue number;
		[[maybe_unused]] auto success = Invoke("push", std::addressof(number), args, kNumArgs);
		assert(success);

		return number.GetNumber();
	}


	void Array::Reverse()
	{
		[[maybe_unused]] auto success = Invoke("reverse");
		assert(success);
	}


	Object Array::Shift()
	{
		RE::GFxValue object;
		[[maybe_unused]] auto success = Invoke("shift", std::addressof(object));
		assert(success);
		return Object(object);
	}
}
