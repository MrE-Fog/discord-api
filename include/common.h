#ifndef __COMMON_H
#define __COMMON_H

#define DEBUG (1)

#include <stdio.h>
#include <math.h>
#include <string.h>

#define debug_print(msg, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " msg "\n", __FILE__, \
                                __LINE__, __func__); } while (0)

#define DISCORD_GATEWAY_HOSTNAME  "gateway.discord.gg"
#define DISCORD_GATEWAY_PORT      443
#define DISCORD_GATEWAY_VERSION   "8"
#define DISCORD_GATEWAY_ENC       "json"

static char convert[64];

const static char*
int_to_string (size_t n)
{
  /* simple divmod algorithm, no safety checks
   * /definitely/ prone to errors due to limited
   * convert array size */

  memset (convert, 0, sizeof (convert));
  size_t i = sizeof (convert) - 1;
  while (i >= 0 && n > 0)
    {
      convert[i--] = (n % 10) + '0';
      n -= n % 10;
      n /= 10;
    }
  memcpy (convert, &convert[i+1], sizeof (convert ) - 1 - i);
  return convert;
}

#endif  /* __COMMON_H */
