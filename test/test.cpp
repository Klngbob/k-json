#include <iostream>

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

/* 测试解析null */
static void test_parse_null() {

}

/* 测试解析true */
static void test_parse_true() {

}

/* 测试解析false */
static void test_parse_false() {
    
}

/* 测试解析数值 */
static void test_parse_number() {
    
}

/* 测试解析字符串 */
static void test_parse_string() {
    
}

/* 测试解析数组 */
static void test_parse_array() {
    
}

/* 测试解析对象 */
static void test_parse_object() {
    
}

/* 测试解析“期望一个值” */
static void test_parse_expect_value() {
    
}

/* 测试解析无效值 */
static void test_parse_invalid_value() {
    
}

/* 测试解析不止一个数据 */
static void test_parse_root_not_singular() {
    
}

/* 测试解析数值溢出 */
static void test_parse_number_too_big() {
    
}

/* 测试解析字符串引号缺失 */
static void test_parse_miss_quotation_mark() {
    
}

/* 测试解析字符串中无效的转义字符 */
static void test_parse_invalid_string_escape() {
    
}

/* 测试解析字符串中无效的字符（无效的编码单元） */
static void test_parse_invalid_string_char() {
    
}

/* 测试解析字符串中不合法的十六进位编码 */
static void test_parse_invalid_unicode_hex() {
    
}

/* 测试解析字符串中不正确的代理项 */
static void test_parse_invalid_unicode_surrogate() {
    
}

/* 测试解析数组中缺失逗号或 ] */
static void test_parse_miss_comma_or_square_bracket() {
    
}

/* 测试解析对象缺失键 */
static void test_parse_miss_key() {
    
}

/* 测试解析对象缺失冒号 */
static void test_parse_miss_colon() {
    
}

/* 测试解析对象缺失都好或 } */
static void test_parse_miss_comma_or_curly_bracket() {
    
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_string();
    test_parse_array();
    test_parse_array();

    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
    test_parse_miss_quotation_mark();
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

}

/* 测试访问布尔值 */
static void test_access_boolean() {
    
}

/* 测试访问数值 */
static void test_access_number() {
    
}

/* 测试访问字符串 */
static void test_access_string() {
    
}

/* 测试访问json数组 */
static void test_access_array() {
    
}

/* 测试访问json对象 */
static void test_access_object() {

}

static void test_access() {
    test_access_null();
    test_access_boolean();
    test_access_string();
    test_access_number();

    test_access_array();
    test_access_object();
}

/* 测试字符串化数值 */
static void test_stringify_number() {

}

/* 测试字符串化json字符串 */
static void test_stringify_string() {

}

/* 测试字符串化数组 */
static void test_stringify_array() {

}

/* 测试字符串化对象 */
static void test_stringify_object() {

}

static void test_stringify() {

    test_stringify_number();
    test_stringify_string();
    test_stringify_array();
    test_stringify_object();
}

static void test_copy() {

}

static void test_equal() {

}

static void test_move() {

}

static void test_swap() {

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