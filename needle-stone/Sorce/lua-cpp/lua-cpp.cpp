#include <iostream>
#include <string>
#include "lua.hpp"
#include <filesystem>
#include "animal.h"
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
    // ����Lua״̬��
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    fs::path exePath = fs::path(argv[0]).parent_path();
    fs::path luaScriptPath = exePath / "babe-lua" / "kits.lua";

    // ����Lua�ű�
    if (!checkLua(L, luaL_dofile(L, luaScriptPath.string().c_str()))) {
        lua_close(L);
        return 1;
    }

    // ����Lua�е�sayHello����
    lua_getglobal(L, "sayHello"); // ��ȡ����

    if (!lua_isfunction(L, -1)) {
        std::cerr << "Error: 'sayHello' is not a function" << std::endl;
        lua_close(L);
        return 1;
    }

    // ���ú�����0��������0������ֵ��
    if (!checkLua(L, lua_pcall(L, 0, 0, 0))) {
        lua_close(L);
        return 1;
    }

    // �������ĵ���ʾ��
    lua_getglobal(L, "greet"); // ��ȡgreet����
    if (lua_isfunction(L, -1)) {
        lua_pushstring(L, "Alice"); // ѹ���һ������
        lua_pushinteger(L, 25);     // ѹ��ڶ�������
        if (!checkLua(L, lua_pcall(L, 2, 1, 0))) { // 2��������1������ֵ
            lua_close(L);
            return 1;
        }

        // ��ȡ����ֵ
        if (lua_isstring(L, -1)) {
            std::cout << "Lua returned: " << lua_tostring(L, -1) << std::endl;
        }
        lua_pop(L, 1); // ��������ֵ
    }

    lua_close(L);

    Cat cat;
    Dog dog;

    printAnimalInfo(cat);
    printAnimalInfo(dog);

    return 0;
}