#pragma warning (disable : 4047)

#include "events.h"
#include "messages.h"

#include "data.h"


PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO LoadedImageInfo)
{

	//PWCHAR wszFullImageName = ExAllocatePool(NonPagedPool, FullImageName->Length + 2);
	//RtlZeroMemory(wszFullImageName, FullImageName->Length + 2);
	//RtlCopyMemory(wszFullImageName, FullImageName->Buffer, FullImageName->Length);

	// const char* imageString = L"\\Path of Exile\\PathOfExile_x64.exe";
	//const char* imageString = L"\\notepad.exe";

	if (imageNameStr == NULL) {
		return STATUS_UNSUCCESSFUL;
	}




	//wcsstr(FullImageName->Buffer, L"\\Path of Exile\\PathOfExile_x64.exe"))
	
	// Find the process
	if (wcsstr(FullImageName->Buffer, imageNameStr)){

		DEBUG_MESSAGE("Process found!\n");
		ImageInfo = LoadedImageInfo;
		ImageBaseAddress = ImageInfo->ImageBase;
		PID = ProcessId;


		
		//HMODULE hModuleArray[100];
		//DWORD numBytesNeeded;
		//
		//ProcessEntryPoint = 0x0;
		//
		// TODO
		//if (EnumProcessModulesEx(ProcessId, hModuleArray, sizeof(hModuleArray), &numBytesNeeded, LIST_MODULES_64BIT))
		//{
		//	LPSTR lpFilename = "";
		//	
		//	// Iterate over module handle to find the one we want
		//	for (int index = 0; index < 100; index++ )
		//	{
		//		GetModuleFileNameEx(ProcessId, hModuleArray[index], lpFilename, 256);
		//
		//
		//		if(hModuleArray[index])
		//	}
		//
		//
		//
		//	GetModuleInformation(ProcessId,);
		//}

		
		/*
		 *
		 *
		 *	GetModuleInformation() -> MODULEINFO
			MODULEINFO			   -> EntryPoint
		 */

		//DEBUG_MESSAGE("ImageBase Address: 0x%x \n", ProcessEntryPoint);

		DEBUG_MESSAGE("ImageBase Address: 0x%x \n", ImageBaseAddress);
		DEBUG_MESSAGE("ProcessID: %d (Hex: 0x%x)\n", PID, PID);
	}

	//ExFreePool(wszFullImageName);
	
	return STATUS_SUCCESS;
}

