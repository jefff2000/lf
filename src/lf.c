#include "lf.h"
#include <stdio.h>

#define error_closure(method, errorstr)						\
int lf_##method##_closure(lua_State *L) {					\
return luaL_error(L, errorstr);								\
}
#define push_error_closure(prefix, key)						\
lua_pushstring(L, #key);									\
lua_pushcclosure(L, lf##prefix##key##_closure, 0);			\
lua_rawset(L, -3)

int lf_class__index_closure(lua_State *);
error_closure(class__newindex, "unexpected symbol near '.'")
int lf_class__call_closure(lua_State *);
error_closure(class__tostring, "unexpected symbol near 'class'")
error_closure(class__len, "unexpected symbol near '#'")
error_closure(class__pairs, "unexpected symbol near 'class'")
error_closure(class__ipairs, "unexpected symbol near 'class'")
error_closure(class__unm, "unexpected symbol near '-'")
error_closure(class__add, "unexpected symbol near '+'")
error_closure(class__sub, "unexpected symbol near '-'")
error_closure(class__mul, "unexpected symbol near '*'")
error_closure(class__div, "unexpected symbol near '/'")
error_closure(class__idiv, "unexpected symbol near '//'")
error_closure(class__mod, "unexpected symbol near '%'")
error_closure(class__pow, "unexpected symbol near '^'")
error_closure(class__concat, "unexpected symbol near '..'")
error_closure(class__band, "unexpected symbol near '&'")
error_closure(class__bor, "unexpected symbol near '|'")
error_closure(class__bxor, "unexpected symbol near '~'")
error_closure(class__bnot, "unexpected symbol near '~'")
error_closure(class__shl, "unexpected symbol near '<<'")
error_closure(class__shr, "unexpected symbol near '>>'")
error_closure(class__eq, "unexpected symbol near '=='")
error_closure(class__lt, "unexpected symbol near '<' or '>'")
error_closure(class__le, "unexpected symbol near '<=' or '>='")

void lf_string__index_closure(lua_State *);

void lf_rm_global(lua_State *L, const char *name) {
	lua_pushnil(L);
	lua_setglobal(L, name);
	return;
}

void lf_generate_class(lua_State *L) {
	void *class_memory = lua_newuserdatauv(L, 1, 1);
	lua_createtable(L, 0, 0);							// table (stores as an array all classes)
	lua_setiuservalue(L, -1, 1);						// sets the table as the user value of class userdata
	lua_createtable(L, 0, 26);							// metatable of class userdata

	lua_pushstring(L, "__index");						// sets the __index key of the metatable
	lua_createtable(L, 0, 0);
	lua_pushcclosure(L, lf_class__index_closure, 1);
	lua_rawset(L, -3);

	push_error_closure(_class, __newindex);

	lua_pushstring(L, "__call");
	lua_pushcclosure(L, lf_class__call_closure, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__metatable");
	lua_pushnil(L);
	lua_rawset(L, -3);

	push_error_closure(_class, __tostring);
	push_error_closure(_class, __len);
	push_error_closure(_class, __pairs);
	push_error_closure(_class, __ipairs);
	push_error_closure(_class, __unm);
	push_error_closure(_class, __add);
	push_error_closure(_class, __sub);
	push_error_closure(_class, __mul);
	push_error_closure(_class, __div);
	push_error_closure(_class, __idiv);
	push_error_closure(_class, __mod);
	push_error_closure(_class, __pow);
	push_error_closure(_class, __concat);
	push_error_closure(_class, __band);
	push_error_closure(_class, __bor);
	push_error_closure(_class, __bxor);
	push_error_closure(_class, __bnot);
	push_error_closure(_class, __shl);
	push_error_closure(_class, __shr);
	push_error_closure(_class, __eq);
	push_error_closure(_class, __lt);
	push_error_closure(_class, __le);

	lua_setmetatable(L, -2);
	lua_setglobal(L, "class");
}

void lf_improve_string(lua_State *L) {
	lua_pushstring(L, "__index");
	lua_getmetatable(L, -1);
	lua_pushvalue(L, -2);
	lua_getglobal(L, "string");
	lua_pushcclosure(L, lf_string__index_closure, 1);
	lua_rawset(L, -3);
	lua_pop(L, 2);
}

void lf_extend(lua_State *L) {
	lf_rm_global(L, "getmetatable");	// metatables are replaced by classes in lf
	lf_rm_global(L, "setmetatable");	// metatables are replaced by classes in lf

	lf_improve_string(L);
	lf_generate_class(L);
	//tonumber
	//	select
	//	assert
	//	warn
	//	rawset
	//	string
	//	pcall
	//	coroutine
	//	io
	//	os
	//	print
	//	dofile
	//	rawequal
	//	error
	//	pairs
	//	tostring
	//	utf8
	//	ipairs
	//	package
	//	arg
	//	debug
	//	type
	//	math
	//	require
	//	_VERSION
	//	_G
	//	rawget
	//	getmetatable
	//	loadfile
	//	setmetatable
	//	table
	//	collectgarbage
	//	xpcall
	//	load
	//	next
	//	rawlen
}

int lf_class__index_closure(lua_State *L) {
	if (!lua_isstring(L, -1)) return luaL_error(L, "unexpected symbol near 'class'");
	lua_getmetatable(L, -2); // table index metatable
	lua_pushstring(L, "__index"); // table index metatable __index
	lua_rawget(L, -2); // table index metatable closure
	lua_getupvalue(L, -1, 1); // table index metatable closure previousrecords
	lua_len(L, -1);
	lua_Integer length = lua_tointegerx(L, -1, NULL);
	lua_pushinteger(L, length + 1); // table index metatable closure previousrecords length length+1
	lua_pushnil(L);
	lua_copy(L, -7, -1); // table index metatable closure previousrecords length length+1 index
	lua_rawset(L, -4); // table index metatable closure previousrecords length
	lua_settop(L, -2);
	lua_setupvalue(L, -2, 1); // table index metatable closure
	lua_settop(L, 1);
	return 1;
}

int lf_class__call_closure(lua_State *L) {
	int size = lua_gettop(L);
	if (size != 2) return luaL_error(L, "member list expected near 'class'");
	lua_getmetatable(L, 1); // table ... metatable
	lua_pushstring(L, "__index"); // table ... metatable __index
	lua_rawget(L, -2); // table ... metatable closure
	lua_getupvalue(L, -1, 1); // table ... metatable closure previousrecords
	lua_createtable(L, 0, 0);
	lua_setupvalue(L, -3, 1); // table ... metatable closure previousrecords
	lua_copy(L, -4, -2); // table .. metatable ... previousrecords
	int err = lf_extension_create_class(L);
	if (err == 1 << 1) return luaL_error(L, "member list expected near 'class'");
	if (err == 1 << 2) return luaL_error(L, "unexpected symbol near 'class'");
	else return 0;
}

int lf_extension_create_class(lua_State *L) {
	if (!lua_istable(L, -1)) return 1 << 0;
	if (!lua_istable(L, -2)) return 1 << 1;
	// ??? member_list inheritance
	lua_len(L, -1); // ??? member_list inheritance length
	lua_Integer length = lua_tointegerx(L, -1, NULL);
	if (length == 0) return 1 << 2;
	/* create inheritance for ... */
	lua_getglobal(L, "class"); // ??? member_list inheritance length class
	if (lua_getiuservalue(L, -1, 1) != LUA_TTABLE) return luaL_error(L, "unexpected error during class declaration");
	// ??? member_list inheritance length class uservalue
	lua_pushvalue(L, -3); // ??? member_list inheritance length class uservalue length
	lua_rawget(L, -5); // ??? member_list inheritance length class uservalue length class_name
	lua_pushvalue(L, -7);
	lua_rawset(L, -4); // ??? member_list inheritance length class uservalue length
	/* create an empty userdata (similar to class) (used as a constructor or static functions only) */
	/* set global variable to be uesrdata */
	return 0;
}

void lf_string__index_closure(lua_State *L) {
	if (lua_isstring(L, -1) && (!lua_isinteger(L, -1))) {
		lua_getmetatable(L, -2);
		lua_pushstring(L, "__index");
		lua_rawget(L, -2);
		lua_getupvalue(L, -1, 1);
		lua_pushvalue(L, -4);
		lua_rawget(L, -2);
	}
	else if (lua_isinteger(L, -1) && (lua_len(L, -2), lua_tointeger(L, -1)) >= lua_tointeger(L, -2)) {
		if (lua_tointeger(L, -2) > 0) lua_pushlstring(L, &(lua_tostring(L, -3)[lua_tointeger(L, -2) - 1]), 1);
		else if (lua_tointeger(L, -2) < 0) lua_pushlstring(L, &(lua_tostring(L, -3)[lua_tointeger(L, -1) + lua_tointeger(L, -2)]), 1);
		else return 0;
	}
	else return 0;
	return 1;
}
