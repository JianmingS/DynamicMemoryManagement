#pragma once
#include <deque>
#include <vector>
#include <fstream>
using namespace std;

// �ڴ����
struct Partition {
	int ID; // ���̱�ʶ
	int Address; // ������ַ
	int Size; // ������С
	bool Status; // æ��true �У�false
	Partition() {
		ID = -1; // ����IDĬ�ϳ�ֵ
	}
};
// Ϊʵ��BestFit,WorstFit������ƵĿ��е��ڴ���������ݽṹ
struct myNode
{
	int Index; // ���е��ڴ�������ڴ�ռ��λ��
	int Size; // ���е��ڴ�����Ĵ�С
};
class Memory
{
private:
	const int totalSize = 100; // �ܹ����ڴ�ռ��С
	const int beginIndex = 0; // �ڴ�ռ���ʼ��ַ
	const int endIndex = 100; // �ڴ�ռ������ַ
	int unusedSize; // �ڴ�ռ�δʹ�õĴ�С
	int searchPtrForNF; // next fit ��ѯָ��
	deque<Partition> deqMem; // ���������ڴ��״̬
	vector<myNode> vecNode; // ���水�����ڴ�ռ��С�����Ľ��
	Partition Ptn; // �ڴ������ʱ����
	myNode Nd; // ���е��ڴ������ʱ����
	bool FirstFit(int Size, int ID); // �״���Ӧ�㷨
	bool NextFit(int Size, int ID); // ѭ���״���Ӧ�㷨
	bool BestFit(int Size, int ID); // �����Ӧ�㷨
	bool WorstFit(int Size, int ID); // ���Ӧ�㷨
	void CreateVec();
	void Increse(); // ��������
	void Decrese(); // �ݼ�����
	
	// �ȽϺ���
	static bool CmpIncrese(const myNode &N1, const myNode &N2);
	static bool CmpDecrese(const myNode &N1, const myNode &N2);

	void Merge(int Index); // �ϲ������Ŀ����ڴ����
	const string Filelog = "MemLog"; // �ڴ涯̬������־��Ϊ�˻طţ�
	ofstream outFile;
	void getLog(); // ��ȡ�ڴ涯̬������־
public:
	Memory();
	~Memory();
	bool Allocate(int Size, int algType, int ID); // ����ռ�
	deque<Partition> getMemStatus(); // ��ȡ�ڴ�ռ�״̬
	void ReleaseSpace(int ID, int Size); // �ͷ��ڴ�ռ䣨����������Ŀ����ڴ�����Ļ��գ�
	deque<Partition> Compact(); // �����㷨����Ƭ����
	string getFileName();  // ��ȡ��־�ļ���
	void Distroy(); // ���ڴ�״̬����
	int getMaxSize(); // ��ȡ�ܹ����ڴ�ռ��С
};

