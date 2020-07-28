#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/**
 * 从左往右遍历
 * 数据分为2种
 * 1. 中括号外面的数据
 * 2. 中括号里面的数据, (里面的数据进行递归调用)
 *
 */



// 参考代码,
char *decodeString(char *s)
{
    if (s == NULL) {
        return NULL;
    }
    //
    char *a = strdup(s);
    while (true) {
        int len = strlen(a);
        int left = 0, right = len;
        int i = len - 1, num = 0, w = 1;
        // 从右侧开始遍历, 碰到的第一个数字, 就是最里面的嵌套, 定位"["和"]"的位置
        for (; i >= 0; i--) {
            if (a[i] == ']') {
                right = i;
            } else if(a[i] == '[') {
                left = i;
            } else if (a[i] >= '0' && a[i] <= '9') {
                do { // 组合数字
                    num += w * (a[i] - '0');
                    w *= 10;
                    i--;
                } while (i >= 0 && (a[i] >= '0' && a[i] <= '9'));
                break;
            }
        }
        // 遍历掉所有的中括号则返回结果
        if (num == 0) { //没有[]了
            return a;
        } else {
            // 对找到子串做展开操作, 直到没有子串
            // 子串长度
            int slen = (right - left - 1);
            //
            int count = (i + 1) + (len - right - 1) + num * slen + 1;
            char *t = (char*)calloc(count, sizeof(char));
            if (i + 1 > 0) { // 左侧复制
                memcpy(t, a, i + 1);
            }
            for (int k = 0; k < num; k++) { // 中,即子串复制
                memcpy(t + (i + 1) + k * slen, a + left + 1, slen);
            }
            if (len - right - 1 > 0) { // 尾巴复制
                memcpy(t + (i + 1) + num * slen, a + right + 1, len - right - 1);
            }
            free(a);
            a = t;
        }
    }
}
//
// 作者：ni8fun
// 链接：https://leetcode-cn.com/problems/decode-string/solution/fan-fu-sao-miao-shuang-100-by-ni8fun/
// 来源：力扣（LeetCode）
// 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



// 字符数组转int
int char2int(char *s, int count) {
    int number = 0;
    for (int i = 0; i < count; i++) {
        number = number * 10 + (*s - '0');
        s++;
    }
    return number;
}


// 同级解析
char *decode(char *data, unsigned int len, unsigned int *real_len) {
    // 调试变量;
    unsigned int _d1 = 0;
    char *result = NULL;
    char *tmp = NULL;
    char *sub_decode = NULL;
    unsigned result_size = 0;
    unsigned result_len = 0;
    unsigned count = 0;
    unsigned num = 1;
    char *number_start = NULL;
    char *sub_start = NULL;
    char *out_start = data;

    char *i = data;
    for (; i < data + len; i++) {
        // 记录外部数字开头
        if (count == 0 & number_start == NULL & *i <= '9' & *i >= '0') {
            number_start = i;
            // 外部子串复制
            if ((i - out_start) > 0) {
                tmp = result;
                result_size = result_len + (i - out_start) + 1;
                result = (char *) malloc(result_size * sizeof(char));
                if (result_len > 0) {
                    memcpy(result, tmp, result_len);
                }
                memcpy(result + result_len, out_start, i - out_start);
                result_len = result_len + (i - out_start);
                free(tmp);
            }

            out_start = NULL;
        }

        if (*i == '[') {
            // 数字提取
            if (number_start != NULL) {
                num = char2int(number_start, i - number_start);
                number_start = NULL;
            }
            // 记录子串开头
            if (count == 0) {
                sub_start = i + 1;
            }
            count++;
        }
        if (*i == ']') {
            count--;


            if (count == 0) {


                // 子串递归调用
                unsigned int _d;
                _d = *real_len;
                sub_decode = decode(sub_start, i - sub_start, real_len);

                // 内存扩容, 等子串调用结束才知道需要的扩容大小
                tmp = result;
                result_size = result_len + num * (*real_len) + 1;
//                _d1 = result_size * sizeof(char);
//                printf("===== ");
//                printf("malloc size %d\n", _d1);

                result = (char *) malloc(result_size * sizeof(char));
                if (result_len > 0) {
                    memcpy(result, tmp, result_len);
                }
                free(tmp);


//                printf("pass\n");
                printf("result: %u-%u (size-len), tmp: %lu-%u(strlen-len), sub need:%u\n", result_size, result_len,
                       strlen(sub_decode), *real_len, result_len + num * (*real_len)
                );
                for (int j = 0; j < num; j++) {
//                    printf("copy ")
                    memcpy(result + result_len + j * (*real_len), sub_decode, *real_len);
                }
                free(sub_decode);

                result_len = result_len + num * (*real_len);
                sub_start = NULL;
                //新的外部子串开头, 猜测错误在长度判断时直接处理
                out_start = i + 1;
            }
        }
    }
    // 最后的外部数据合并
    if (out_start != NULL) {
        // 外部子串复制
        tmp = result;
        result_size = result_len + (i - out_start) + 1;
        result = (char *) malloc(result_size * sizeof(char));
        if (result_len > 0) {
            memcpy(result, tmp, result_len);
        }

        memcpy(result + result_len, out_start, i - out_start);
        result_len = result_len + (i - out_start);
        free(tmp);
        out_start = NULL;

    }
    *real_len = result_len;
    *(result + result_len) = '\0';
    printf("%d-%lu,  %s\n", *real_len, strlen(result), result);
    // 存储返回的结果
//    char result_a[result_size+1];
//    result_a[result_size] = '\0';
//    memcpy(result_a, result, result_len);
//    for(int i= 0;i< sizeof(result_a);i++){
//        printf("array point: %lu\n", &result_a[i]);
//    }
//    printf("reuslt_a: %lu > %s\n", &(result_a[0]), &result_a[0]);
//    free(result);
//    return &result_a[0];
    return result;
}


char *decodeString_2(char *s) {
    unsigned int real_len = 0;
//    return decode(s, strlen(s), &real_len);
    char *result = decode(s, strlen(s), &real_len);
    printf("%s\n", result);
    int t = strlen(result);
    return result;
}


void test_lc_394() {
//    char *data = "cc2[acc2[abc]3[cd]efbc]3[cd]ef";
//    char *data = "3[a]2[b2[c]c1[3[d]]]";
//    char *data = "bbbbb";
    char *data = "3[z]2[2[y]pq4[2[jk]e1[f]]]ef";
//    char *data = "3[z]2[2[y]pq4[2[jk]e1[f]]]ef";
//    char *data = "2[abc]3[cd]ef";
    clock_t start = clock();
    printf("start ...\n");
    char *result = decodeString_2(data);

    printf("encode data: %s\n", data);
    printf("decode data: %s\n", result);
    printf("use time %fs\n", (float) (clock() - start) / CLOCKS_PER_SEC);
    printf("exit!");
    free(result);
}


void test_strtol() {
    char *endptr;
    char *var = "555l";
    unsigned int var_num;
    bool is_numeric;
    // 基于字符窜 is null
    var_num = strtol(var, &endptr, 16);
    printf("number: %d\n", var_num);
    printf("string: %s\n", endptr);
    printf("string: %p\n", endptr);
    printf("string: %d\n", *endptr);
    is_numeric = !*endptr;
    printf("is number: %d\n", is_numeric);
}

void test_char2int() {
    char *data = "999c";
    printf("number is:%d ", char2int(data, 3));

}

void test_p_d() {
    int data[5];
    for (int i = 0; i < 5; i++) {
        data[i] = i * 10 + 3;
    }
    int *p1 = &data[1];
    int *p4 = &data[4];
    unsigned int c = p4 - p1;
    printf("\n");


}

void test_free() {
    char *t1 = malloc(32);
    char *t2 = NULL;
    free(t1);
    free(t2);


}

void test_string() {
    char *a = "Hello";
    char *b = "World";
//    char *c = a + b;
}

void test_address() {
    int *array = (int *) malloc(10 * sizeof(int));
    array[8] = 0xffff;
    array[9] = 0xdddd;
    int res = array[10];  // BOOM
    printf("%d", res);
}


int main() {
    printf("Hello, World!\n");
//    test_strtol();
    test_lc_394();
//    test_char2int();
//    test_free();
//    test_p_d();
//    test_string();
//    test_address();
    return 0;
}

// cc2[acc2[abc]3[cd]efbc]3[cd]ef
// ccaccabcabccdcdcdefbcaccabcabccdcdcdefbccdcdef
// ccaccabcabccdcdcdefbaccabcabccdcdcdefbcdcdcdef
// 3[z]2[2[y]pq4[2[jk]e1[f]]]ef
// 2[2[y]pq4[2[jk]e1[f]]]ef
// 3[z]2[2[y]pq4[2[jk]e1[f]]]ef