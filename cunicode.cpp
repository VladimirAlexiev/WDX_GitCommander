#include "StdAfx.h"
#include "cunicode.h"

#include <stdio.h>
#include <stdlib.h>

char usysychecked = 0;

BOOL usys()
{
    if (!usysychecked) {
        OSVERSIONINFO vx;
        vx.dwOSVersionInfoSize = sizeof(vx);
        GetVersionEx(&vx);
        if (vx.dwPlatformId == VER_PLATFORM_WIN32_NT)
            usysychecked = 1;
        else
            usysychecked = 2;
    }
    return (usysychecked == 1);
}

wchar_t* cp2u(UINT codePage, const char* src)
{
    if (!src)
        return 0;
    int srcLen = (int)strlen(src);
    if (!srcLen) {
        wchar_t* w = new wchar_t[1];
        w[0] = 0;
        return w;
    }

    int requiredSize = MultiByteToWideChar(codePage, 0, src, srcLen, 0, 0);

    if (!requiredSize) {
        return 0;
    }

    wchar_t* w = new wchar_t[requiredSize + 1];
    w[requiredSize] = 0;

    int retval = MultiByteToWideChar(codePage, 0, src, srcLen, w, requiredSize);
    if (!retval) {
        delete[] w;
        return 0;
    }

    return w;
}

char* u2cp(UINT codePage, const wchar_t* src)
{
    if (!src)
        return 0;
    int srcLen = (int)wcslen(src);
    if (!srcLen) {
        char* x = new char[1];
        x[0] = '\0';
        return x;
    }

    int requiredSize = WideCharToMultiByte(codePage, 0, src, srcLen, 0, 0, 0, 0);

    if (!requiredSize) {
        return 0;
    }

    char* x = new char[requiredSize + 1];
    x[requiredSize] = 0;

    int retval = WideCharToMultiByte(codePage, 0, src, srcLen, x, requiredSize, 0, 0);
    if (!retval) {
        delete[] x;
        return 0;
    }

    return x;
}

char* walcopy(char* outname, const wchar_t* inname, int maxSize)
{
    if (inname) {
        WideCharToMultiByte(CP_ACP, 0, inname, -1, outname, maxSize, NULL, NULL);
        outname[maxSize - 1] = 0;
        return outname;
    } else
        return nullptr;
}

wchar_t* awlcopy(wchar_t* outname, const char* inname, int maxSize)
{
    if (inname) {
        MultiByteToWideChar(CP_ACP, 0, inname, -1, outname, maxSize);
        outname[maxSize - 1] = L'\0';
        return outname;
    } else
        return nullptr;
}

wchar_t* wcslcpy(wchar_t* str1, const wchar_t* str2, int maxSize)
{
    if ((int)wcslen(str2) >= maxSize - 1) {
        wcsncpy(str1, str2, maxSize - 1);
        str1[maxSize - 1] = L'\0';
    } else
        wcscpy(str1, str2);
    return str1;
}

wchar_t* wcslcat(wchar_t* str1, const wchar_t* str2, int maxSize)
{
    int l1 = (int)wcslen(str1);
    if ((int)wcslen(str2) >= maxSize - 1 - l1) {
        wcsncat(str1, str2, maxSize - 1 - l1);
        str1[maxSize - 1] = L'\0';
    } else
        wcscat(str1, str2);
    return str1;
}

// return true if name wasn't cut
BOOL MakeExtraLongNameW(wchar_t* outbuf, const wchar_t* inbuf, int maxSize)
{
    if (wcslen(inbuf) > 259) {
        if (inbuf[0] == '\\' && inbuf[1] == '\\') {  // UNC-Path! Use \\?\UNC\server\share\subdir\name.ext
            wcslcpy(outbuf, L"\\\\?\\UNC", maxSize);
            wcslcat(outbuf, inbuf + 1, maxSize);
        } else {
            wcslcpy(outbuf, L"\\\\?\\", maxSize);
            wcslcat(outbuf, inbuf, maxSize);
        }
    } else
        wcslcpy(outbuf, inbuf, maxSize);
    return (int)wcslen(inbuf) + 3 <= maxSize;
}

/***********************************************************************************************/

void copyfinddatawa(WIN32_FIND_DATAA* lpFindFileDataA, WIN32_FIND_DATAW* lpFindFileDataW)
{
    walcopy(lpFindFileDataA->cAlternateFileName, lpFindFileDataW->cAlternateFileName,
            sizeof(lpFindFileDataW->cAlternateFileName));
    walcopy(lpFindFileDataA->cFileName, lpFindFileDataW->cFileName, sizeof(lpFindFileDataW->cFileName));
    lpFindFileDataA->dwFileAttributes = lpFindFileDataW->dwFileAttributes;
    lpFindFileDataA->dwReserved0 = lpFindFileDataW->dwReserved0;
    lpFindFileDataA->dwReserved1 = lpFindFileDataW->dwReserved1;
    lpFindFileDataA->ftCreationTime = lpFindFileDataW->ftCreationTime;
    lpFindFileDataA->ftLastAccessTime = lpFindFileDataW->ftLastAccessTime;
    lpFindFileDataA->ftLastWriteTime = lpFindFileDataW->ftLastWriteTime;
    lpFindFileDataA->nFileSizeHigh = lpFindFileDataW->nFileSizeHigh;
    lpFindFileDataA->nFileSizeLow = lpFindFileDataW->nFileSizeLow;
}

void copyfinddataaw(WIN32_FIND_DATAW* lpFindFileDataW, WIN32_FIND_DATAA* lpFindFileDataA)
{
    awlcopy(lpFindFileDataW->cAlternateFileName, lpFindFileDataA->cAlternateFileName,
            countof(lpFindFileDataW->cAlternateFileName));
    awlcopy(lpFindFileDataW->cFileName, lpFindFileDataA->cFileName, countof(lpFindFileDataW->cFileName));
    lpFindFileDataW->dwFileAttributes = lpFindFileDataA->dwFileAttributes;
    lpFindFileDataW->dwReserved0 = lpFindFileDataA->dwReserved0;
    lpFindFileDataW->dwReserved1 = lpFindFileDataA->dwReserved1;
    lpFindFileDataW->ftCreationTime = lpFindFileDataA->ftCreationTime;
    lpFindFileDataW->ftLastAccessTime = lpFindFileDataA->ftLastAccessTime;
    lpFindFileDataW->ftLastWriteTime = lpFindFileDataA->ftLastWriteTime;
    lpFindFileDataW->nFileSizeHigh = lpFindFileDataA->nFileSizeHigh;
    lpFindFileDataW->nFileSizeLow = lpFindFileDataA->nFileSizeLow;
}

/***********************************************************************************************/

BOOL CopyFileT(wchar_t* lpExistingFileName, wchar_t* lpNewFileName, BOOL bFailIfExists)
{
    if (usys()) {
        wchar_t wbuf1[wdirtypemax + longnameprefixmax];
        wchar_t wbuf2[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf1, lpExistingFileName, wdirtypemax + longnameprefixmax) &&
            MakeExtraLongNameW(wbuf2, lpNewFileName, wdirtypemax + longnameprefixmax))
            return CopyFileW(wbuf1, wbuf2, bFailIfExists);
        else
            return false;
    } else {
        char buf1[_MAX_PATH];
        char buf2[_MAX_PATH];
        return CopyFileA(wafilenamecopy(buf1, lpExistingFileName), wafilenamecopy(buf2, lpNewFileName), bFailIfExists);
    }
}

BOOL CreateDirectoryT(wchar_t* lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    if (usys()) {
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpPathName, wdirtypemax + longnameprefixmax))
            return CreateDirectoryW(wbuf, lpSecurityAttributes);
        else
            return false;
    } else {
        char buf[_MAX_PATH];
        return CreateDirectoryA(wafilenamecopy(buf, lpPathName), lpSecurityAttributes);
    }
}

BOOL RemoveDirectoryT(wchar_t* lpPathName)
{
    if (usys()) {
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpPathName, wdirtypemax + longnameprefixmax))
            return RemoveDirectoryW(wbuf);
        else
            return false;
    } else {
        char buf[_MAX_PATH];
        return RemoveDirectoryA(wafilenamecopy(buf, lpPathName));
    }
}

BOOL DeleteFileT(wchar_t* lpFileName)
{
    if (usys()) {
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpFileName, wdirtypemax + longnameprefixmax))
            return DeleteFileW(wbuf);
        else
            return false;
    } else {
        char buf[_MAX_PATH];
        return DeleteFileA(wafilenamecopy(buf, lpFileName));
    }
}

BOOL MoveFileT(wchar_t* lpExistingFileName, wchar_t* lpNewFileName)
{
    if (usys()) {
        wchar_t wbuf1[wdirtypemax + longnameprefixmax];
        wchar_t wbuf2[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf1, lpExistingFileName, wdirtypemax + longnameprefixmax) &&
            MakeExtraLongNameW(wbuf2, lpNewFileName, wdirtypemax + longnameprefixmax))
            return MoveFileW(wbuf1, wbuf2);
        else
            return false;
    } else {
        char buf1[_MAX_PATH];
        char buf2[_MAX_PATH];
        return MoveFileA(wafilenamecopy(buf1, lpExistingFileName), wafilenamecopy(buf2, lpNewFileName));
    }
}

BOOL SetFileAttributesT(wchar_t* lpFileName, DWORD dwFileAttributes)
{
    if (usys()) {
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpFileName, wdirtypemax + longnameprefixmax))
            return SetFileAttributesW(wbuf, dwFileAttributes);
        else
            return false;
    } else {
        char buf[_MAX_PATH];
        return SetFileAttributesA(wafilenamecopy(buf, lpFileName), dwFileAttributes);
    }
}

HANDLE CreateFileT(const wchar_t* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                   HANDLE hTemplateFile)
{
    if (usys()) {
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpFileName, wdirtypemax + longnameprefixmax))
            return CreateFileW(wbuf, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
                               dwFlagsAndAttributes, hTemplateFile);
        else
            return INVALID_HANDLE_VALUE;
    } else {
        char buf[_MAX_PATH];
        return CreateFileA(wafilenamecopy(buf, lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes,
                           dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }
}

HANDLE FindFirstFileT(const wchar_t* lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
{
    if (usys()) {
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpFileName, wdirtypemax + longnameprefixmax))
            return FindFirstFileW(wbuf, lpFindFileData);
        else
            return INVALID_HANDLE_VALUE;
    } else {
        char buf[_MAX_PATH];
        WIN32_FIND_DATAA FindFileDataA;
        HANDLE retval = FindFirstFileA(wafilenamecopy(buf, lpFileName), &FindFileDataA);
        if (retval != INVALID_HANDLE_VALUE) {
            awlcopy(lpFindFileData->cAlternateFileName, FindFileDataA.cAlternateFileName,
                    countof(lpFindFileData->cAlternateFileName));
            awlcopy(lpFindFileData->cFileName, FindFileDataA.cFileName, countof(lpFindFileData->cFileName));
            lpFindFileData->dwFileAttributes = FindFileDataA.dwFileAttributes;
            lpFindFileData->dwReserved0 = FindFileDataA.dwReserved0;
            lpFindFileData->dwReserved1 = FindFileDataA.dwReserved1;
            lpFindFileData->ftCreationTime = FindFileDataA.ftCreationTime;
            lpFindFileData->ftLastAccessTime = FindFileDataA.ftLastAccessTime;
            lpFindFileData->ftLastWriteTime = FindFileDataA.ftLastWriteTime;
            lpFindFileData->nFileSizeHigh = FindFileDataA.nFileSizeHigh;
            lpFindFileData->nFileSizeLow = FindFileDataA.nFileSizeLow;
        }
        return retval;
    }
}

BOOL FindNextFileT(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
    if (usys()) {
        return FindNextFileW(hFindFile, lpFindFileData);
    } else {
        WIN32_FIND_DATAA FindFileDataA;
        memset(&FindFileDataA, 0, sizeof(FindFileDataA));
        BOOL retval = FindNextFileA(hFindFile, &FindFileDataA);
        if (retval) {
            awlcopy(lpFindFileData->cAlternateFileName, FindFileDataA.cAlternateFileName,
                    countof(lpFindFileData->cAlternateFileName));
            awlcopy(lpFindFileData->cFileName, FindFileDataA.cFileName, countof(lpFindFileData->cFileName));
            lpFindFileData->dwFileAttributes = FindFileDataA.dwFileAttributes;
            lpFindFileData->dwReserved0 = FindFileDataA.dwReserved0;
            lpFindFileData->dwReserved1 = FindFileDataA.dwReserved1;
            lpFindFileData->ftCreationTime = FindFileDataA.ftCreationTime;
            lpFindFileData->ftLastAccessTime = FindFileDataA.ftLastAccessTime;
            lpFindFileData->ftLastWriteTime = FindFileDataA.ftLastWriteTime;
            lpFindFileData->nFileSizeHigh = FindFileDataA.nFileSizeHigh;
            lpFindFileData->nFileSizeLow = FindFileDataA.nFileSizeLow;
        }
        return retval;
    }
}

int MessageBoxT(HWND hWnd, const wchar_t* lpText, const wchar_t* lpCaption, UINT uType)
{
    if (usys()) {
        return MessageBoxW(hWnd, lpText, lpCaption, uType);
    } else {
        char* buf1 = (char*)malloc((wcslen(lpText) + 1) * sizeof(char));
        char* buf2 = (char*)malloc((wcslen(lpCaption) + 1) * sizeof(char));
        if (buf1 && buf2) {
            walcopy(buf1, lpText, (int)wcslen(lpText) + 1);
            walcopy(buf2, lpCaption, (int)wcslen(lpCaption) + 1);
            int res = MessageBoxA(hWnd, buf1, buf2, uType);
            free(buf1);
            free(buf2);
            return res;
        } else {
            if (buf1)
                free(buf1);
            if (buf2)
                free(buf2);
            return 0;
        }
    }
}

DWORD GetModuleFileNameT(HMODULE hModule, wchar_t* lpFileName, DWORD nSize)
{
    if (usys()) {
        return GetModuleFileNameW(hModule, lpFileName, nSize);
    } else {
        DWORD res = 0;
        char* buf = (char*)malloc(nSize * sizeof(char));
        if (buf) {
            res = GetModuleFileNameA(hModule, buf, nSize);
            if (res != 0)
                awlcopy(lpFileName, buf, nSize);
            free(buf);
        }
        return res;
    }
}

void DisplayLastErrorMsgA(const char* lpFunction, const char* lpFileName, UINT msgType)
{
    // retrieve the system error message for the last-error code
    DWORD dw = GetLastError();
    if (dw) {
        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&lpMsgBuf, 0, NULL);
        // display the error message
        size_t s = strlen((const char*)lpMsgBuf) + strlen((const char*)lpFunction) + 40;
        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (s + 1) * sizeof(char));
        _snprintf((char*)lpDisplayBuf, s, "%s failed with error %d: %s", lpFunction, dw, (char*)lpMsgBuf);
        MessageBoxA(NULL, (const char*)lpDisplayBuf, lpFileName, msgType);
        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
    }
}

void DisplayLastErrorMsgT(const wchar_t* lpFunction, const wchar_t* lpFileName, UINT msgType)
{
    // retrieve the system error message for the last-error code
    LPVOID lpMsgBuf = nullptr;
    LPVOID lpDisplayBuf = nullptr;
    DWORD dw = GetLastError();
    if (dw) {
        if (usys()) {
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (wchar_t*)&lpMsgBuf, 0, NULL);
            // display the error message
            size_t s = wcslen((const wchar_t*)lpMsgBuf) + wcslen((const wchar_t*)lpFunction) + 40;
            lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (s + 1) * sizeof(wchar_t));
            _snwprintf((wchar_t*)lpDisplayBuf, s, L"%s failed with error %d: %s", lpFunction, dw, (wchar_t*)lpMsgBuf);
            wchar_t wbuf[wdirtypemax + longnameprefixmax];
            if (MakeExtraLongNameW(wbuf, lpFileName, wdirtypemax + longnameprefixmax))
                MessageBoxW(NULL, (const wchar_t*)lpDisplayBuf, (const wchar_t*)wbuf, msgType);
        } else {
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&lpMsgBuf, 0, NULL);
            // display the error message
            char* buf1 = (char*)malloc((wcslen(lpFunction) + 1) * sizeof(char));
            if (buf1) {
                walcopy(buf1, lpFunction, (int)wcslen(lpFunction) + 1);
                size_t s = strlen((const char*)lpMsgBuf) + strlen((const char*)lpFunction) + 40;
                lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (s + 1) * sizeof(char));
                _snprintf((char*)lpDisplayBuf, s, "%s failed with error %d: %s", buf1, dw, (char*)lpMsgBuf);
                char buf[_MAX_PATH];
                MessageBoxA(NULL, (const char*)lpDisplayBuf, (const char*)wafilenamecopy(buf, lpFileName), msgType);
                free(buf1);
            }
        }
    }

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

void DisplayLastErrorMsgW(const wchar_t* lpFunction, const wchar_t* lpFileName, UINT msgType)
{
    // retrieve the system error message for the last-error code
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError();
    if (dw) {
        LPVOID lpDisplayBuf;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (wchar_t*)&lpMsgBuf, 0, NULL);
        // display the error message
        size_t s = wcslen((const wchar_t*)lpMsgBuf) + wcslen((const wchar_t*)lpFunction) + 40;
        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (s + 1) * sizeof(wchar_t));
        _snwprintf((wchar_t*)lpDisplayBuf, s, L"%s failed with error %d: %s", lpFunction, dw, (wchar_t*)lpMsgBuf);
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpFileName, wdirtypemax + longnameprefixmax))
            MessageBoxW(NULL, (const wchar_t*)lpDisplayBuf, (const wchar_t*)wbuf, msgType);
        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
    }
}

BOOL SetDlgItemTextT(HWND hDlg, int nIDDlgItem, const wchar_t* lpString)
{
    if (usys()) {
        return SetDlgItemTextW(hDlg, nIDDlgItem, lpString);
    } else {
        BOOL res = FALSE;
        char* buf = new char[wcslen(lpString) + 1];
        if (buf) {
            walcopy(buf, lpString, (int)wcslen(lpString) + 1);
            res = SetDlgItemTextA(hDlg, nIDDlgItem, buf);
            delete[] buf;
        }
        return res;
    }
}

UINT GetDlgItemTextT(HWND hDlg, int nIDDlgItem, wchar_t* lpString, int cchMax)
{
    if (usys()) {
        return GetDlgItemTextW(hDlg, nIDDlgItem, lpString, cchMax);
    } else {
        BOOL res = FALSE;
        char* buf = new char[cchMax + 1];
        if (buf) {
            walcopy(buf, lpString, min(cchMax, (int)wcslen(lpString)) + 1);
            res = GetDlgItemTextA(hDlg, nIDDlgItem, buf, cchMax);
            if (res)
                awlcopy(lpString, buf, cchMax + 1);
            delete[] buf;
        }
        return res;
    }
}

DWORD GetPrivateProfileStringT(const wchar_t* lpAppName, const wchar_t* lpKeyName, const wchar_t* lpDefault,
                               wchar_t* lpReturnedString, DWORD nSize, const wchar_t* lpFileName)
{
    if (usys()) {
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpFileName, wdirtypemax + longnameprefixmax))
            return GetPrivateProfileStringW(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, wbuf);
        else
            return 0;
    } else {
        char* buf1 = lpAppName ? (char*)malloc((wcslen(lpAppName) + 1) * sizeof(char)) : nullptr;
        char* buf2 = lpKeyName ? (char*)malloc((wcslen(lpKeyName) + 1) * sizeof(char)) : nullptr;
        char* buf3 = lpDefault ? (char*)malloc((wcslen(lpDefault) + 1) * sizeof(char)) : nullptr;
        char* buf4 = (char*)malloc(nSize * sizeof(char));
        if ((lpAppName && buf1 && lpKeyName && buf2 && lpDefault && buf3 && buf4) || (buf4)) {
            char buf5[_MAX_PATH];
            if (lpAppName)
                walcopy(buf1, lpAppName, (int)wcslen(lpAppName) + 1);
            if (lpKeyName)
                walcopy(buf2, lpKeyName, (int)wcslen(lpKeyName) + 1);
            if (lpDefault)
                walcopy(buf3, lpDefault, (int)wcslen(lpDefault) + 1);
            DWORD res = GetPrivateProfileStringA(buf1, buf2, buf3, buf4, nSize, wafilenamecopy(buf5, lpFileName));
            if (res > 0) {
                MultiByteToWideChar(CP_ACP, 0, buf4, res, lpReturnedString, nSize);
                lpReturnedString[res] = 0;
            }
            if (lpAppName)
                free(buf1);
            if (lpKeyName)
                free(buf2);
            if (lpDefault)
                free(buf3);
            free(buf4);
            return res;
        } else {
            free(buf4);
            return 0;
        }
    }
}

UINT GetPrivateProfileIntT(const wchar_t* lpAppName, const wchar_t* lpKeyName, INT nDefault, const wchar_t* lpFileName)
{
    if (usys()) {
        wchar_t wbuf[wdirtypemax + longnameprefixmax];
        if (MakeExtraLongNameW(wbuf, lpFileName, wdirtypemax + longnameprefixmax))
            return GetPrivateProfileIntW(lpAppName, lpKeyName, nDefault, wbuf);
        else
            return 0;
    } else {
        char* buf1 = lpAppName ? (char*)malloc((wcslen(lpAppName) + 1) * sizeof(char)) : nullptr;
        char* buf2 = lpKeyName ? (char*)malloc((wcslen(lpKeyName) + 1) * sizeof(char)) : nullptr;
        {
            char buf5[_MAX_PATH];
            if (lpAppName)
                walcopy(buf1, lpAppName, (int)wcslen(lpAppName) + 1);
            if (lpKeyName)
                walcopy(buf2, lpKeyName, (int)wcslen(lpKeyName) + 1);
            UINT res = GetPrivateProfileIntA(buf1, buf2, nDefault, wafilenamecopy(buf5, lpFileName));
            if (lpAppName)
                free(buf1);
            if (lpKeyName)
                free(buf2);
            return res;
        }
    }
}
