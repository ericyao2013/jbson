//          Copyright Christian Manning 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
using namespace std::literals;
#include <list>
#include <deque>
#include <fstream>

#include <jbson/element.hpp>
#include <jbson/document.hpp>
#include <jbson/builder.hpp>
using namespace jbson;

#include <gtest/gtest.h>

struct find_by_element_name_impl {
    template <typename SetT, typename NameT> auto operator()(SetT&& set, NameT&& name) const {
        return std::find_if(set.begin(), set.end(), [&](auto&& el) { return el.name() == name; });
    }
} find_by_element_name;

// copmile-time tests
static_assert(std::is_constructible<std::vector<element>, array>::value, "");
static_assert(std::is_same<decltype(std::declval<document>().data()), std::vector<char>&&>::value, "");
static_assert(std::is_same<decltype(std::declval<document&>().data()), const std::vector<char>&>::value, "");
static_assert(std::is_same<decltype(std::declval<document&&>().data()), std::vector<char>&&>::value, "");
#ifndef JBSON_NO_CONST_RVALUE_THIS
static_assert(std::is_same<decltype(std::declval<const document&&>().data()), std::vector<char>>::value, "");
#endif // JBSON_NO_CONST_RVALUE_THIS

static_assert(std::is_nothrow_move_assignable<basic_document<std::vector<char>>>::value, "");
static_assert(std::is_nothrow_move_constructible<basic_document<std::vector<char>>>::value, "");

static_assert(std::is_nothrow_move_assignable<basic_document<std::deque<char>>>::value, "");
// static_assert(std::is_nothrow_move_constructible<basic_document<std::deque<char>>>::value, "");

// vector
static_assert(std::is_default_constructible<document>::value, "");
static_assert(std::is_constructible<document, builder>::value, "");
static_assert(!std::is_constructible<document, array_builder>::value, "");
static_assert(std::is_constructible<document, std::array<char, 12>>::value, "");
static_assert(!std::is_constructible<document, std::array<char, 2>>::value, "");
static_assert(!std::is_constructible<document, char[12]>::value, "");
static_assert(!std::is_constructible<document, char[2]>::value, "");
static_assert(std::is_constructible<document, std::deque<char>>::value, "");
static_assert(std::is_constructible<document, std::list<char>>::value, "");
static_assert(std::is_constructible<document, std::list<char>::const_iterator, std::list<char>::const_iterator>::value,
              "");
static_assert(std::is_constructible<document, document_set::const_iterator, document_set::const_iterator>::value, "");
static_assert(std::is_constructible<document, document::const_iterator, document::const_iterator>::value, "");
static_assert(std::is_constructible<document, boost::iterator_range<char*>>::value, "");
static_assert(std::is_constructible<document, boost::iterator_range<std::vector<char>::const_iterator>>::value, "");
static_assert(std::is_constructible<document, boost::iterator_range<std::list<char>::const_iterator>>::value, "");
static_assert(
    std::is_constructible<document, basic_document<boost::iterator_range<std::vector<char>::const_iterator>>>::value,
    "");
static_assert(std::is_constructible<document, basic_document<std::deque<char>>>::value, "");
static_assert(
    std::is_constructible<document, basic_document<boost::iterator_range<std::deque<char>::const_iterator>>>::value,
    "");

// iterator_range<vector>
using document_range = basic_document<boost::iterator_range<std::vector<char>::const_iterator>>;
// static_assert(!std::is_default_constructible<document_range>::value, "");
// static_assert(!std::is_constructible<document_range, builder>::value, "");
// static_assert(!std::is_constructible<std::vector<char>::const_iterator, char*>::value, "");
static_assert(!std::is_constructible<document_range, array_builder>::value, "");
// static_assert(!std::is_constructible<document_range, std::array<char,12>>::value, "");
static_assert(!std::is_constructible<document_range, std::array<char, 2>>::value, "");
static_assert(!std::is_constructible<document_range, std::deque<char>>::value, "");
static_assert(!std::is_constructible<document_range, std::list<char>>::value, "");
static_assert(
    !std::is_constructible<document_range, std::list<char>::const_iterator, std::list<char>::const_iterator>::value,
    "");
static_assert(!std::is_constructible<document_range, document_set::const_iterator, document_set::const_iterator>::value,
              "");
static_assert(!std::is_constructible<document_range, document::const_iterator, document::const_iterator>::value, "");
static_assert(
    !std::is_constructible<document_range, document_range::const_iterator, document_range::const_iterator>::value, "");
// static_assert(!std::is_constructible<document_range, boost::iterator_range<char*>>::value, "");
static_assert(std::is_constructible<document_range, boost::iterator_range<std::vector<char>::const_iterator>>::value,
              "");
static_assert(!std::is_constructible<document_range, boost::iterator_range<std::list<char>::const_iterator>>::value,
              "");
static_assert(std::is_constructible<document_range,
                                    basic_document<boost::iterator_range<std::vector<char>::const_iterator>>>::value,
              "");
static_assert(std::is_constructible<document_range, document>::value, "");
static_assert(std::is_convertible<document, document_range>::value, "");
// static_assert(!std::is_constructible<document_range, basic_document<std::deque<char>>>::value, "");
// static_assert(!std::is_constructible<document_range,
// basic_document<boost::iterator_range<std::deque<char>::const_iterator>>>::value, "");
static_assert(std::is_convertible<document_range, document>::value, "");

// deque
using document_deque = basic_document<std::deque<char>>;
static_assert(std::is_default_constructible<document_deque>::value, "");
static_assert(std::is_constructible<document_deque, builder>::value, "");
static_assert(!std::is_constructible<document_deque, array_builder>::value, "");
static_assert(std::is_constructible<document_deque, std::array<char, 12>>::value, "");
static_assert(!std::is_constructible<document_deque, std::array<char, 2>>::value, "");
static_assert(std::is_constructible<document_deque, std::deque<char>>::value, "");
static_assert(std::is_constructible<document_deque, std::list<char>>::value, "");
static_assert(
    std::is_constructible<document_deque, std::list<char>::const_iterator, std::list<char>::const_iterator>::value, "");
static_assert(std::is_constructible<document_deque, document_set::const_iterator, document_set::const_iterator>::value,
              "");
static_assert(std::is_constructible<document_deque, document::const_iterator, document::const_iterator>::value, "");
static_assert(std::is_constructible<document_deque, boost::iterator_range<char*>>::value, "");
static_assert(std::is_constructible<document_deque, boost::iterator_range<std::vector<char>::const_iterator>>::value,
              "");
static_assert(std::is_constructible<document_deque, boost::iterator_range<std::list<char>::const_iterator>>::value, "");
static_assert(std::is_constructible<document_deque,
                                    basic_document<boost::iterator_range<std::vector<char>::const_iterator>>>::value,
              "");
static_assert(std::is_constructible<document_deque, basic_document<std::deque<char>>>::value, "");
static_assert(std::is_constructible<document_deque,
                                    basic_document<boost::iterator_range<std::deque<char>::const_iterator>>>::value,
              "");

// list
using document_list = basic_document<std::list<char>>;
static_assert(std::is_default_constructible<document_list>::value, "");
static_assert(std::is_constructible<document_list, builder>::value, "");
static_assert(!std::is_constructible<document_list, array_builder>::value, "");
static_assert(std::is_constructible<document_list, std::array<char, 12>>::value, "");
static_assert(!std::is_constructible<document_list, std::array<char, 2>>::value, "");
static_assert(std::is_constructible<document_list, std::deque<char>>::value, "");
static_assert(std::is_constructible<document_list, std::list<char>>::value, "");
static_assert(
    std::is_constructible<document_list, std::list<char>::const_iterator, std::list<char>::const_iterator>::value, "");
static_assert(std::is_constructible<document_list, document_set::const_iterator, document_set::const_iterator>::value,
              "");
static_assert(std::is_constructible<document_list, document::const_iterator, document::const_iterator>::value, "");
static_assert(std::is_constructible<document_list, boost::iterator_range<char*>>::value, "");
static_assert(std::is_constructible<document_list, boost::iterator_range<std::vector<char>::const_iterator>>::value,
              "");
static_assert(std::is_constructible<document_list, boost::iterator_range<std::list<char>::const_iterator>>::value, "");
static_assert(std::is_constructible<document_list,
                                    basic_document<boost::iterator_range<std::vector<char>::const_iterator>>>::value,
              "");
static_assert(std::is_constructible<document_list, basic_document<std::deque<char>>>::value, "");
static_assert(std::is_constructible<document_list,
                                    basic_document<boost::iterator_range<std::deque<char>::const_iterator>>>::value,
              "");

// iterator_range<list>
using document_list_range = basic_document<boost::iterator_range<std::list<char>::const_iterator>>;
// static_assert(!std::is_default_constructible<document_list_range>::value, "");
// static_assert(!std::is_constructible<document_range, builder>::value, "");
static_assert(!std::is_constructible<document_list_range, array_builder>::value, "");
static_assert(!std::is_constructible<document_list_range, std::array<char, 12>>::value, "");
static_assert(!std::is_constructible<document_list_range, std::array<char, 2>>::value, "");
static_assert(!std::is_constructible<document_list_range, std::deque<char>>::value, "");
static_assert(std::is_constructible<document_list_range, std::list<char>>::value, "");
static_assert(
    std::is_constructible<document_list_range, std::list<char>::const_iterator, std::list<char>::const_iterator>::value,
    "");
static_assert(
    !std::is_constructible<document_list_range, document_set::const_iterator, document_set::const_iterator>::value, "");
static_assert(!std::is_constructible<document_list_range, document::const_iterator, document::const_iterator>::value,
              "");
static_assert(!std::is_constructible<document_list_range, document_list_range::const_iterator,
                                     document_list_range::const_iterator>::value,
              "");
static_assert(!std::is_constructible<document_list_range, boost::iterator_range<char*>>::value, "");
static_assert(
    !std::is_constructible<document_list_range, boost::iterator_range<std::vector<char>::const_iterator>>::value, "");
static_assert(std::is_constructible<document_list_range, boost::iterator_range<std::list<char>::const_iterator>>::value,
              "");
// static_assert(!std::is_constructible<document_list_range,
// basic_document<boost::iterator_range<std::vector<char>::const_iterator>>>::value, "");
// static_assert(!std::is_constructible<document_list_range, document>::value, "");
static_assert(std::is_convertible<document_list, document_list_range>::value, "");
// static_assert(!std::is_constructible<document_list_range, basic_document<std::deque<char>>>::value, "");
// static_assert(!std::is_constructible<document_list_range,
// basic_document<boost::iterator_range<std::deque<char>::const_iterator>>>::value, "");
static_assert(std::is_convertible<document_list_range, document>::value, "");

TEST(DocumentTest, EmptyDocumentTest) {
    EXPECT_NO_THROW(EXPECT_EQ(0, boost::distance(document())));
    EXPECT_NO_THROW(EXPECT_EQ(0, boost::distance(basic_document<std::array<char, 5>>())));

    EXPECT_ANY_THROW(EXPECT_EQ(0, boost::distance(basic_document<boost::iterator_range<const char*>>())));

    EXPECT_ANY_THROW(
        EXPECT_EQ(0, boost::distance(basic_document<boost::iterator_range<std::vector<char>::const_iterator>>())));

    EXPECT_NO_THROW(EXPECT_EQ(0, boost::distance(array())));
    EXPECT_NO_THROW(EXPECT_EQ(0, boost::distance(basic_array<std::array<char, 5>>())));

    EXPECT_ANY_THROW(EXPECT_EQ(0, boost::distance(basic_array<boost::iterator_range<const char*>>())));
}

TEST(DocumentTest, ValidityTest1) {
    EXPECT_FALSE(basic_document<boost::iterator_range<std::vector<char>::const_iterator>>().valid());
    EXPECT_TRUE(document().valid(validity_level::bson_size));
    EXPECT_TRUE(document().valid(validity_level::element_construct));
}

TEST(DocumentTest, DocumentParseTest1) {
    const auto test_bson = "\x16\x00\x00\x00\x02hello\x00\x06\x00\x00\x00world\x00\x00"s;
    auto doc = document{test_bson};

    auto begin = doc.begin();
    static_assert(std::is_same<std::decay_t<decltype(*begin)>, document::element_type>::value, "");
    auto e = *begin;
    ASSERT_EQ(element_type::string_element, e.type());
    EXPECT_EQ("hello", e.name());
    EXPECT_EQ("world", get<element_type::string_element>(e));
    for(auto e : doc) {
        ASSERT_EQ(element_type::string_element, e.type());
        EXPECT_EQ("hello", e.name());
        EXPECT_EQ("world", get<element_type::string_element>(e));
    }
}

TEST(DocumentTest, DocumentParseTest2) {
    auto test_bson = "x02hello\x00\x06\x00\x00\x00world\x00\x00"s;
    ASSERT_THROW(document{test_bson}, invalid_document_size);
}

TEST(DocumentTest, DocumentParseTest3) {
    std::ifstream ifs{JBSON_FILES "/test.bson", std::ios::binary};
    ASSERT_FALSE(ifs.fail());
    ifs.seekg(0, std::ios::end);
    auto n = static_cast<std::streamoff>(ifs.tellg());
    ASSERT_GT(n, 0);
    auto binstring = std::vector<char>(n);
    ifs.seekg(0, std::ios::beg);
    ifs.read(binstring.data(), n);

    auto doc = basic_document<boost::iterator_range<std::vector<char>::const_iterator>>{binstring};

    auto it = doc.begin();
    auto end = doc.end();
    ASSERT_NE(it, end);

    auto arr_el = *it++;
    ASSERT_EQ(it, end);

    EXPECT_EQ(element_type::array_element, arr_el.type());
    auto arr = get<element_type::array_element>(arr_el);
    static_assert(detail::is_document<decltype(arr)>::value, "");
    static_assert(
        std::is_same<basic_array<boost::iterator_range<std::vector<char>::const_iterator>>, decltype(arr)>::value, "");

    auto i = int32_t{0};
    for(auto&& e : arr) {
        EXPECT_LT(e.size(), arr.size());
        EXPECT_EQ(std::to_string(i), e.name());
        ++i;
    }

    it = arr.begin();
    end = arr.end();
    EXPECT_EQ("0", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("awesome", get<element_type::string_element>(*it));
    it++;
    EXPECT_EQ("1", it->name());
    EXPECT_EQ(element_type::double_element, it->type());
    EXPECT_EQ(5.05, it->value<double>());
    it++;
    EXPECT_EQ("2", it->name());
    EXPECT_EQ(element_type::int32_element, it->type());
    EXPECT_EQ(1986, it->value<int32_t>());
    it++;
    ASSERT_EQ(it, end);
}

TEST(DocumentTest, DocumentParseTest4) {
    std::ifstream ifs{JBSON_FILES "/test.bson", std::ios::binary};
    ASSERT_FALSE(ifs.fail());
    ifs.seekg(0, std::ios::end);
    auto n = static_cast<std::streamoff>(ifs.tellg());
    ASSERT_GT(n, 0);
    auto binstring = std::vector<char>(n);
    ifs.seekg(0, std::ios::beg);
    ifs.read(binstring.data(), n);

    auto doc = basic_document<boost::iterator_range<std::vector<char>::const_iterator>>{binstring};

    auto end = doc.end();
    auto it = doc.find("BSON");
    ASSERT_NE(it, end);

    auto arr_el = *it++;
    ASSERT_EQ(it, end);

    EXPECT_EQ(element_type::array_element, arr_el.type());
    auto arr = get<element_type::array_element>(arr_el);
    static_assert(detail::is_document<decltype(arr)>::value, "");
    static_assert(
        std::is_same<basic_array<boost::iterator_range<std::vector<char>::const_iterator>>, decltype(arr)>::value, "");

    it = arr.find(0);
    end = arr.end();
    ASSERT_NE(it, end);
    EXPECT_EQ("0", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("awesome", get<element_type::string_element>(*it));
    it = arr.find(1);
    ASSERT_NE(it, end);
    EXPECT_EQ("1", it->name());
    EXPECT_EQ(element_type::double_element, it->type());
    EXPECT_EQ(5.05, it->value<double>());
    it = arr.find(2);
    ASSERT_NE(it, end);
    EXPECT_EQ("2", it->name());
    EXPECT_EQ(element_type::int32_element, it->type());
    EXPECT_EQ(1986, it->value<int32_t>());
    it++;
    ASSERT_EQ(it, end);
}

TEST(DocumentTest, ArrayTest1) {
    std::ifstream ifs{JBSON_FILES "/test.bson", std::ios::binary};
    ASSERT_FALSE(ifs.fail());
    ifs.seekg(0, std::ios::end);
    auto n = static_cast<std::streamoff>(ifs.tellg());
    ASSERT_GT(n, 0);
    auto binstring = std::vector<char>(n);
    ifs.seekg(0, std::ios::beg);
    ifs.read(binstring.data(), n);

    auto doc = basic_document<boost::iterator_range<std::vector<char>::const_iterator>>{binstring};

    auto end = doc.end();
    auto it = doc.find("BSON");
    ASSERT_NE(it, end);

    auto arr_el = *it++;
    ASSERT_EQ(it, end);

    EXPECT_EQ(element_type::array_element, arr_el.type());
    auto arr = get<element_type::array_element>(arr_el);
    static_assert(detail::is_document<decltype(arr)>::value, "");
    static_assert(
        std::is_same<basic_array<boost::iterator_range<std::vector<char>::const_iterator>>, decltype(arr)>::value, "");

    static_assert(std::is_constructible<std::vector<element>, decltype(arr)>::value, "");
    static_assert(std::is_constructible<std::deque<element>, decltype(arr)>::value, "");
    auto vec = std::vector<element>(arr);
    ASSERT_EQ(3, vec.size());

    auto vec_it = vec.begin();
    auto vec_end = vec.end();
    ASSERT_NE(vec_it, vec_end);
    EXPECT_EQ("0", vec_it->name());
    EXPECT_EQ(element_type::string_element, vec_it->type());
    EXPECT_EQ("awesome", get<element_type::string_element>(*vec_it));
    vec_it++;
    ASSERT_NE(vec_it, vec_end);
    EXPECT_EQ("1", vec_it->name());
    EXPECT_EQ(element_type::double_element, vec_it->type());
    EXPECT_EQ(5.05, vec_it->value<double>());
    vec_it++;
    ASSERT_NE(vec_it, vec_end);
    EXPECT_EQ("2", vec_it->name());
    EXPECT_EQ(element_type::int32_element, vec_it->type());
    EXPECT_EQ(1986, vec_it->value<int32_t>());
    vec_it++;
    ASSERT_EQ(vec_it, vec_end);
}

TEST(DocumentTest, VectorToDocTest1) {
    std::ifstream ifs{JBSON_FILES "/test.bson", std::ios::binary};
    ASSERT_FALSE(ifs.fail());
    ifs.seekg(0, std::ios::end);
    auto n = static_cast<std::streamoff>(ifs.tellg());
    ASSERT_GT(n, 0);
    auto binstring = std::vector<char>(n);
    ifs.seekg(0, std::ios::beg);
    ifs.read(binstring.data(), n);

    auto doc = basic_document<boost::iterator_range<std::vector<char>::const_iterator>>{binstring};
    EXPECT_GE(&*doc.data().begin(), binstring.data());
    EXPECT_LT(&*doc.data().begin(), binstring.data() + binstring.size());

    auto end = doc.end();
    auto it = doc.find("BSON");
    ASSERT_NE(it, end);

    auto arr_el = *it++;
    ASSERT_EQ(it, end);

    EXPECT_EQ(element_type::array_element, arr_el.type());
    auto arr = get<element_type::array_element>(arr_el);
    static_assert(detail::is_document<decltype(arr)>::value, "");
    static_assert(
        std::is_same<basic_array<boost::iterator_range<std::vector<char>::const_iterator>>, decltype(arr)>::value, "");
    EXPECT_GE(&*arr.data().begin(), binstring.data());
    EXPECT_LT(&*arr.data().begin(), binstring.data() + binstring.size());
    auto arr_size = arr.size();

    {
        auto vec = std::vector<element>(arr);
        ASSERT_EQ(3, vec.size());

        auto doc = document(vec);
        ASSERT_EQ(arr_size, doc.size());

        auto it = doc.find("0");
        auto end = doc.end();
        ASSERT_NE(it, end);
        EXPECT_EQ("0", it->name());
        EXPECT_EQ(element_type::string_element, it->type());
        EXPECT_EQ("awesome", get<element_type::string_element>(*it));
        it = doc.find("1");
        ASSERT_NE(it, end);
        EXPECT_EQ("1", it->name());
        EXPECT_EQ(element_type::double_element, it->type());
        EXPECT_EQ(5.05, it->value<double>());
        it = doc.find("2");
        ASSERT_NE(it, end);
        EXPECT_EQ("2", it->name());
        EXPECT_EQ(element_type::int32_element, it->type());
        EXPECT_EQ(1986, it->value<int32_t>());
        it++;
        ASSERT_EQ(it, end);
    }
}

TEST(DocumentTest, SetTest1) {
    auto doc = static_cast<document>(builder("first name", element_type::string_element, "Chris")(
        "surname", element_type::string_element, "Manning")("yob", element_type::int32_element, 1991));
    static_assert(std::is_same<decltype(doc), document>::value, "");
    auto set = static_cast<document_set>(doc);
    ASSERT_EQ(3, set.size());
    auto it = find_by_element_name(set, "yob");
    ASSERT_NE(set.end(), it);
    EXPECT_EQ(1991, it->value<int32_t>());
    it = find_by_element_name(set, "first name");
    ASSERT_NE(set.end(), it);
    EXPECT_EQ("Chris", it->value<std::experimental::string_view>());
    it = find_by_element_name(set, "surname");
    ASSERT_NE(set.end(), it);
    EXPECT_EQ("Manning", it->value<std::experimental::string_view>());
}

TEST(DocumentTest, SetTest2) {
    document_set set;
    set.emplace("first name", element_type::string_element, "Chris");
    set.emplace("surname", element_type::string_element, "Manning");
    set.emplace("yob", element_type::int32_element, 1991);
    auto doc = document{set};
    auto it = doc.find("yob");
    ASSERT_NE(doc.end(), it);
    EXPECT_EQ(1991, it->value<int32_t>());
    it = doc.find("first name");
    ASSERT_NE(doc.end(), it);
    EXPECT_EQ("Chris", it->value<std::experimental::string_view>());
    it = doc.find("surname");
    ASSERT_NE(doc.end(), it);
    EXPECT_EQ("Manning", it->value<std::experimental::string_view>());
}

TEST(DocumentTest, SetTest3) {
    document_set set;
    set.emplace("key", element_type::string_element, "ææ");
    set.emplace("key", element_type::string_element, "aa");
    set.emplace("key", element_type::string_element, "bb");
    set.emplace("key", element_type::string_element, "ää");
    set.emplace("key", element_type::string_element, "ââ");
    auto it = set.begin();
    ASSERT_NE(set.end(), it);
    EXPECT_EQ("aa", it->value<std::experimental::string_view>());
    it++;
    ASSERT_NE(set.end(), it);
    EXPECT_EQ("ââ", it->value<std::experimental::string_view>());
    it++;
    ASSERT_NE(set.end(), it);
    EXPECT_EQ("ää", it->value<std::experimental::string_view>());
    it++;
    ASSERT_NE(set.end(), it);
    EXPECT_EQ("ææ", it->value<std::experimental::string_view>());
    it++;
    ASSERT_NE(set.end(), it);
    EXPECT_EQ("bb", it->value<std::experimental::string_view>());
    it++;
    ASSERT_EQ(set.end(), it);
}

TEST(DocumentTest, DocumentContainerTest1) {
    std::array<char, 5> data;
    *reinterpret_cast<int32_t*>(data.data()) = data.size();
    basic_document<std::array<char, 5>> doc{data};
    ASSERT_EQ(0, boost::distance(doc));
}

TEST(DocumentTest, DocumentInsertTest1) {
    document doc = document(builder("first name", element_type::string_element, "Chris")(
        "surname", element_type::string_element, "Manning")("yob", element_type::int32_element, 1991));
    ASSERT_EQ(3, boost::distance(doc));

    auto it = doc.insert(doc.begin(), element{"country", "United Kingdom"});
    ASSERT_EQ(doc.begin(), it);
    ASSERT_EQ(4, boost::distance(doc));

    const auto end = doc.end();
    ASSERT_NE(end, it);

    EXPECT_EQ("country", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("United Kingdom", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("first name", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Chris", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("surname", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Manning", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("yob", it->name());
    EXPECT_EQ(element_type::int32_element, it->type());
    EXPECT_EQ(1991, it->value<int32_t>());

    it++;
    ASSERT_EQ(end, it);
}

TEST(DocumentTest, DocumentInsertTest2) {
    document doc = document(builder("first name", element_type::string_element, "Chris")(
        "surname", element_type::string_element, "Manning")("yob", element_type::int32_element, 1991));
    ASSERT_EQ(3, boost::distance(doc));

    auto it = doc.insert(doc.end(), element{"country", "United Kingdom"});
    ASSERT_EQ(std::next(doc.begin(), 3), it);
    it = doc.begin();
    ASSERT_EQ(4, boost::distance(doc));

    const auto end = doc.end();
    ASSERT_NE(end, it);

    EXPECT_EQ("first name", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Chris", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("surname", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Manning", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("yob", it->name());
    EXPECT_EQ(element_type::int32_element, it->type());
    EXPECT_EQ(1991, it->value<int32_t>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("country", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("United Kingdom", it->value<std::string>());

    it++;
    ASSERT_EQ(end, it);
}

TEST(DocumentTest, DocumentEmplaceTest1) {
    document doc = document(builder("first name", element_type::string_element, "Chris")(
        "surname", element_type::string_element, "Manning")("yob", element_type::int32_element, 1991));
    ASSERT_EQ(3, boost::distance(doc));

    auto it = doc.emplace(doc.begin(), "country", "United Kingdom");
    ASSERT_EQ(doc.begin(), it);
    ASSERT_EQ(4, boost::distance(doc));

    const auto end = doc.end();
    ASSERT_NE(end, it);

    EXPECT_EQ("country", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("United Kingdom", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("first name", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Chris", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("surname", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Manning", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("yob", it->name());
    EXPECT_EQ(element_type::int32_element, it->type());
    EXPECT_EQ(1991, it->value<int32_t>());

    it++;
    ASSERT_EQ(end, it);
}

TEST(DocumentTest, DocumentEmplaceTest2) {
    document doc = document(builder("first name", element_type::string_element, "Chris")(
        "surname", element_type::string_element, "Manning")("yob", element_type::int32_element, 1991));
    ASSERT_EQ(3, boost::distance(doc));

    auto it = doc.emplace(doc.end(), "country", "United Kingdom");
    ASSERT_EQ(std::next(doc.begin(), 3), it);
    it = doc.begin();
    ASSERT_EQ(4, boost::distance(doc));

    const auto end = doc.end();
    ASSERT_NE(end, it);

    EXPECT_EQ("first name", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Chris", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("surname", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Manning", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("yob", it->name());
    EXPECT_EQ(element_type::int32_element, it->type());
    EXPECT_EQ(1991, it->value<int32_t>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("country", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("United Kingdom", it->value<std::string>());

    it++;
    ASSERT_EQ(end, it);
}

TEST(DocumentTest, DocumentEraseTest1) {
    document doc = document(builder("first name", element_type::string_element, "Chris")(
        "surname", element_type::string_element, "Manning")("yob", element_type::int32_element, 1991));
    ASSERT_EQ(3, boost::distance(doc));

    auto it = doc.erase(doc.begin());
    ASSERT_EQ(doc.begin(), it);
    ASSERT_NE(doc.end(), it);
    ASSERT_EQ(2, boost::distance(doc));

    const auto end = doc.end();
    ASSERT_NE(end, it);

    EXPECT_EQ("surname", it->name());
    EXPECT_EQ(element_type::string_element, it->type());
    EXPECT_EQ("Manning", it->value<std::string>());

    it++;
    ASSERT_NE(end, it);

    EXPECT_EQ("yob", it->name());
    EXPECT_EQ(element_type::int32_element, it->type());
    EXPECT_EQ(1991, it->value<int32_t>());

    it++;
    ASSERT_EQ(end, it);
}
