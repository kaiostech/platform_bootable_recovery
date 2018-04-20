/*
 * Created by Kaios
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mount.h>  // for _IOW, _IOR, mount()
#include <sys/stat.h>
#include "ubi-user.h"


int ubi_mount(const char *dev_name, const char *mount_point,
                        const char *filesystem, int read_only)
{
    const unsigned long flags = MS_NOATIME | MS_NODEV | MS_NODIRATIME;
    int rv = -1;

    if (!read_only) {
        rv = mount(dev_name, mount_point, filesystem, flags, NULL);
    }
    if (read_only || rv < 0) {
        rv = mount(dev_name, mount_point, filesystem, flags | MS_RDONLY, NULL);
        if (rv < 0) {
            printf("Failed to mount %s on %s: %s\n",
                   dev_name, mount_point, strerror(errno));
        } else {
            printf("Mount %s on %s read-only\n", dev_name, mount_point);
        }
    }
#if 1   //TODO: figure out why this is happening; remove include of stat.h
    if (rv >= 0) {
        /* For some reason, the x bits sometimes aren't set on the root
         * of mounted volumes.
         */
        struct stat st;
        rv = stat(mount_point, &st);
        if (rv < 0) {
            return rv;
        }
        mode_t new_mode = st.st_mode | S_IXUSR | S_IXGRP | S_IXOTH;
        if (new_mode != st.st_mode) {
            printf("Fixing execute permissions for %s\n", mount_point);
            rv = chmod(mount_point, new_mode);
            if (rv < 0) {
                printf("Couldn't fix permissions for %s: %s\n",
                       mount_point, strerror(errno));
            }
        }
    }
#endif

    return rv;
}

int ubi_update(const char *dev_name, long long len, const char *mountpoint)
{
    int rv = -1;

    int fd = open(dev_name, O_RDWR);
    if (fd < 0) {
        printf("Update ubi volume, open device %s error: %s\n",
               dev_name, strerror(errno));
        return rv;
    }
    rv = ioctl(fd, UBI_IOCVOLUP, &len);
    close(fd);

    return rv;
}

int ubi_fupdate(int fd, long long len) {
    return ioctl(fd, UBI_IOCVOLUP, &len);
}

