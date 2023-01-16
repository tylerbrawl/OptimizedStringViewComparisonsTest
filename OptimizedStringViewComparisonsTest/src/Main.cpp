#include <array>
#include <chrono>
#include <format>
#include <iostream>
#include <ranges>
#include <string_view>
#include <list>

import Brawler.OptimizedStringView;
import Brawler.StringComparisons;

namespace
{
	static constexpr std::size_t TEST_ARRAY_SIZE = 50000;

	static constexpr std::array<std::string_view, TEST_ARRAY_SIZE> EMPTY_STRING_VIEW_ARR{};
	static constexpr std::array<Brawler::OptimizedStringView, TEST_ARRAY_SIZE> EMPTY_OPTIMIZED_STRING_VIEW_ARR{};

	static constexpr std::string_view TEST_STR_ONE{ "Test String 1" };
	static constexpr std::string_view TEST_STR_TWO{ "Test String 2" };
	static constexpr std::string_view TEST_STR_THREE{ "Test String 3" };
	static constexpr std::string_view TEST_STR_FOUR{ "Test String 4" };

	template <typename StringViewType>
	consteval std::array<StringViewType, TEST_ARRAY_SIZE> CreateTestArray(const std::string_view str)
	{
		std::array<StringViewType, TEST_ARRAY_SIZE> testArr{};

		for (auto& element : testArr)
			element = StringViewType{ str.data(), str.size() };

		return testArr;
	}

	static constexpr std::array<std::string_view, TEST_ARRAY_SIZE> TEST_STR_ONE_ARR{ CreateTestArray<std::string_view>(TEST_STR_ONE) };
	static constexpr std::array<std::string_view, TEST_ARRAY_SIZE> TEST_STR_TWO_ARR{ CreateTestArray<std::string_view>(TEST_STR_TWO) };

	static constexpr std::array<Brawler::OptimizedStringView, TEST_ARRAY_SIZE> TEST_OPTIMIZED_STRING_VIEW_ONE_ARR{ CreateTestArray<Brawler::OptimizedStringView>(TEST_STR_THREE) };
	static constexpr std::array<Brawler::OptimizedStringView, TEST_ARRAY_SIZE> TEST_OPTIMIZED_STRING_VIEW_TWO_ARR{ CreateTestArray<Brawler::OptimizedStringView>(TEST_STR_FOUR) };
}

template <typename Range>
std::chrono::nanoseconds CompareStringRanges(const Range& lhsRange, const Range& rhsRange)
{
	static constexpr std::size_t ITERATION_COUNT = 500;

	std::chrono::steady_clock::duration elapsedTicks{};

	for ([[maybe_unused]] const auto i : std::views::iota(0ull, ITERATION_COUNT))
	{
		const std::chrono::steady_clock::time_point beforeComparisonTime{ std::chrono::steady_clock::now() };
		Brawler::GetEquivalentStringCount(lhsRange, rhsRange);
		const std::chrono::steady_clock::time_point afterComparisonTime{ std::chrono::steady_clock::now() };

		elapsedTicks += (afterComparisonTime - beforeComparisonTime);
	}

	return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedTicks / ITERATION_COUNT);
}

struct TimingResults
{
	std::chrono::nanoseconds StringViewTimeNS;
	std::chrono::nanoseconds OptimizedStringViewTimeNS;
};

void DisplayEmptyStringTimingResults(const TimingResults& timingResults)
{
	const float speedup = (static_cast<float>(timingResults.StringViewTimeNS.count()) / static_cast<float>(timingResults.OptimizedStringViewTimeNS.count()));

	std::cout << std::format("Time Taken for 50,000 Empty std::string_view Comparisons: {0:%Q}{0:%q}\n", timingResults.StringViewTimeNS);
	std::cout << std::format("Time Taken for 50,000 Empty Brawler::OptimizedStringView Comparisons: {0:%Q}{0:%q}\n", timingResults.OptimizedStringViewTimeNS);
	std::cout << std::format("Empty String Speed-Up: {:#.2g}x\n\n", speedup);
}

void DisplayNonEmptyStringTimingResults(const TimingResults& timingResults)
{
	const float speedup = (static_cast<float>(timingResults.StringViewTimeNS.count()) / static_cast<float>(timingResults.OptimizedStringViewTimeNS.count()));

	std::cout << std::format("Time Taken for 50,000 Non-Empty std::string_view Comparisons: {0:%Q}{0:%q}\n", timingResults.StringViewTimeNS);
	std::cout << std::format("Time Taken for 50,000 Non-Empty Brawler::OptimizedStringView Comparisons: {0:%Q}{0:%q}\n", timingResults.OptimizedStringViewTimeNS);
	std::cout << std::format("Non-Empty String Speed-Up: {:#.2g}x\n", speedup);
}

void ExecuteArraysTest()
{
	{
		const TimingResults emptyStrTimingResults{
			.StringViewTimeNS{ CompareStringRanges(EMPTY_STRING_VIEW_ARR, EMPTY_STRING_VIEW_ARR) },
			.OptimizedStringViewTimeNS{ CompareStringRanges(EMPTY_OPTIMIZED_STRING_VIEW_ARR, EMPTY_OPTIMIZED_STRING_VIEW_ARR) }
		};

		DisplayEmptyStringTimingResults(emptyStrTimingResults);
	}

	{
		const TimingResults nonEmptyStrTimingResults{
			.StringViewTimeNS{ CompareStringRanges(TEST_STR_ONE_ARR, TEST_STR_TWO_ARR) },
			.OptimizedStringViewTimeNS{ CompareStringRanges(TEST_OPTIMIZED_STRING_VIEW_ONE_ARR, TEST_OPTIMIZED_STRING_VIEW_TWO_ARR) }
		};

		DisplayNonEmptyStringTimingResults(nonEmptyStrTimingResults);
	}
}

void ExecuteListsTest()
{
	constexpr auto CREATE_LIST_FROM_ARRAY_LAMBDA = []<typename T, std::size_t NumElements>(const std::array<T, NumElements>&arr)
	{
		std::list<T> generatedList{};

		for (const auto& element : arr)
			generatedList.push_back(element);

		return generatedList;
	};

	{
		const std::list<std::string_view> emptyStrLHSList{ CREATE_LIST_FROM_ARRAY_LAMBDA(EMPTY_STRING_VIEW_ARR) };
		const std::list<std::string_view> emptyStrRHSList{ CREATE_LIST_FROM_ARRAY_LAMBDA(EMPTY_STRING_VIEW_ARR) };

		const std::list<Brawler::OptimizedStringView> emptyOptimizedStrLHSList{ CREATE_LIST_FROM_ARRAY_LAMBDA(EMPTY_OPTIMIZED_STRING_VIEW_ARR) };
		const std::list<Brawler::OptimizedStringView> emptyOptimizedStrRHSList{ CREATE_LIST_FROM_ARRAY_LAMBDA(EMPTY_OPTIMIZED_STRING_VIEW_ARR) };

		const TimingResults emptyStrTimingResults{
			.StringViewTimeNS{ CompareStringRanges(emptyStrLHSList, emptyStrRHSList) },
			.OptimizedStringViewTimeNS{ CompareStringRanges(emptyOptimizedStrLHSList, emptyOptimizedStrRHSList) }
		};

		DisplayEmptyStringTimingResults(emptyStrTimingResults);
	}

	{
		const std::list<std::string_view> nonEmptyStrLHSList{ CREATE_LIST_FROM_ARRAY_LAMBDA(TEST_STR_ONE_ARR) };
		const std::list<std::string_view> nonEmptyStrRHSList{ CREATE_LIST_FROM_ARRAY_LAMBDA(TEST_STR_TWO_ARR) };

		const std::list<Brawler::OptimizedStringView> nonEmptyOptimizedStrLHSList{ CREATE_LIST_FROM_ARRAY_LAMBDA(TEST_OPTIMIZED_STRING_VIEW_ONE_ARR) };
		const std::list<Brawler::OptimizedStringView> nonEmptyOptimizedStrRHSList{ CREATE_LIST_FROM_ARRAY_LAMBDA(TEST_OPTIMIZED_STRING_VIEW_TWO_ARR) };

		const TimingResults nonEmptyStrTimingResults{
			.StringViewTimeNS{ CompareStringRanges(nonEmptyStrLHSList, nonEmptyStrRHSList) },
			.OptimizedStringViewTimeNS{ CompareStringRanges(nonEmptyOptimizedStrLHSList, nonEmptyOptimizedStrRHSList) }
		};

		DisplayNonEmptyStringTimingResults(nonEmptyStrTimingResults);
	}
}

int main(const int, const char* const [])
{
	std::cout << std::format("{0:*^20}\n", "Array Tests");
	ExecuteArraysTest();

	std::cout << std::format("\n{0:*^20}\n", "List Tests");
	ExecuteListsTest();

	return 0;
}