#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

extern int h_errno;

int main(int argc, char *argv[])
{
   int i, j;
   in_addr_t ipv4_addr;
   struct in6_addr ipv6_addr;
   struct hostent *res;

   for (i = 1; i < argc; i++) {
      if (strchr(argv[i], ':') == NULL) { /* No colon = IPv4 address */
         if (inet_pton(AF_INET, argv[i], &ipv4_addr) == 0) {
            fprintf(stderr, "%s: invalid address %s\n", argv[0], argv[i]);
            continue;
         }

         if ((res = gethostbyaddr(&ipv4_addr, sizeof(ipv4_addr), AF_INET)) == NULL) {
            fprintf(stderr, "%s: error in resolving address %s: %s\n", argv[0], argv[i], gai_strerror(h_errno));
            continue;
         }

      } else {                            /* Colon = IPv6 address */
         if (inet_pton(AF_INET6, argv[i], &ipv6_addr) == 0) {
            fprintf(stderr, "%s: invalid address %s\n", argv[0], argv[i]);
            continue;
         }

         if ((res = gethostbyaddr(&ipv6_addr, sizeof(ipv6_addr), AF_INET6)) == NULL) {
            fprintf(stderr, "%s: error in resolving address %s: %s\n", argv[0], argv[i], gai_strerror(h_errno));
            continue;
         }
      }

      printf("%s\t%s\n", argv[i], res->h_name);
      for (j = 0; res->h_aliases[j] != NULL; j++)
         printf("%s\t%s\n", argv[i], res->h_aliases[j]);
   }

   return 0;
}
