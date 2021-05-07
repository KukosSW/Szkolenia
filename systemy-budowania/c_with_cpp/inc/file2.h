#ifndef FILE2_H
#define FILE2_H

/* C++ musi widziec ze funkcja jest C style, C jednak nie zna extern "C" wiec musimy ifowac */
#ifdef __cplusplus
extern "C" {
#endif
void bar(void);
#ifdef __cplusplus
}
#endif

#endif