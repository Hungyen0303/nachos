// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "kernel.h"
#define MaxFileLength 32
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

char *User2System(int virtAddr, int limit)
{
	int i; // index
	int oneChar;
	char *kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; // need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0, limit + 1);
	// printf("\n Filename u2s:");
	for (i = 0; i < limit; i++)
	{
		kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		// printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}
// NgÆ°á»£c láº¡i ta cÃ³ hÃ m System2User nhÆ° sau:
//  Input: - User space address (int)
//  - Limit of buffer (int)
//  - Buffer (char[])
//  Output:- Number of bytes copied (int)
//  Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr, int len, char *buffer)
{
	if (len < 0)
		return -1;
	if (len == 0)
		return len;
	int i = 0;
	int oneChar = 0;
	do
	{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}
void ExceptionHandler(ExceptionType which)
{

	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case SyscallException:
		switch (type)
		{
		case SC_Create:
		{
			int virtAddr;
			char *filename = new char[MaxFileLength + 1];
			DEBUG('a', "\n SC_Create call ...");
			DEBUG('a', "\n Reading virtual address of filename");
			// Láº¥y tham sá»‘ tÃªn táº­p tin tá»« thanh ghi r4
			virtAddr = kernel->machine->ReadRegister(4);
			DEBUG('a', "\n Reading filename.");

			// MaxFileLength lÃ  = 32
			filename = User2System(virtAddr, MaxFileLength + 1);
			if (filename == NULL)
			{
				printf("\n Not enough memory in system");
				DEBUG('a', "\n Not enough memory in system");
				kernel->machine->WriteRegister(2, -1); // tráº£ vá» lá»—i cho chÆ°Æ¡ng
				// trÃ¬nh ngÆ°á»i dÃ¹ng
				delete filename;
				return;
			}

			DEBUG('a', "\n Finish reading filename.");
			FileSystem *fileSystem = new FileSystem();

			// DEBUG(â€˜aâ€™,"\n File name : '"<<filename<<"'");
			//  Create file with size = 0
			//  DÃ¹ng Ä‘á»‘i tÆ°á»£ng fileSystem cá»§a lá»›p OpenFile Ä‘á»ƒ táº¡o file,
			//  viá»‡c táº¡o file nÃ y lÃ  sá»­ dá»¥ng cÃ¡c thá»§ tá»¥c táº¡o file cá»§a há»‡ Ä‘iá»u
			//  hÃ nh Linux, chÃºng ta khÃ´ng quáº£n ly trá»±c tiáº¿p cÃ¡c block trÃªn
			//  Ä‘Ä©a cá»©ng cáº¥p phÃ¡t cho file, viá»‡c quáº£n ly cÃ¡c block cá»§a file
			//  trÃªn á»• Ä‘Ä©a lÃ  má»™t Ä‘á»“ Ã¡n khÃ¡c
			if (!fileSystem->Create(filename))
			{
				printf("\n Error create file '%s'", filename);
				kernel->machine->WriteRegister(2, -1);
				delete filename;
				return;
			}
			kernel->machine->WriteRegister(2, 0); // tráº£ vá» cho chÆ°Æ¡ng trÃ¬nh
												  // ngÆ°á»i dÃ¹ng thÃ nh cÃ´ng
			delete filename;
			// delete fileSystem;
			break;
		}
		case SC_Open:
		{

			
			// int bufAddr = kernel->machine->ReadRegister(4); // read string pointer from user
			// int type = kernel->machine->ReadRegister(5);    // vua sua lai
			// //char *buf = User2System(bufAddr, MaxFileLength );
			// FileSystem *fileSystem = new FileSystem();
			DEBUG(dbgSys, "Here\n");
			DEBUG(dbgSys, "Stop\n");
			// if (fileSystem->index > 10)
			// {
			// 	kernel->machine->WriteRegister(2, -1);
			// 	delete[] buf;
			// 	break;
			// }

			// if (strcmp(buf, "stdin") == 0)
			// {
			// 	printf("stdin mode\n");
			// 	kernel->machine->WriteRegister(2, 0);
			// 	break;
			// }
			// if (strcmp(buf, "stdout") == 0)
			// {
			// 	printf("stdout mode\n");
			// 	kernel->machine->WriteRegister(2, 1);
			// 	break;
			// }

			// DEBUG(dbgSys, "\nOpened file");
			// DEBUG(dbgSys, "Here\n" << buf );
			// OpenFile* temp = kernel->fileSystem->Open(buf);

			// kernel->machine->WriteRegister(2, fileSystem->index - 1);

			// if ((fileSystem->openf[fileSystem->index] = fileSystem->Open(buf)) != NULL)
			// {
			// 	DEBUG('f', "open file successfully");
			// 	kernel->machine->WriteRegister(2, fileSystem->index - 1);
			// }
			// else
			// {
			// 	DEBUG('f', "can not open file");
			// 	kernel->machine->WriteRegister(2, -1);
			// };
			// delete[] buf;

			break;
		}
		// case SC_Close:
		// {
		// 	int m_index = kernel->machine->ReadRegister(4);
		// 	if (fileSystem->openf[m_index] == NULL)
		// 		break;
		// 	delete fileSystem->openf[m_index];
		// 	fileSystem->openf[m_index] = NULL;
		// 	break;
		// }
		// case SC_Read:
		// {
		// 	int bufAddr = machine->ReadRegister(4);
		// 	int NumBuf = machine->ReadRegister(5);
		// 	int m_index = machine->ReadRegister(6);
		// 	int OldPos;
		// 	int NewPos;
		// 	char *buf = new char[NumBuf];
		// 	int i = 0;
		// 	// Check m_index
		// 	if (m_index < 0 || m_index > 10)
		// 	{
		// 		machine->WriteRegister(2, -1);
		// 		delete[] buf;
		// 		break;
		// 	}
		// 	// check openf[m_index]
		// 	if (fileSystem->openf[m_index] == NULL)
		// 	{
		// 		machine->WriteRegister(2, -1);
		// 		delete[] buf;
		// 		break;
		// 	}
		// 	OldPos = fileSystem->openf[m_index]->GetCurrentPos();
		// 	buf = machine->User2System(bufAddr, NumBuf);
		// 	if (fileSystem->openf[m_index]->type == 2)
		// 	{
		// 		/*  printf("NumBuf = %d\n", NumBuf);*/
		// 		int sz = gSynchConsole->Read(buf, NumBuf);
		// 		/*  machine->System2User(bufAddr, sz, buf);*/

		// 		machine->System2User(bufAddr, sz, buf);
		// 		machine->WriteRegister(2, sz);
		// 		break;
		// 	}

		// 	if ((fileSystem->openf[m_index]->Read(buf, NumBuf)) > 0)
		// 	{
		// 		// Copy data from kernel to user space
		// 		NewPos = fileSystem->openf[m_index]->GetCurrentPos();
		// 		machine->System2User(bufAddr, NewPos - OldPos + 1, buf);
		// 		machine->WriteRegister(2, NewPos - OldPos + 1);
		// 	}
		// 	else
		// 	{
		// 		machine->WriteRegister(2, -1);
		// 		delete[] buf;
		// 		break;
		// 	}
		// 	// read data from console

		// 	/*
		// 	if (fileOpen.type == 2)
		// 	{
		// 		int sz = gSynchConsole->Read(buf, NumBuf);
		// 		machine->System2User(bufAddr, sz, buf);
		// 		machine->WriteRegister(2, sz);
		// 	}*/
		// 	delete[] buf;
		// 	break;
		// }
		// case SC_Write:
		// {
		// 	int bufAddr = kernel->machine->ReadRegister(4);
		// 	int NumBuf = kernel->machine->ReadRegister(5);
		// 	int m_index = kernel->machine->ReadRegister(6);
		// 	int OldPos;
		// 	int NewPos;
		// 	char *buf = new char[NumBuf];
		// 	// Check m_index
		// 	if (m_index < 0 || m_index > 10)
		// 	{
		// 		kernel->machine->WriteRegister(2, -1);
		// 		delete[] buf;
		// 		break;
		// 	}
		// 	// check openf[m_index]
		// 	if (fileSystem->openf[m_index] == NULL)
		// 	{
		// 		kernel->machine->WriteRegister(2, -1);
		// 		delete[] buf;
		// 		break;
		// 	}
		// 	OldPos = fileSystem->openf[m_index]->GetCurrentPos();

		// 	// type must equals '0'
		// 	buf = kernel->machine->User2System(bufAddr, NumBuf);
		// 	if (fileSystem->openf[m_index]->type == 0 || fileSystem->openf[m_index]->type == 3)
		// 	{
		// 		if ((fileSystem->openf[m_index]->Write(buf, NumBuf)) > 0)
		// 		{
		// 			// Copy data from kernel to user space
		// 			printf("%s", buf);
		// 			NewPos = fileSystem->openf[m_index]->GetCurrentPos();
		// 			kernel->machine->WriteRegister(2, NewPos - OldPos + 1);
		// 		}
		// 		else if (fileSystem->openf[m_index]->type == 1)
		// 			;
		// 		{
		// 			kernel->machine->WriteRegister(2, -1);
		// 			delete[] buf;
		// 			break;
		// 		}
		// 	}
		// 	// Write data to console
		// 	if (fileSystem->openf[m_index]->type == 3)
		// 	{
		// 		int i = 0;
		// 		printf("stdout mode\n");
		// 		while (buf[i] != 0 && buf[i] != '\n')
		// 		{
		// 			gSynchConsole->Write(buf + i, 1);
		// 			i++;
		// 		}
		// 		buf[i] = '\n';
		// 		gSynchConsole->Write(buf + i, 1);
		// 		kernel->machine->WriteRegister(2, i - 1);
		// 	}
		// 	delete[] buf;
		// 	break;
		// }
		case SC_Halt:
		{
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;
		}

		case SC_Add:
		{
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Modify return point */
			{
				/* set previous programm counter (debugging only)*/
				kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

				/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
				kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

				/* set next programm counter for brach execution */
				kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			}

			return;

			ASSERTNOTREACHED();

			break;
		}

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		ASSERTNOTREACHED();
		break;
	}
}
