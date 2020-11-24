#pragma once
#pragma warning (disable : 4047 4024)

#include "ntifs.h"

NTSTATUS NTAPI MmCopyVirtualMemory(
	PEPROCESS SourceProcess,
	PVOID SourceAddress,
	PEPROCESS DestinationProcess,
	PVOID DestinationAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T NumberOfBytesCopied
);

// Read from a source process to our kernel driver process
NTSTATUS TxnKernelReadVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PVOID DestinationAddress, SIZE_T Size);

// Write from our kernel driver process to a source process
NTSTATUS TxnKernelWriteVirtualMemory(PEPROCESS DestinationProcess, PVOID DestinationAddress, PVOID SourceAddress, SIZE_T Size);
