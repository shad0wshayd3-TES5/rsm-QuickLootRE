#pragma once

#include <vector>

class BaseExtraList;


namespace QuickLootRE
{
	class ExtendDataListVisitor
	{
	public:
		ExtendDataListVisitor(std::vector<UInt32> a_whiteList, std::vector<UInt32> a_blackList);
		~ExtendDataListVisitor();

		bool Accept(BaseExtraList* a_xList);
		bool Found();

	private:
		bool CheckWhiteList(BaseExtraList* a_xList);
		bool CheckBlackList(BaseExtraList* a_xList);

		std::vector<UInt32> _whiteList;
		std::vector<UInt32> _blackList;
		bool _found;
	};
}
