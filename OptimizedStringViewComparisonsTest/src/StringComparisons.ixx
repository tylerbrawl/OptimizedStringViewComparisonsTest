module;
#include <array>
#include <list>

export module Brawler.StringComparisons;

namespace Brawler
{
	constexpr std::size_t TEST_ARRAY_SIZE = 50000;
}

export namespace Brawler
{
	template <typename StringViewType>
	std::size_t GetEquivalentStringCount(const std::array<StringViewType, TEST_ARRAY_SIZE>& lhsArr, const std::array<StringViewType, TEST_ARRAY_SIZE>& rhsArr);

	template <typename StringViewType>
	std::size_t GetEquivalentStringCount(const std::list<StringViewType>& lhsList, const std::list<StringViewType>& rhsList);
}