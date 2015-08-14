#pragma once
#include <deque>
#include <vector>
#include <fstream>
using namespace std;

// 内存分区
struct Partition {
	int ID; // 进程标识
	int Address; // 分区地址
	int Size; // 分区大小
	bool Status; // 忙：true 闲：false
	Partition() {
		ID = -1; // 进程ID默认初值
	}
};
// 为实现BestFit,WorstFit排序，设计的空闲的内存分区的数据结构
struct myNode
{
	int Index; // 空闲的内存分区在内存空间的位置
	int Size; // 空闲的内存分区的大小
};
class Memory
{
private:
	const int totalSize = 100; // 总共的内存空间大小
	const int beginIndex = 0; // 内存空间起始地址
	const int endIndex = 100; // 内存空间结束地址
	int unusedSize; // 内存空间未使用的大小
	int searchPtrForNF; // next fit 查询指针
	deque<Partition> deqMem; // 保存整个内存的状态
	vector<myNode> vecNode; // 保存按空闲内存空间大小排序后的结果
	Partition Ptn; // 内存分区临时变量
	myNode Nd; // 空闲的内存分区临时变量
	bool FirstFit(int Size, int ID); // 首次适应算法
	bool NextFit(int Size, int ID); // 循环首次适应算法
	bool BestFit(int Size, int ID); // 最佳适应算法
	bool WorstFit(int Size, int ID); // 最坏适应算法
	void CreateVec();
	void Increse(); // 递增排序
	void Decrese(); // 递减排序
	
	// 比较函数
	static bool CmpIncrese(const myNode &N1, const myNode &N2);
	static bool CmpDecrese(const myNode &N1, const myNode &N2);

	void Merge(int Index); // 合并连续的空闲内存分区
	const string Filelog = "MemLog"; // 内存动态分配日志（为了回放）
	ofstream outFile;
	void getLog(); // 获取内存动态分配日志
public:
	Memory();
	~Memory();
	bool Allocate(int Size, int algType, int ID); // 分配空间
	deque<Partition> getMemStatus(); // 获取内存空间状态
	void ReleaseSpace(int ID, int Size); // 释放内存空间（并完成连续的空闲内存分区的回收）
	deque<Partition> Compact(); // 紧凑算法（碎片整理）
	string getFileName();  // 获取日志文件名
	void Distroy(); // 将内存状态重置
	int getMaxSize(); // 获取总共的内存空间大小
};

