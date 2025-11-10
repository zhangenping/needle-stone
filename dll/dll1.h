#pragma once



#if defined(_DL_DLL)
#define CC_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define CC_DLL     __declspec(dllimport)
#endif

class CC_DLL dll1
{
	public:
		int add(int a, int b);
};