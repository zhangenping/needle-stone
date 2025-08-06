#include <iostream>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int main() {
    // ���� Lua ״̬
    lua_State* L = luaL_newstate();
    luaL_openlibs(L); // �򿪱�׼��

    // ִ�� Lua ����
    const char* luaCode = "print('Hello from Lua!')";
    if (luaL_dostring(L, luaCode) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    // �ر� Lua ״̬
    lua_close(L);
    return 0;
}