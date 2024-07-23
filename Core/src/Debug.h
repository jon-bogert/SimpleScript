#pragma once

#include <cstdio>

#ifdef _DEBUG
#define LOG(format, ...)\
	do{\
		char buffer[256];\
		int res = snprintf(buffer, std::size(buffer), ##format##"\n", __VA_ARGS__);\
		printf("%s", buffer);\
	}while(false)
#else
#define LOG(format, ...)
#endif