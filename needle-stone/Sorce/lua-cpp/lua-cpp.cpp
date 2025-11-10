#include <iostream>
#include <string>
#include "lua.hpp"
#include <filesystem>
#include "animal.h"
#include "dll1.h"

using namespace std;

namespace fs = std::filesystem;

void printAnimalInfo(const Animal& animal);

bool checkLua(lua_State* L, int status) {
    if (status != LUA_OK) {
        const char* errorMsg = lua_tostring(L, -1);
        std::cerr << "Lua Error: " << errorMsg << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    // 创建Lua状态机
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    fs::path exePath = fs::path(argv[0]).parent_path();
    fs::path luaScriptPath = exePath / "babe-lua" / "kits.lua";

    // 加载Lua脚本
    if (!checkLua(L, luaL_dofile(L, luaScriptPath.string().c_str()))) {
        lua_close(L);
        return 1;
    }

    // 调用Lua中的sayHello函数
    lua_getglobal(L, "sayHello"); // 获取函数

    if (!lua_isfunction(L, -1)) {
        std::cerr << "Error: 'sayHello' is not a function" << std::endl;
        lua_close(L);
        return 1;
    }

    // 调用函数（0个参数，0个返回值）
    if (!checkLua(L, lua_pcall(L, 0, 0, 0))) {
        lua_close(L);
        return 1;
    }

    // 带参数的调用示例
    lua_getglobal(L, "greet"); // 获取greet函数
    if (lua_isfunction(L, -1)) {
        lua_pushstring(L, "Alice"); // 压入第一个参数
        lua_pushinteger(L, 25);     // 压入第二个参数
        if (!checkLua(L, lua_pcall(L, 2, 1, 0))) { // 2个参数，1个返回值
            lua_close(L);
            return 1;
        }

        // 获取返回值
        if (lua_isstring(L, -1)) {
            std::cout << "Lua returned: " << lua_tostring(L, -1) << std::endl;
        }
        lua_pop(L, 1); // 弹出返回值
    }

    lua_close(L);

    Cat cat;
    Dog dog;

    printAnimalInfo(cat);
    printAnimalInfo(dog);
    
    dll1 mydll;
    mydll.add(1, 2);


    return 0;
}