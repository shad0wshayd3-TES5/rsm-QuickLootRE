#pragma once

#include "RE/Skyrim.h"
#include "REL/Relocation.h"
#include "SKSE/SKSE.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <variant>
#include <vector>

#include <boost/regex.hpp>
#include <frozen/map.h>
#include <nonstd/span.hpp>
#include <spdlog/sinks/basic_file_sink.h>

#ifndef NDEBUG
#include <spdlog/sinks/msvc_sink.h>
#endif

using RE::observer;
using RE::owner;

using namespace std::literals;

namespace logger = SKSE::log;

namespace stl
{
	using namespace SKSE::stl;

	using SKSE::util::to_underlying;

	namespace detail
	{
		template <class, class = void>
		struct implements_size :
			std::false_type
		{};

		template <class T>
		struct implements_size<
			T,
			std::void_t<
				decltype(std::declval<T>().size())>> :
			std::is_integral<
				decltype(std::declval<T>().size())>
		{};

		template <class T>
		inline constexpr bool implements_size_v = implements_size<T>::value;
	}

	template <
		class C,
		std::enable_if_t<
			detail::implements_size_v<const C&>,
			int> = 0>
	[[nodiscard]] constexpr auto ssize(const C& a_container)
		-> std::common_type_t<
			std::ptrdiff_t,
			std::make_signed_t<decltype(a_container.size())>>
	{
		using result_t =
			std::common_type_t<
				std::ptrdiff_t,
				std::make_signed_t<decltype(a_container.size())>>;
		return static_cast<result_t>(a_container.size());
	}

	template <class T, std::ptrdiff_t N>
	[[nodiscard]] constexpr std::ptrdiff_t ssize(const T (&a_array)[N]) noexcept
	{
		return N;
	}
}

constexpr std::string_view safe_string(const char* a_str) { return a_str ? a_str : ""sv; }

#define DLLEXPORT __declspec(dllexport)
