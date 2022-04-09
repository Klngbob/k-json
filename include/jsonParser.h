#ifndef KJSON_JSONPARSER_H_
#define KJSON_JSONPARSER_H_

#include "json.h"
#include "jsonValue.h"

namespace k_json {
    class Parser final {
    public:
        explicit Parser(JsonValue &val, const std::string &context);
    private:
        void parse_whitespace() noexcept; /* 去除待解析json字符串中的多余空格 */
        void parse_value();
        void parse_literal(const char *literal, k_type t);
        void parse_number();
        void parse_hex4(size_t& index, unsigned &u);   /* 解析转义字符\uxxxx为十六进制并保存在u中 */
        void encode_utf8(std::string &s, unsigned &u) const noexcept;   /* 将转义序列编码成utf-8 */
        void parse_string_raw(std::string &s);
        void parse_string();
        void parse_array();
        void parse_object();

        JsonValue &val_;
        const char *cur_;
    };
}

#endif /* KJSON_JSONPARSER_H_ */