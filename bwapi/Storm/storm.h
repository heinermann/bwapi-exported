#pragma once
#include <windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <stdlib.h>
#include <ddraw.h>

#define arrsize(x) (sizeof(x)/sizeof(x[0]))   // Official name
#define countof(x) arrsize(x)                 // Personal name

#ifndef BLIZZ_STRUCTS
#define BLIZZ_STRUCTS
struct WRECT
{
  WORD  left;
  WORD  top;
  WORD  right;
  WORD  bottom;
};

struct WPOINT
{
  WORD  x;
  WORD  y;
};

struct WSIZE
{
  WORD  cx;
  WORD  cy;
};
#endif

BOOL __stdcall SNetCreateGame(char *Source, char *a2, char *a3, int a4, int a5, int a6, char *a7, char *a8, int *playerid);
BOOL __stdcall SNetDestroy();
BOOL __stdcall SNetEnumProviders(int (__stdcall *callback)(DWORD, DWORD, DWORD, DWORD), int mincaps);

BOOL __stdcall SNetEnumGames(int (__stdcall *callback)(DWORD, DWORD, DWORD), int *hintnextcall);
BOOL __stdcall SNetDropPlayer(int playerid, DWORD flags);
BOOL __stdcall SNetGetGameInfo(int type, char *src, unsigned int length, int *byteswritten);

BOOL __stdcall SNetGetNumPlayers(int *firstplayerid, int *lastplayerid, int *activeplayers);

BOOL __stdcall SNetGetPlayerCaps(char playerid, int *caps);
BOOL __stdcall SNetGetPlayerName(int playerid, char *buffer, size_t buffersize);
BOOL __stdcall SNetGetProviderCaps(DWORD *caps);
BOOL __stdcall SNetGetTurnsInTransit(int *turns);
BOOL __stdcall SNetInitializeDevice(int a1, int a2, int a3, int a4, int *a5);
BOOL __stdcall SNetInitializeProvider(DWORD providerName, int *providerData, int *userData, int *a4, int *a5);
BOOL __stdcall SNetJoinGame(unsigned int a1, char *gameName, char *gamePassword, char *playerName, char *userStats, int *playerid);
BOOL __stdcall SNetLeaveGame(int type);
BOOL __stdcall SNetPerformUpgrade(DWORD *upgradestatus);
BOOL __stdcall SNetReceiveMessage(int *senderplayerid, char **data, int *databytes);
BOOL __stdcall SNetReceiveTurns(int a1, int arraysize, char **arraydata, unsigned int *arraydatabytes, DWORD *arrayplayerstatus);
HANDLE __stdcall SNetRegisterEventHandler(int type, int sevent);

int __stdcall SNetSelectGame(int a1, int a2, int a3, int a4, int a5, int *playerid);

BOOL __stdcall SNetSendMessage(unsigned int playerID, char *data, unsigned int databytes);
BOOL __stdcall SNetSendTurn(char *data, unsigned int databytes);

BOOL __stdcall SNetSetGameMode(DWORD modeFlags, char a2);

BOOL __stdcall SNetEnumGamesEx(int a1, int a2, int (__fastcall *callback)(DWORD, DWORD, DWORD), int *hintnextcall);
int __stdcall SNetSendServerChatCommand(const char *command);

BOOL __stdcall SNetGetPlayerNames(DWORD flags);
BOOL __stdcall SNetCreateLadderGame(char *gameName, char *gamePassword, char *gameDataString, DWORD gameType, int a5, int a6, char *GameTemplateData, int GameTemplateSize, int playerCount, char *playerName, char *a11, int *playerID);
BOOL __stdcall SNetReportGameResult(unsigned int a1, int size, int a3, int a4, int a5);

int __stdcall SNetSendLeagueCommand(char *cmd, char *callback);
int __stdcall SNetSendReplayPath(int a1, int a2, char *replayPath);
int __stdcall SNetGetLeagueName(int leagueID);
BOOL __stdcall SNet144(char *buffer);
int __stdcall SNetLeagueLogout(char *bnetName);
int __stdcall SNetGetLeaguePlayerName(int *curPlayerID, size_t nameSize);

HGDIOBJ __stdcall SDlgDefDialogProc(HWND hDlg, signed int DlgType, HDC textLabel, HWND hWnd);

HANDLE __stdcall SDlgDialogBoxIndirectParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam);

BOOL __stdcall SDlgEndDialog(HWND hDlg, HANDLE nResult);

BOOL __stdcall SDlgSetControlBitmaps(HWND parentwindow, int *id, int a3, char *buffer2, char *buffer, int flags, int mask);

BOOL __stdcall SDlgBltToWindowE(HWND hWnd, HRGN a2, char *a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);
BOOL __stdcall SDlgSetBitmapE(HWND hWnd, int a2, char *src, int mask1, int flags, int a6, int a7, int width, int a9, int mask2);

int __stdcall Ordinal224(int a1);

BOOL __stdcall SFileCloseArchive(HANDLE hArchive);
BOOL __stdcall SFileCloseFile(HANDLE hFile);

BOOL __stdcall SFileDdaBeginEx(HANDLE directsound, DWORD flags, DWORD mask, unsigned __int32 lDistanceToMove, signed __int32 volume, signed int a6, int a7);
BOOL __stdcall SFileDdaDestroy();
BOOL __stdcall SFileDdaEnd(HANDLE directsound);
BOOL __stdcall SFileDdaGetPos(HANDLE directsound, int a2, int a3);

BOOL __stdcall SFileDdaInitialize(HANDLE directsound);
BOOL __stdcall SFileDdaSetVolume(HANDLE directsound, signed int bigvolume, signed int volume);

BOOL __stdcall SFileGetFileArchive(HANDLE hFile, HANDLE archive);
LONG __stdcall SFileGetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);
BOOL __stdcall SFileOpenArchive(const char *szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE *phMpq);

// values for dwFlags
#define MPQ_NO_LISTFILE       0x0010
#define MPQ_NO_ATTRIBUTES     0x0020
#define MPQ_FORCE_V1          0x0040
#define MPQ_CHECK_SECTOR_CRC  0x0080

BOOL __stdcall SFileOpenFile(char *filename, HANDLE *phFile);
BOOL __stdcall SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile);

// values for dwSearchScope
#define SFILE_FROM_MPQ        0x00000000
#define SFILE_FROM_ABSOLUTE   0x00000001
#define SFILE_FROM_RELATIVE   0x00000002
#define SFILE_UNKNOWN_04      0x00000004

BOOL __stdcall SFileReadFile(HANDLE hFile, void *buffer, DWORD nNumberOfBytesToRead, DWORD *read, LONG lpDistanceToMoveHigh);

void __stdcall SFileSetLocale(LCID lcLocale);

BOOL __stdcall SFileSetIoErrorMode(int mode, BOOL (__stdcall *callback)(char*,int,int) );
BOOL __stdcall SFileGetArchiveName(HANDLE hArchive, char *name, int length);
BOOL __stdcall SFileGetFileName(HANDLE hFile, char *buffer, int length);

BOOL __stdcall SFileLoadFile(char *filename, void *buffer, int buffersize, int a4, int a5);
BOOL __stdcall SFileUnloadFile(HANDLE hFile);
BOOL __stdcall SFileLoadFileEx(void *hArchive, char *filename, int a3, int a4, int a5, DWORD searchScope, struct _OVERLAPPED *lpOverlapped);

BOOL __stdcall SBltROP3(int maxiterations, int lpSurface, int width, int height, int width2, int pitch, int a7, DWORD rop);
BOOL __stdcall SBltROP3Clipped(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);

BOOL __stdcall SBmpDecodeImage(int type, signed int *srcbuffer, unsigned int a3, int a4, void *dstbuffer, int size, int a7, int a8, int a9);

BOOL __stdcall SBmpLoadImage(const char *fileName, int size, void *buffer, int buffersize, int width, int height, int depth);

HANDLE __stdcall SBmpAllocLoadImage(const char *fileName, int *palette, void **buffer, int *width, int *height, int unused6, int unused7, int (__stdcall *allocFunction)(DWORD));

BOOL __stdcall SCodeCompile(char *directives1, char *directives2, char *loopstring, unsigned int maxiterations, unsigned int flags, HANDLE handle);
BOOL __stdcall SCodeDelete(HANDLE handle);

int __stdcall SCodeExecute(HANDLE handle, int a2);

BOOL __stdcall SDrawCaptureScreen(char *source);

HWND __stdcall SDrawGetFrameWindow(HWND sdraw_framewindow);

BOOL __stdcall SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused);
BOOL __stdcall SDrawManualInitialize(HWND hWnd, IDirectDraw *ddInterface, IDirectDrawSurface *primarySurface, int a4, int a5, IDirectDrawSurface *backSurface, IDirectDrawPalette *palette, int a8);

BOOL __stdcall SDrawRealizePalette();

BOOL __stdcall SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect);
BOOL __stdcall SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4);

BOOL __stdcall SGdiDeleteObject(HANDLE handle);

BOOL __stdcall SGdiExtTextOut(int a1, int a2, int a3, int a4, unsigned int a8, signed int a6, signed int a7, const char *string, unsigned int arg20);
BOOL __stdcall SGdiImportFont(HGDIOBJ handle, int windowsfont);

BOOL __stdcall SGdiSelectObject(int handle);
BOOL __stdcall SGdiSetPitch(int pitch);

BOOL __stdcall Ordinal393(char *string, int, int);

void* __stdcall SMemAlloc(int amount, char *logfilename, int logline, int defaultValue);

BOOL __stdcall SMemFree(void *location, char *logfilename, int logline, char defaultValue);

BOOL __stdcall SRegLoadData(char *keyname, char *valuename, int size, LPBYTE lpData, BYTE flags, LPDWORD lpcbData);
BOOL __stdcall SRegLoadString(char *keyname, char *valuename, BYTE flags, char *buffer, size_t buffersize);
BOOL __stdcall SRegLoadValue(char *keyname, char *valuename, BYTE flags, int *value);
BOOL __stdcall SRegSaveData(char *keyname, char *valuename, int size, BYTE *lpData, DWORD cbData);
BOOL __stdcall SRegSaveString(char *keyname, char *valuename, BYTE flags, char *string);
BOOL __stdcall SRegSaveValue(char *keyname, char *valuename, BYTE flags, DWORD result);

BOOL __stdcall SRegDeleteValue(char *keyname, char *valuename, BYTE flags);

BOOL __stdcall STransBlt(int handle, int a2, int a3, int a4, int a5);
BOOL __stdcall STransBltUsingMask(int lpSurface, int a2, int pitch, int width, int handle);

BOOL __stdcall STransDelete(int handle);

BOOL __stdcall STransDuplicate(int handle, int source);
BOOL __stdcall STransIntersectDirtyArray(int handle, int dirtyarraymask, unsigned __int8 dirtyarray, int sourcemask);
BOOL __stdcall STransInvertMask(int handle, int sourcemask);

BOOL __stdcall STransSetDirtyArrayInfo(int width, int height, int depth, int bits);

BOOL __stdcall STrans442(int a1, int a2, int a3, char *a4, size_t a5);
BOOL __stdcall STransCombineMasks(int handle, int a2, int a3, int a4, int depth, int a6);

BOOL __stdcall STransCreateE(int a1, int a2, int a3, int bpp, int a5, int bufferSize, int a7);

BOOL __stdcall SVidDestroy();
BOOL __stdcall SVidGetSize(HANDLE video, int width, int height, int zero);
BOOL __stdcall SVidInitialize(HANDLE video);
BOOL __stdcall SVidPlayBegin(char *filename, int arg4, int a3, int a4, int a5, int a6, HANDLE video);

BOOL __stdcall SVidPlayContinueSingle(HANDLE video, int a2, int a3);
BOOL __stdcall SVidPlayEnd(HANDLE video);

BOOL __stdcall SErrGetErrorStr(DWORD dwErrCode, char *buffer, size_t bufferchars);
DWORD __stdcall SErrGetLastError();

void __stdcall SErrSetLastError(DWORD dwErrCode);

void __stdcall SErrSuppressErrors(BOOL suppressErrors);


// Values for dwErrCode
#define STORM_ERROR_ASSERTION                    0x85100000
#define STORM_ERROR_BAD_ARGUMENT                 0x85100065
#define STORM_ERROR_GAME_ALREADY_STARTED         0x85100066
#define STORM_ERROR_GAME_FULL                    0x85100067
#define STORM_ERROR_GAME_NOT_FOUND               0x85100068
#define STORM_ERROR_GAME_TERMINATED              0x85100069
#define STORM_ERROR_INVALID_PLAYER               0x8510006a
#define STORM_ERROR_NO_MESSAGES_WAITING          0x8510006b
#define STORM_ERROR_NOT_ARCHIVE                  0x8510006c
#define STORM_ERROR_NOT_ENOUGH_ARGUMENTS         0x8510006d
#define STORM_ERROR_NOT_IMPLEMENTED              0x8510006e
#define STORM_ERROR_NOT_IN_ARCHIVE               0x8510006f
#define STORM_ERROR_NOT_IN_GAME                  0x85100070
#define STORM_ERROR_NOT_INITIALIZED              0x85100071
#define STORM_ERROR_NOT_PLAYING                  0x85100072
#define STORM_ERROR_NOT_REGISTERED               0x85100073
#define STORM_ERROR_REQUIRES_CODEC1              0x85100074
#define STORM_ERROR_REQUIRES_CODEC2              0x85100075
#define STORM_ERROR_REQUIRES_CODEC3              0x85100076
#define STORM_ERROR_REQUIRES_UPGRADE             0x85100077
#define STORM_ERROR_STILL_ACTIVE                 0x85100078
#define STORM_ERROR_VERSION_MISMATCH             0x85100079
#define STORM_ERROR_MEM_NOT_ALLOCATED            0x8510007a
#define STORM_ERROR_MEM_CORRUPTED                0x8510007b
#define STORM_ERROR_MEM_INVALID                  0x8510007c
#define STORM_ERROR_MEM_MANAGER_NOT_INITIALIZED  0x8510007d
#define STORM_ERROR_MEM_NOT_FREED                0x8510007e
#define STORM_ERROR_RESOURCES_NOT_RELEASED       0x8510007f
#define STORM_ERROR_OUT_OF_BOUNDS                0x85100080
#define STORM_ERROR_NULL_POINTER                 0x85100081
#define STORM_ERROR_CDKEY_MISMATCH               0x85100082
#define STORM_ERROR_FILE_CORRUPTED               0x85100083
#define STORM_ERROR_FATAL                        0x85100084
#define STORM_ERROR_GAMETYPE_UNAVAILABLE         0x85100085
#define STORM_DDERR_ALREADYINITIALIZED           0x88760005
#define STORM_DDERR_CANNOTATTACHSURFACE          0x8876000a
#define STORM_DDERR_CANNOTDETACHSURFACE          0x88760014
#define STORM_DDERR_CURRENTLYNOTAVAIL            0x88760028
#define STORM_DDERR_EXCEPTION                    0x88760037
#define STORM_DDERR_HEIGHTALIGN                  0x8876005a
#define STORM_DDERR_INCOMPATIBLEPRIMARY          0x8876005f
#define STORM_DDERR_INVALIDCAPS                  0x88760064
#define STORM_DDERR_INVALIDCLIPLIST              0x8876006e
#define STORM_DDERR_INVALIDMODE                  0x88760078
#define STORM_DDERR_INVALIDOBJECT                0x88760082
#define STORM_DDERR_INVALIDPIXELFORMAT           0x88760091
#define STORM_DDERR_INVALIDRECT                  0x88760096
#define STORM_DDERR_LOCKEDSURFACES               0x887600a0
#define STORM_DDERR_NO3D                         0x887600aa
#define STORM_DDERR_NOALPHAHW                    0x887600b4
#define STORM_DDERR_NOCLIPLIST                   0x887600cd
#define STORM_DDERR_NOCOLORCONVHW                0x887600d2
#define STORM_DDERR_NOCOOPERATIVELEVELSET        0x887600d4
#define STORM_DDERR_NOCOLORKEY                   0x887600d7
#define STORM_DDERR_NOCOLORKEYHW                 0x887600dc
#define STORM_DDERR_NODIRECTDRAWSUPPORT          0x887600de
#define STORM_DDERR_NOEXCLUSIVEMODE              0x887600e1
#define STORM_DDERR_NOFLIPHW                     0x887600e6
#define STORM_DDERR_NOGDI                        0x887600f0
#define STORM_DDERR_NOMIRRORHW                   0x887600fa
#define STORM_DDERR_NOTFOUND                     0x887600ff
#define STORM_DDERR_NOOVERLAYHW                  0x88760104
#define STORM_DDERR_NORASTEROPHW                 0x88760118
#define STORM_DDERR_NOROTATIONHW                 0x88760122
#define STORM_DDERR_NOSTRETCHHW                  0x88760136
#define STORM_DDERR_NOT4BITCOLOR                 0x8876013c
#define STORM_DDERR_NOT4BITCOLORINDEX            0x8876013d
#define STORM_DDERR_NOT8BITCOLOR                 0x88760140
#define STORM_DDERR_NOTEXTUREHW                  0x8876014a
#define STORM_DDERR_NOVSYNCHW                    0x8876014f
#define STORM_DDERR_NOZBUFFERHW                  0x88760154
#define STORM_DDERR_NOZOVERLAYHW                 0x8876015e
#define STORM_DDERR_OUTOFCAPS                    0x88760168
#define STORM_DDERR_OUTOFVIDEOMEMORY             0x8876017c
#define STORM_DDERR_OVERLAYCANTCLIP              0x8876017e
#define STORM_DDERR_OVERLAYCOLORKEYONLYONEACTIVE 0x88760180
#define STORM_DDERR_PALETTEBUSY                  0x88760183
#define STORM_DDERR_COLORKEYNOTSET               0x88760190
#define STORM_DDERR_SURFACEALREADYATTACHED       0x8876019a
#define STORM_DDERR_SURFACEALREADYDEPENDENT      0x887601a4
#define STORM_DDERR_SURFACEBUSY                  0x887601ae
#define STORM_DDERR_CANTLOCKSURFACE              0x887601b3
#define STORM_DDERR_SURFACEISOBSCURED            0x887601b8
#define STORM_DDERR_SURFACELOST                  0x887601c2
#define STORM_DDERR_SURFACENOTATTACHED           0x887601cc
#define STORM_DDERR_TOOBIGHEIGHT                 0x887601d6
#define STORM_DDERR_TOOBIGSIZE                   0x887601e0
#define STORM_DDERR_TOOBIGWIDTH                  0x887601ea
#define STORM_DDERR_UNSUPPORTEDFORMAT            0x887601fe
#define STORM_DDERR_UNSUPPORTEDMASK              0x88760208
#define STORM_DDERR_VERTICALBLANKINPROGRESS      0x88760219
#define STORM_DDERR_WASSTILLDRAWING              0x8876021c
#define STORM_DDERR_XALIGN                       0x88760230
#define STORM_DDERR_INVALIDDIRECTDRAWGUID        0x88760231
#define STORM_DDERR_DIRECTDRAWALREADYCREATED     0x88760232
#define STORM_DDERR_NODIRECTDRAWHW               0x88760233
#define STORM_DDERR_PRIMARYSURFACEALREADYEXISTS  0x88760234
#define STORM_DDERR_NOEMULATION                  0x88760235
#define STORM_DDERR_REGIONTOOSMALL               0x88760236
#define STORM_DDERR_CLIPPERISUSINGHWND           0x88760237
#define STORM_DDERR_NOCLIPPERATTACHED            0x88760238
#define STORM_DDERR_NOHWND                       0x88760239
#define STORM_DDERR_HWNDSUBCLASSED               0x8876023a
#define STORM_DDERR_HWNDALREADYSET               0x8876023b
#define STORM_DDERR_NOPALETTEATTACHED            0x8876023c
#define STORM_DDERR_NOPALETTEHW                  0x8876023d
#define STORM_DDERR_BLTFASTCANTCLIP              0x8876023e
#define STORM_DDERR_NOBLTHW                      0x8876023f
#define STORM_DDERR_NODDROPSHW                   0x88760240
#define STORM_DDERR_OVERLAYNOTVISIBLE            0x88760241
#define STORM_DDERR_NOOVERLAYDEST                0x88760242
#define STORM_DDERR_INVALIDPOSITION              0x88760243
#define STORM_DDERR_NOTAOVERLAYSURFACE           0x88760244
#define STORM_DDERR_EXCLUSIVEMODEALREADYSET      0x88760245
#define STORM_DDERR_NOTFLIPPABLE                 0x88760246
#define STORM_DDERR_CANTDUPLICATE                0x88760247
#define STORM_DDERR_NOTLOCKED                    0x88760248
#define STORM_DDERR_CANTCREATEDC                 0x88760249
#define STORM_DDERR_NODC                         0x8876024a
#define STORM_DDERR_WRONGMODE                    0x8876024b
#define STORM_DDERR_IMPLICITLYCREATED            0x8876024c
#define STORM_DDERR_NOTPALETTIZED                0x8876024d
#define STORM_DDERR_UNSUPPORTEDMODE              0x8876024e
#define STORM_DDERR_NOMIPMAPHW                   0x8876024f
#define STORM_DDERR_INVALIDSURFACETYPE           0x88760250
#define STORM_DDERR_DCALREADYCREATED             0x8876026c
#define STORM_DDERR_CANTPAGELOCK                 0x88760280
#define STORM_DDERR_CANTPAGEUNLOCK               0x88760294
#define STORM_DDERR_NOTPAGELOCKED                0x887602a8
#define STORM_DSERR_ALLOCATED                    0x8878000a
#define STORM_DSERR_CONTROLUNAVAIL               0x8878001e
#define STORM_DSERR_INVALIDCALL                  0x88780032
#define STORM_DSERR_PRIOLEVELNEEDED              0x88780046
#define STORM_DSERR_BADFORMAT                    0x88780064
#define STORM_DSERR_NODRIVER                     0x88780078
#define STORM_DSERR_ALREADYINITIALIZED           0x88780082
#define STORM_DSERR_BUFFERLOST                   0x88780096
#define STORM_DSERR_OTHERAPPHASPRIO              0x887800a0
#define STORM_DSERR_UNINITIALIZED                0x887800aa


void __stdcall SMemCopy(void *dest, const void *source, size_t size);
int __stdcall SMemFill(void *location, size_t length, char fillWith);

int __stdcall SMemZero(void *location, DWORD length);
int __stdcall SMemCmp(void *location1, void *location2, DWORD size);

int __stdcall SStrCopy(char *dest, const char *src, DWORD max_length);
DWORD __stdcall SStrHash(const char *string, DWORD flags, DWORD Seed);
int __stdcall SStrNCat(char *dest, const char *src, DWORD max_length);

int __stdcall SStrLen(const char* string);

int __stdcall SStrCmp(const char *string1, const char *string2, size_t size);
int __stdcall SStrCmpI(const char *string1, const char *string2, size_t size);
char* __stdcall SStrUpper(char* string);

void __stdcall SRgn523(int handle, RECT *rect, int a3, int a4);
void __stdcall SRgn524(int handle, int a2);
void __stdcall SRgn525(int handle);

void __stdcall SRgn529i(int handle, int a2, int a3);

char* __stdcall SStrChr(const char *string, char c);
char* __stdcall SStrChrR(const char *string, char c);

size_t __cdecl SStrVPrintf(char *dest, size_t size, const char *format, ...);

int __stdcall SBigDel(void *buffer);

int __stdcall SBigFromBinary(void *buffer, const void *str, size_t size);

int __stdcall SBigNew(void **buffer);

int __stdcall SBigPowMod(void *buffer1, void *buffer2, int a3, int a4);

int __stdcall SBigToBinaryBuffer(void *buffer, int length, int a3, int a4);
