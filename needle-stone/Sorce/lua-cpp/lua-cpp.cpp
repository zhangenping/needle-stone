#include <iostream>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int main() {
    // 创建 Lua 状态
    lua_State* L = luaL_newstate();
    luaL_openlibs(L); // 打开标准库

    // 执行 Lua 代码
    const char* luaCode = "print('Hello from Lua!')";
    if (luaL_dostring(L, luaCode) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    // 关闭 Lua 状态
    lua_close(L);
    return 0;
}