/*
how to use

1. Place 32bitEFI.c and 64 bit linux iso into the same directory
2. Rename linux iso to blackmacbook64.iso
3. Open a terminal and navigate into that folder, if you don't how to navigate file structure,
  you should learn it is very easy. All you need are the "cd" command and the "pwd" command.
4. Compile 32bitEFI.c with "cc -g -Wall -o 32bitEFI 32bitEFI.c" 
4. Make it executable with "chmod +x 32bitEFI"
5. Run the program that has just been compiled as "32bitEFI" (NOT 32bitEFI.c, this is the name of this C script)
  with "./32bitEFI".
5. Burn new iso
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
