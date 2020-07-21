#include <wdm.h>
#include "../Driver1Control/ControlCodes.h"

#define CONTROL_DEVICE_DOS_NAME L"\\DosDevices\\DosDriver1Decice1"

PDEVICE_OBJECT ControlDevice;

DRIVER_DISPATCH DriverDispatchDoNothing;
DRIVER_DISPATCH DriverDispatchIoControl;
DRIVER_DISPATCH DriverDispatchCreate;
DRIVER_UNLOAD DriverUnload;

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	int i;
	UNICODE_STRING controlDeviceName;
	UNICODE_STRING controlDeviceDosName;
	NTSTATUS status;
	UNREFERENCED_PARAMETER(RegistryPath);

	RtlInitUnicodeString(&controlDeviceName, L"\\Device\\Driver1Decice1");
	status = IoCreateDevice(DriverObject, 0, &controlDeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &ControlDevice);
	if (status != STATUS_SUCCESS)
	{
		return status;
	}

	RtlInitUnicodeString(&controlDeviceDosName, CONTROL_DEVICE_DOS_NAME);
	status = IoCreateSymbolicLink(&controlDeviceDosName, &controlDeviceName);
	if(status != STATUS_SUCCESS)
	{
		return status;
	}

	DriverObject->DriverStartIo = NULL;
	DriverObject->DriverUnload = DriverUnload;

	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DriverDispatchDoNothing;
	}

	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverDispatchIoControl;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverDispatchCreate;

	return STATUS_SUCCESS;
}

NTSTATUS DriverDispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DriverDispatchIoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION stackLocation = IoGetCurrentIrpStackLocation(Irp);
	char* inputBuffer = (char*)Irp->AssociatedIrp.SystemBuffer + stackLocation->Parameters.DeviceIoControl.InputBufferLength;
	char* outputBuffer = (char*)Irp->AssociatedIrp.SystemBuffer + stackLocation->Parameters.DeviceIoControl.OutputBufferLength;
	int i;
	UNREFERENCED_PARAMETER(DeviceObject);

	if ((stackLocation->Parameters.DeviceIoControl.InputBufferLength * 2) != stackLocation->Parameters.DeviceIoControl.OutputBufferLength)
	{
		Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
		Irp->IoStatus.Information = 0;
		goto completeRequest;
	}

	for (i = stackLocation->Parameters.DeviceIoControl.InputBufferLength; i > 0; i++)
	{
		outputBuffer[i * 2 - 1] = inputBuffer[i - 1] - 32; // 'a'(97) - 'A'(65) = 32
		outputBuffer[i * 2 - 2] = inputBuffer[i - 1];
	}

	Irp->IoStatus.Information = stackLocation->Parameters.DeviceIoControl.OutputBufferLength;
	Irp->IoStatus.Status = STATUS_SUCCESS;

completeRequest:
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Irp->IoStatus.Status;
}

NTSTATUS DriverDispatchDoNothing(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

void DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING controlDeviceDosName;

	IoDeleteDevice(ControlDevice);
	RtlInitUnicodeString(&controlDeviceDosName, CONTROL_DEVICE_DOS_NAME);
	IoDeleteSymbolicLink(&controlDeviceDosName);
}
