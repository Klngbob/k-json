#ifndef KJSON_JSONEXCEPTION_H_
#define KJSON_JSONEXCEPTION_H_

#include <string>
#include <stdexcept>

namespace k_json {

    class Exception final : public std::logic_error {
    public:
        Exception(const std::string &errMsg) : logic_error(errMsg) { }
    };
}

#endif /* KJSON_JSONEXCEPTION_H_ */