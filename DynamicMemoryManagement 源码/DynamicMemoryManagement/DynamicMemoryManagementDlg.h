
// DynamicMemoryManagementDlg.h : 头文件
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

// CDynamicMemoryManagementDlg 对话框
class CDynamicMemoryManagementDlg : public CDialogEx
{
// 构造
public:
	CDynamicMemoryManagementDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DYNAMICMEMORYMANAGEMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeoperatingmode();
	// 选择进程产生方式
	CComboBox m_operatingMode;
	// 选择分配算法
	CComboBox m_SelectAlgorithm;
	afx_msg void OnBnClickedExecute();
	// 手工创建进程空间大小
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
	// 进程ID
	CComboBox m_ProcessID;
	void Display(deque<Partition> deqMem);
	void DisplayProcessList(deque<ProcessMemVar> deqProcessVar);
	void InsertProcessList(int ID, int Size);
	void DeleteProcessList(int ID);
	void ChangeProcessList(int ID, int Size);
	int Cbmm_ProcessIDIndex;
	// 改变进程空间大小
	CEdit m_ChangeSize;
	map<int, int> mapIDtoSize;
	afx_msg void OnBnClickedDefrag();
	//afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	// 显示进程情况
	CListCtrl m_ProcessLIST;
	afx_msg void OnLvnItemchangedProcesslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedProcessstop();
	afx_msg void OnBnClickedReplay();
	afx_msg void OnBnClickedReset();
	void OnDraw(CDC* pDC, CRect rt, int x, int y, int z);
	// 终止进程
	CButton m_ProcessStop;
	void CDynamicMemoryManagementDlg::InsertProcessList(int ID, int Size, int Time);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 回放
	CButton m_RePlay;
	void getLog();
	void getLogForAuto();
	void initListControl();
};
