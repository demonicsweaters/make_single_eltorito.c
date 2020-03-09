/*
how to use

1. place 32bitEFI.c and 64 bit linux iso into the same directory
2. rename linux iso to blackmacbook64.iso
3. compile 32bitEFI.c with "cc -g -Wall -o 32bitEFI.c"
4. make it executable 'chmod +x 32bitEFI
5. burn new iso
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char *iso_name = {"blackmacbook64.iso"};

int main(int argc, char **argv)
{
  int fd, ret;
  unsigned char buf[2048 - 64];
  off_t lba;
  size_t buf_size = 2048 - 64;

  fd = open(iso_name, O_RDWR);
  if (fd == -1)
    goto err_ex;
  if (lseek(fd, (off_t) 32768 + 2048 + 71, SEEK_SET) == -1)
    goto err_ex;
  ret = read(fd, buf, 4);
  if (ret == -1)
    goto err_ex;
  if (ret < 4) {
    fprintf(stderr, "Cannot read 4 bytes from %s\n", iso_name);
    exit(1);
  }
  lba = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
  if (lseek(fd, lba * 2048 + 64, SEEK_SET) == -1)
    goto err_ex;
  memset(buf, 0, buf_size);
  ret = write(fd, buf, buf_size);
  if (ret == -1)
    goto err_ex;
  if (ret < buf_size) {
    fprintf(stderr, "Cannot write %d bytes to %s\n", (int) buf_size, iso_name);
    exit(1);
  }
  close(fd);
  printf("done\n");
  exit(0);
err_ex:;
  perror(iso_name);
  exit(1);
}
