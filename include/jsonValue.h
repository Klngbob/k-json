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
        void parse(const std::string &json); /* 提供json解析接口 */
        void stringify(std::string& json) const noexcept;  /* 生成Json到json中 */

        void set_type(k_type t) noexcept; /* 设置json变量类型 */
        k_type get_type() const noexcept; /* 获取json变量类型 */

        void set_number(double num) noexcept;   /* 设置json数值 */
        double get_number() const noexcept;     /* 获取json数值 */

        void set_string(const std::string &s) noexcept;   /* 设置json字符串 */
        const std::string get_string() const noexcept;     /* 获取json字符串 */

        void set_array(std::vector<JsonValue*>& arr) noexcept; /* 设置数组 */
        size_t get_array_size() const noexcept; /* 获取数组长度 */
        const JsonValue& get_array_element(size_t index) const noexcept;
        void pushback_array_element(JsonValue* val) noexcept;
        void popback_array_element() noexcept;
        void insert_array_element(JsonValue* val, size_t index) noexcept;
        void erase_array_element(size_t index, size_t count) noexcept;  /* 抹除count个元素 */
        void clear_array() noexcept;
        
        void set_object(std::unordered_map<std::string, JsonValue*>& obj) noexcept; /* 设置对象 */
        size_t get_object_size() const noexcept;
        bool find_object_key(const std::string& key) const; /* 查找是否存在相应的键 */
        const JsonValue& get_object_value(const std::string& key) const noexcept; /* 获取键对应的值 */
        const std::unordered_map<std::string, JsonValue*>& get_object() const noexcept; /* 供生成器使用 */
        void set_object_value(const std::string& key, JsonValue* value) noexcept; /* 设置键值对 */
        void remove_object_value(const std::string& key) noexcept;
        void clear_object() noexcept;

        JsonValue() { type_ = k_null; num_ = 0; }
        JsonValue(const JsonValue& rhs) noexcept { init_rhs(rhs); }
        JsonValue& operator=(const JsonValue& rhs) noexcept;
        ~JsonValue();
        
        friend bool operator==(const JsonValue& lhs, const JsonValue& rhs) noexcept;

    private:
        void init_rhs(const JsonValue& rhs) noexcept;
        void free() noexcept;   /* 释放JsonValue内存 */

        k_type type_ = k_null;
        union {
            double num_;
            std::string str_;
            std::vector<JsonValue*> arr_;
            std::unordered_map<std::string, JsonValue*> obj_;
        };
    };

    bool operator==(const JsonValue& lhs, const JsonValue& rhs) noexcept;
    bool operator!=(const JsonValue& lhs, const JsonValue& rhs) noexcept;
} /* namespace k_json */

#endif /* KJSON_JSONVALUE_H_ */
