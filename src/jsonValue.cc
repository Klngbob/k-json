#include <assert.h>
#include "jsonValue.h"
#include "jsonParser.h"
#include "jsonGenerator.h"

namespace k_json {

    /* 提供json解析接口 */
    void JsonValue::parse(const std::string &json) {
        Parser(*this, json);
    }

    /* 生成Json到json中 */
    void JsonValue::stringify(std::string& json) const noexcept {
        Generator(this, json);
    } 

    /* 设置json变量类型 */
    void JsonValue::set_type(k_type t) noexcept {
        free();
        type_ = t;
    }

    /* 获取json变量类型 */
    k_type JsonValue::get_type() const noexcept {
        return type_;
    }

    /* 设置json数值 */
    void JsonValue::set_number(double num) noexcept {
        free();
        type_ = k_number;
        num_ = num;
    }

    /* 获取json数值 */
    double JsonValue::get_number() const noexcept {
        assert(type_ == k_number);
        return num_;
    }

    /* 设置json字符串 */
    void JsonValue::set_string(const std::string &s) noexcept {
        if(type_ == k_string)
            str_ = s;
        else {
            free();
            type_ = k_string;
            new(&str_) std::string(s);
        }
    }

    /* 获取json字符串 */
    const std::string JsonValue::get_string() const noexcept {
        assert(type_ == k_string);
        return str_;
    }

    size_t JsonValue::get_array_size() const noexcept {
        assert(type_ == k_array);
        return arr_.size();
    }

    const JsonValue& JsonValue::get_array_element(size_t index) const noexcept {
        assert(type_ == k_array);
        return *arr_[index];
    }

    void JsonValue::pushback_array_element(JsonValue* val) noexcept {
        assert(type_ == k_array);
        arr_.push_back(val);
    }

    void JsonValue::popback_array_element() noexcept {
        assert(type_ == k_array);
        arr_.pop_back();
    }

    void JsonValue::insert_array_element(JsonValue* val, size_t index) noexcept {
        assert(type_ == k_array);
        arr_.insert(arr_.begin()+index, val);
    }

    /* 抹除count个元素 */
    void JsonValue::erase_array_element(size_t index, size_t count) noexcept {
        assert(type_ == k_array);
        arr_.erase(arr_.begin()+index, arr_.begin()+index+count);
    }

    void JsonValue::clear_array() noexcept {
        assert(type_ == k_array);
        arr_.clear();
    }

    /* 设置数组 */
    void JsonValue::set_array(std::vector<JsonValue*>& arr) noexcept {
        if(type_ == k_array) {
            arr_ = arr;
        }
        else {
            free();
            type_ = k_array;
            new(&arr_) std::vector<JsonValue*>(arr);
        }
    }
    /* 设置对象 */
    void JsonValue::set_object(std::unordered_map<std::string, JsonValue*>& obj) noexcept {
        if(type_ == k_object) {
            obj_ = obj;
        }
        else {
            free();
            type_ = k_object;
            new(&obj_) std::unordered_map<std::string, JsonValue*>(obj); 
        }
    } 

    size_t JsonValue::get_object_size() const noexcept {
        assert(type_ == k_object);
        return obj_.size();
    }

    /* 查找是否存在相应的键 */
    bool JsonValue::find_object_key(const std::string& key) const {
        assert(type_ == k_object);
        return obj_.find(key) != obj_.end();
    }

    const JsonValue& JsonValue::get_object_value(const std::string& key) const noexcept {
        assert(type_ == k_object);
        return *(obj_.at(key));
    }

    /* 供生成器使用 */
    const std::unordered_map<std::string, JsonValue*>& JsonValue::get_object() const noexcept {
        assert(type_ == k_object);
        return obj_;
    }

    /* 设置键值对 */
    void JsonValue::set_object_value(const std::string& key, JsonValue* value) noexcept {
        assert(type_ == k_object);
        obj_[key] = value;
    }

    void JsonValue::remove_object_value(const std::string& key) noexcept {
        assert(type_ == k_object);
        obj_.erase(key);
    }

    void JsonValue::clear_object() noexcept {
        assert(type_ == k_object);
        obj_.clear();
    }

    void JsonValue::init_rhs(const JsonValue& rhs) noexcept {
        type_ = rhs.type_;
        num_ = 0;
        switch (type_)
        {
        case k_number: num_ = rhs.num_; 
            break;
        case k_string: new(&str_) std::string(rhs.str_);
            break;
        case k_array: new(&arr_) std::vector<JsonValue*>(rhs.arr_);
            break;
        case k_object: new(&obj_) std::unordered_map<std::string, JsonValue*>(rhs.obj_);
            break;
        }
    }

    void JsonValue::free() noexcept {
        using std::string;
        using std::unordered_map;
        int i;
        switch (type_)
        {
            case k_string: 
                str_.~string();
                break;
            case k_array: 
                arr_.~vector<JsonValue*>();
                break;
            case k_object: 
                obj_.~unordered_map<string, JsonValue*>();
                break;
        }
        type_ = k_null;
    }

    JsonValue::~JsonValue() {
        free();
    }

    JsonValue& JsonValue::operator=(const JsonValue& rhs) noexcept
    {
        free(); 
        init_rhs(rhs); 
    }

    bool operator==(const JsonValue& lhs, const JsonValue& rhs) noexcept {
        int i;
        if(lhs.type_ != rhs.type_) return false;
        switch (lhs.type_)
        {
        case k_number: return lhs.num_ == rhs.num_;
        case k_string: return lhs.str_ == rhs.str_;
        case k_array:  
            if(lhs.arr_.size() != rhs.arr_.size()) return false;
            for(i = 0; i < lhs.arr_.size(); ++i) {
                if(*(lhs.arr_[i]) != *(rhs.arr_[i])){
                    return false;
                }
            }
            return true;
        case k_object:
            if(lhs.obj_.size() != rhs.obj_.size()) return false;
            for(auto it = lhs.obj_.begin(); it !=lhs.obj_.end(); ++it) {
                if(rhs.obj_.find(it->first) != rhs.obj_.end()){ /* rhs找到对应键值对 */
                    if(*(it->second) != *(rhs.obj_.at(it->first))){
                        return false;
                    }
                }
                else {
                    return false;
                }
            }
            return true;
       default:
            return true;
        }
    }

    bool operator!=(const JsonValue& lhs, const JsonValue& rhs) noexcept {
        return !(lhs == rhs);
    }
} /* namespace k_json */