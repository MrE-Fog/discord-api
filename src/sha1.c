#include "common.h"
#include "sha1.h"

/*
 int SHA1_Init(SHA_CTX *c);
 int SHA1_Update(SHA_CTX *c, const void *data, size_t len);
 int SHA1_Final(unsigned char *md, SHA_CTX *c);
 unsigned char *SHA1(const unsigned char *d, size_t n,
      unsigned char *md); */

char*
sha1_create_digest (const char *msg)
{
  debug_print ("computing SHA-1 digest");
  char *digest = (char *)calloc (sizeof (char), SHA_DIGEST_LENGTH);
  SHA_CTX ctx;
  SHA1_Init (&ctx);
  SHA1_Update (&ctx, msg, strlen (msg));
  SHA1_Final ((unsigned char*)digest, &ctx);
  return digest;  
}