#ifndef N14_PATH_H
#define N14_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

int n14_path_init(void);
void n14_path_final(void);

int n14_path_open(void);
void n14_path_close(void);

#ifdef __cplusplus
}
#endif

#endif /* N14_PATH_H */