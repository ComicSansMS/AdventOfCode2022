#include <distress_signal.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/split.hpp>

#include <algorithm>
#include <cassert>
#include <compare>
#include <string>
#include <tuple>

List parseList_rec(std::string_view line);
ListElement parseListElement(std::string_view line);

List parseList_rec(std::string_view line)
{
    auto const it_start = line.begin();
    auto const it_end = line.end();
    assert(*it_start == '[');
    std::vector<std::string_view::iterator> item_seperators;
    int nest_count = 0;
    for (auto it = it_start; it != it_end; ++it) {
        char const c = *it;
        if (c == '[') {
            ++nest_count;
        } else if (c == ']') {
            assert(nest_count > 0);
            --nest_count;
            if (nest_count == 0) {
                item_seperators.push_back(it);
                break;
            }
        } else if ((c == ',') && (nest_count == 1)) {
            item_seperators.push_back(it);
        }
    }
    List ret;
    ret.elements.reserve(item_seperators.size());
    auto it = it_start;
    for (auto const it_sep : item_seperators) {
        std::string_view element_str{ it + 1, it_sep };
        if (!element_str.empty()) {
            ret.elements.emplace_back(parseListElement(element_str));
        }
        it = it_sep;
    }
    return ret;
}

ListElement parseListElement(std::string_view line)
{
    assert(!line.empty());
    if (line[0] == '[') {
        List l = parseList_rec(line);
        return std::make_unique<List>(std::move(l));
    } else {
        int64_t n = std::stoi(std::string{line});
        return n;
    }
}


List parseList(std::string_view line)
{
    return parseList_rec(line);
}

std::vector<ListPair> parseInput(std::string_view input)
{
    std::vector<ListPair> ret;

    enum class Parser {
        ExpectFirst, ExpectSecond, ExpectNewLine
    } state = Parser::ExpectFirst;
    for (auto const line : input | ranges::views::split('\n')) {
        std::string l = line | ranges::to<std::string>;
        if (state == Parser::ExpectFirst) {
            ret.emplace_back();
            ret.back().lhs = parseList(l);
            state = Parser::ExpectSecond;
        } else if (state == Parser::ExpectSecond) {
            ret.back().rhs = parseList(l);
            state = Parser::ExpectNewLine;
        } else {
            assert(state == Parser::ExpectNewLine);
            assert(l.empty());
            state = Parser::ExpectFirst;
        }
    }
    return ret;
}

std::weak_ordering operator<=>(ListElement const& lhs, ListElement const& rhs)
{
    auto wrapNumber = [](std::int64_t n) -> ListElement {
        auto n_list = std::make_unique<List>();
        n_list->elements.push_back(ListElement{ n });
        return n_list;
    };
    if (isNumber(lhs) && isNumber(rhs)) {
        return lhs.asInt() <=> rhs.asInt();
    } else if (isList(lhs) && isList(rhs)) {
        return lhs.asList() <=> rhs.asList();
    } else {
        if (isNumber(lhs)) {
            return wrapNumber(lhs.asInt()) <=> rhs;
        } else {
            assert(isNumber(rhs));
            return lhs <=> wrapNumber(rhs.asInt());
        }
    }
}

std::weak_ordering operator<=>(List const& lhs, List const& rhs)
{
    std::size_t i_end = std::min(lhs.elements.size(), rhs.elements.size());
    for (std::size_t i = 0; i < i_end; ++i) {
        if (auto const cmp = lhs.elements[i] <=> rhs.elements[i]; cmp != std::weak_ordering::equivalent) {
            return cmp;
        }
    }
    return lhs.elements.size() <=> rhs.elements.size();
}

bool checkOrder(ListPair const& lp)
{
    return lp.lhs < lp.rhs;
}

std::int64_t answer1(std::vector<ListPair> const& listpairs)
{
    std::int64_t ret = 0;
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(listpairs.size()); ++i) {
        if (checkOrder(listpairs[i])) {
            ret += i + 1;
        }
    }
    return ret;
}

List createDecoderPacket(std::int64_t n)
{
    List inner_list;
    inner_list.elements.push_back(n);
    ListElement le = std::make_unique<List>(std::move(inner_list));
    List outer_list;
    outer_list.elements.push_back(std::move(le));
    return outer_list;
}

List createDecoderPacket1()
{
    return createDecoderPacket(2);
}

List createDecoderPacket2()
{
    return createDecoderPacket(6);
}

bool isDecoderPacket(List const& l, std::int64_t n)
{
    if ((l.elements.size() != 1) ||
        (!isList(l.elements[0])) ||
        (l.elements[0].asList().elements.size() != 1) ||
        (!isNumber(l.elements[0].asList().elements[0])))
    {
        return false;
    }
    return l.elements[0].asList().elements[0].asInt() == n;
}

bool isDecoderPacket1(List const& l)
{
    return isDecoderPacket(l, 2);
}

bool isDecoderPacket2(List const& l)
{
    return isDecoderPacket(l, 6);
}

std::int64_t answer2(std::vector<ListPair>&& listpairs)
{
    std::vector<List> data;
    for (auto& lp : listpairs) {
        data.emplace_back(std::move(lp.lhs));
        data.emplace_back(std::move(lp.rhs));
    }
    data.emplace_back(createDecoderPacket1());
    data.emplace_back(createDecoderPacket2());
    std::sort(data.begin(), data.end());

    auto it_p1 = std::find_if(data.begin(), data.end(), isDecoderPacket1);
    auto it_p2 = std::find_if(data.begin(), data.end(), isDecoderPacket2);

    return (std::distance(data.begin(), it_p1) + 1) * (std::distance(data.begin(), it_p2) + 1);
}
