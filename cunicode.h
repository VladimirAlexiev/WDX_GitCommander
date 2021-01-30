#ifndef _CUNICODE_H_
#define _CUNICODE_H_

BOOL usys();

#define wdirtypemax 1024
#define longnameprefixmax 6

#ifndef countof
#define countof(str) (sizeof(str) / sizeof(str[0]))
#endif  // countof

wchar_t* cp2u(UINT codePage, const char* src);
char* u2cp(UINT codePage, const wchar_t* src);

wchar_t* wcslcpy(wchar_t* str1, const wchar_t* str2, int maxSize);
wchar_t* wcslcat(wchar_t* str1, const wchar_t* str2, int maxSize);
char* walcopy(char* outname, const wchar_t* inname, int maxSize);
wchar_t* awlcopy(wchar_t* outname, const char* inname, int maxSize);

#define wafilenamecopy(outname, inname) walcopy(outname, inname, countof(outname))
#define awfilenamecopy(outname, inname) awlcopy(outname, inname, countof(outname))

void copyfinddatawa(WIN32_FIND_DATAA* lpFindFileDataA, WIN32_FIND_DATAW* lpFindFileDataW);
void copyfinddataaw(WIN32_FIND_DATAW* lpFindFileDataW, WIN32_FIND_DATAA* lpFindFileDataA);

BOOL MakeExtraLongNameW(wchar_t* outbuf, const wchar_t* inbuf, int maxSize);
BOOL CopyFileT(wchar_t* lpExistingFileName, wchar_t* lpNewFileName, BOOL bFailIfExists);
BOOL CreateDirectoryT(wchar_t* lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
BOOL RemoveDirectoryT(wchar_t* lpPathName);
BOOL DeleteFileT(wchar_t* lpFileName);
BOOL MoveFileT(wchar_t* lpExistingFileName, wchar_t* lpNewFileName);
BOOL SetFileAttributesT(wchar_t* lpFileName, DWORD dwFileAttributes);
HANDLE CreateFileT(const wchar_t* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                   HANDLE hTemplateFile);
//UINT ExtractIconExT(wchar_t* lpszFile, int nIconIndex, HICON *phiconLarge, HICON *phiconSmall, UINT nIcons);
HANDLE FindFirstFileT(const wchar_t* lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
BOOL FindNextFileT(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);

int MessageBoxT(HWND hWnd, const wchar_t* lpText, const wchar_t* lpCaption, UINT uType);
DWORD GetModuleFileNameT(HMODULE hModule, wchar_t* lpFileName, DWORD nSize);
void DisplayLastErrorMsgA(const char* lpFunction, const char* lpFileName, UINT msgType);
void DisplayLastErrorMsgT(const wchar_t* lpszFunction, const wchar_t* lpszFileName, UINT msgType);
void DisplayLastErrorMsgW(const wchar_t* lpszFunction, const wchar_t* lpszFileName, UINT msgType);
BOOL SetDlgItemTextT(HWND hDlg, int nIDDlgItem, const wchar_t* lpString);
UINT GetDlgItemTextT(HWND hDlg, int nIDDlgItem, wchar_t* lpString, int cchMax);
DWORD GetPrivateProfileStringT(const wchar_t* lpAppName, const wchar_t* lpKeyName, const wchar_t* lpDefault,
                               wchar_t* lpReturnedString, DWORD nSize, const wchar_t* lpFileName);
UINT GetPrivateProfileIntT(const wchar_t* lpAppName, const wchar_t* lpKeyName, INT nDefault, const wchar_t* lpFileName);

#endif
