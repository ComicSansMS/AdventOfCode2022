#ifndef ADVENT_OF_CODE_13_DISTRESS_SIGNAL_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_13_DISTRESS_SIGNAL_HPP_INCLUDE_GUARD

#include <cstdint>
#include <memory>
#include <string_view>
#include <variant>
#include <vector>

struct ListElement;

struct List {
    std::vector<ListElement> elements;
};
using ListPtr = std::unique_ptr<List>;

struct ListElement {
    std::variant<std::int64_t, ListPtr> v;

    ListElement() = default;
    ListElement(std::int64_t i)
        :v(i)
    {}
    ListElement(ListPtr&& p)
        :v(std::move(p))
    {}

    ListElement(ListElement&&) noexcept = default;
    ListElement& operator=(ListElement&&) noexcept = default;

    List const& asList() const {
        return *std::get<ListPtr>(v);
    }

    std::int64_t asInt() const {
        return std::get<std::int64_t>(v);
    }
    
    friend inline bool operator==(ListElement const& lhs, ListElement const& rhs) noexcept;
};

inline bool isNumber(ListElement const& l) {
    return l.v.index() == 0;
}

inline bool isList(ListElement const& l) {
    return l.v.index() == 1;
}

inline bool operator==(ListElement const& lhs, ListElement const& rhs) noexcept
{
    if (lhs.v.index() != rhs.v.index()) {
        return false;
    }
    if (isNumber(lhs)) {
        return std::get<std::int64_t>(lhs.v) == std::get<std::int64_t>(rhs.v);
    } else {
        return std::get<ListPtr>(lhs.v)->elements == std::get<ListPtr>(rhs.v)->elements;
    }
}

std::weak_ordering operator<=>(ListElement const& lhs, ListElement const& rhs);
std::weak_ordering operator<=>(List const& lhs, List const& rhs);

struct ListPair {
    List lhs;
    List rhs;
};

std::vector<ListPair> parseInput(std::string_view input);

bool checkOrder(ListPair const& lp);

std::int64_t answer1(std::vector<ListPair> const& listpairs);

List createDecoderPacket1();
List createDecoderPacket2();

bool isDecoderPacket1(List const& l);
bool isDecoderPacket2(List const& l);

std::int64_t answer2(std::vector<ListPair>&& listpairs);

#endif
