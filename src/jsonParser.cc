#include <assert.h> /* assert() */
#include <errno.h>  /* errno, ERANGE */
#include <math.h>   /* HUGE_VAL */
#include <stdlib.h> /* strtod() */
#include "jsonParser.h"
#include "jsonException.h"

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
    
    /* 清除字符串空白格 */
    void Parser::parse_whitespace() noexcept {
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

    /* 解析json字面值 */
    void  Parser::parse_literal(const char *literal, k_type t) {
        expect(cur_, literal[0]);
        size_t i;
        for(i = 0; literal[i+1]; ++i) {
            if(cur_[i] != literal[i+1])
                throw(Exception("Parse invalid value"));
        }
        cur_ += i;
        val_.set_type(t);
    }

    /* 解析json数值 */
    void Parser::parse_number() {
        size_t i = 0;
        /* json数值只能以负号或数字开头 */
        if(cur_[i] == '-') i++;
        if(cur_[i] == '0') i++;
        else {
            if(!isdigit(cur_[i])) throw(Exception("Parse invalid value"));
            for(i++; isdigit(cur_[i]); i++);
        }
        if(cur_[i] == '.') { /* 如果是小数 */
            i++;
            if(!isdigit(cur_[i])) throw(Exception("Parse invalid value"));
            for(i++; isdigit(cur_[i]); i++);
        }
        if(cur_[i] == 'e' || cur_[i] == 'E') {
            i++;
            if(cur_[i] == '+' || cur_[i] == '-') i++;
            if(!isdigit(cur_[i])) throw(Exception("Parse invalid value"));
            for(i++; isdigit(cur_[i]); i++);
        }
        errno = 0;
        double n = strtod(cur_, NULL);
        if(errno == ERANGE && (n == HUGE_VAL || n == -HUGE_VAL))
            throw(Exception("Parse number too big"));
        val_.set_type(k_number);
        val_.set_number(n);
        cur_ += i;
    }

    /* 解析转义字符\uxxxx为十六进制并保存在u中 */
    void Parser::parse_hex4(size_t& index, unsigned &u) {
        size_t i = 0;
        u = 0;
        for(i; i < 4; ++i) {
            char ch = cur_[index + i];
            u <<= 4;
            if      (isdigit(ch)) u |= ch -'0';
            else if (ch >= 'A' && ch <= 'F') u |= ch - ('A' - 10);
            else if (ch >= 'a' && ch <= 'f') u |= ch - ('a' - 10);
            else throw(Exception("Parse invalid unicode hex"));
        }
        index += 4;
    }
    
    /* 将转义序列编码成utf-8 */
    void Parser::encode_utf8(std::string &s, unsigned &u) const noexcept {
        if(u <= 0x7F) s += static_cast<char>(u & 0xFF);
        else if (u <= 0x7FF) {
            s += static_cast<char>(0xC0 | ((u >> 6) & 0xFF));
            s += static_cast<char>(0x80 | (u        & 0x3F));
        }
        else if (u <= 0xFFFF) {
            s += static_cast<char>(0xE0 | ((u >> 12) & 0xFF));
            s += static_cast<char>(0x80 | ((u >> 6)  & 0x3F));
            s += static_cast<char>(0x80 | (u         & 0x3F));
        }
        else {
            assert(u <= 0x10FFFF);
            s += static_cast<char>(0xF0 | ((u >> 18) & 0xFF));
            s += static_cast<char>(0x80 | ((u >> 12) & 0x3F));
            s += static_cast<char>(0x80 | ((u >>  6) & 0x3F));
            s += static_cast<char>(0x80 | (u         & 0x3F));
        }
    }

    void Parser::parse_string_raw(std::string &s) {
        expect(cur_, '\"');
        size_t i = 0;
        unsigned uh, ul;
        while(cur_[i] != '\"') {
            char ch = cur_[i++];
            switch (ch)
            {
                case '\\' : /* utf-8转义字符 */
                    switch (cur_[i++])
                    {
                    case '\"': s+='\"'; break;
                    case '\\': s+='\\'; break;
                    case '/': s+='/'; break;
                    case 'b': s+='\b'; break;
                    case 'f': s+='\f'; break;
                    case 'n': s+='\n'; break;
                    case 'r': s+='\r'; break;
                    case 't': s+='\t'; break;
                    case 'u': /* 特殊转移字符 */
                        parse_hex4(i, uh); /* 解析高代理项 */
                        if(uh >= 0xD800 && uh <= 0xDBFF)  /* 高代理项 */
                        {
                            if(cur_[i] != '\\' || cur_[i+1] !='u')
                                throw(Exception("Parse invalid unicode surrogate")); /* 无效的代理项 */
                            i += 2;
                            parse_hex4(i, ul); /* 解析低代理项 */
                            if(ul < 0xDC00 || ul > 0xDFFF)
                                throw(Exception("Parse invalid unicode surrogate"));
                            /*
                                uh=H, ul=L, 计算码点:
                                codepoint = 0x10000 + (H − 0xD800) × 0x400 + (L − 0xDC00)
                            */
                            uh = 0x10000 + (((uh - 0xD800) << 10) | (ul - 0xDC00));
                            
                        }
                        encode_utf8(s, uh);
                        break;
                    default:
                        throw(Exception("Parse invalid string escape"));
                    }
                    break;
                case '\0' :
                    throw(Exception("Parse miss quotation mark"));
                default :
                    if((unsigned char)ch < 0x20)
                        throw(Exception("Parse invalid string char"));  /* 无效的编码单元 */
                    s+=ch;
                    break;
            }
        }
        cur_ += (i + 1); /* cur_指向字符\"后一个位置 */
    }

    /* 解析json字符串 */
    void Parser::parse_string() {
        std::string s;
        parse_string_raw(s);
        val_.set_string(s);
    }

    /* 解析数组 */
    void Parser::parse_array() {
        std::vector<JsonValue*> arr;
        expect(cur_, '[');
        parse_whitespace();
        if(*cur_ == ']') {
            cur_++;
            val_.set_array(arr);
        }
        else {
            for(;;) {
                try{
                    parse_value();
                } catch(Exception) {
                    val_.set_type(k_null);
                    throw;
                }
                JsonValue* p = new JsonValue(val_);
                arr.push_back(p);
                parse_whitespace();
                if(*cur_ == ',') {
                    cur_++;
                    parse_whitespace();
                }
                else if(*cur_ == ']') {
                    cur_++;
                    val_.set_array(arr);
                    return;
                }
                else {
                    val_.set_type(k_null);
                    throw(Exception("Parse miss comma or square bracket"));
                }
            }
        }
    }

    /* 解析Json对象 */
    void Parser::parse_object() {
        expect(cur_, '{');
        parse_whitespace();
        std::unordered_map<std::string, JsonValue*> tmp;
        std::string key;
        if(*cur_ == '}') {
            cur_++;
            val_.set_object(tmp);
            return;
        }
        for(;;) {
            if(*cur_ != '\"') throw(Exception("Parse miss key"));
            try{
                parse_string_raw(key);
            } catch(Exception) {
                throw(Exception("Parse miss key"));
            }
            parse_whitespace();
            if(*cur_++ != ':') throw(Exception("Parse miss colon"));
            parse_whitespace();
            try{
                parse_value();
            } catch(Exception) {
                val_.set_type(k_null);
                throw;
            }
            JsonValue* p = new JsonValue(val_);
            tmp.emplace(key, p);
            val_.set_type(k_null);
            key.clear();
            parse_whitespace();
            if(*cur_ == ',') {
                cur_++;
                parse_whitespace();
            }
            else if(*cur_ == '}') {
                cur_++;
                val_.set_object(tmp);
                return;
            }
            else {
                val_.set_type(k_null);
                throw(Exception("Parse miss comma or curly bracket"));
            }
        }
    }
}