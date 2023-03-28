#ifndef PMCONV_H
#define PMCONV_H
#ifdef __cplusplus
extern "C" {
#endif

#define UNICODE

unsigned short* utf8to16(unsigned short* org_str, char* str);
char* utf16to8(char* org_str, unsigned short* str);

#ifdef __cplusplus
}
#endif
#endif
