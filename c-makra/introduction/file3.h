#ifndef FILE3_H
#define FILE3_H

#define TOSTRING(x) #x
#define CONCAT(x, y) x ## y

#define TOSTRING_VAL(x) TOSTRING(x)
#define CONCAT_VAL(x, y) CONCAT(x, y)

#define MACRO_DEBUG(MACRO) #MACRO " = " TOSTRING(MACRO)

#endif