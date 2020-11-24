#pragma warning (disable : 4100 4047 4024)

#include "events.h"
#include "communication.h"
#include "toxxin.h"
#include "data.h"
#include "messages.h"


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	UNREFERENCED_PARAMETER(pRegistryPath);
	UNREFERENCED_PARAMETER(pDeviceObject);
	
	pDriverObject->DriverUnload = UnloadDriver;
	

	// Registers call backs here
	PsSetLoadImageNotifyRoutine(ImageLoadCallback);

	// Registering device and dos device strings
	RtlInitUnicodeString(&dev, L"\\Device\\toxxindriver");
	RtlInitUnicodeString(&dos, L"\\DosDevices\\toxxindriver");

	//Create the device and establish a symbolic link between the dos device and device
	IoCreateDevice(pDriverObject, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	IoCreateSymbolicLink(&dos, &dev);


	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;

	if (pDeviceObject != NULL) {
		pDeviceObject->Flags |= DO_DIRECT_IO;
		pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	}

	imageNameSet = FALSE;
	imageNameStr = NULL;
	
	DEBUG_MESSAGE("Loaded Toxxin Driver!\n");
	
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject) 
{
	UNREFERENCED_PARAMETER(pDriverObject);

	// Unregister call backs here
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);


	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(pDriverObject->DeviceObject);

	
	DEBUG_MESSAGE("Unloaded Toxxin driver!\n");
	return STATUS_SUCCESS;
}