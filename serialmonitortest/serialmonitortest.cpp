// serialdump.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <Windows.h>
#include<iostream>
#include<fstream>
#include<time.h>
#include <vector>



int _tmain(int argc, _TCHAR* argv[])
{

	// Declare variables and structures
	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };



	// Open the highest available serial port number
	fprintf(stderr, "Opening serial port...");
	hSerial = CreateFile("\\\\.\\COM30", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	else fprintf(stderr, "OK\n");

	// Set device parameters (38400 baud, 1 start bit,
	// 1 stop bit, no parity)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error getting device state\n");
		CloseHandle(hSerial);
		return 1;
	}

	dcbSerialParams.BaudRate = CBR_115200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (SetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error setting device parameters\n");
		CloseHandle(hSerial);
		return 1;
	}

	// Set COM port timeout settings
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (SetCommTimeouts(hSerial, &timeouts) == 0)
	{
		fprintf(stderr, "Error setting timeouts\n");
		CloseHandle(hSerial);
		return 1;
	}

	//ReadFile(hSerial, read_bytes, 128, &bytes_read, NULL)
	size_t send_size = 64;
	std::vector<uint8_t> send_data;
	send_data.resize(send_size);

	uint32_t* p_packet = (uint32_t*)&send_data[0];
	// 55 AA 01 3A
	p_packet[0] = 0x3a01aa55;
	// 99 B1 F7 99
	p_packet[1] = 0x99F7B199;
	// B5 C4 E0 41
	p_packet[2] = 0x41E0C4B5;
	// CA 56 0E A2
	p_packet[3] = 0xA20E56CA;
	// 6A AC BC BD
	p_packet[4] = 0xBDBCAC6A;
	// 0C BB BD BD
	p_packet[5] = 0xBDBDBD0C;
	// 43 B4 BD BD
	p_packet[6] = 0xBDBDB443;
	// BD BD BD BD
	p_packet[7] = 0xBDBDBDBD;
	// 41 42 42 42
	p_packet[8] = 0x42424241;
	// BE BD BD BD
	p_packet[9] = 0xBDBDBDBE;
	// 43 42 42 42
	p_packet[10] = 0x42424243;
	// E1 BC 9A BC
	p_packet[11] = 0xBC9ABCE1;
	// 14 BD FB BD
	p_packet[12] = 0xBDFBBD14;
	// 07 00 BE BD
	p_packet[13] = 0xBDBE0007;
	// 60 BD BD BD
	p_packet[14] = 0xBDBDBD60;
	// 23 07 87 C9
	p_packet[15] = 0xC9870723;

	//for (size_t i = 0; i < send_size; ++i) {
	//	//send_data[i] = (uint8_t)'A';
	//	send_data[i] += (uint8_t)(i % 256);
	//}

	DWORD bytes_written = 0;
	DWORD total_data = 0;

	while (total_data < 32 * 1024) {
		WriteFile(hSerial, &send_data[0], send_data.size(), &bytes_written, NULL);
		total_data += bytes_written;
	}

	fprintf(stderr, "Closing serial port...");
	if (CloseHandle(hSerial) == 0)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	fprintf(stderr, "OK\n");
	return 0;
}



