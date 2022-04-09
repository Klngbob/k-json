#ifndef KJSON_JSONGENERATOR_H_
#define KJSON_JSONGENERATOR_H_

#include "jsonValue.h"

namespace k_json {
    
    class Generator final {
    public:
        Generator(const JsonValue* val, std::string& json);
    private:
        void stringify_value(const JsonValue* v);
        void stringify_string(const std::string& str);

        std::string& res_;
    };
    
}   /* namespace k_json */

#endif  /* KJSON_JSONGENERATOR_H_ */