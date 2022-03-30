#include "../include/jsonValue.h"

namespace k_json {

    JsonValue::~JsonValue() {

    }

    /* 设置json变量类型 */
    void JsonValue::set_type(k_type t) {
        free();
        type_ = t;
    }

    /* 获取json变量类型 */
    k_type JsonValue::get_type() const noexcept {
        return type_;
    }

    void JsonValue::free() noexcept {
        using std::string;
        switch (type_)
        {
            case k_string: break;
            case k_array: break;
            case k_object: break;
        }
    }
}