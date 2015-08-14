#include "stdafx.h"
#include "Process.h"


Process::Process()
{
	nextID = -1;
}


Process::~Process()
{
}

void Process::CreateProcess(int Size) {
	PID = ++nextID;
	PSize = Size;
	PselType = 0;
}
void Process::CreateProcess(int Size, int Time) {
	PID = ++nextID;
	PSize = Size;
	PTime = Time;
	PselType = 1;
}

int Process::getPID() {
	return PID;
}
int Process::getPSize() {
	return PSize;
}

int Process::getPTime() {
	return PTime;
}

int Process::getPselType() {
	return PselType;
}

void Process::ChangeSize(int ChangeSize){
	PSize += ChangeSize;
}

void Process::decrease() {
	--nextID;
}