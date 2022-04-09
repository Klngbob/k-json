#ifndef KJSON_JSON_H_
#define KJSON_JSON_H_

#include <memory>
#include <string>
#include <utility>

namespace k_json {
    enum k_type : int {
        k_null,
        k_true,
        k_false,
        k_number,
        k_string,
        k_array,
        k_object
    };

    class JsonValue;


class KJson final {
public:
    void parse(const std::string& json, std::string& status);
    void stringify(std::string& json) const noexcept;   /* 生成Json到字符串json中 */

    k_json::k_type get_type() const noexcept;

    void set_null() noexcept; /* 设置为null */
    void set_boolean(bool b) noexcept; /* 设置boolean */

    double get_number() const noexcept;
    void set_number(double num) noexcept; /* 设置数值 */

    const std::string get_string() const noexcept;
    void set_string(const std::string& str) noexcept; /* 设置字符串 */

    void set_array() noexcept; /* 设置数组 */
    size_t get_array_size() const noexcept; /* 获取数组长度 */
    KJson get_array_element(size_t index) const noexcept;
    void pushback_array_element(const KJson& val) noexcept;
    void popback_array_element() noexcept;
    void insert_array_element(const KJson& val, size_t index) noexcept;
    void erase_array_element(size_t index, size_t count) noexcept;  /* 抹除count个元素 */
    void clear_array() noexcept;

    void set_object() noexcept;
    size_t get_object_size() const noexcept;
    bool find_object_key(const std::string& key) const;
    KJson get_object_value(const std::string& key) const noexcept;
    void set_object_value(const std::string& key, const KJson& value) noexcept; /* 设置键值对 */
    void remove_object_value(const std::string& key) noexcept;
    void clear_object() noexcept;

    KJson() noexcept;
    ~KJson() noexcept;
    KJson(const KJson& rhs) noexcept;
    KJson& operator=(const KJson& rhs) noexcept;
    KJson(KJson&& rhs) noexcept; 
    KJson& operator=(KJson&& rhs) noexcept; /* std::move函数中需要转换为右值引用 */
    void swap(KJson& rhs) noexcept;
    friend bool operator==(const KJson& lhs, const KJson& rhs) noexcept;
    friend bool operator!=(const KJson& lhs, const KJson& rhs) noexcept;

private:
    void parse(const std::string &json);
    std::unique_ptr<k_json::JsonValue> pv;

};

bool operator==(const KJson& lhs, const KJson& rhs) noexcept;
bool operator!=(const KJson& lhs, const KJson& rhs) noexcept;
void swap(KJson& lhs, KJson& rhs) noexcept;

} /* namespace k_json */
#endif /* KJSON_JSON_H_ */