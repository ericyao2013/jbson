//          Copyright Christian Manning 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>

#include <gtest/gtest.h>

#include <jbson/json_reader.hpp>
#include <jbson/json_writer.hpp>
using namespace jbson;

class PerfTest : public ::testing::Test {
public:
    virtual void SetUp() {
        std::ifstream ifs(JBSON_FILES"json_test_suite_sample.json");
        ASSERT_FALSE(ifs.fail());

        ifs.seekg(0, std::ios::end);
        auto n = static_cast<std::streamoff>(ifs.tellg());
        json_.resize(n);
        ifs.seekg(0, std::ios::beg);
        ifs.read(json_.data(), n);

        // whitespace test
        whitespace_.resize((1024 * 1024) + 3);
        char *p = whitespace_.data();
        for(size_t i = 0; i < whitespace_.size()-3; i += 4) {
            *p++ = ' ';
            *p++ = '\n';
            *p++ = '\r';
            *p++ = '\t';
        }
        *p++ = '[';
        *p++ = '0';
        *p++ = ']';

        json_reader r;
        ASSERT_NO_THROW(r.parse(json_));
        doc = document(std::move(r));
    }

    virtual void TearDown() {
    }

protected:
    std::vector<char> json_;
    std::vector<char> whitespace_;
    document doc;

    static const size_t kTrialCount = 1000;
};

TEST_F(PerfTest, WriteTest) {
    for (size_t i = 0; i < kTrialCount; i++) {
        auto str = std::string{};
        ASSERT_NO_THROW(write_json(doc, std::back_inserter(str)));
    }
}

TEST_F(PerfTest, ParseTest) {
    for (size_t i = 0; i < kTrialCount; i++) {
        json_reader reader;
        ASSERT_NO_THROW(reader.parse(json_));
    }
}

TEST_F(PerfTest, WhitespaceTest) {
    for (size_t i = 0; i < kTrialCount; i++) {
        json_reader reader;
        ASSERT_NO_THROW(reader.parse(whitespace_));
        auto arr = array(std::move(reader));
        auto beg = arr.begin();
        ASSERT_NE(arr.end(), beg);
        ASSERT_EQ("0", beg->name());
        ASSERT_EQ(0, beg->value<int32_t>());
    }
}

TEST_F(PerfTest, LvalueTest) {
    for (size_t i = 0; i < kTrialCount; i++) {
        json_reader reader;
        ASSERT_NO_THROW(reader.parse(json_));
        auto d = document(reader);
    }
}

TEST_F(PerfTest, RvalueTest) {
    for (size_t i = 0; i < kTrialCount; i++) {
        json_reader reader;
        ASSERT_NO_THROW(reader.parse(json_));
        auto d = document(std::move(reader));
    }
}

TEST_F(PerfTest, ParseToSetTest) {
    for (size_t i = 0; i < kTrialCount; i++) {
        json_reader reader;
        ASSERT_NO_THROW(reader.parse(json_));
        auto set = document_set(document(std::move(reader)));
    }
}