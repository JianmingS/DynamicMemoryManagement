
// DynamicMemoryManagementDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Memory.h"
#include "Process.h"
#include <map>
#include "afxcmn.h"
#include <fstream>
#include <windows.h>
#include <ctime>
#include <iostream>
#include <algorithm>


struct ProcessMemVar
{
	int ID;
	int Size;
	int Time;
	int selType;
};

// CDynamicMemoryManagementDlg �Ի���
class CDynamicMemoryManagementDlg : public CDialogEx
{
// ����
public:
	CDynamicMemoryManagementDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DYNAMICMEMORYMANAGEMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeoperatingmode();
	// ѡ����̲�����ʽ
	CComboBox m_operatingMode;
	// ѡ������㷨
	CComboBox m_SelectAlgorithm;
	afx_msg void OnBnClickedExecute();
	// �ֹ��������̿ռ��С
	//CEdit m_InitialSize;
private:
	Memory Mem;
	int slcAlg;
	ofstream outFile;
	const string FileProcessLog = "ProcessLog";
public:
	afx_msg void OnCbnSelchangeProcessid();
public:
	deque<Process> deqProcess;
	Process ProTmp;
	// ����ID
	CComboBox m_ProcessID;
	void Display(deque<Partition> deqMem);
	void DisplayProcessList(deque<ProcessMemVar> deqProcessVar);
	void InsertProcessList(int ID, int Size);
	void DeleteProcessList(int ID);
	void ChangeProcessList(int ID, int Size);
	int Cbmm_ProcessIDIndex;
	// �ı���̿ռ��С
	CEdit m_ChangeSize;
	map<int, int> mapIDtoSize;
	afx_msg void OnBnClickedDefrag();
	//afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	// ��ʾ�������
	CListCtrl m_ProcessLIST;
	afx_msg void OnLvnItemchangedProcesslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedProcessstop();
	afx_msg void OnBnClickedReplay();
	afx_msg void OnBnClickedReset();
	void OnDraw(CDC* pDC, CRect rt, int x, int y, int z);
	// ��ֹ����
	CButton m_ProcessStop;
	void CDynamicMemoryManagementDlg::InsertProcessList(int ID, int Size, int Time);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// �ط�
	CButton m_RePlay;
	void getLog();
	void getLogForAuto();
	void initListControl();
};
