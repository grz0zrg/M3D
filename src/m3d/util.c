#include "util.h"

int m3d_UtilNextPowerOfTwo(int value) {
    return pow(2, ceil(log10(value) / log10(2)));
}

int m3d_UtilIsPowerOfTwo(int value) {
	return (!(value & (value - 1)) && value);
}

void m3d_UtilFreeSplitResult(char **split_result, long n) {
    while(n--) {
        free(split_result[n]);
    };
    free(split_result);
}

char **m3d_UtilSplitString(const char *str, char delimiter, long *n) {
    if (!str) return NULL;

    char **split_result = 0;
    long strings_count = 0;

    const char *begin = str;
    while (*str++ != '\0') {
        size_t new_str_length = (str-begin);
        //if (*(str-1) == delimiter) {
           // begin = str; // make it ignore a sequence of delimiters
           // continue;
        //}

        if (*str == delimiter || *str == '\0') {
            char *new_str = NULL;
            if (new_str_length > 0) {
                new_str = (char *)malloc(sizeof(char) * (new_str_length+1));
                if (!new_str) continue;

                strncpy(new_str, begin, new_str_length);
                new_str[new_str_length] = '\0';
            }

            begin = str+1;

            char **split_result_tmp = (char **)realloc(split_result,
                                        sizeof(char **) * (strings_count+1));
            if (!split_result_tmp) {
                free(new_str);
                continue;
            }

            split_result = split_result_tmp;
            split_result[strings_count] = new_str;

            strings_count++;
        }
    }

    *n = strings_count;

    return split_result;
}

void m3d_UtilToLowercase(char *string) {
    if (!string) return;

    char *p = string;
    while (*p != '\0') {
        if (*p >= 'A' && *p <= 'Z') {
            *p += 32;
        }

        p++;
    }
}

char *m3d_UtilGetPath(const char *string) {
    if (!string)  return NULL;
    if (!*string) return NULL;

    size_t length = strlen(string);

    const char *end = string+length;
    int new_length = 0;
    while (*--end != '/' && (new_length = end-string) > 0);

    if (!new_length) {
        return NULL;
    }

    char *path = (char *)malloc(sizeof(char) * (new_length+1));
    strncpy(path, string, new_length);
    path[new_length] = '\0';

    return path;
}

char *m3d_UtilParseValue(char **sp) {
    if (!sp) return NULL;

	int j = 0;
	char *value_s = (char *)malloc(sizeof(char) * 32);

	if (!value_s) return NULL;

    memset(value_s, 0, sizeof(char) * 32);

    int ignore = 0;
	while ((*(*sp)) != '\0') {
        if ((*(*sp) >= 48 && *(*sp) < 58 && j < 32 && ignore == 0) ||
			*(*sp) == '.' || *(*sp) == '-') {
            value_s[j++] = (*(*sp));
        } else {
            if (j) {
                ignore = 1;

                if ((*(*sp)) == ' ') {
                    break;
                }
            }
        }

        *sp = *sp+1;
	}

	return value_s;
}

float m3d_UtilParseFloat(char **sp) {
	char *value_s = m3d_UtilParseValue(sp);
	float value = atof(value_s);

	free(value_s);

	return value;
}

int m3d_UtilParseInt(char **sp) {
	char *value_s = m3d_UtilParseValue(sp);
	int value = atoi(value_s);

	free(value_s);

	return value;
}

float m3d_UtilDeg2Rad(float deg) {
    return deg / 180.0f * M_PI;
}

float m3d_UtilRad2Deg(float rad) {
    return rad * 180.0f / M_PI;
}
