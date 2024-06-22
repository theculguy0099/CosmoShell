#include "iman.h"
int min(int x, int y)
{
    if (x < y)
        return x;
    else
        return y;
}
void remove_html_tags(char *str)
{
    int in_tag = 0;  // Flag to track if we are inside an HTML tag
    char *dst = str; // Destination pointer to overwrite the input string

    for (char *src = str; *src != '\0'; src++)
    {
        if (*src == '<')
        {
            in_tag = 1;
            continue;
        }
        if (*src == '>')
        {
            in_tag = 0;
            continue;
        }
        if (!in_tag)
        {
            *dst++ = *src;
        }
    }
    *dst = '\0'; // Null-terminate the result string
}

void fetchManPage(char *url)
{
    char *hostname = "man.he.net\0"; // Replace with the hostname of the website
    char *path = (char *)malloc(sizeof(char) * 1024);
    // strcat(path, "/");
    // char *path = "/";              // Replace with the path you want to request
    // strcat(path, "?topic=");
    // strcat(path, url);
    // strcat(path, "&section=all");
    // path[strlen(path)]='\0';
    // Create a socket
    snprintf(path,1024,"?topic=%s&section=all",url);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        exit(1);
    }

    // Resolve the hostname to an IP address
    struct hostent *host = gethostbyname(hostname);
    if (host == NULL)
    {
        perror("gethostbyname");
        close(sockfd);
        exit(1);
    }

    // Create a sockaddr_in structure to store server information
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port
    memcpy(&server_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        close(sockfd);
        exit(1);
    }

    // Prepare the HTTP GET request
    char request[1000000];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, hostname);
    // printf("\n\nthhis%sthiss\n\n", request);

    // Send the request
    if (write(sockfd, request, strlen(request)) == -1)
    {
        perror("write");
        close(sockfd);
        exit(1);
    }

    // Receive and print the response
    char response[1000000];
    int bytes_received;
    int f = 0;
    int ct = 0;
    bytes_received = read(sockfd, response, sizeof(response));
    int br = 0;
    while (bytes_received > 0)
    {
        ct++;
        // Remove HTML tags from the response
        remove_html_tags(response);
        if (f == 0)
        {
            for (int i = 0; i < bytes_received; i++)
            {
                if (f == 0)
                {
                    char *x = "NAME\n";
                    char *y1 = "AUTHO";
                    char *y = (char *)malloc(sizeof(char) * 10);
                    int ct = 0;
                    for (int j = i; j < min(i + 5, bytes_received); j++)
                    {
                        y[ct++] = response[j];
                    }
                    y[ct] = '\0';
                    if (strcmp(x, y) == 0)
                    {
                        i--;
                        f = 1;
                        // break;
                    }
                    else if (strcmp(y1, y) == 0)
                    {
                        br = 1;
                        break;
                    }
                    free(y);
                }
                else if (f == 1)
                {
                    char *x = "NAME\n";
                    char *y1 = "AUTHO";
                    char *y = (char *)malloc(sizeof(char) * 10);
                    int ct = 0;
                    for (int j = i; j < min(i + 5, bytes_received); j++)
                    {
                        y[ct++] = response[j];
                    }
                    y[ct] = '\0';
                    // if (strcmp(x, y) == 0)
                    // {
                    //     i--;
                    //     f = 1;
                    //     // break;
                    // }
                    if (strcmp(y1, y) == 0)
                    {
                        br = 1;
                        break;
                    }
                    printf("%c", response[i]);
                    free(y);
                }
            }
        }
        else
            printf("%s\n", response);
        if (br)
            break;
        printf("%s", response);
        // Print the response without HTML tags
        // fwrite(response, 1, bytes_received, stdout);
        bytes_received = read(sockfd, response, sizeof(response));
    }
    // printf("%d\n", ct);
    printf("\n");

    // Close the socket
    close(sockfd);

    return;
}
