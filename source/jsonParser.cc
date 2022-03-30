#include <assert.h> /* assert() */
#include <errno.h>  /* errno, ERANGE */
#include <math.h>   /* HUGE_VAL */
#include <stdlib.h> /* strtod() */
#include "../include/jsonParser.h"
#include "../include/jsonException.h"

namespace k_json {
    inline void expect(const char* &c, char ch) {
        assert(*c == ch);
        ++c;
    }

    Parser::Parser(JsonValue &val, const std::string &context) 
        : val_(val), cur_(context.c_str()) {
            val_.set_type(k_null);
            parse_whitespace();
            parse_value();
            parse_whitespace();
            if(*cur_ != '\0') {
                val_.set_type(k_null);
                throw(Exception("Parse root not singular"));
            }
        }
    
    void Parser::parse_whitespace() {
        while(*cur_ == ' ' || *cur_ == '\t' || *cur_ == '\n' || *cur_ == '\r')
            cur_++;
    }

    void Parser::parse_value() {
        switch (*cur_)
        {
        case 'n'  : parse_literal("null", k_null); break;
        case 't'  : parse_literal("true", k_true); break;
        case 'f'  : parse_literal("false", k_false); break;
        case '\"' : parse_string(); break;
        case '['  : parse_array(); break;
        case '{'  : parse_object(); break;
        default   : parse_number(); break;
        case '\0' : throw(Exception("Expect value")); break;
        }
    }

}