//
// Created by abel walga on 03/05/2018.
//

#ifndef ODNCRYPTO_SODIUM_IMPORT_HPP
#define ODNCRYPTO_SODIUM_IMPORT_HPP
#include <sodium.h>
#if defined(__linux__)
# include <fcntl.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <linux/random.h>
#endif

#if defined(__linux__) && defined(RNDGETENTCNT)
int fd;
int c;

if ((fd = open("/dev/random", O_RDONLY)) != -1) {
    if (ioctl(fd, RNDGETENTCNT, &c) == 0 && c < 160) {
        fputs("This system doesn't provide enough entropy to quickly generate high-quality random numbers.\n"
              "Installing the rng-utils/rng-tools, jitterentropy or haveged packages may help.\n"
              "On virtualized Linux environments, also consider using virtio-rng.\n"
              "The service will not start until enough entropy has been collected.\n", stderr);
    }
    (void) close(fd);
}
#endif

#endif //ODNCRYPTO_SODIUM_IMPORT_HPP
