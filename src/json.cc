#include "json.h"
#include "jsonParser.h"
#include "jsonValue.h"
#include "jsonException.h"

namespace k_json {

    void KJson::parse(const std::string &json)
    {
        pv->parse(json);
    }

    void KJson::parse(const std::string &json, std::string &status)
    {
        try
        {
            parse(json);
            status = "Parse ok";
        }
        catch (const k_json::Exception &msg)
        {
            status = msg.what();
        }
        catch (...)
        {
        }
    }

    /* 生成Json到字符串json中 */
    void KJson::stringify(std::string& json) const noexcept {
        pv->stringify(json);
    }

    k_json::k_type KJson::get_type() const noexcept
    {
        if(pv == nullptr)
            return k_null;
        return pv->get_type();
    }

    void KJson::set_null() noexcept {
        pv->set_type(k_json::k_null);
    }

    /* 设置boolean */
    void KJson::set_boolean(bool b) noexcept {
        if(b) {
            pv->set_type(k_json::k_true);
        }
        else {
            pv->set_type(k_json::k_false);
        }
    }

    /* 设置数字 */
    void KJson::set_number(double num) noexcept {
        pv->set_number(num);
    }

    double KJson::get_number() const noexcept {
        return pv->get_number();
    }

    /* 设置字符串 */
    void KJson::set_string(const std::string& str) noexcept {
        pv->set_string(str);
    }

    const std::string KJson::get_string() const noexcept {
        return pv->get_string();
    }

    /* 设置数组 */
    void KJson::set_array() noexcept {
        std::vector<k_json::JsonValue*> arr={};
        pv->set_array(arr);
    }

    /* 获取数组长度 */
    size_t KJson::get_array_size() const noexcept {
        return pv->get_array_size();
    }

    KJson KJson::get_array_element(size_t index) const noexcept {
        KJson ret;
        ret.pv.reset(new k_json::JsonValue(pv->get_array_element(index)));
        return ret;
    }

    void KJson::pushback_array_element(const KJson& val) noexcept {
        k_json::JsonValue tmp= *val.pv;
        k_json::JsonValue *p = new k_json::JsonValue(tmp);
        pv->pushback_array_element(p);
    }

    void KJson::popback_array_element() noexcept {
        pv->popback_array_element();
    }

    void KJson::insert_array_element(const KJson& val, size_t index) noexcept {
        k_json::JsonValue tmp= *val.pv;
        k_json::JsonValue *p = new k_json::JsonValue(tmp);
        pv->insert_array_element(p, index);
    }

    /* 抹除count个元素 */
    void KJson::erase_array_element(size_t index, size_t count) noexcept {
        pv->erase_array_element(index, count);
    }

    void KJson::clear_array() noexcept {
        pv->clear_array();
    }

    void KJson::set_object() noexcept {
        std::unordered_map<std::string, k_json::JsonValue*> obj;
        pv->set_object(obj);
    }

    size_t KJson::get_object_size() const noexcept {
        return pv->get_object_size();
    }

    bool KJson::find_object_key(const std::string& key) const {
        return pv->find_object_key(key);
    }

    KJson KJson::get_object_value(const std::string& key) const noexcept {
        KJson ret;
        ret.pv.reset(new k_json::JsonValue(pv->get_object_value(key)));
        return ret;
    }

    /* 设置键值对 */
    void KJson::set_object_value(const std::string& key, const KJson& value) noexcept {
        k_json::JsonValue tmp= *value.pv;
        k_json::JsonValue *p = new k_json::JsonValue(tmp);
        pv->set_object_value(key, p);
    }

    void KJson::remove_object_value(const std::string& key) noexcept {
        pv->remove_object_value(key);
    }

    void KJson::clear_object() noexcept {
        pv->clear_object();
    }


    KJson::KJson() noexcept : pv(new k_json::JsonValue) {}
    KJson::KJson(const KJson& rhs) noexcept {
        pv.reset(new k_json::JsonValue(*(rhs.pv)));
    }

    KJson::~KJson() noexcept {} /* pv中申请的内存由JsonValue类的析构函数释放 */

    KJson& KJson::operator=(const KJson& rhs) noexcept {
        pv.reset(new k_json::JsonValue(*(rhs.pv)));
        return *this;
    }

    KJson::KJson(KJson&& rhs) noexcept {
        pv.reset(rhs.pv.release());
    }

    KJson& KJson::operator=(KJson&& rhs) noexcept {
        pv.reset(rhs.pv.release());
        return *this;
    }

    void KJson::swap(KJson& rhs) noexcept {
        std::swap(pv, rhs.pv);
    }

    bool operator==(const KJson& lhs, const KJson& rhs) noexcept {
        return *lhs.pv == *rhs.pv;
    }

    bool operator!=(const KJson& lhs, const KJson& rhs) noexcept {
        return *lhs.pv != *rhs.pv;
    }

    void swap(KJson& lhs, KJson& rhs) noexcept {
        lhs.swap(rhs);
    }

}   /* namespace k_json */