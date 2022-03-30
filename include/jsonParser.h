#ifndef KJSON_JSONPARSER_H_
#define KJSON_JSONPARSER_H_

#include "json.h"
#include "jsonValue.h"

namespace k_json {
    class Parser final {
    public:
        Parser(JsonValue &val, const std::string &context);
    private:
        void parse_whitespace() noexcept; /* 去除待解析json字符串中的多余空格 */
        void parse_value();
        void parse_literal(const char *literal, k_type t);
        void parse_number();
        void parse_string();
        void parse_array();
        void parse_object();

        JsonValue &val_;
        const char *cur_;
    };
}

#endif /* KJSON_JSONPARSER_H_ */