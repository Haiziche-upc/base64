#include "base64.h"

int base64_encode_str(const char *src, const int src_len, char *dst, int dst_size) {
    //Check that the array is long enough to store
    long long dst_len = (src_len % 3 == 0) ? (src_len / 3) * 4 : (src_len / 3 + 1) * 4;
    if (dst_len >= dst_size)
        return -1;//space is not enough
    //encode
    long long dst_index = 0;
    long long src_index = 0;
    int int63 = 0x3F; //  00111111  6bit
    int int255 = 0xFF;
    while (src_index < src_len) {
        //take three Bytes from src and encode them into four Bytes BASE64 characters
        char b0 = src[src_index++];
        char b1 = (src_index < src_len) ? src[src_index++] : 0;
        char b2 = (src_index < src_len) ? src[src_index++] : 0;
        dst[dst_index++] = ALPHA_BASE[(b0 >> 2) & int63];
        dst[dst_index++] = ALPHA_BASE[((b0 << 4) | ((b1 & int255) >> 4)) & int63];
        dst[dst_index++] = ALPHA_BASE[((b1 << 2) | ((b2 & int255) >> 6)) & int63];
        dst[dst_index++] = ALPHA_BASE[b2 & int63];
    }
    //padding
    switch (src_len % 3) {
        case 1:
            dst[--dst_index] = '=';
        case 2:
            dst[--dst_index] = '=';
    }
    dst[dst_len] = '\0';
    return dst_len;
}
int base64_decode_str(const char *src, const int src_len, char *dst, int dst_size){
    //determines  whether the string is valid
    if (src_len % 4 != 0)
        return -2;
    //calc dst len
    long long dst_len = src_len / 4 * 3;
    if (src[src_len - 2] == '=') {
        dst_len -= 2;
    } else if (src[src_len - 1] == '=') {
        dst_len--;
    }
    //space is not enough
    if (dst_size <= dst_len)
        return -1;
    //decode
    int int255 = 0xFF;
    long long dst_index = 0;
    long long src_index = 0;
    while (src_index < src_len) {
        int c0 = toInt[src[src_index++]];
        int c1 = toInt[src[src_index++]];
        int c2 = toInt[src[src_index++]];
        int c3 = toInt[src[src_index++]];
        dst[dst_index++] = (char)((c0 << 2) | (c1 >> 4));
        dst[dst_index++] = (char)(((c1 << 4) | (c2 >> 2)) & int255);
        dst[dst_index++] = (char)(((c2 << 6) | c3) & int255);
    }

    dst[dst_len] = '\0';
    return dst_len;
}