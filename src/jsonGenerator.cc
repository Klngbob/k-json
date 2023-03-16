#include "jsonGenerator.h"

namespace k_json
{
    Generator::Generator(const JsonValue* val, std::string &json) :res_(json) {
        res_.clear();
        stringify_value(val);
    }

    void Generator::stringify_value(const JsonValue* v) {
        char buffer[32] = {0};  /* 避免在case中定义变量 */
        int i;
        switch (v->get_type())
        {
        case k_null: res_ += "null"; break;
        case k_true: res_ += "true"; break;
        case k_false: res_ += "false"; break;
        case k_string: stringify_string(v->get_string()); break;
        case k_number:
            sprintf(buffer, "%.17g", v->get_number());
            res_ += buffer;
            break;
        case k_array:
            res_ += '[';
            for(i = 0; i < v->get_array_size(); ++i) {
                if(i > 0) res_ += ',';
                stringify_value(&v->get_array_element(i));
            }
            res_ += ']';
            break;
        case k_object:
            res_ += '{';
            i = 0;
            for(auto it = v->get_object().begin(); it != v->get_object().end(); ++it, ++i) {
                if(i > 0) res_ += ',';
                stringify_string(it->first);
                res_ += ':';
                stringify_value(it->second);
            }
            res_ += '}';
            break;
        }
    }

    void Generator::stringify_string(const std::string& str) {
        res_ += '\"';
        for(auto it = str.begin(); it != str.end(); ++it) {
            unsigned char ch = *it;
            char buffer[7] = {0};
            switch (ch)
            {
            case '\"': res_ += "\\\""; break;
            case '\\': res_ += "\\\\"; break;
             case '\b': res_ += "\\b";  break;
            case '\f': res_ += "\\f";  break;
            case '\n': res_ += "\\n";  break;
            case '\r': res_ += "\\r";  break;
            case '\t': res_ += "\\t";  break;
            default:
                if(ch < 0x20) {
                    sprintf(buffer, "\\u%04X", ch);
                    res_ += buffer;
                }
                else {
                    res_ += *it;
                }
                break;
            }
        }
        res_ += '\"';
    }
} /* namespace k_json */
