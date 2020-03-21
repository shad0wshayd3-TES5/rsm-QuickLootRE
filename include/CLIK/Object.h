#pragma once

#include <cassert>
#include <cstddef>
#include <optional>
#include <string_view>

#include "RE/Skyrim.h"


namespace CLIK
{
	class Object
	{
	public:
		Object();
		Object(const Object& a_rhs);
		Object(Object&& a_rhs);
		explicit Object(std::nullptr_t);
		explicit Object(double a_rhs);
		explicit Object(bool a_rhs);
		explicit Object(const char* a_rhs);
		explicit Object(std::string_view a_rhs);
		explicit Object(const wchar_t* a_rhs);
		explicit Object(std::wstring_view a_rhs);
		explicit Object(const RE::GFxValue& a_rhs);
		explicit Object(RE::GFxValue&& a_rhs);
		~Object();

		Object& operator=(const Object& a_rhs);
		Object& operator=(Object&& a_rhs);
		Object& operator=(std::nullptr_t);
		Object& operator=(double a_rhs);
		Object& operator=(bool a_rhs);
		Object& operator=(const char* a_rhs);
		Object& operator=(std::string_view a_rhs);
		Object& operator=(const wchar_t* a_rhs);
		Object& operator=(std::wstring_view a_rhs);
		Object& operator=(const RE::GFxValue& a_rhs);
		Object& operator=(RE::GFxValue&& a_rhs);

		RE::GFxValue& GetInstance();

		// properties
		Object Constructor() const;

		Object Prototype() const;

		Object Resolve() const;
		void Resolve(Object& a_resolve);

		// methods
		//bool AddProperty(std::string_view a_name, Function& a_getter, Function& a_setter);
		bool HasOwnProperty(std::string_view a_name);
		bool IsPropertyEnumerable(std::string_view a_name);

		bool IsPrototypeOf(Object& a_theClass);

		//bool Watch(std::string_view a_name, Function& a_callback, Object& a_userData);
		bool Unwatch(std::string_view a_name);

		std::string ToString();
		Object ValueOf();

	protected:
		bool IsArray() const;
		bool IsObject() const;

		bool Invoke(const char* a_name);
		bool Invoke(const char* a_name, RE::GFxValue* a_result);
		bool Invoke(const char* a_name, RE::GFxValue* a_result, const RE::GFxValue* a_args, RE::UPInt a_numArgs);

		bool GetBoolean(const char* a_path) const;
		double GetNumber(const char* a_path) const;
		Object GetObject(const char* a_path) const;
		std::string_view GetString(const char* a_path) const;

		void CreateArray(RE::GPtr<RE::GFxMovieView>& a_view);
		void CreateArray(RE::GFxMovieView* a_view);

		void SetBoolean(const char* a_path, bool a_boolean);
		void SetNumber(const char* a_path, double a_number);
		void SetObject(const char* a_path, const Object& a_object);
		void SetString(const char* a_path, std::string_view a_string);

		void SetInstance(std::nullptr_t);
		void SetInstance(double a_val);
		void SetInstance(bool a_val);
		void SetInstance(const char* a_val);
		void SetInstance(std::string_view a_val);
		void SetInstance(const wchar_t* a_val);
		void SetInstance(std::wstring_view a_val);
		void SetInstance(const RE::GFxValue& a_val);
		void SetInstance(RE::GFxValue&& a_val);

	private:
		void SetInstance(const Object& a_val);
		void SetInstance(Object&& a_val);


		RE::GFxValue _instance;
	};
}
