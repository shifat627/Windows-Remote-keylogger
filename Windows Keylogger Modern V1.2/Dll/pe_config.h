#ifndef _PE_CONFIG_H_
#define _PE_CONFIG_H_

#include <windows.h>
#include<winnt.h>
#include<winternl.h>

typedef struct _relocation_entry_
{
    WORD offset:12;WORD type:4;
}TYPE_ENTRY , *LPTYPE_ENTRY;

typedef struct CUSTOM_TEB {
  PVOID Reserved1[12];
  PPEB  ProcessEnvironmentBlock;
  PVOID Reserved2[399];
  BYTE  Reserved3[1952];
  PVOID TlsSlots[64];
  BYTE  Reserved4[8];
  PVOID Reserved5[26];
  PVOID ReservedForOle;
  PVOID Reserved6[4];
  PVOID TlsExpansionSlots;
} MYTEB, *PMYTEB;


typedef struct _LDR_MODULE {



  LIST_ENTRY              InLoadOrderModuleList;
  LIST_ENTRY              InMemoryOrderModuleList;
  LIST_ENTRY              InInitializationOrderModuleList;
  PVOID                   BaseAddress;
  PVOID                   EntryPoint;
  ULONG                   SizeOfImage;
  UNICODE_STRING          FullDllName;
  UNICODE_STRING          BaseDllName;
  ULONG                   Flags;
  SHORT                   LoadCount;
  SHORT                   TlsIndex;
  LIST_ENTRY              HashTableEntry;
  ULONG                   TimeDateStamp;

} LDR_MODULE, *PLDR_MODULE;


#ifdef _WIN64
#define BASE_REL_TYPE 10
#else
#define BASE_REL_TYPE 3
#endif // _WIN64
typedef PVOID __stdcall (*LoadLibraryAPointer)(LPSTR name);
typedef PVOID __stdcall (*GetProcAddressPointer)(PVOID base,LPSTR func);

BOOL __stdcall GetEssentialFunctionAddress(UINT_PTR * GetProc, UINT_PTR * LoadLib);
__declspec(dllexport) void __stdcall PeConfig(void * BaseAddress);

#endif

