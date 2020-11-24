#include "memory.h"
#include "messages.h"

NTSTATUS TxnKernelReadVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PVOID DestinationAddress, SIZE_T Size)
{
	PSIZE_T Bytes;

	// Source process and address are from the process/game we are reading from
	// Destination process and address are from kernel driver
	// PsGetCurrentProcess() returns process of kernel driver
	NTSTATUS status = MmCopyVirtualMemory(SourceProcess, SourceAddress, PsGetCurrentProcess(), DestinationAddress, Size, KernelMode, &Bytes);

	// status returned in STATUS_PARTIAL_COPY, dont know why
	// Tried setting to KernelMode and UserMode, not difference
	
	if (status == STATUS_SUCCESS)
	{
		DEBUG_MESSAGE("TxnKernelReadVirtualMemory success!!");
	}else 
	{ 
		DEBUG_MESSAGE("status code: %lx", status);
	}

	return status;
	
}

NTSTATUS TxnKernelWriteVirtualMemory(PEPROCESS DestinationProcess, PVOID DestinationAddress, PVOID SourceAddress, SIZE_T Size)
{
	PSIZE_T Bytes;

	// Destination process and address are from the process/game we are writing to
	// Source process and address are from kernel driver
	// PsGetCurrentProcess() returns process of kernel driver

	NTSTATUS status = MmCopyVirtualMemory(PsGetCurrentProcess(), SourceAddress, DestinationProcess, DestinationAddress, Size, KernelMode, &Bytes);

	if (status == STATUS_SUCCESS)
	{
		DEBUG_MESSAGE("TxnKernelWriteVirtualMemory success!!");
	}
	else
	{
		DEBUG_MESSAGE("status code: %lx", status);
	}

	return status;
}