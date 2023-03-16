#include <iostream>
#include <string.h>
#include "json.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;
static std::string status;

using namespace k_json;

#define EXPECT_EQ_BASE(expect, actual) \
    do {\
        test_count++;\
        if (expect == actual)\
            test_pass++;\
        else {\
            std::cerr << __FILE__ << ":" << __LINE__ << ": expect: "<< expect << " actual: " << actual <<std::endl;\
            main_ret = 1;\
        }\
    } while(0)


#define TEST_LITERAL(expect, json)\
    do {\
        KJson kj;\
        kj.parse(json, status);\
        EXPECT_EQ_BASE("Parse ok", status);\
        EXPECT_EQ_BASE(expect, kj.get_type());\
    } while(0)

/* 测试解析字面量 */
static void test_parse_literal() {
    TEST_LITERAL(k_null, "null");
    TEST_LITERAL(k_true, "true");
    TEST_LITERAL(k_false, "false");
}

#define TEST_NUMBER(expect, json)\
    do {\
        KJson kj;\
        kj.parse(json, status);\
        EXPECT_EQ_BASE("Parse ok", status);\
        EXPECT_EQ_BASE(k_number, kj.get_type());\
        EXPECT_EQ_BASE(expect, kj.get_number());\
    } while(0)

/* 测试解析数值 */
static void test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

#define TEST_STRING(expect, json)\
    do {\
        KJson kj;\
        kj.parse(json, status);\
        EXPECT_EQ_BASE("Parse ok", status);\
        EXPECT_EQ_BASE(k_string, kj.get_type());\
        EXPECT_EQ_BASE(0, memcmp(expect, kj.get_string().c_str(), kj.get_string().size()));\
    } while(0)


/* 测试解析字符串 */
static void test_parse_string() {
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
    TEST_STRING("Hello\0World", "\"Hello\\u0000World\"");
    TEST_STRING("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
    TEST_STRING("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
    TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
}

/* 测试解析数组 */
static void test_parse_array() {
    KJson v;

    v.parse("[ ]", status);
    EXPECT_EQ_BASE("Parse ok", status);
    EXPECT_EQ_BASE(k_array, v.get_type());
    EXPECT_EQ_BASE(0, v.get_array_size());

    v.parse("[ null , false , true , 123 , \"abc\" ]", status);
    EXPECT_EQ_BASE("Parse ok", status);
    EXPECT_EQ_BASE(k_array, v.get_type());
    EXPECT_EQ_BASE(5, v.get_array_size());
    EXPECT_EQ_BASE(k_null, v.get_array_element(0).get_type());
    EXPECT_EQ_BASE(k_false, v.get_array_element(1).get_type());
    EXPECT_EQ_BASE(k_true, v.get_array_element(2).get_type());
    EXPECT_EQ_BASE(123.0, v.get_array_element(3).get_number());
    EXPECT_EQ_BASE("abc", v.get_array_element(4).get_string());

    v.parse("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]", status);
    EXPECT_EQ_BASE("Parse ok", status);
    EXPECT_EQ_BASE(k_array, v.get_type());
    EXPECT_EQ_BASE(4, v.get_array_size());
    for(int i=0; i<4; ++i) {
        KJson a = v.get_array_element(i);
        EXPECT_EQ_BASE(k_array, a.get_type());
        EXPECT_EQ_BASE(i, a.get_array_size());
        for(int j=0; j<i; ++j) {
            KJson e = a.get_array_element(j);
            EXPECT_EQ_BASE(k_array, a.get_type());
            EXPECT_EQ_BASE((double)j, e.get_number());
        }
    }
}

/* 测试解析对象 */
static void test_parse_object() {
    KJson kj;
    kj.parse(" { } ", status);
    EXPECT_EQ_BASE("Parse ok", status);
    EXPECT_EQ_BASE(k_object, kj.get_type());
    EXPECT_EQ_BASE(0, kj.get_object_size());

    kj.parse(" { "
	        "\"n\" : null , "
	        "\"f\" : false , "
	        "\"t\" : true , "
	        "\"i\" : 123 , "
	        "\"s\" : \"abc\", "
	        "\"a\" : [ 1, 2, 3 ],"
	        "\"o\" : { \"1\" : 1, \"2\" : 2, \"3\" : 3 }"
	        " } ", status);
    EXPECT_EQ_BASE("Parse ok", status);
    EXPECT_EQ_BASE(k_object, kj.get_type());
    EXPECT_EQ_BASE(7, kj.get_object_size());

    EXPECT_EQ_BASE(true, kj.find_object_key("n"));
    EXPECT_EQ_BASE(k_null, kj.get_object_value("n").get_type());

    EXPECT_EQ_BASE(true, kj.find_object_key("f"));
    EXPECT_EQ_BASE(k_false, kj.get_object_value("f").get_type());

    EXPECT_EQ_BASE(true, kj.find_object_key("t"));
    EXPECT_EQ_BASE(k_true, kj.get_object_value("t").get_type());

    EXPECT_EQ_BASE(true, kj.find_object_key("i"));
    EXPECT_EQ_BASE(k_number, kj.get_object_value("i").get_type());
    EXPECT_EQ_BASE(123.0, kj.get_object_value("i").get_number());

    EXPECT_EQ_BASE(true, kj.find_object_key("s"));
    EXPECT_EQ_BASE(k_string, kj.get_object_value("s").get_type());
    EXPECT_EQ_BASE("abc", kj.get_object_value("s").get_string());

    EXPECT_EQ_BASE(true, kj.find_object_key("a"));
    EXPECT_EQ_BASE(k_array, kj.get_object_value("a").get_type());
    EXPECT_EQ_BASE(3, kj.get_object_value("a").get_array_size());
    for(int i = 0; i < 3; ++i) {
        KJson a = kj.get_object_value("a").get_array_element(i);
        EXPECT_EQ_BASE(k_number, a.get_type());
        EXPECT_EQ_BASE(i + 1.0, a.get_number());
    }

    EXPECT_EQ_BASE(true, kj.find_object_key("o"));
    EXPECT_EQ_BASE(k_object, kj.get_object_value("o").get_type());
    EXPECT_EQ_BASE(3, kj.get_object_value("o").get_object_size());
    for(int i = 0; i < 3; ++i) {
        char tmp = '1' + i;
        std::string key;
        key.push_back(tmp);
        EXPECT_EQ_BASE(true, kj.get_object_value("o").find_object_key(key));
        KJson o = kj.get_object_value("o").get_object_value(key);
        EXPECT_EQ_BASE(k_number, o.get_type());
        EXPECT_EQ_BASE(i + 1.0, o.get_number());
    }
}

#define TEST_ERROR(error, json) \
    do {\
        KJson kj;\
        kj.parse(json, status);\
        EXPECT_EQ_BASE(error, status);\
        EXPECT_EQ_BASE(k_null, kj.get_type());\
    } while (0)
    

/* 测试解析“期望一个值” */
static void test_parse_expect_value() {
	TEST_ERROR("Expect value", "");
	TEST_ERROR("Expect value", " ");
}

/* 测试解析无效值 */
static void test_parse_invalid_value() {
    TEST_ERROR("Parse invalid value", "nul");
    TEST_ERROR("Parse invalid value", "?");

    /* invalid number */
    TEST_ERROR("Parse invalid value", "+0");
    TEST_ERROR("Parse invalid value", "+1");
    TEST_ERROR("Parse invalid value", ".123"); /* at least one digit before '.' */
    TEST_ERROR("Parse invalid value", "1.");   /* at least one digit after '.' */
    TEST_ERROR("Parse invalid value", "INF");
    TEST_ERROR("Parse invalid value", "inf");
    TEST_ERROR("Parse invalid value", "NAN");
    TEST_ERROR("Parse invalid value", "nan");

    /* invalid value in array */
    TEST_ERROR("Parse invalid value", "[1,]");
    TEST_ERROR("Parse invalid value", "[\"a\", nul]");
}

/* 测试解析不止一个数据 */
static void test_parse_root_not_singular() {
    TEST_ERROR("Parse root not singular", "null x");

    /* invalid number */
    TEST_ERROR("Parse root not singular", "0123"); /* after zero should be '.' or nothing */
    TEST_ERROR("Parse root not singular", "0x0");
    TEST_ERROR("Parse root not singular", "0x123");
}

/* 测试解析数值溢出 */
static void test_parse_number_too_big() {
    TEST_ERROR("Parse number too big", "1e309");
    TEST_ERROR("Parse number too big", "-1e309");
}

/* 测试解析字符串引号缺失 */
static void test_parse_miss_quotation_mark() {
    TEST_ERROR("Parse miss quotation mark", "\"");
    TEST_ERROR("Parse miss quotation mark", "\"abc");
}

/* 测试解析字符串中无效的转义字符 */
static void test_parse_invalid_string_escape() {
    TEST_ERROR("Parse invalid string escape", "\"\\v\"");
    TEST_ERROR("Parse invalid string escape", "\"\\'\"");
    TEST_ERROR("Parse invalid string escape", "\"\\0\"");
    TEST_ERROR("Parse invalid string escape", "\"\\x12\"");
}

/* 测试解析字符串中无效的字符（无效的编码单元） */
static void test_parse_invalid_string_char() {
    TEST_ERROR("Parse invalid string char", "\"\x01\"");
    TEST_ERROR("Parse invalid string char", "\"\x1F\"");
}

/* 测试解析字符串中不合法的十六进位编码 */
static void test_parse_invalid_unicode_hex() {
    TEST_ERROR("Parse invalid unicode hex", "\"\\u\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u0\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u01\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u012\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u/000\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\uG000\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u0/00\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u0G00\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u0/00\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u00G0\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u000/\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u000G\"");
    TEST_ERROR("Parse invalid unicode hex", "\"\\u 123\"");
}

/* 测试解析字符串中不正确的代理项 */
static void test_parse_invalid_unicode_surrogate() {
    TEST_ERROR("Parse invalid unicode surrogate", "\"\\uD800\"");
    TEST_ERROR("Parse invalid unicode surrogate", "\"\\uDBFF\"");
    TEST_ERROR("Parse invalid unicode surrogate", "\"\\uD800\\\\\"");
    TEST_ERROR("Parse invalid unicode surrogate", "\"\\uD800\\uDBFF\"");
    TEST_ERROR("Parse invalid unicode surrogate", "\"\\uD800\\uE000\"");
}

/* 测试解析数组中缺失逗号或 ] */
static void test_parse_miss_comma_or_square_bracket() {
    TEST_ERROR("Parse miss comma or square bracket", "[1");
    TEST_ERROR("Parse miss comma or square bracket", "[1}");
    TEST_ERROR("Parse miss comma or square bracket", "[1 2");
    TEST_ERROR("Parse miss comma or square bracket", "[[]");
}

/* 测试解析对象缺失键 */
static void test_parse_miss_key() {
    TEST_ERROR("Parse miss key", "{:1,");
    TEST_ERROR("Parse miss key", "{1:1,");
    TEST_ERROR("Parse miss key", "{true:1,");
    TEST_ERROR("Parse miss key", "{false:1,");
    TEST_ERROR("Parse miss key", "{null:1,");
    TEST_ERROR("Parse miss key", "{[]:1,");
    TEST_ERROR("Parse miss key", "{{}:1,");
    TEST_ERROR("Parse miss key", "{\"a\":1,");
}

/* 测试解析对象缺失冒号 */
static void test_parse_miss_colon() {
    TEST_ERROR("Parse miss colon", "{\"a\"}");
    TEST_ERROR("Parse miss colon", "{\"a\",\"b\"}");
}

/* 测试解析对象缺失都好或 } */
static void test_parse_miss_comma_or_curly_bracket() {
    TEST_ERROR("Parse miss comma or curly bracket", "{\"a\":1");
    TEST_ERROR("Parse miss comma or curly bracket", "{\"a\":1]");
    TEST_ERROR("Parse miss comma or curly bracket", "{\"a\":1 \"b\"");
    TEST_ERROR("Parse miss comma or curly bracket", "{\"a\":{}");
}

static void test_parse() {
    test_parse_literal();
    test_parse_number();
    test_parse_string();
    test_parse_array();
    test_parse_object();

    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
    test_parse_miss_quotation_mark();
    test_parse_invalid_string_escape();
    test_parse_invalid_string_char();
    test_parse_invalid_unicode_hex();
    test_parse_invalid_unicode_surrogate();
    test_parse_miss_comma_or_square_bracket();
    test_parse_miss_key();
    test_parse_miss_colon();
    test_parse_miss_comma_or_curly_bracket();
}

/* 测试访问null */
static void test_access_null() {
    KJson kj;
    kj.set_string("a");
    kj.set_null();
    EXPECT_EQ_BASE(k_null, kj.get_type());
}

/* 测试访问布尔值 */
static void test_access_boolean() {
    KJson kj;
    kj.set_string("a");
    kj.set_boolean(true);
    EXPECT_EQ_BASE(k_true, kj.get_type());
    kj.set_boolean(false);
    EXPECT_EQ_BASE(k_false, kj.get_type());
}

/* 测试访问数值 */
static void test_access_number() {
    KJson kj;
    kj.set_string("a");
    kj.set_number(1234.5);
    EXPECT_EQ_BASE(1234.5, kj.get_number());
}

/* 测试访问字符串 */
static void test_access_string() {
    KJson kj;
    kj.set_string("");
    EXPECT_EQ_BASE("", kj.get_string());
    kj.set_string("Hello");
    EXPECT_EQ_BASE("Hello", kj.get_string());
}

/* 测试访问json数组 */
static void test_access_array() {
    KJson a, e;
    
    for(size_t j = 0; j < 5; j += 5) {
        a.set_array();
        EXPECT_EQ_BASE(0, a.get_array_size());
        for(int i = 0; i < 10; ++i) {
            e.set_number(i);
            a.pushback_array_element(e);
        }

        EXPECT_EQ_BASE(10, a.get_array_size());
        for(int i = 0; i < 10; ++i) {
            EXPECT_EQ_BASE(static_cast<double>(i), a.get_array_element(i).get_number());
        }
    }

    a.popback_array_element();
    EXPECT_EQ_BASE(9, a.get_array_size());
    for(int i = 0; i < 9; ++i) {
        EXPECT_EQ_BASE(static_cast<double>(i), a.get_array_element(i).get_number());
    }

    a.erase_array_element(4, 0);
    EXPECT_EQ_BASE(9, a.get_array_size());
    for(int i = 0; i < 9; ++i) {
        EXPECT_EQ_BASE(static_cast<double>(i), a.get_array_element(i).get_number());
    }

    a.erase_array_element(8, 1);
    EXPECT_EQ_BASE(8, a.get_array_size());
    for(int i = 0; i < 8; ++i) {
        EXPECT_EQ_BASE(static_cast<double>(i), a.get_array_element(i).get_number());
    }

    a.erase_array_element(0, 2);
    EXPECT_EQ_BASE(6, a.get_array_size());
    for(int i = 0; i < 6; ++i) {
        EXPECT_EQ_BASE(static_cast<double>(i) + 2, a.get_array_element(i).get_number());
    }

    for(int i = 0; i < 2; ++i) {
        e.set_number(i);
        a.insert_array_element(e, i);
    }

    EXPECT_EQ_BASE(8, a.get_array_size());
    for(int i = 0; i < 8; ++i) {
        EXPECT_EQ_BASE(static_cast<double>(i), a.get_array_element(i).get_number());
    }

    e.set_string("Hello");
    a.pushback_array_element(e);
    
    a.clear_array();
    EXPECT_EQ_BASE(0, a.get_array_size());
}

/* 测试访问json对象 */
static void test_access_object() {
    KJson o, v;

    for(int j = 0; j <= 5; ++j) {
        o.set_object();
        EXPECT_EQ_BASE(0, o.get_object_size());
        for(int i = 0; i < 10; ++i) {
            std::string key = "a";
            key[0] += i;
            v.set_number(i);
            o.set_object_value(key, v);
        }

        EXPECT_EQ_BASE(10, o.get_object_size());
        for(int i = 0; i < 10; ++i) {
            std::string key = "a";
            key[0] += i;
            EXPECT_EQ_BASE(true, o.find_object_key(key));
            v = o.get_object_value(key);
            EXPECT_EQ_BASE(static_cast<double>(i), v.get_number());
        }
    }

    EXPECT_EQ_BASE(true, o.find_object_key("j"));
    o.remove_object_value("j");
    EXPECT_EQ_BASE(false, o.find_object_key("j"));
    EXPECT_EQ_BASE(9, o.get_object_size());

    EXPECT_EQ_BASE(true, o.find_object_key("a"));
    o.remove_object_value("a");
    EXPECT_EQ_BASE(false, o.find_object_key("a"));
    EXPECT_EQ_BASE(8, o.get_object_size());

    for(int i = 0; i < 8; ++i) {
        std::string key = "a";
        key[0] += i + 1;
        EXPECT_EQ_BASE(k_number, o.get_object_value(key).get_type());
        EXPECT_EQ_BASE((double)i + 1, o.get_object_value(key).get_number());
    }

    v.set_string("Hello");
    o.set_object_value("World", v);

    EXPECT_EQ_BASE(true, o.find_object_key("World"));
    v = o.get_object_value("World");
    EXPECT_EQ_BASE("Hello", v.get_string());

    o.clear_object();
    EXPECT_EQ_BASE(0, o.get_object_size());
}

static void test_access() {
    test_access_null();
    test_access_boolean();
    test_access_string();
    test_access_number();

    test_access_array();
    test_access_object();
}

#define TEST_ROUNDTRIP(json)\
    do {\
        KJson kj;\
        kj.parse(json, status);\
        EXPECT_EQ_BASE("Parse ok", status);\
        kj.stringify(status);\
        EXPECT_EQ_BASE(json, status);\
    } while (0)
    

/* 测试字符串化数值 */
static void test_stringify_number() {
    TEST_ROUNDTRIP("0");
    TEST_ROUNDTRIP("-0");
    TEST_ROUNDTRIP("1");
    TEST_ROUNDTRIP("-1");
    TEST_ROUNDTRIP("1.5");
    TEST_ROUNDTRIP("-1.5");
    TEST_ROUNDTRIP("3.25");
    TEST_ROUNDTRIP("1e+20");
    TEST_ROUNDTRIP("1.234e+20");
    TEST_ROUNDTRIP("1.234e-20");

    TEST_ROUNDTRIP("1.0000000000000002"); /* the smallest number > 1 */
    TEST_ROUNDTRIP("4.9406564584124654e-324"); /* minimum denormal */
    TEST_ROUNDTRIP("-4.9406564584124654e-324");
    TEST_ROUNDTRIP("2.2250738585072009e-308");  /* Max subnormal double */
    TEST_ROUNDTRIP("-2.2250738585072009e-308");
    TEST_ROUNDTRIP("2.2250738585072014e-308");  /* Min normal positive double */
    TEST_ROUNDTRIP("-2.2250738585072014e-308");
    TEST_ROUNDTRIP("1.7976931348623157e+308");  /* Max double */
    TEST_ROUNDTRIP("-1.7976931348623157e+308");
}

/* 测试字符串化json字符串 */
static void test_stringify_string() {
    TEST_ROUNDTRIP("\"\"");
    TEST_ROUNDTRIP("\"Hello\"");
    TEST_ROUNDTRIP("\"Hello\\nWorld\"");
    TEST_ROUNDTRIP("\"\\\" \\\\ / \\b \\f \\n \\r \\t\"");
    TEST_ROUNDTRIP("\"Hello\\u0000World\"");
}

/* 测试字符串化数组 */
static void test_stringify_array() {
    TEST_ROUNDTRIP("[]");
    TEST_ROUNDTRIP("[null,false,true,123,\"abc\",[1,2,3]]");
}

/* 测试字符串化对象 */
static void test_stringify_object() {
    TEST_ROUNDTRIP("{}");
   /*  由于此json库中object是由unordered_map实现，
    *  键值对的顺序是不确定的；因此以下测试无法通过，但注意解析和生成的结果是正确且相同的
    */

    //TEST_ROUNDTRIP("{\"n\":null,\"f\":false,\"t\":true,\"i\":123,\"s\":\"abc\",\"a\":[1,2,3],\"o\":{\"1\":1,\"2\":2,\"3\":3}}");
}

static void test_stringify() {
    TEST_ROUNDTRIP("null");
    TEST_ROUNDTRIP("false");
    TEST_ROUNDTRIP("true");
    test_stringify_number();
    test_stringify_string();
    test_stringify_array();
    test_stringify_object();
}

static void test_copy() {
    KJson kj1, kj2;
    kj1.parse("{\"t\":true,\"f\":false,\"n\":null,\"d\":1.5,\"a\":[1,2,3]}", status);
    kj2 = kj1;
    EXPECT_EQ_BASE(true, (kj1 == kj2));
}

#define TEST_EQUAL(json1, json2, equality)\
    do {\
        KJson kj1, kj2;\
        kj1.parse(json1, status);\
        EXPECT_EQ_BASE("Parse ok", status);\
        kj2.parse(json2, status);\
        EXPECT_EQ_BASE("Parse ok", status);\
        EXPECT_EQ_BASE(equality, (kj1 == kj2));\
    } while (0)
    

static void test_equal() {
	TEST_EQUAL("true", "true", true);
	TEST_EQUAL("true", "false", false);
	TEST_EQUAL("false", "false", true);
	TEST_EQUAL("null", "null", true);
	TEST_EQUAL("null", "0", false);
	TEST_EQUAL("123", "123", true);
	TEST_EQUAL("123", "456", false);
	TEST_EQUAL("\"abc\"", "\"abc\"", true);
	TEST_EQUAL("\"abc\"", "\"abcd\"", false);
	TEST_EQUAL("[]", "[]", true);
	TEST_EQUAL("[]", "null", false);
	TEST_EQUAL("[1,2,3]", "[1,2,3]", true);
	TEST_EQUAL("[1,2,3]", "[1,2,3,4]", false);
	TEST_EQUAL("[[]]", "[[]]", true);
	TEST_EQUAL("{}", "{}", true);
	TEST_EQUAL("{}", "null", false);
	TEST_EQUAL("{}", "[]", false);
	TEST_EQUAL("{\"a\":1,\"b\":2}", "{\"a\":1,\"b\":2}", true);
	TEST_EQUAL("{\"a\":1,\"b\":2}", "{\"b\":2,\"a\":1}", true);
	TEST_EQUAL("{\"a\":1,\"b\":2}", "{\"a\":1,\"b\":3}", false);
	TEST_EQUAL("{\"a\":1,\"b\":2}", "{\"a\":1,\"b\":2,\"c\":3}", false);
	TEST_EQUAL("{\"a\":{\"b\":{\"c\":{}}}}", "{\"a\":{\"b\":{\"c\":{}}}}", true);
	TEST_EQUAL("{\"a\":{\"b\":{\"c\":{}}}}", "{\"a\":{\"b\":{\"c\":[]}}}", false);
}

static void test_move() {
    KJson kj1, kj2, kj3;
    kj1.parse("{\"t\":true,\"f\":false,\"n\":null,\"d\":1.5,\"a\":[1,2,3]}", status);
    kj2 = kj1;
    kj3 = std::move(kj2);
    EXPECT_EQ_BASE(k_null, kj2.get_type());
    EXPECT_EQ_BASE(true, (kj3 == kj1));
}

static void test_swap() {
    KJson kj1, kj2;
    kj1.set_string("Hello");
    kj2.set_string("World!");
    k_json::swap(kj1, kj2);
	EXPECT_EQ_BASE("World!", kj1.get_string());
	EXPECT_EQ_BASE("Hello",  kj2.get_string());
}

int main() {
    test_parse();
    test_access();
    test_stringify();
    test_copy();
    test_equal();
    test_move();
    test_swap();

    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}