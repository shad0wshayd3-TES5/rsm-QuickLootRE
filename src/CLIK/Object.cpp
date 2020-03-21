#include "CLIK/Object.h"

#include <memory>


namespace CLIK
{
	Object::Object() :
		_instance()
	{}


	Object::Object(const Object& a_rhs) :
		_instance()
	{
		SetInstance(a_rhs);
	}


	Object::Object(Object&& a_rhs) :
		_instance()
	{
		SetInstance(std::move(a_rhs));
	}


	Object::Object([[maybe_unused]] std::nullptr_t) :
		_instance()
	{
		SetInstance(nullptr);
	}


	Object::Object(double a_rhs) :
		_instance()
	{
		SetInstance(a_rhs);
	}


	Object::Object(bool a_rhs) :
		_instance()
	{
		SetInstance(a_rhs);
	}


	Object::Object(const char* a_rhs) :
		_instance()
	{
		SetInstance(a_rhs);
	}


	Object::Object(std::string_view a_rhs) :
		_instance()
	{
		SetInstance(a_rhs);
	}


	Object::Object(const wchar_t* a_rhs) :
		_instance()
	{
		SetInstance(a_rhs);
	}


	Object::Object(std::wstring_view a_rhs) :
		_instance()
	{
		SetInstance(a_rhs);
	}


	Object::Object(const RE::GFxValue& a_rhs) :
		_instance()
	{
		SetInstance(a_rhs);
		assert(IsObject());
	}


	Object::Object(RE::GFxValue&& a_rhs) :
		_instance()
	{
		SetInstance(std::move(a_rhs));
		assert(IsObject());
	}


	Object::~Object()
	{}


	Object& Object::operator=(const Object& a_rhs)
	{
		SetInstance(a_rhs);
		return *this;
	}


	Object& Object::operator=(Object&& a_rhs)
	{
		SetInstance(std::move(a_rhs));
		return *this;
	}


	Object& Object::operator=([[maybe_unused]] std::nullptr_t)
	{
		SetInstance(nullptr);
		return *this;
	}


	Object& Object::operator=(double a_rhs)
	{
		SetInstance(a_rhs);
		return *this;
	}


	Object& Object::operator=(bool a_rhs)
	{
		SetInstance(a_rhs);
		return *this;
	}


	Object& Object::operator=(const char* a_rhs)
	{
		SetInstance(a_rhs);
		return *this;
	}


	Object& Object::operator=(std::string_view a_rhs)
	{
		SetInstance(a_rhs);
		return *this;
	}


	Object& Object::operator=(const wchar_t* a_rhs)
	{
		SetInstance(a_rhs);
		return *this;
	}


	Object& Object::operator=(std::wstring_view a_rhs)
	{
		SetInstance(a_rhs);
		return *this;
	}


	Object& Object::operator=(const RE::GFxValue& a_rhs)
	{
		SetInstance(a_rhs);
		assert(IsObject());
		return *this;
	}


	Object& Object::operator=(RE::GFxValue&& a_rhs)
	{
		SetInstance(std::move(a_rhs));
		assert(IsObject());
		return *this;
	}


	RE::GFxValue& Object::GetInstance()
	{
		return _instance;
	}


	Object Object::Constructor() const
	{
		return GetObject("constructor");
	}


	Object Object::Prototype() const
	{
		return GetObject("__proto__");
	}


	Object Object::Resolve() const
	{
		return GetObject("__resolve");
	}


	void Object::Resolve(Object& a_resolve)
	{
		SetObject("__resolve", a_resolve);
	}


	bool Object::HasOwnProperty(std::string_view a_name)
	{
		enum
		{
			kName,
			kNumArgs
		};

		RE::GFxValue args[kNumArgs];

		args[kName] = a_name;
		assert(args[kName].IsString());

		RE::GFxValue boolean;
		[[maybe_unused]] auto success = Invoke("hasOwnProperty", std::addressof(boolean), args, kNumArgs);
		assert(success);

		return boolean.GetBool();
	}


	bool Object::IsPropertyEnumerable(std::string_view a_name)
	{
		enum
		{
			kName,
			kNumArgs
		};

		RE::GFxValue args[kNumArgs];

		args[kName] = a_name;
		assert(args[kName].IsString());

		RE::GFxValue boolean;
		[[maybe_unused]] auto success = Invoke("isPropertyEnumerable", std::addressof(boolean), args, kNumArgs);
		assert(success);

		return boolean.GetBool();
	}


	bool Object::IsPrototypeOf(Object& a_theClass)
	{
		enum
		{
			kTheClass,
			kNumArgs
		};

		RE::GFxValue args[kNumArgs];

		args[kTheClass] = a_theClass._instance;
		assert(args[kTheClass].IsObject());

		RE::GFxValue boolean;
		[[maybe_unused]] auto success = Invoke("isPrototypeOf", std::addressof(boolean), args, kNumArgs);
		assert(success);

		return boolean.GetBool();
	}


	bool Object::Unwatch(std::string_view a_name)
	{
		enum
		{
			kName,
			kNumArgs
		};

		RE::GFxValue args[kNumArgs];

		args[kName] = a_name;
		assert(args[kName].IsString());

		RE::GFxValue boolean;
		[[maybe_unused]] auto success = Invoke("unwatch", std::addressof(boolean), args, kNumArgs);
		assert(success);

		return boolean.GetBool();
	}


	std::string Object::ToString()
	{
		RE::GFxValue str;
		[[maybe_unused]] auto success = Invoke("toString", std::addressof(str));
		assert(success);
		return str.GetString();
	}


	Object Object::ValueOf()
	{
		RE::GFxValue object;
		[[maybe_unused]] auto success = Invoke("valueOf", std::addressof(object));
		assert(success);
		return Object(object);
	}


	bool Object::IsArray() const
	{
		return _instance.IsArray();
	}


	bool Object::IsObject() const
	{
		return _instance.IsObject();
	}


	bool Object::Invoke(const char* a_name)
	{
		return _instance.Invoke(a_name, nullptr, nullptr, 0);
	}


	bool Object::Invoke(const char* a_name, RE::GFxValue* a_result)
	{
		return _instance.Invoke(a_name, a_result, nullptr, 0);
	}


	bool Object::Invoke(const char* a_name, RE::GFxValue* a_result, const RE::GFxValue* a_args, RE::UPInt a_numArgs)
	{
		return _instance.Invoke(a_name, a_result, a_args, a_numArgs);
	}


	bool Object::GetBoolean(const char* a_path) const
	{
		RE::GFxValue boolean;
		[[maybe_unused]] auto success = _instance.GetMember(a_path, &boolean);
		assert(success);
		return boolean.GetBool();
	}


	double Object::GetNumber(const char* a_path) const
	{
		RE::GFxValue number;
		[[maybe_unused]] auto success = _instance.GetMember(a_path, &number);
		assert(success);
		return number.GetNumber();
	}


	Object Object::GetObject(const char* a_path) const
	{
		RE::GFxValue object;
		[[maybe_unused]] auto success = _instance.GetMember(a_path, &object);
		assert(success);
		return Object(object);
	}


	std::string_view Object::GetString(const char* a_path) const
	{
		RE::GFxValue str;
		[[maybe_unused]] auto success = _instance.GetMember(a_path, &str);
		assert(success);
		return str.GetString();
	}


	void Object::CreateArray(RE::GPtr<RE::GFxMovieView>& a_view)
	{
		a_view->CreateArray(std::addressof(_instance));
		assert(IsArray());
	}


	void Object::CreateArray(RE::GFxMovieView* a_view)
	{
		a_view->CreateArray(std::addressof(_instance));
		assert(IsArray());
	}


	void Object::SetBoolean(const char* a_path, bool a_boolean)
	{
		RE::GFxValue boolean(a_boolean);
		[[maybe_unused]] auto success = _instance.SetMember(a_path, boolean);
		assert(success);
	}


	void Object::SetNumber(const char* a_path, double a_number)
	{
		RE::GFxValue number(a_number);
		[[maybe_unused]] auto success = _instance.SetMember(a_path, number);
		assert(success);
	}


	void Object::SetObject(const char* a_path, const Object& a_object)
	{
		[[maybe_unused]] auto success = _instance.SetMember(a_path, a_object._instance);
		assert(success);
	}


	void Object::SetString(const char* a_path, std::string_view a_string)
	{
		RE::GFxValue str(a_string);
		[[maybe_unused]] auto success = _instance.SetMember(a_path, str);
		assert(success);
	}


	void Object::SetInstance([[maybe_unused]] std::nullptr_t)
	{
		_instance = nullptr;
	}


	void Object::SetInstance(double a_val)
	{
		_instance = a_val;
	}


	void Object::SetInstance(bool a_val)
	{
		_instance = a_val;
	}


	void Object::SetInstance(const char* a_val)
	{
		_instance = a_val;
	}


	void Object::SetInstance(std::string_view a_val)
	{
		_instance = a_val;
	}


	void Object::SetInstance(const wchar_t* a_val)
	{
		_instance = a_val;
	}


	void Object::SetInstance(std::wstring_view a_val)
	{
		_instance = a_val;
	}


	void Object::SetInstance(const RE::GFxValue& a_val)
	{
		_instance = a_val;
	}


	void Object::SetInstance(RE::GFxValue&& a_val)
	{
		_instance = std::move(a_val);
	}


	void Object::SetInstance(const Object& a_val)
	{
		if (this != std::addressof(a_val)) {
			_instance = a_val._instance;
		}
	}


	void Object::SetInstance(Object&& a_val)
	{
		if (this != std::addressof(a_val)) {
			_instance = std::move(a_val._instance);
		}
	}
}
