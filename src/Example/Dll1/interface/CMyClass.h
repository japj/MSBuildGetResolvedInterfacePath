#pragma once
#ifdef DLL1_EXPORTS
#define DLL1_API __declspec(dllexport)
#else
#define DLL1_API __declspec(dllimport)
#endif

class DLL1_API CMyClass
{
public:
    CMyClass();
    ~CMyClass();

    void foo();
};

