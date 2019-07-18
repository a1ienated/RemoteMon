#include "CPU_Monitor.h"

double filetime2double_msec(FILETIME ft) {
	return 1e-4*((UINT64(ft.dwHighDateTime) << 32) + (UINT64)ft.dwLowDateTime);
}

CPU_Monitor::CPU_Monitor(unsigned _delay) : Overall(0), Busy(0), usageCPU(0), delay(_delay)
{
}

void CPU_Monitor::paramRequestCPU()
{
	Sleep(delay);
	HINSTANCE hLib = LoadLibraryA("kernel32.dll");
	GetSystemTimes_API_ptr GetSystemTimes_API =
		(GetSystemTimes_API_ptr)GetProcAddress(hLib, "GetSystemTimes");

	//Загрузка процессора для одного процесса определяется как отношение количества
	//полностью использованных квантов времени к общему их числу на некотором промежутке.
	GetSystemTimes_API(&IdleTime1, &KernelTime1, &UserTime1);
	Sleep(200);//Промежуток между замерами
	GetSystemTimes_API(&IdleTime2, &KernelTime2, &UserTime2);
	FreeLibrary(hLib);

	//Kernel Time включает и время простоя, поэтому полное время и чистое занятое:
	Overall = filetime2double_msec(KernelTime2) +
		filetime2double_msec(UserTime2) -
		filetime2double_msec(KernelTime1) -
		filetime2double_msec(UserTime1);
	Busy = Overall -
		filetime2double_msec(IdleTime2) +
		filetime2double_msec(IdleTime1);

	usageCPU = 100.0*(Busy / Overall); //Процент занятости
}

void CPU_Monitor::printToConsole()
{
	printf("IdleTime2 - IdleTime1 = %f msec\n", filetime2double_msec(IdleTime2)
		- filetime2double_msec(IdleTime1));
	printf("KernelTime2 - KernelTime1 = %f msec\n",
		filetime2double_msec(KernelTime2) - filetime2double_msec(KernelTime1));
	printf("UserTime2 - UserTime1 = %f msec\n",
		filetime2double_msec(UserTime2) - filetime2double_msec(UserTime1));

	printf("\nOverall = %f, msec\n", Overall);
	printf("Busy = %f, msec\n", Busy);
	printf("Usage = %f%%\n", usageCPU);
}

double CPU_Monitor::getCPU_Usage() const
{
	return usageCPU;
}

double CPU_Monitor::getOverall() const
{
	return Overall;
}

double CPU_Monitor::getBusy() const
{
	return Busy;
}