#include "bussiness.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "string.h"

int main_service(char *buff)
{
    int flag;
    printf("start main service!\n");
    flag = business_lua(buff);
    printf("end main service!\n");
    return flag;
}

int business_lua(char *buff) 
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
    if(luaL_loadfile(L,LUA_MAIN_PATH) != 0){
        perror("business_lua,luaL_loadfile()");
        printf("%s\n",lua_tostring(L, -1));
        lua_close(L);
        return EOF;
    }
    if(lua_pcall(L,0,0,0) != 0) {
        perror("business_lua,lua_pcall().1");
        printf("%s\n",lua_tostring(L, -1));
        lua_close(L);
        return EOF;
    }
    lua_getglobal(L, "main");
    lua_pushstring(L, buff);
    if(lua_pcall(L,1,0,0) != 0) {
        perror("business_lua,lua_pcall().2");
        printf("%s\n",lua_tostring(L, -1));
        lua_close(L);
        return EOF;
    }
    lua_getglobal(L, "message");
    strcpy(buff, lua_tostring(L,-1));
	lua_close(L);
    return strlen(buff);
}
