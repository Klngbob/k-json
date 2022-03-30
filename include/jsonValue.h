#ifndef KJSON_JSONVALUE_H_
#define KJSON_JSONVALUE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "json.h"

namespace k_json
{
    class JsonValue final {
    public:
        void set_type(k_type t) noexcept; /* 设置json变量类型 */
        k_type get_type() const noexcept; /* 获取json变量类型 */

        JsonValue() { num_ = 0; }
        ~JsonValue();
    private:
        void free() noexcept;   /* 释放JsonValue内存 */

        k_type type_ = k_null;
        union {
            double num_;
            std::string str_;
            std::vector<JsonValue> arr_;
            std::unordered_map<std::string, JsonValue> obj_;
        };
        
    };
} /* namespace k_json */

#endif /* KJSON_JSONVALUE_H_ */
