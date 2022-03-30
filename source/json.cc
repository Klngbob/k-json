#include "../include/json.h"
#include "../include/jsonParser.h"
#include "../include/jsonValue.h"

namespace k_json {

    KJson::KJson() noexcept: pv(new k_json::JsonValue) { }

    KJson::~KJson() noexcept { }   /* pv中申请的内存由JsonValue类的析构函数释放 */
}

