#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
  if (argc <= 2) {
    printf("usage: %s ip_address port_number\n", argv[0]);
    return 0;
  }

  const char *ip = argv[1];
  int port = atoi(argv[2]);

  int listenfd = socket(PF_INET, SOCK_STREAM, 0);
  assert(listenfd >= 1);

  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &address.sin_addr);

  int ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
  assert(ret != -1);

  ret = listen(listenfd, 5);
  assert(ret != -1);

  int sockfd = accept(listenfd, (struct sockaddr *)&address, &addrlen);

  char buf_size[1024] = {0};
  int recv_size = recv(sockfd, buf_size, sizeof(buf_size), 0);

  int send_size = send(sockfd, buf_size, recv_size, 0);

  close(sockfd);
  close(listenfd);

  return 0;
}
