#pragma once
#include <cstdio>

#define INFO(_format_,...)  printf("[+] INFO      "); printf((_format_),##__VA_ARGS__); printf("\n");
