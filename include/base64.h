#ifndef __BASE64_H
#define __BASE64_H

#define BASE64_ENCODE_LEN(len) ((len + 2)/3 * 4)

int
base64_decode_len (const char *bufcoded);

int
base64_decode (char *bufplain, const char *bufcoded);

int
base64_encode (char *encoded, const char *string, int len);

#endif  /* __BASE64_H */