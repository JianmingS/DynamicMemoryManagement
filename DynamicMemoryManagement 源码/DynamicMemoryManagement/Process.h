#pragma once
class Process
{
private:
	/*
	ע��
	��1���ֹ����룺
		���̾��У� ���̱�ʶ + ��������ռ��С
	��2���Զ�������
		���̾��У� ���̱�ʶ + ��������ռ��С + ������������ʱ��
	*/
	int PID; // ���̱�ʶ
	int PSize; // ��������ռ��С
	int PTime; // ������������ʱ��
	int PselType; 
	// ���ֽ��̲�����ʽ��
	// �ֹ����룺PselType = 0; 
	// �Զ�������PselType = 1;
	int nextID; // ��һ���������̵ı�ʶ
public:
	Process();
	~Process();
	void CreateProcess(int Size); //�ֹ��������
	void CreateProcess(int Size, int Time);//�Զ���������
	int getPID(); // ��ȡ����ID
	int getPSize(); // ��ȡ���̴�С
	int getPTime(); // ��ȡ��������ʱ��
	int getPselType(); // ��ȡ���̲�������
	void ChangeSize(int ChangeSize); // �ı���̿ռ��С
	void decrease(); // �������̱�ʶ��ʹ���̱�ʶ��0��������������
};

