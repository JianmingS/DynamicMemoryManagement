#pragma once
class Process
{
private:
	/*
	注：
	（1）手工输入：
		进程具有： 进程标识 + 进程所需空间大小
	（2）自动产生：
		进程具有： 进程标识 + 进程所需空间大小 + 进程所需运行时间
	*/
	int PID; // 进程标识
	int PSize; // 进程所需空间大小
	int PTime; // 进程所需运行时间
	int PselType; 
	// 区分进程产生方式：
	// 手工输入：PselType = 0; 
	// 自动产生：PselType = 1;
	int nextID; // 下一个产生进程的标识
public:
	Process();
	~Process();
	void CreateProcess(int Size); //手工输入进程
	void CreateProcess(int Size, int Time);//自动产生进程
	int getPID(); // 获取进程ID
	int getPSize(); // 获取进程大小
	int getPTime(); // 获取进程运行时间
	int getPselType(); // 获取进程产生类型
	void ChangeSize(int ChangeSize); // 改变进程空间大小
	void decrease(); // 纠正进程标识（使进程标识从0连续递增产生）
};

