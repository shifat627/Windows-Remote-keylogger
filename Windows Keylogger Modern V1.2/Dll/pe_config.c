#include "pe_config.h"
#include "str.h"

BOOL __stdcall GetEssentialFunctionAddress(UINT_PTR * GetProc, UINT_PTR * LoadLib){
	
	
	PPEB peb = ((PMYTEB)NtCurrentTeb())->ProcessEnvironmentBlock;
	PLIST_ENTRY entry = peb->Ldr->InMemoryOrderModuleList.Flink;
	PLDR_MODULE module;
	
	
	UINT_PTR base,LoadLib_Addr,GetProc_Addr;LoadLib_Addr=GetProc_Addr=0;
	PIMAGE_EXPORT_DIRECTORY exp;
	PIMAGE_NT_HEADERS nt;
	
	
	PDWORD addr_list,name_list;
	PWORD ord_list;
	char * func_name;
	
	module = ((LPVOID)entry->Flink->Flink) - FIELD_OFFSET(LDR_MODULE,InMemoryOrderModuleList);
	
	//printf("Address of Kernel32 (Manual)%p-(Function)%p\n",module->BaseAddress,LoadLibraryA("kernel32.dll"));
	
	base = (UINT_PTR) module->BaseAddress;
	
	nt = (PIMAGE_NT_HEADERS)(base+ ((PIMAGE_DOS_HEADER)base)->e_lfanew );
	
	if(nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress){
		exp = base + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
		
		addr_list = base + exp->AddressOfFunctions;
		name_list = base + exp->AddressOfNames;
		ord_list = base + exp->AddressOfNameOrdinals;
		
		int i = 0;
		
		for(i=0;i<exp->NumberOfNames;i++){
			func_name = base + name_list[i];
			if(StringCompare(func_name,"LoadLibraryA")){
				LoadLib_Addr = base + addr_list[ord_list[i]];
			}
			
			if(StringCompare(func_name,"GetProcAddress")){
				GetProc_Addr = base + addr_list[ord_list[i]];
			}
		}
	}
	
	if(LoadLib_Addr){
		*LoadLib = LoadLib_Addr;
	}
	
	if(GetProc_Addr){
		*GetProc = GetProc_Addr;
	}
	
	
	if(GetProc_Addr &&  LoadLib_Addr )
	return 1;
	
	
	return 0;
}

__declspec(dllexport) void __stdcall PeConfig(void * BaseAddress){
	
	PIMAGE_BASE_RELOCATION  relocation;
	PIMAGE_NT_HEADERS nt;
	
	GetProcAddressPointer GetProc;
	LoadLibraryAPointer LoadLib;
	
	UINT_PTR base_delta=0;
	
	nt = (PIMAGE_NT_HEADERS) ( BaseAddress + ((PIMAGE_DOS_HEADER)BaseAddress)->e_lfanew );
	
	//Relocating Base
	if(nt->OptionalHeader.ImageBase != (UINT_PTR) BaseAddress){
		
		base_delta = (UINT_PTR) BaseAddress - nt->OptionalHeader.ImageBase;
		
		if(nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress){
			
			relocation = (PIMAGE_BASE_RELOCATION) (BaseAddress + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
			
			while(relocation->VirtualAddress){
				
				LPVOID Dest_addr = BaseAddress + relocation->VirtualAddress;
				LPTYPE_ENTRY entry = (LPTYPE_ENTRY)(relocation+1);
				
				int nEntry = (relocation->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION))/2;
				
				int i;
				
				for(i=0;i<nEntry;i++,entry++){
					if(entry->type == BASE_REL_TYPE){
						
						UINT_PTR * p = (PUINT_PTR)(Dest_addr + entry->offset);
						*p = *p + base_delta;
						
					}
					
					
				}
				
				relocation = (PIMAGE_BASE_RELOCATION) ((LPVOID)(relocation) + relocation->SizeOfBlock );
			}
				
		}
		else{
			return;
		}
		
		
		
	}
	
	//-----------------------------------------------------
	
	//Finding Address Of GetProcAddress() And LoadLibrary()
	
	if(!GetEssentialFunctionAddress((UINT_PTR *)&GetProc,(PUINT_PTR)&LoadLib)){
		return ;
	}
	
	//-----------------------------------------------------
	
	//Importing Functions
	if(nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress){
		PIMAGE_IMPORT_DESCRIPTOR import =(PIMAGE_IMPORT_DESCRIPTOR) (BaseAddress + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		PIMAGE_THUNK_DATA fthunk,othunk;
		
		
		while(import->Name){
			LPVOID dll = LoadLib(BaseAddress + import->Name);
			
			if(dll){
				fthunk = (PIMAGE_THUNK_DATA)(BaseAddress+import->FirstThunk);
				othunk = (PIMAGE_THUNK_DATA)(BaseAddress+import->OriginalFirstThunk);
				
				if(!import->OriginalFirstThunk){
					othunk = fthunk;
				}
				
				while(othunk->u1.AddressOfData){
					if(othunk->u1.Ordinal & IMAGE_ORDINAL_FLAG){
						fthunk->u1.Function = GetProc(dll,IMAGE_ORDINAL(othunk->u1.Ordinal));
					}
					else{
						PIMAGE_IMPORT_BY_NAME nm = (PIMAGE_IMPORT_BY_NAME)(BaseAddress+othunk->u1.AddressOfData);
						fthunk->u1.Function = GetProc(dll,nm->Name);
					}
					
					++othunk;
					++fthunk;
				}
				
			}
			++import;
		}
	}
	
	
	//------------------------------------------------------------------------------
	//calling TLS callbacks
	
	if(nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress){
		PIMAGE_TLS_DIRECTORY tls = (PIMAGE_TLS_DIRECTORY)(BaseAddress+nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		
		if(tls->AddressOfCallBacks){
			PIMAGE_TLS_CALLBACK * callback = tls->AddressOfCallBacks;
			while(*callback){
				PIMAGE_TLS_CALLBACK func = *callback;
				func(BaseAddress,1,NULL);
				callback++;
			}
			
		}
	}
	
	PIMAGE_TLS_CALLBACK entry = (PIMAGE_TLS_CALLBACK)(BaseAddress+nt->OptionalHeader.AddressOfEntryPoint);
	entry(BaseAddress,1,NULL);
}
