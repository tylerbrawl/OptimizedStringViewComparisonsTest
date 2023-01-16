module;
#include <string_view>

export module Brawler.OptimizedStringView;

export namespace Brawler
{
	template <typename CharT, typename Traits = std::char_traits<CharT>>
	class BasicOptimizedStringView final : private std::basic_string_view<CharT, Traits>
	{
	public:
		constexpr BasicOptimizedStringView() = default;

		constexpr BasicOptimizedStringView(const CharT* const srcCStr, const std::size_t numChars);
		constexpr BasicOptimizedStringView(const CharT* const srcCStr);

		template <typename Iterator, typename Sentinel>
		constexpr BasicOptimizedStringView(const Iterator first, const Sentinel end);

		template <typename Range>
		constexpr explicit BasicOptimizedStringView(Range&& range);

		constexpr BasicOptimizedStringView(const BasicOptimizedStringView& rhs) = default;
		constexpr BasicOptimizedStringView& operator=(const BasicOptimizedStringView& rhs) = default;

		constexpr BasicOptimizedStringView(BasicOptimizedStringView&& rhs) noexcept = default;
		constexpr BasicOptimizedStringView& operator=(BasicOptimizedStringView&& rhs) noexcept = default;

		constexpr std::size_t Size() const noexcept;
		constexpr bool Empty() const noexcept;

		friend constexpr bool operator==(const BasicOptimizedStringView lhs, const BasicOptimizedStringView rhs) noexcept
		{
			if (lhs.Empty() || rhs.Empty())
				return (lhs.Empty() == rhs.Empty());

			return (static_cast<const std::basic_string_view<CharT, Traits>&>(lhs) == static_cast<const std::basic_string_view<CharT, Traits>&>(rhs));
		}
	};
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace Brawler
{
	template <typename CharT, typename Traits>
	constexpr BasicOptimizedStringView<CharT, Traits>::BasicOptimizedStringView(const CharT* const srcCStr, const std::size_t numChars) :
		std::basic_string_view<CharT, Traits>(srcCStr, numChars)
	{}

	template <typename CharT, typename Traits>
	constexpr BasicOptimizedStringView<CharT, Traits>::BasicOptimizedStringView(const CharT* const srcCStr) :
		std::basic_string_view<CharT, Traits>(srcCStr)
	{}

	template <typename CharT, typename Traits>
	template <typename Iterator, typename Sentinel>
	constexpr BasicOptimizedStringView<CharT, Traits>::BasicOptimizedStringView(const Iterator first, const Sentinel end) :
		std::basic_string_view<CharT, Traits>(first, end)
	{}

	template <typename CharT, typename Traits>
	template <typename Range>
	constexpr BasicOptimizedStringView<CharT, Traits>::BasicOptimizedStringView(Range&& range) :
		std::basic_string_view<CharT, Traits>(std::forward<Range>(range))
	{}

	template <typename CharT, typename Traits>
	constexpr std::size_t BasicOptimizedStringView<CharT, Traits>::Size() const noexcept
	{
		return std::basic_string_view<CharT, Traits>::size();
	}

	template <typename CharT, typename Traits>
	constexpr bool BasicOptimizedStringView<CharT, Traits>::Empty() const noexcept
	{
		return (Size() == 0);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------

export namespace Brawler
{
	using OptimizedStringView = BasicOptimizedStringView<char>;
	using OptimizedWStringView = BasicOptimizedStringView<wchar_t>;
}