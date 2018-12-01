#pragma once

#include <exception>  // exception


namespace QuickLootRE
{
	class bad_gfx_value_interface : public std::exception
	{
	public:
		virtual const char* what() const throw() override
		{
			return "GFxValue did not have an object interface";
		}
	};
}
