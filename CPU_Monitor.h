#pragma once
#include <windows.h>
#include <iostream>

typedef BOOL(__stdcall * GetSystemTimes_API_ptr)(LPFILETIME, LPFILETIME, LPFILETIME);

class CPU_Monitor
{
public:
	CPU_Monitor(unsigned _delay);
	double getCPU_Usage() const;
	double getOverall() const;
	double getBusy() const;
	void paramRequestCPU();
	void printToConsole();

private:	
	FILETIME IdleTime1, IdleTime2, KernelTime1,
		KernelTime2, UserTime1, UserTime2;
	unsigned delay;
	double usageCPU;
	double Overall;
	double Busy;

	
};

