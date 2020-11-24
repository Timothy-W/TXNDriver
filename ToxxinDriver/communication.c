#pragma warning (disable : 6273)



#include "communication.h"
#include "data.h"
#include "messages.h"
#include "memory.h"


NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	DEBUG_MESSAGE("CreateCall was called, connection established.\n");
	
	return STATUS_SUCCESS;
	
}

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	DEBUG_MESSAGE("Connection terminated.\n");

	
	return STATUS_SUCCESS;
}

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	NTSTATUS Status = STATUS_UNSUCCESSFUL; // Return value
	ULONG NumBytesIO = 0; 

	// Get location of stack.
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

	// From stack, get the IO control code that was passed.
	ULONG ControlCode = stack->Parameters.DeviceIoControl.IoControlCode;

	// Check to make sure we are not performing any operations without
	// first having set the image/process name to look for.
	if (!imageNameSet && ControlCode != IO_SET_IMAGE_NAME_STRING)
	{
		DEBUG_MESSAGE("Image/process name not set.\n");
		Irp->IoStatus.Status = Status;
		Irp->IoStatus.Information = NumBytesIO;

		// Can not change status or Information once IoCompleteRequest() has been called.
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return Status;
	}
	
	// Some logic to determine which code was passed
	// These are defined in communications.h
	switch (ControlCode)
	{
		case IO_SET_IMAGE_NAME_STRING:
			{
				imageNameStr = Irp->AssociatedIrp.SystemBuffer;
				DEBUG_MESSAGE("Name of image/process set to: %ws\n", imageNameStr);

				Status = STATUS_SUCCESS;
				NumBytesIO = 0;
				break;
			}
		case IO_GET_PROCESS_ID:
			{

				DEBUG_MESSAGE("Client PID requested.\n");

				PULONG outPut = (PULONG)Irp->AssociatedIrp.SystemBuffer;
				*outPut = PID;

				Status = STATUS_SUCCESS;
				NumBytesIO = sizeof(outPut);
				break;
			}
		case IO_GET_CLIENT_ADDRESS:
			{
				DEBUG_MESSAGE("ClientAddress requested.\n");

				PULONG outPut = (PULONG)Irp->AssociatedIrp.SystemBuffer; // Can cast this to struct,string,char or whatever output we wana give to the user mode application
				*outPut = ImageBaseAddress;
				

				Status = STATUS_SUCCESS;
				NumBytesIO = sizeof(*outPut);
				break;
			}
		case IO_READ_REQUEST:
			{
			
				PKERNEL_READ_REQUEST ReadInput = (PKERNEL_READ_REQUEST)Irp->AssociatedIrp.SystemBuffer;
				PEPROCESS Process;

				DEBUG_MESSAGE("PID %d (Hex: 0x%x) Reading %d byte(s) at address 0x%x\n", ReadInput->ProcessId, ReadInput->ProcessId, ReadInput->Size, ReadInput->Address);
				
				if (NT_SUCCESS(PsLookupProcessByProcessId(ReadInput->ProcessId, &Process)))
				{
					DEBUG_MESSAGE("PreValue: %lu (Hex: 0x%x)\n", ReadInput->pBuff, ReadInput->pBuff);

					TxnKernelReadVirtualMemory(Process, /* Process we are reading from. */
						ReadInput->Address/* Address we want read. */, 
						ReadInput->pBuff/* Pointer to a buffer that we are going to store read data to. */, 
						ReadInput->Size); /* Size of the stored data. */ 

					DEBUG_MESSAGE("Read Params:  %lu, %#010x \n", ReadInput->ProcessId, ReadInput->Address);
					DEBUG_MESSAGE("Value: %lu (Hex: 0x%x)\n", ReadInput->pBuff, ReadInput->pBuff);
					
					Status = STATUS_SUCCESS;
					NumBytesIO = sizeof(KERNEL_READ_REQUEST);
				}
				break;
			}
		case IO_WRITE_REQUEST:
			{

				PKERNEL_WRITE_REQUEST WriteInput = (PKERNEL_WRITE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
				PEPROCESS Process;

				DEBUG_MESSAGE("PID %d Writing %d byte(s) at address 0x%x\n", WriteInput->ProcessId, WriteInput->Size, WriteInput->Address);

				if (NT_SUCCESS(PsLookupProcessByProcessId(WriteInput->ProcessId, &Process)))
				{
					TxnKernelWriteVirtualMemory(Process, /* Process we are writing to. */
						WriteInput->pBuff /* Buffer that stores the data we are going to write.  */,
						WriteInput->Address/* Some address within the destination process which are writing to. */,
						WriteInput->Size); /* Size of data we are writing. */

					DEBUG_MESSAGE("Write Params:  %lu, %#010x \n", WriteInput->pBuff, WriteInput->Address);
					
					Status = STATUS_SUCCESS;
					NumBytesIO = sizeof(KERNEL_READ_REQUEST);
				}
				break;
			}
		case IO_PATTERN_SCAN:
			{
				DEBUG_MESSAGE("Signature scan requested.\n");

				PSIGNATURE_REQUEST SignatureInput = (PSIGNATURE_REQUEST)Irp->AssociatedIrp.SystemBuffer;
				
				PEPROCESS Process;
				//PRKAPC_STATE ApcState;
				
				
				if (NT_SUCCESS(PsLookupProcessByProcessId(SignatureInput->ProcessId, &Process)))
				{
					// KeStackAttachProcess routine attaches the current thread to the address space of the target process.
					// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-kestackattachprocess
					__try
					{
						//KeStackAttachProcess(Process, &ApcState);
						// At this point we should be attached to the process
						// Not sure if all these arguments will be in scope
						//SignatureInput->addressBuff = (PULONG)FindSignature(ImageInfo->ImageBase, ImageInfo->ImageSize, SignatureInput->Signature, SignatureInput->Mask);
						//KeUnstackDetachProcess(&ApcState);
					}
					__except (EXCEPTION_EXECUTE_HANDLER)
					{
						int exceptionCode = GetExceptionCode();
						DEBUG_MESSAGE("Exception occured with error code: " + exceptionCode);
					}
		
					Status = STATUS_SUCCESS;
					NumBytesIO = sizeof(SIGNATURE_REQUEST);
				}
				
				break;
				
			}
		default: //Invalid parameter
			{
				Status = STATUS_INVALID_PARAMETER;
				NumBytesIO = 0;	

			}

	}

	

	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = NumBytesIO;

	// Can not change status or Information once IoCompleteRequest() has been called.
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;	
}

