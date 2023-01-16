module;
#include <array>
#include <list>
#include <ranges>
#include <string_view>

module Brawler.StringComparisons;
import Brawler.OptimizedStringView;

namespace Brawler
{
	template <typename Range>
	std::size_t GetEquivalentStringCountIMPL(const Range& lhsRange, const Range& rhsRange)
	{
		std::size_t equalStrCount = 0;

		for (const auto [lhs, rhs] : std::views::zip(lhsRange, rhsRange))
		{
			if (lhs == rhs)
				++equalStrCount;
		}

		return equalStrCount;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------

namespace Brawler
{
	template <>
	std::size_t GetEquivalentStringCount<std::string_view>(const std::array<std::string_view, TEST_ARRAY_SIZE>& lhsArr, const std::array<std::string_view, TEST_ARRAY_SIZE>& rhsArr)
	{
		return GetEquivalentStringCountIMPL(lhsArr, rhsArr);
	}

	template <>
	std::size_t GetEquivalentStringCount<OptimizedStringView>(const std::array<OptimizedStringView, TEST_ARRAY_SIZE>& lhsArr, const std::array<OptimizedStringView, TEST_ARRAY_SIZE>& rhsArr)
	{
		return GetEquivalentStringCountIMPL(lhsArr, rhsArr);
	}

	template <>
	std::size_t GetEquivalentStringCount<std::string_view>(const std::list<std::string_view>& lhsList, const std::list<std::string_view>& rhsList)
	{
		return GetEquivalentStringCountIMPL(lhsList, rhsList);
	}

	template <>
	std::size_t GetEquivalentStringCount<OptimizedStringView>(const std::list<OptimizedStringView>& lhsList, const std::list<OptimizedStringView>& rhsList)
	{
		return GetEquivalentStringCountIMPL(lhsList, rhsList);
	}
}