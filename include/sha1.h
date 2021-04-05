#ifndef __SHA1_H
#define __SHA1_H

#include <openssl/sha.h>

char*
sha1_create_digest (const char *msg);

#endif  /* __SHA1_H */