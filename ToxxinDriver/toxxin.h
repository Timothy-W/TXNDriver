#pragma once

#ifndef TOXXIN_H
#define TOXXIN_H	


#include <ntifs.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject);


#endif
