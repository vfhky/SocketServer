#include "bussiness.h"
#include "printnolog.h"

#include <lua.h>
#include "lauxlib.h"
#include "lualib.h"
#include "string.h"

int main_service(char *buff)
{
    int flag;
    PRINTNOLOG("start main service!\n");
    flag = business_lua(buff);
    PRINTNOLOG("end main service!\n");
    return flag;
}

int business_lua(char *buff) 
{
	lua_State *L = luaL_newstate();
    PRINTNOLOG( "111111111111111\n" );
	luaL_openlibs(L);
    if(luaL_loadfile(L,LUA_MAIN_PATH) != 0){
        perror("business_lua,luaL_loadfile()");
        PRINTNOLOG("%s\n",lua_tostring(L, -1));
        lua_close(L);
        return EOF;
    }
    PRINTNOLOG( "111111111111111\n" );
    if(lua_pcall(L,0,0,0) != 0) {
    PRINTNOLOG( "111111111111111\n" );
        perror("business_lua,lua_pcall().1");
        PRINTNOLOG("%s\n",lua_tostring(L, -1));
        lua_close(L);
        return EOF;
    }
    PRINTNOLOG( "111111111111111\n" );
    lua_getglobal(L, "main");
    lua_pushstring(L, buff);
    PRINTNOLOG( "111111111111111\n" );
    if(lua_pcall(L,1,0,0) != 0) {
    PRINTNOLOG( "111111111111111\n" );
        perror("business_lua,lua_pcall().2");
        PRINTNOLOG("%s\n",lua_tostring(L, -1));
        lua_close(L);
        return EOF;
    }
    PRINTNOLOG( "111111111111111\n" );
    lua_getglobal(L, "message");
    PRINTNOLOG( "4444444444\n" );
    strcpy(buff, lua_tostring(L,-1));
    PRINTNOLOG( "2222222222\n" );
	lua_close(L);
    PRINTNOLOG( "33333333333\n" );
    return strlen(buff);
}
