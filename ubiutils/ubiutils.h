/*
 * Created by Kaios
 */


#ifndef UBIUTILS_H_
#define UBIUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* mount_point is like "/system"
 * filesystem is like "ubifs"
 */
int ubi_mount(const char *dev_name, const char *mount_point,
                        const char *filesystem, int read_only);

/* used for format ubifs volume */
int ubi_update(const char *dev_name, long long len, const char *mountpoint);
int ubi_fupdate(int fd, long long len);

#ifdef __cplusplus
}
#endif

#endif  // UBIUTILS_H_
