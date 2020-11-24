#pragma once

#ifndef DATA_H
#define DATA_H

#include <ntdef.h>
// DO NOT ASSIGN THESE DECLARATIONS ANY VALUES
// ASSIGNING A VALUE HERE CAUSES A "MULTIPLY DEFINED SYMBOL" LINKING ERROR
LPCWSTR imageNameStr;
BOOLEAN imageNameSet;

ULONG PID;
ULONG ProcessEntryPoint;
ULONG ImageBaseAddress; // Store process here

ULONG AddressOfPattern;

PIMAGE_INFO ImageInfo;
PDEVICE_OBJECT pDeviceObject;
UNICODE_STRING dev, dos;


#endif
