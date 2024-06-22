#include "headers.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <regex.h>
#include "math.h"
#define MAN_HE_NET_IP "216.239.32.21" // IP address for man.he.net
#define HTTP_PORT 80

void fetchManPage(char *url);
