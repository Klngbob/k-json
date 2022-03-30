#ifndef KJSON_JSON_H_
#define KJSON_JSON_H_

#include <memory>
#include <string>

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
}

class KJson final {
public:
    
    KJson() noexcept;
    ~KJson() noexcept;
private:
    std::unique_ptr<k_json::JsonValue> pv;
};

#endif /* KJSON_JSON_H_ */