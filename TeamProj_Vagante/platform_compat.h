#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>

typedef struct { int32_t left, top, right, bottom; } RECT;
typedef struct { int32_t x, y; } POINT;

typedef uint32_t DWORD;
typedef uint8_t BYTE;
typedef int32_t BOOL;
typedef uint32_t COLORREF;
typedef void* HINSTANCE;
typedef void* HWND;
typedef void* HDC;
typedef void* HBITMAP;
typedef void* HBRUSH;
typedef void* HCURSOR;
typedef void* HICON;
typedef void* HMENU;
typedef void* HDC_;
typedef void* LPARAM;
typedef void* WPARAM;
typedef void* LPVOID;
typedef uint32_t UINT;
typedef int32_t INT;
typedef uint16_t WORD;
typedef int32_t LONG;
typedef uintptr_t LRESULT;
typedef char* LPSTR;
typedef const char* LPCSTR;

typedef int HRESULT;

#define TRUE 1
#define FALSE 0

#define S_OK 0
#define E_FAIL 1
#define FAILED(hr) ((HRESULT)(hr) != S_OK)
#define SUCCEEDED(hr) ((HRESULT)(hr) == S_OK)

#define CALLBACK
#define APIENTRY

#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)) | ((WORD)((BYTE)(g)) << 8) | ((DWORD)((BYTE)(b)) << 16)))
#define itoa(n, buf, base) (sprintf(buf, "%d", n), buf)

// VK constants mapped to SDL scancode values
#define VK_LEFT    80
#define VK_UP      82
#define VK_RIGHT   79
#define VK_DOWN    81
#define VK_SPACE   44
#define VK_LSHIFT  225
#define VK_LBUTTON 254
#define VK_A       4
#define VK_Z       29
#define VK_X       27
#define VK_C       6
#define VK_V       55
#define VK_F3      60
#define VK_F4      61

inline void ZeroMemory(void* ptr, size_t size) {
    memset(ptr, 0, size);
}

inline BOOL IntersectRect(RECT* dst, const RECT* a, const RECT* b) {
    dst->left = (a->left > b->left) ? a->left : b->left;
    dst->top = (a->top > b->top) ? a->top : b->top;
    dst->right = (a->right < b->right) ? a->right : b->right;
    dst->bottom = (a->bottom < b->bottom) ? a->bottom : b->bottom;
    if (dst->left < dst->right && dst->top < dst->bottom)
        return TRUE;
    dst->left = dst->top = dst->right = dst->bottom = 0;
    return FALSE;
}

inline void OffsetRect(RECT* rc, int dx, int dy) {
    rc->left += dx;
    rc->top += dy;
    rc->right += dx;
    rc->bottom += dy;
}

inline BOOL PtInRect(const RECT* rc, POINT pt) {
    return (pt.x >= rc->left && pt.x < rc->right && pt.y >= rc->top && pt.y < rc->bottom);
}
