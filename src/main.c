#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "xway_pkg.h"

// 00000001 00 ??/08 00 F0 30 10 14 10 25 06
// 00000001 00 ??/07 00 F0 14 10 30 10 FE/FD

// FD impossible de faire, FE ok c'est fait
//
// Utiliser requete 37 à la place du 25 06 : 37codereq 06cat 68segmt 07cat 27(39) 00 addr 0200 3a00(adresse moi) 0300(tronçon)
// 30 --> adresse
// Adresse en 39 (48 xway), valeur en 40
// réponse ...... 30 10 FE
// retour F1 14 10 30 10 09 XX 37 ....
// il faut récupérer XX
// réponse F1 30 10 14 10 19(0 to 1) XX FE
//
// write --> read --> read --> write

int main() {
  int sock;
  struct sockaddr_in server;
  char message[MAX_CHAR], server_reply[MAX_CHAR];
  char bytes[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0xF0, 0x30, 0x10, 0x14, 0x10, 0x24, 0x06};

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Could not create socket");
  }
  puts("Socket created");
  
  server.sin_addr.s_addr = inet_addr("10.31.125.14");
  server.sin_family = AF_INET;
  server.sin_port = htons( 502 );

  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Connect failed. Error");
    return 1;
  }
  
  puts("Connected\n");
  
  memcpy(message, bytes, bytes_length(bytes));
  print_byte_array(message);
  if (write(sock, message, bytes_length(bytes)) < 0) {
    puts("Send failed");
    return 1;
  }
  puts("Data Sent\n");

  if (read(sock, server_reply, MAX_CHAR) < 0) {
    puts("recv failed");
  }
  else {
    puts("Server reply :");
    print_byte_array((server_reply));
  }

  int c = getc(stdin);

  close(sock);

  return 0;
}

