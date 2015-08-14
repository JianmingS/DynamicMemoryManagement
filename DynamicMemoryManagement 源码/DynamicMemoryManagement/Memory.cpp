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
		// �״���Ӧ�㷨
		Flag = FirstFit(Size, ID);
		break;
	}
	case 1:{
		// ѭ���״���Ӧ�㷨
		Flag = NextFit(Size, ID);
		break;
	}
	case 2:{
		// �����Ӧ�㷨
		Flag = BestFit(Size, ID);
		break;
	}
	case 3:{
		// ���Ӧ�㷨
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
		// �����ڴ�ռ�
		if ((!deqMem[i].Status) && (Size <= deqMem[i].Size)) {
			// ����ռ�û��ʹ�ã����Ҵ�С�����������ռ䣬��ʹ�ô˿ռ䣬�����������
			if (Size != deqMem[i].Size) {
				// �������������ڽ�������ռ䣬����Խ�ʣ�µĿռ���Ϊһ������
				Ptn.Address = deqMem[i].Address + Size;
				Ptn.Size = deqMem[i].Size - Size;
				Ptn.Status = false;
				deqMem.insert(deqMem.begin() + i + 1, Ptn);
				// �����ڽ��̷����ڴ�ʱ��������ѡ��̬���������㷨�����ԶԲ�ѯָ����и��¡�
				searchPtrForNF = (i + 1)%deqMem.size();
			}
			// �ı��ڴ�״̬������
			deqMem[i].ID = ID;
			deqMem[i].Size = Size;
			deqMem[i].Status = true;
			isSuccess = true;
			break;
		}
	}
	// ���ؽ����Ƿ��ѳɹ��õ�������Ŀռ�
	return isSuccess;
}
bool Memory::NextFit(int Size, int ID) {
	bool isSuccess = false;
	int i = searchPtrForNF;
	int flag = searchPtrForNF;
	// �Ӳ�ѯָ�뿪ʼ�������ڴ�ռ�
	do 
	{
		if ((!deqMem[i].Status) && (Size <= deqMem[i].Size)) {
			// ����ռ�û��ʹ�ã����Ҵ�С�����������ռ䣬��ʹ�ô˿ռ䣬�����������
			if (Size != deqMem[i].Size) {
				// �������������ڽ�������ռ䣬����Խ�ʣ�µĿռ���Ϊһ������
				Ptn.Address = deqMem[i].Address + Size;
				Ptn.Size = deqMem[i].Size - Size;
				Ptn.Status = false;
				deqMem.insert(deqMem.begin() + i + 1, Ptn);
				// �����ڽ��̷����ڴ�ʱ��������ѡ��̬���������㷨�����ԶԲ�ѯָ����и��¡�
				searchPtrForNF = (i + 1)%deqMem.size();
			}
			// �ı��ڴ�״̬������
			deqMem[i].ID = ID;
			deqMem[i].Size = Size;
			deqMem[i].Status = true;
			isSuccess = true;
			break;
		}
		// ������һ���ڴ����
		i = (i + 1) % deqMem.size();
	} while (i != flag);
	// ���ؽ����Ƿ��ѳɹ��õ�������Ŀռ�
	return isSuccess;
}
bool Memory::BestFit(int Size, int ID) {
	bool isSuccess = false;
	// �Կ����ڴ���������տռ��С����˳���������
	Increse();
	for (int m = 0; m < vecNode.size(); ++m) {
		// ���������ڴ�ռ�
		int i = vecNode[m].Index;
		if ((!deqMem[i].Status) && (Size <= deqMem[i].Size)) {
			// ����ռ�û��ʹ�ã����Ҵ�С�����������ռ䣬��ʹ�ô˿ռ䣬�����������
			if (Size != deqMem[i].Size) {
				// �������������ڽ�������ռ䣬����Խ�ʣ�µĿռ���Ϊһ������
				Ptn.Address = deqMem[i].Address + Size;
				Ptn.Size = deqMem[i].Size - Size;
				Ptn.Status = false;
				deqMem.insert(deqMem.begin() + i + 1, Ptn);
				// �����ڽ��̷����ڴ�ʱ��������ѡ��̬���������㷨�����ԶԲ�ѯָ����и���
				searchPtrForNF = (i + 1) % deqMem.size();
			}
			// �ı��ڴ�״̬������
			deqMem[i].ID = ID;
			deqMem[i].Size = Size;
			deqMem[i].Status = true;
			isSuccess = true;
			break;
		}
	}
	vecNode.clear();
	// ���ؽ����Ƿ��ѳɹ��õ�������Ŀռ�
	return isSuccess;
}
bool Memory::WorstFit(int Size, int ID){
	bool isSuccess = false;
	// �Կ����ڴ���������տռ��С�ݼ�˳���������
	Decrese();
	for (int m = 0; m < vecNode.size(); ++m) {
		// ���������ڴ�ռ�
		int i = vecNode[m].Index;
		if ((!deqMem[i].Status) && (Size <= deqMem[i].Size)) {
			// ����ռ�û��ʹ�ã����Ҵ�С�����������ռ䣬��ʹ�ô˿ռ䣬�����������
			if (Size != deqMem[i].Size) {
				// �������������ڽ�������ռ䣬����Խ�ʣ�µĿռ���Ϊһ������
				Ptn.Address = deqMem[i].Address + Size;
				Ptn.Size = deqMem[i].Size - Size;
				Ptn.Status = false;
				deqMem.insert(deqMem.begin() + i + 1, Ptn);
				// �����ڽ��̷����ڴ�ʱ��������ѡ��̬���������㷨�����ԶԲ�ѯָ����и���
				searchPtrForNF = (i + 1) % deqMem.size();
			}
			// �ı��ڴ�״̬������
			deqMem[i].ID = ID;
			deqMem[i].Size = Size;
			deqMem[i].Status = true;
			isSuccess = true;
			break;
		}
	}
	vecNode.clear();
	// ���ؽ����Ƿ��ѳɹ��õ�������Ŀռ�
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
	// ��ȡ��Ҫ���յ��ܵĿռ��С
	for (int i = 0; i < deqMem.size(); ++i) {
		if (!deqMem[i].Status) {
			mergeSize += deqMem[i].Size;
		}
	}
	// �ض�λ
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
		// �����ڴ�ռ䣬�����ڴ�ռ��Ƿ�ʹ�ã��Լ����̱�ʶȷ�����յ��ڴ����
		if (deqMem[i].Status && (deqMem[i].ID == ID)) {
			if (Size == deqMem[i].Size) {
				// ���̻��յ��ڴ�ռ��С == ��ǰ�洢�˽����ڴ�����Ŀռ��С
				// ֱ�ӻ���
				deqMem[i].Status = false;
				Merge(i); // �����ڽӵĿ����ڴ�������кϲ�
				unusedSize += Size;
				break;
			}
			if (Size > deqMem[i].Size) {
				// ���̻��յ��ڴ�ռ��С > ��ǰ�洢�˽����ڴ�����Ŀռ��С
				// ���մ��ڴ�����Ŀռ䣬������Ѱ����Ҫ���յ���һ���ڴ����
				deqMem[i].Status = false;
				Merge(i); // �����ڽӵĿ����ڴ�������кϲ�
				unusedSize += deqMem[i].Size;
				--i;
				continue;
			}
			// ���̻��յ��ڴ�ռ��С < ��ǰ�洢�˽����ڴ�����Ŀռ��С
			// �ڴ�ռ��м����»��յ��ڴ����
			deqMem[i].Size -= Size;
			Ptn.Address = deqMem[i].Address + deqMem[i].Size;
			Ptn.Size = Size;
			Ptn.Status = false;
			deqMem.insert(deqMem.begin() + i + 1, Ptn);
			Merge(i + 1); // �����ڽӵĿ����ڴ�������кϲ�
			unusedSize += Size;
			break;
		}
	}
	getLog(); // ������־
}

void Memory::Merge(int Index) {
	int beginAddress = deqMem[Index].Address;
	int beginIndex = Index, endIndex = Index;
	int j = Index;
	int sizeTmp = 0;
	// ���ַ�ݼ���������������жϿ��Ժϲ��Ŀ����ڴ������С��ַ
	while (j >= 0 && !deqMem[j].Status) {
		beginAddress = deqMem[j].Address;
		beginIndex = j;
		sizeTmp += deqMem[j].Size;
		--j;
	}
	j = Index + 1;
	// ���ַ������������������жϿ��Ժϲ��Ŀ����ڴ��������ַ
	while (j < deqMem.size() && !deqMem[j].Status) {
		endIndex = j;
		sizeTmp += deqMem[j].Size;
		++j;
	}
	// �Բ�ѯָ����и��¡�
	searchPtrForNF -= (endIndex - beginIndex);
	// �������Ŀ����ڴ�������кϲ���
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

// �����ڴ涯̬�仯��־
void Memory::getLog() {
	outFile << deqMem.size() << endl; // �ڴ�����ĸ���
	for (int i = 0; i < deqMem.size(); ++i) {
		// �ڴ�����н��̱�ʶ���ڴ����ʼַ���ڴ������С���ڴ����״̬���Ƿ�ʹ�ã�
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