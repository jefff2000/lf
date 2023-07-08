#ifndef lf_h
#define lf_h

#include "lua.h"

#define LF_VERSION_MAJOR	"0"
#define LF_VERSION_MINOR	"0"
#define LF_VERSION_RELEASE	"1"

#define LF_VERSION_NUM			000
#define LF_RELEASE_NUM			001
#define LF_VERSION_RELEASE_NUM		(LF_VERSION_NUM * 100 + LF_RELEASE_NUM)

#define LF_VERSION	"lf " LF_VERSION_MAJOR "." LF_VERSION_MINOR
#define LF_RELEASE	LF_VERSION "." LF_VERSION_RELEASE
#define LF_COPYRIGHT	LF_RELEASE "  Copyright (C) 2023 Jeff"
#define LF_AUTHORS	"Jeff"

// The main function of the lf extension
void lf_extend(lua_State *);

int lf_extension_create_class(lua_State *);

#endif