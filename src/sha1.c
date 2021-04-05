#include "common.h"
#include "sha1.h"

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