#include "stdafx.h"
#include "Memory.h"
#include <algorithm>
#include <iostream>
using namespace std;


Memory::Memory()
{
	unusedSize = totalSize;
	searchPtrForNF = 0;
	Ptn.Address = 0;
	Ptn.Size = totalSize;
	Ptn.Status = false;
	deqMem.push_back(Ptn);
	outFile.open(Filelog);
	if (!outFile) {
		cerr << "Open Error!" << endl;
		exit(1);
	}
}


Memory::~Memory()
{
	deqMem.clear();
}

bool Memory::Allocate(int Size, int algType, int ID) {
	bool Flag = false;
	if (Size > unusedSize) {
		return false;
	}
	else{
		unusedSize = unusedSize - Size;
	}
	switch (algType)
	{
	case 0:{
		// 首次适应算法
		Flag = FirstFit(Size, ID);
		break;
	}
	case 1:{
		// 循环首次适应算法
		Flag = NextFit(Size, ID);
		break;
	}
	case 2:{
		// 最佳适应算法
		Flag = BestFit(Size, ID);
		break;
	}
	case 3:{
		// 最坏适应算法
		Flag = WorstFit(Size, ID);
		break;
	}
	default:
		break;
	}
	if (Flag) {
		getLog();
	}
	return Flag;
}

bool Memory::FirstFit(int Size, int ID) {
	bool isSuccess = false;
	for (int i = 0; i < deqMem.size(); ++i) {
		// 遍历内存空间
		if ((!deqMem[i].Status) && (Size <= deqMem[i].Size)) {
			// 如果空间没被使用，并且大小满足进程所需空间，则使用此空间，否则继续遍历
			if (Size != deqMem[i].Size) {
				// 如果空闲区间大于进程所需空间，则可以将剩下的空间作为一个分区
				Ptn.Address = deqMem[i].Address + Size;
				Ptn.Size = deqMem[i].Size - Size;
				Ptn.Status = false;
				deqMem.insert(deqMem.begin() + i + 1, Ptn);
				// 由于在进程分配内存时可以任意选择动态分区分配算法，所以对查询指针进行更新。
				searchPtrForNF = (i + 1)%deqMem.size();
			}
			// 改变内存状态：已用
			deqMem[i].ID = ID;
			deqMem[i].Size = Size;
			deqMem[i].Status = true;
			isSuccess = true;
			break;
		}
	}
	// 返回进程是否已成功得到其申请的空间
	return isSuccess;
}
bool Memory::NextFit(int Size, int ID) {
	bool isSuccess = false;
	int i = searchPtrForNF;
	int flag = searchPtrForNF;
	// 从查询指针开始，遍历内存空间
	do 
	{
		if ((!deqMem[i].Status) && (Size <= deqMem[i].Size)) {
			// 如果空间没被使用，并且大小满足进程所需空间，则使用此空间，否则继续遍历
			if (Size != deqMem[i].Size) {
				// 如果空闲区间大于进程所需空间，则可以将剩下的空间作为一个分区
				Ptn.Address = deqMem[i].Address + Size;
				Ptn.Size = deqMem[i].Size - Size;
				Ptn.Status = false;
				deqMem.insert(deqMem.begin() + i + 1, Ptn);
				// 由于在进程分配内存时可以任意选择动态分区分配算法，所以对查询指针进行更新。
				searchPtrForNF = (i + 1)%deqMem.size();
			}
			// 改变内存状态：已用
			deqMem[i].ID = ID;
			deqMem[i].Size = Size;
			deqMem[i].Status = true;
			isSuccess = true;
			break;
		}
		// 遍历下一个内存分区
		i = (i + 1) % deqMem.size();
	} while (i != flag);
	// 返回进程是否已成功得到其申请的空间
	return isSuccess;
}
bool Memory::BestFit(int Size, int ID) {
	bool isSuccess = false;
	// 对空闲内存分区，按照空间大小递增顺序进行排序
	Increse();
	for (int m = 0; m < vecNode.size(); ++m) {
		// 遍历空闲内存空间
		int i = vecNode[m].Index;
		if ((!deqMem[i].Status) && (Size <= deqMem[i].Size)) {
			// 如果空间没被使用，并且大小满足进程所需空间，则使用此空间，否则继续遍历
			if (Size != deqMem[i].Size) {
				// 如果空闲区间大于进程所需空间，则可以将剩下的空间作为一个分区
				Ptn.Address = deqMem[i].Address + Size;
				Ptn.Size = deqMem[i].Size - Size;
				Ptn.Status = false;
				deqMem.insert(deqMem.begin() + i + 1, Ptn);
				// 由于在进程分配内存时可以任意选择动态分区分配算法，所以对查询指针进行更新
				searchPtrForNF = (i + 1) % deqMem.size();
			}
			// 改变内存状态：已用
			deqMem[i].ID = ID;
			deqMem[i].Size = Size;
			deqMem[i].Status = true;
			isSuccess = true;
			break;
		}
	}
	vecNode.clear();
	// 返回进程是否已成功得到其申请的空间
	return isSuccess;
}
bool Memory::WorstFit(int Size, int ID){
	bool isSuccess = false;
	// 对空闲内存分区，按照空间大小递减顺序进行排序
	Decrese();
	for (int m = 0; m < vecNode.size(); ++m) {
		// 遍历空闲内存空间
		int i = vecNode[m].Index;
		if ((!deqMem[i].Status) && (Size <= deqMem[i].Size)) {
			// 如果空间没被使用，并且大小满足进程所需空间，则使用此空间，否则继续遍历
			if (Size != deqMem[i].Size) {
				// 如果空闲区间大于进程所需空间，则可以将剩下的空间作为一个分区
				Ptn.Address = deqMem[i].Address + Size;
				Ptn.Size = deqMem[i].Size - Size;
				Ptn.Status = false;
				deqMem.insert(deqMem.begin() + i + 1, Ptn);
				// 由于在进程分配内存时可以任意选择动态分区分配算法，所以对查询指针进行更新
				searchPtrForNF = (i + 1) % deqMem.size();
			}
			// 改变内存状态：已用
			deqMem[i].ID = ID;
			deqMem[i].Size = Size;
			deqMem[i].Status = true;
			isSuccess = true;
			break;
		}
	}
	vecNode.clear();
	// 返回进程是否已成功得到其申请的空间
	return isSuccess;
}


bool Memory::CmpIncrese(const myNode &N1, const myNode &N2) {
	if (N1.Size == N2.Size) return N1.Index < N2.Index;
	return N1.Size < N2.Size;
}
bool Memory::CmpDecrese(const myNode &N1, const myNode &N2) {
	if (N1.Size == N2.Size) return N1.Index < N2.Index;
	return N1.Size > N2.Size;
}
deque<Partition> Memory::Compact() {
	deque<Partition> deqTmp;
	int mergeSize = 0;
	// 获取需要紧凑的总的空间大小
	for (int i = 0; i < deqMem.size(); ++i) {
		if (!deqMem[i].Status) {
			mergeSize += deqMem[i].Size;
		}
	}
	// 重定位
	Ptn.Address = 0;
	Ptn.Size = mergeSize;
	Ptn.Status = false;
	deqTmp.push_back(Ptn);
	int deqTmpSize = 0;
	for (int i = 0; i < deqMem.size(); ++i) {
		if (deqMem[i].Status) {
			Ptn.ID = deqMem[i].ID;
			Ptn.Address = deqTmp[deqTmpSize].Address + deqTmp[deqTmpSize].Size;
			Ptn.Size = deqMem[i].Size;
			Ptn.Status = true;
			deqTmp.push_back(Ptn);
			++deqTmpSize;
		}
	}
	deqMem.clear();
	deqMem = deqTmp;
	deqTmp.clear();
	getLog();
	return deqMem;
}

deque<Partition> Memory::getMemStatus() {
	return deqMem;
}

void Memory::ReleaseSpace(int ID, int Size) {
	for (int i = 0; i < deqMem.size(); ++i) {
		// 遍历内存空间，根据内存空间是否使用，以及进程标识确定回收的内存分区
		if (deqMem[i].Status && (deqMem[i].ID == ID)) {
			if (Size == deqMem[i].Size) {
				// 进程回收的内存空间大小 == 当前存储此进程内存分区的空间大小
				// 直接回收
				deqMem[i].Status = false;
				Merge(i); // 对于邻接的空闲内存分区进行合并
				unusedSize += Size;
				break;
			}
			if (Size > deqMem[i].Size) {
				// 进程回收的内存空间大小 > 当前存储此进程内存分区的空间大小
				// 回收此内存分区的空间，并继续寻找需要回收的下一个内存分区
				deqMem[i].Status = false;
				Merge(i); // 对于邻接的空闲内存分区进行合并
				unusedSize += deqMem[i].Size;
				--i;
				continue;
			}
			// 进程回收的内存空间大小 < 当前存储此进程内存分区的空间大小
			// 内存空间中加入新回收的内存分区
			deqMem[i].Size -= Size;
			Ptn.Address = deqMem[i].Address + deqMem[i].Size;
			Ptn.Size = Size;
			Ptn.Status = false;
			deqMem.insert(deqMem.begin() + i + 1, Ptn);
			Merge(i + 1); // 对于邻接的空闲内存分区进行合并
			unusedSize += Size;
			break;
		}
	}
	getLog(); // 记入日志
}

void Memory::Merge(int Index) {
	int beginAddress = deqMem[Index].Address;
	int beginIndex = Index, endIndex = Index;
	int j = Index;
	int sizeTmp = 0;
	// 向地址递减方向进行搜索，判断可以合并的空闲内存分区最小地址
	while (j >= 0 && !deqMem[j].Status) {
		beginAddress = deqMem[j].Address;
		beginIndex = j;
		sizeTmp += deqMem[j].Size;
		--j;
	}
	j = Index + 1;
	// 向地址递增方向进行搜索，判断可以合并的空闲内存分区最大地址
	while (j < deqMem.size() && !deqMem[j].Status) {
		endIndex = j;
		sizeTmp += deqMem[j].Size;
		++j;
	}
	// 对查询指针进行更新。
	searchPtrForNF -= (endIndex - beginIndex);
	// 对连续的空闲内存分区进行合并。
	deqMem.erase(deqMem.begin() + beginIndex, deqMem.begin() + endIndex + 1);
	Ptn.Address = beginAddress;
	Ptn.Size = sizeTmp;
	Ptn.Status = false;
	deqMem.insert(deqMem.begin() + beginIndex, Ptn);
}

void Memory::Increse() {
	CreateVec();
	sort(vecNode.begin(), vecNode.end(), CmpIncrese);
}
void Memory::Decrese() {
	CreateVec();
	sort(vecNode.begin(), vecNode.end(), CmpDecrese);
}

void Memory::CreateVec() {
	for (int i = 0; i < deqMem.size(); ++i) {
		if (!deqMem[i].Status) {
			Nd.Index = i;
			Nd.Size = deqMem[i].Size;
			vecNode.push_back(Nd);
		}
	}
}

// 保存内存动态变化日志
void Memory::getLog() {
	outFile << deqMem.size() << endl; // 内存分区的个数
	for (int i = 0; i < deqMem.size(); ++i) {
		// 内存分区中进程标识、内存分区始址、内存分区大小、内存分区状态（是否被使用）
		outFile << deqMem[i].ID << " " << deqMem[i].Address
			<< " " << deqMem[i].Size << " " << deqMem[i].Status 
			<< endl;
	}
}

string Memory::getFileName() {
	return Filelog;
}

void Memory::Distroy() {
	deqMem.clear();
	unusedSize = totalSize;
	searchPtrForNF = 0;
	Ptn.Address = 0;
	Ptn.Size = totalSize;
	Ptn.Status = false;
	deqMem.push_back(Ptn);
	getLog();
}

int Memory::getMaxSize() {
	return totalSize;
}