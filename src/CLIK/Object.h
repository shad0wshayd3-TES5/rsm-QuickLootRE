#pragma once

namespace CLIK
{
	class Object
	{
	public:
		inline Object() :
			_instance()
		{}

		inline Object(const Object& a_rhs) :
			_instance()
		{
			SetInstance(a_rhs);
		}

		inline Object(Object&& a_rhs) :
			_instance()
		{
			SetInstance(std::move(a_rhs));
		}

		explicit inline Object(std::nullptr_t) :
			_instance()
		{
			SetInstance(nullptr);
		}

		explicit inline Object(double a_rhs) :
			_instance()
		{
			SetInstance(a_rhs);
		}

		explicit inline Object(bool a_rhs) :
			_instance()
		{
			SetInstance(a_rhs);
		}

		explicit inline Object(const char* a_rhs) :
			_instance()
		{
			SetInstance(a_rhs);
		}

		explicit inline Object(std::string_view a_rhs) :
			_instance()
		{
			SetInstance(a_rhs);
		}

		explicit inline Object(const wchar_t* a_rhs) :
			_instance()
		{
			SetInstance(a_rhs);
		}

		explicit inline Object(std::wstring_view a_rhs) :
			_instance()
		{
			SetInstance(a_rhs);
		}

		explicit inline Object(const RE::GFxValue& a_rhs) :
			_instance()
		{
			SetInstance(a_rhs);
			assert(IsObject());
		}

		explicit inline Object(RE::GFxValue&& a_rhs) :
			_instance()
		{
			SetInstance(std::move(a_rhs));
			assert(IsObject());
		}

		~Object() = default;

		inline Object& operator=(const Object& a_rhs)
		{
			SetInstance(a_rhs);
			return *this;
		}

		inline Object& operator=(Object&& a_rhs)
		{
			SetInstance(std::move(a_rhs));
			return *this;
		}

		inline Object& operator=(std::nullptr_t)
		{
			SetInstance(nullptr);
			return *this;
		}

		inline Object& operator=(double a_rhs)
		{
			SetInstance(a_rhs);
			return *this;
		}

		inline Object& operator=(bool a_rhs)
		{
			SetInstance(a_rhs);
			return *this;
		}

		inline Object& operator=(const char* a_rhs)
		{
			SetInstance(a_rhs);
			return *this;
		}

		inline Object& operator=(std::string_view a_rhs)
		{
			SetInstance(a_rhs);
			return *this;
		}

		inline Object& operator=(const wchar_t* a_rhs)
		{
			SetInstance(a_rhs);
			return *this;
		}

		inline Object& operator=(std::wstring_view a_rhs)
		{
			SetInstance(a_rhs);
			return *this;
		}

		inline Object& operator=(const RE::GFxValue& a_rhs)
		{
			SetInstance(a_rhs);
			assert(IsObject());
			return *this;
		}

		inline Object& operator=(RE::GFxValue&& a_rhs)
		{
			SetInstance(std::move(a_rhs));
			assert(IsObject());
			return *this;
		}

		inline RE::GFxValue& GetInstance() { return _instance; }

		// properties
		inline Object Constructor() const { return GetObject("constructor"); }

		inline Object Prototype() const { return GetObject("__proto__"); }

		inline Object Resolve() const { return GetObject("__resolve"); }
		inline void Resolve(Object& a_resolve) { SetObject("__resolve", a_resolve); }

		// methods
		//bool AddProperty(std::string_view a_name, Function& a_getter, Function& a_setter);
		inline bool HasOwnProperty(std::string_view a_name)
		{
			enum
			{
				kName,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kName] = a_name;
			assert(args[kName].IsString());

			RE::GFxValue boolean;
			[[maybe_unused]] const auto success =
				Invoke("hasOwnProperty", std::addressof(boolean), args.data(), args.size());
			assert(success);

			return boolean.GetBool();
		}

		inline bool IsPropertyEnumerable(std::string_view a_name)
		{
			enum
			{
				kName,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kName] = a_name;
			assert(args[kName].IsString());

			RE::GFxValue boolean;
			[[maybe_unused]] const auto success =
				Invoke("isPropertyEnumerable", std::addressof(boolean), args.data(), args.size());
			assert(success);

			return boolean.GetBool();
		}

		inline bool IsPrototypeOf(Object& a_theClass)
		{
			enum
			{
				kTheClass,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kTheClass] = a_theClass._instance;
			assert(args[kTheClass].IsObject());

			RE::GFxValue boolean;
			[[maybe_unused]] const auto success =
				Invoke("isPrototypeOf", std::addressof(boolean), args.data(), args.size());
			assert(success);

			return boolean.GetBool();
		}

		//bool Watch(std::string_view a_name, Function& a_callback, Object& a_userData);
		inline bool Unwatch(std::string_view a_name)
		{
			enum
			{
				kName,
				kNumArgs
			};

			std::array<RE::GFxValue, kNumArgs> args;

			args[kName] = a_name;
			assert(args[kName].IsString());

			RE::GFxValue boolean;
			[[maybe_unused]] const auto success =
				Invoke("unwatch", std::addressof(boolean), args.data(), args.size());
			assert(success);

			return boolean.GetBool();
		}

		inline std::string ToString()
		{
			RE::GFxValue str;
			[[maybe_unused]] const auto success =
				Invoke("toString", std::addressof(str));
			assert(success);
			return str.GetString();
		}

		inline Object ValueOf()
		{
			RE::GFxValue object;
			[[maybe_unused]] const auto success =
				Invoke("valueOf", std::addressof(object));
			assert(success);
			return Object(object);
		}

	protected:
		inline bool IsArray() const { return _instance.IsArray(); }
		inline bool IsObject() const { return _instance.IsObject(); }

		inline bool Invoke(const char* a_name) { return _instance.Invoke(a_name, nullptr, nullptr, 0); }
		inline bool Invoke(const char* a_name, RE::GFxValue* a_result) { return _instance.Invoke(a_name, a_result, nullptr, 0); }
		inline bool Invoke(const char* a_name, RE::GFxValue* a_result, const RE::GFxValue* a_args, RE::UPInt a_numArgs) { return _instance.Invoke(a_name, a_result, a_args, a_numArgs); }

		inline bool GetBoolean(const char* a_path) const
		{
			RE::GFxValue boolean;
			[[maybe_unused]] const auto success =
				_instance.GetMember(a_path, &boolean);
			assert(success);
			return boolean.GetBool();
		}

		inline double GetNumber(const char* a_path) const
		{
			RE::GFxValue number;
			[[maybe_unused]] const auto success =
				_instance.GetMember(a_path, &number);
			assert(success);
			return number.GetNumber();
		}

		inline Object GetObject(const char* a_path) const
		{
			RE::GFxValue object;
			[[maybe_unused]] const auto success =
				_instance.GetMember(a_path, &object);
			assert(success);
			return Object(object);
		}

		inline std::string GetString(const char* a_path) const
		{
			RE::GFxValue str;
			[[maybe_unused]] const auto success =
				_instance.GetMember(a_path, &str);
			assert(success);
			return str.GetString();
		}

		inline void CreateArray(RE::GPtr<RE::GFxMovieView> a_view)
		{
			a_view->CreateArray(std::addressof(_instance));
			assert(IsArray());
		}

		inline void SetBoolean(const char* a_path, bool a_boolean)
		{
			RE::GFxValue boolean(a_boolean);
			[[maybe_unused]] const auto success =
				_instance.SetMember(a_path, boolean);
			assert(success);
		}

		inline void SetNumber(const char* a_path, double a_number)
		{
			RE::GFxValue number(a_number);
			[[maybe_unused]] const auto success =
				_instance.SetMember(a_path, number);
			assert(success);
		}

		inline void SetObject(const char* a_path, const Object& a_object)
		{
			[[maybe_unused]] const auto success =
				_instance.SetMember(a_path, a_object._instance);
			assert(success);
		}

		inline void SetString(const char* a_path, std::string_view a_string)
		{
			RE::GFxValue str(a_string);
			[[maybe_unused]] const auto success =
				_instance.SetMember(a_path, str);
			assert(success);
		}

		inline void SetInstance(std::nullptr_t) { _instance = nullptr; }
		inline void SetInstance(double a_val) { _instance = a_val; }
		inline void SetInstance(bool a_val) { _instance = a_val; }
		inline void SetInstance(const char* a_val) { _instance = a_val; }
		inline void SetInstance(std::string_view a_val) { _instance = a_val; }
		inline void SetInstance(const wchar_t* a_val) { _instance = a_val; }
		inline void SetInstance(std::wstring_view a_val) { _instance = a_val; }
		inline void SetInstance(const RE::GFxValue& a_val) { _instance = a_val; }
		inline void SetInstance(RE::GFxValue&& a_val) { _instance = std::move(a_val); }

	private:
		inline void SetInstance(const Object& a_val)
		{
			if (this != std::addressof(a_val)) {
				_instance = a_val._instance;
			}
		}

		inline void SetInstance(Object&& a_val)
		{
			if (this != std::addressof(a_val)) {
				_instance = std::move(a_val._instance);
			}
		}

		RE::GFxValue _instance;
	};
}
