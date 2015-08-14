
// DynamicMemoryManagementDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DynamicMemoryManagement.h"
#include "DynamicMemoryManagementDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDynamicMemoryManagementDlg �Ի���



CDynamicMemoryManagementDlg::CDynamicMemoryManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDynamicMemoryManagementDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	slcAlg = 0;
	outFile.open(FileProcessLog);
	if (!outFile) {
		cerr << "Open Error!" << endl;
		exit(1);
	}
}

void CDynamicMemoryManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_operatingMode, m_operatingMode);
	DDX_Control(pDX, IDC_SelectAlgorithm, m_SelectAlgorithm);
	//DDX_Control(pDX, IDC_InitialSize, m_InitialSize);
	DDX_Control(pDX, IDC_ProcessID, m_ProcessID);
	Cbmm_ProcessIDIndex = 1;
	DDX_Control(pDX, IDC_ChangeSize, m_ChangeSize);
	DDX_Control(pDX, IDC_ProcessLIST, m_ProcessLIST);
	DDX_Control(pDX, IDC_ProcessStop, m_ProcessStop);
	DDX_Control(pDX, IDC_RePlay, m_RePlay);
}

BEGIN_MESSAGE_MAP(CDynamicMemoryManagementDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_operatingMode, &CDynamicMemoryManagementDlg::OnCbnSelchangeoperatingmode)
	ON_BN_CLICKED(IDC_BUTTON3, &CDynamicMemoryManagementDlg::OnBnClickedExecute)
	ON_CBN_SELCHANGE(IDC_ProcessID, &CDynamicMemoryManagementDlg::OnCbnSelchangeProcessid)
	ON_BN_CLICKED(IDC_Defrag, &CDynamicMemoryManagementDlg::OnBnClickedDefrag)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDynamicMemoryManagementDlg::OnLvnItemchangedList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ProcessLIST, &CDynamicMemoryManagementDlg::OnLvnItemchangedProcesslist)
	ON_BN_CLICKED(IDC_ProcessStop, &CDynamicMemoryManagementDlg::OnBnClickedProcessstop)
	ON_BN_CLICKED(IDC_RePlay, &CDynamicMemoryManagementDlg::OnBnClickedReplay)
	ON_BN_CLICKED(IDC_Reset, &CDynamicMemoryManagementDlg::OnBnClickedReset)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDynamicMemoryManagementDlg ��Ϣ�������

BOOL CDynamicMemoryManagementDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	// m_operatingMode
	m_operatingMode.AddString(_T("�ֹ�����"));
	m_operatingMode.AddString(_T("�Զ�����"));
	m_operatingMode.SetCurSel(0);

	// m_SelectAlgorithm
	m_SelectAlgorithm.AddString(_T("�״���Ӧ�㷨"));
	m_SelectAlgorithm.AddString(_T("ѭ���״���Ӧ�㷨"));
	m_SelectAlgorithm.AddString(_T("�����Ӧ�㷨"));
	m_SelectAlgorithm.AddString(_T("���Ӧ�㷨"));
	m_SelectAlgorithm.SetCurSel(0);

	// m_ProcessID
	m_ProcessID.AddString(_T("����ID��..."));
	m_ProcessID.SetCurSel(0);

	// ��ʼ�� ProcessList

	CRect rect;
	m_ProcessLIST.GetClientRect(&rect);
	m_ProcessLIST.SetExtendedStyle(m_ProcessLIST.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ProcessLIST.InsertColumn(0, _T("ID"), LVCFMT_CENTER, rect.Width() / 2, 0);
	m_ProcessLIST.InsertColumn(1, _T("Size"), LVCFMT_CENTER, rect.Width() / 2, 1);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDynamicMemoryManagementDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDynamicMemoryManagementDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		// ���
		CClientDC dc(this);
		CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
		dc.SelectObject(&pen);
		dc.Rectangle(30, 145, 530, 320);
	}
}

void CDynamicMemoryManagementDlg::OnDraw(CDC* pDC, CRect rt, int x, int y, int z) {
	pDC->FillSolidRect(&rt, RGB(x, y, z));//��ɫ��� 
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDynamicMemoryManagementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDynamicMemoryManagementDlg::OnCbnSelchangeoperatingmode()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	initListControl();
	
}
void CDynamicMemoryManagementDlg::initListControl() {
	if (m_operatingMode.GetCurSel() == 1) {
		if (deqProcess.size() != 0) {
			MessageBox(_T("������Ľ������뷽ʽ���������ã�"), _T("���棡"), MB_OK);
			m_operatingMode.SetCurSel(0);
			return;
		}
		while (m_ProcessLIST.DeleteItem(0));
		m_ChangeSize.EnableWindow(false);
		m_ProcessStop.EnableWindow(false);
		m_ProcessID.EnableWindow(false);
		m_operatingMode.EnableWindow(false);
		m_RePlay.EnableWindow(false);
		while (m_ProcessLIST.DeleteColumn(0));
		CRect rect;
		m_ProcessLIST.GetClientRect(&rect);
		m_ProcessLIST.SetExtendedStyle(m_ProcessLIST.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_ProcessLIST.InsertColumn(0, _T("ID"), LVCFMT_CENTER, rect.Width() / 3, 0);
		m_ProcessLIST.InsertColumn(1, _T("Size"), LVCFMT_CENTER, rect.Width() / 3, 1);
		m_ProcessLIST.InsertColumn(2, _T("Time"), LVCFMT_CENTER, rect.Width() / 3, 2);
		//SetTimer(1, 1500, NULL);
	}
	else {
		if (deqProcess.size() != 0) {
			MessageBox(_T("������Ľ������뷽ʽ���������ã�"), _T("���棡"), MB_OK);
			m_operatingMode.SetCurSel(1);
			return;
		}
		while (m_ProcessLIST.DeleteItem(0));
		m_ChangeSize.EnableWindow(true);
		m_ProcessStop.EnableWindow(true);
		m_ProcessID.EnableWindow(true);
		m_operatingMode.EnableWindow(true);
		m_RePlay.EnableWindow(false);
		while (m_ProcessLIST.DeleteColumn(0));
		CRect rect;
		m_ProcessLIST.GetClientRect(&rect);
		m_ProcessLIST.SetExtendedStyle(m_ProcessLIST.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_ProcessLIST.InsertColumn(0, _T("ID"), LVCFMT_CENTER, rect.Width() / 2, 0);
		m_ProcessLIST.InsertColumn(1, _T("Size"), LVCFMT_CENTER, rect.Width() / 2, 1);
		KillTimer(1);
	}
}

// ִ��
void CDynamicMemoryManagementDlg::OnBnClickedExecute()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_operatingMode.GetCurSel() == 0 && m_ProcessID.GetCurSel() == 0) {
		CString str;
		m_ChangeSize.GetWindowTextW(str);
		if (str.IsEmpty() || 0 >= _ttoi(str)) {
			MessageBox(_T("��δ������Ҫ�������̿ռ��С"), _T("���棡"), MB_OK);
			return;
		}
		ProTmp.CreateProcess(_ttoi(str));
		if (!Mem.Allocate(ProTmp.getPSize(), m_SelectAlgorithm.GetCurSel(), ProTmp.getPID())) {
			MessageBox(_T("�ڴ�ռ䲻��"), _T("���棡"), MB_OK);
			ProTmp.decrease();
			return;
		}
		deqProcess.push_back(ProTmp);
		InsertProcessList(ProTmp.getPID(), ProTmp.getPSize());
		mapIDtoSize.insert(make_pair(ProTmp.getPID(), ProTmp.getPSize()));
		str.Format(_T("%d"), ProTmp.getPID());
		m_ProcessID.AddString(str);
	}
	
	if (m_operatingMode.GetCurSel() == 0 && m_ProcessID.GetCurSel() != 0) {
		CString str;
		m_ChangeSize.GetWindowTextW(str);
		int toChangeSize = _ttoi(str);
		if (str.IsEmpty() || 0 == toChangeSize) {
			MessageBox(_T("��δ������Ҫ�ı���̿ռ�Ĵ�С"), _T("���棡"), MB_OK);
			return;
		}

		CString CStringtoChangeID;
		((CComboBox*)GetDlgItem(IDC_ProcessID))->GetWindowText(CStringtoChangeID);
		int toChangeID = _ttoi(CStringtoChangeID);
		if (toChangeSize < 0) {
			toChangeSize = -toChangeSize;			
			if (mapIDtoSize[toChangeID] < toChangeSize){
				MessageBox(_T("�ͷŵĿռ����"), _T("���棡"), MB_OK);
				return;
			}
			else{

				// ���� Process
				if (mapIDtoSize[toChangeID] == toChangeSize){
					m_ProcessID.DeleteString(m_ProcessID.FindString(0, CStringtoChangeID));
				}
				for (int i = 0; i < deqProcess.size(); ++i) {
					if (deqProcess[i].getPID() == toChangeID) {
						if (deqProcess[i].getPSize() == toChangeSize) {
							DeleteProcessList(toChangeID);
							deqProcess.erase(deqProcess.begin() + i);
							mapIDtoSize[toChangeID] -= toChangeSize;
							break;
						}
						if (deqProcess[i].getPSize() < toChangeSize) {
							ChangeProcessList(toChangeID, -deqProcess[i].getPSize());
							deqProcess.erase(deqProcess.begin() + i);
							mapIDtoSize[toChangeID] -= deqProcess[i].getPSize();
							--i;
							continue;
						}
						ChangeProcessList(toChangeID, -toChangeSize);
						deqProcess[i].ChangeSize(-toChangeSize);
						mapIDtoSize[toChangeID] -= toChangeSize;
						break;
					}
				}
				// ���� deqMem
				Mem.ReleaseSpace(toChangeID, toChangeSize);
				if (mapIDtoSize[toChangeID] == 0) {
					mapIDtoSize.erase(mapIDtoSize.find(toChangeID));
				}
			}
		}
		else {
			if (!Mem.Allocate(toChangeSize, m_SelectAlgorithm.GetCurSel(), toChangeID)) {
				MessageBox(_T("�ڴ�ռ䲻��"), _T("���棡"), MB_OK);
				return;
			}
			int ans = -1;
			for (int i = 0; i < deqProcess.size(); ++i) {
				if (deqProcess[i].getPID() == toChangeID) {
					ans = i;
					break;
				}
			}
			if (ans == -1) {
				MessageBox(_T("���ҽ���IDʧ��"), _T("���棡"), MB_OK);
				return;
			}
			deqProcess[ans].ChangeSize(toChangeSize);
			mapIDtoSize[toChangeID] += toChangeSize;
			ChangeProcessList(deqProcess[ans].getPID(), toChangeSize);
		}
	}

	if (m_operatingMode.GetCurSel() == 1) {
		SetTimer(1, 1500, NULL);
		int totalSize = Mem.getMaxSize();
		int ChangeSize;
		srand((unsigned)(time(NULL)));
		if (deqProcess.size() == 0) {
			slcAlg = rand() % 4;
			m_SelectAlgorithm.SetCurSel(slcAlg);
		}
		while (true) {
			ChangeSize = 1 + rand() % 100;
			if (ChangeSize > totalSize){
				break;
			}
			else totalSize -= ChangeSize;
			int setTime = 1 + rand() % 10;
			ProTmp.CreateProcess(ChangeSize, setTime);
			if (!Mem.Allocate(ProTmp.getPSize(), m_SelectAlgorithm.GetCurSel(), ProTmp.getPID())) {
				ProTmp.decrease();
				break;
			}
			deqProcess.push_back(ProTmp);
			InsertProcessList(ProTmp.getPID(), ProTmp.getPSize(), setTime);
		}
		CString strTime;
		int timeTmp;
		int toChangeSize;
		for (int i = 0; i < m_ProcessLIST.GetItemCount(); ++i) {
			strTime = m_ProcessLIST.GetItemText(i, 2);
			timeTmp = _ttoi(strTime);
			--timeTmp;
			if (0 == timeTmp) {
				CString strID = m_ProcessLIST.GetItemText(i, 0);
				for (int j = 0; j < deqProcess.size(); ++j) {
					if (deqProcess[j].getPID() == _ttoi(strID)) {
						toChangeSize = deqProcess[j].getPSize();
						deqProcess.erase(deqProcess.begin() + j);
						getLogForAuto();
					}
				}
				Mem.ReleaseSpace(_ttoi(strID), toChangeSize);
				totalSize += toChangeSize;
				m_ProcessLIST.DeleteItem(i);
				--i;
				continue;
			}
			strTime.Format(_T("%d"), timeTmp);
			m_ProcessLIST.SetItemText(i, 2, strTime);
		}
	}
	Display(Mem.getMemStatus());
}


void CDynamicMemoryManagementDlg::OnCbnSelchangeProcessid()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDynamicMemoryManagementDlg::OnBnClickedDefrag()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Display(Mem.Compact());
}



void CDynamicMemoryManagementDlg::OnLvnItemchangedProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CString strLangName;       
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
	{
		// ��ȡ��ѡ���б����һ��������ı�   
		strLangName = m_ProcessLIST.GetItemText(pNMListView->iItem, 0);
		int nIndex = m_ProcessID.FindStringExact(0, strLangName);
		m_ProcessID.SetCurSel(nIndex);
	}

}

void CDynamicMemoryManagementDlg::InsertProcessList(int ID, int Size) {
	CString strID, strSize;
	strID.Format(_T("%d"), ID);
	strSize.Format(_T("%d"), Size);
	int pos = m_ProcessLIST.GetItemCount();
	m_ProcessLIST.InsertItem(pos, strID);
	m_ProcessLIST.SetItemText(pos, 1, strSize);
	getLog();
}

void CDynamicMemoryManagementDlg::InsertProcessList(int ID, int Size, int Time) {
	CString strID, strSize, strTime;
	strID.Format(_T("%d"), ID);
	strSize.Format(_T("%d"), Size);
	strTime.Format(_T("%d"), Time);
	int pos = m_ProcessLIST.GetItemCount();
	m_ProcessLIST.InsertItem(pos, strID);
	m_ProcessLIST.SetItemText(pos, 1, strSize);
	m_ProcessLIST.SetItemText(pos, 2, strTime);
	getLogForAuto();
}

void CDynamicMemoryManagementDlg::DeleteProcessList(int ID) {
	CString strID;
	strID.Format(_T("%d"), ID);
	int len = m_ProcessLIST.GetItemCount();
	for (int i = 0; i < len; ++i) {
		if (strID == m_ProcessLIST.GetItemText(i, 0)) {
			m_ProcessLIST.DeleteItem(i);
		}
	}
	getLog();
}

void CDynamicMemoryManagementDlg::ChangeProcessList(int ID, int Size) {
	CString strID, strSize;
	strID.Format(_T("%d"), ID);
	int sizeTmp;
	for (int i = 0; i < m_ProcessLIST.GetItemCount(); ++i) {
		if (strID == m_ProcessLIST.GetItemText(i, 0)) {
			strSize = m_ProcessLIST.GetItemText(i, 1);
			sizeTmp = _ttoi(strSize);
			strSize.Format(_T("%d"), sizeTmp + Size);
			m_ProcessLIST.SetItemText(i, 1, strSize);
		}
	}
	getLog();
}

void CDynamicMemoryManagementDlg::OnBnClickedProcessstop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString toStopID;
	m_ProcessID.GetWindowTextW(toStopID);
	if (toStopID == "����ID��...") {
		MessageBox(_T("��ѡ��ID"), _T("���棡"), MB_OK);
		return;
	}
	int toChangeID = _ttoi(toStopID), toChangeSize = mapIDtoSize[_ttoi(toStopID)];
	for (int i = 0; i < deqProcess.size(); ++i) {
		if (deqProcess[i].getPID() == toChangeID) {
			if (deqProcess[i].getPSize() == toChangeSize) {
				DeleteProcessList(toChangeID);
				deqProcess.erase(deqProcess.begin() + i);
				mapIDtoSize.erase(mapIDtoSize.find(toChangeID));
				break;
			}
		}
	}
	// ���� deqMem
	m_ProcessID.DeleteString(m_ProcessID.FindString(0, toStopID));
	Mem.ReleaseSpace(toChangeID, toChangeSize);
	Display(Mem.getMemStatus());
}
void CDynamicMemoryManagementDlg::Display(deque<Partition> deqMem) {
	CDC* pDC = this->GetDC();
	// ���
	CString currentPID;
	CClientDC dc(this);

	CPen penGreen(PS_SOLID, 1, RGB(0, 255, 0));
	CPen penRed(PS_SOLID, 1, RGB(255, 0, 0));

	for (int i = 0; i < deqMem.size(); ++i) {
		// �����ڴ�����б������Ϣ�����л�ͼչʾ���������ͼ��
		// �ڴ����û�б�ʹ��
		if (!deqMem[i].Status) {
			dc.SelectObject(&penGreen);
			dc.Rectangle(30 + deqMem[i].Address * 5, 145, 30 + deqMem[i].Address * 5 + deqMem[i].Size * 5, 320);
			CRect rt(30 + deqMem[i].Address * 5, 120, 30 + deqMem[i].Address * 5 + deqMem[i].Size * 5, 145);
			OnDraw(pDC, rt, 240, 240, 240);
		}
		else {
			// �ڴ�������ڱ�ʹ��
			dc.SelectObject(&penRed);
			currentPID.Format(_T("%d"), deqMem[i].ID);
			dc.Rectangle(30 + deqMem[i].Address * 5, 145, 30 + deqMem[i].Address * 5 + deqMem[i].Size * 5, 320);
			CRect rt(30 + deqMem[i].Address * 5, 120, 30 + deqMem[i].Address * 5 + deqMem[i].Size * 5, 145);
			OnDraw(pDC, rt, 240, 240, 240);
			SetTextColor(dc, RGB(0, 191, 255));
			SetBkMode(dc, TRANSPARENT);
			TextOut(dc, 30 + deqMem[i].Address * 5 + 0.4*(deqMem[i].Size * 5), 128, currentPID, currentPID.GetLength());
		}
	}
}
// ���ݽ��̶�̬�仯��־����չʾ
void CDynamicMemoryManagementDlg::DisplayProcessList(deque<ProcessMemVar> deqProcessVar) {
	// ��� �ؼ�List Control ԭ�����б���
	int len = m_ProcessLIST.GetItemCount();
	for (int i = len - 1; i >= 0; --i) {
		m_ProcessLIST.DeleteItem(i);
	}
	for (int i = 0; i < deqProcessVar.size(); ++i) {
		// �������б������Ϣ����ʾ�� �ؼ�List Control ��
		m_ProcessLIST.SetRedraw(FALSE);
		CString strID, strSize, strTime;
		strID.Format(_T("%d"), deqProcessVar[i].ID);
		strSize.Format(_T("%d"), deqProcessVar[i].Size);
		m_ProcessLIST.InsertItem(i, strID);
		m_ProcessLIST.SetItemText(i, 1, strSize);
		
		// ���ݽ��̲�����ʽ��ͬ���ؼ�List Control��ʾ���ݲ�ͬ
		if (deqProcessVar[i].selType == 1) {
			strTime.Format(_T("%d"), deqProcessVar[i].Time);
			m_ProcessLIST.SetItemText(i, 2, strTime);
		}
		// ˢ����ʾ �ؼ�List Control 
		m_ProcessLIST.SetRedraw(TRUE);
		m_ProcessLIST.Invalidate();
		m_ProcessLIST.UpdateWindow();
	}
}

void CDynamicMemoryManagementDlg::OnBnClickedReplay()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	string FileMemlog = Mem.getFileName();
	ifstream inFileMem(FileMemlog);

	ifstream inFileProcess(FileProcessLog);
	int listType;
	int changeNum, changeNumMem, changeNumProcess;
	deque<ProcessMemVar> deqTmpProcess;
	ProcessMemVar Prc;
	deque<Partition> deqTmp;
	Partition Ptn;
	while (inFileProcess >> listType) {
		inFileMem >> changeNumMem;
		changeNum = changeNumMem;
		for (int i = 0; i < changeNum; ++i) {
			inFileMem >> Ptn.ID;
			inFileMem >> Ptn.Address;
			inFileMem >> Ptn.Size;
			inFileMem >> Ptn.Status;
			deqTmp.push_back(Ptn);
		}
		Display(deqTmp);
		deqTmp.clear();
		
		inFileProcess >> changeNumProcess;
		changeNum = changeNumProcess;
		for (int i = 0; i < changeNum; ++i) {
			inFileProcess >> Prc.ID;
			inFileProcess >> Prc.Size;
			if (listType) inFileProcess >> Prc.Time;
			Prc.selType = listType;
			deqTmpProcess.push_back(Prc);
		}
		DisplayProcessList(deqTmpProcess);
		deqTmpProcess.clear();
		Sleep(1500);
	}
}


void CDynamicMemoryManagementDlg::OnBnClickedReset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Mem.Distroy();
	deqProcess.clear();
	if (m_operatingMode.GetCurSel() == 1) {
		getLogForAuto();
	}else getLog();

	mapIDtoSize.clear();
	// ProcessList
	int len = m_ProcessLIST.GetItemCount();
	
	for (int i = len - 1; i >= 0; --i) {
		CString strID = m_ProcessLIST.GetItemText(i, 0);
		m_ProcessID.DeleteString(m_ProcessID.FindString(0, strID));
		m_ProcessLIST.DeleteItem(i);
	}
	m_operatingMode.SetCurSel(0);
	m_SelectAlgorithm.SetCurSel(0);
	m_ProcessID.SetCurSel(0);
	SetDlgItemText(IDC_ChangeSize, _T(""));
	KillTimer(1);
	m_ChangeSize.EnableWindow(true);
	m_ProcessStop.EnableWindow(true);
	m_ProcessID.EnableWindow(true);
	m_operatingMode.EnableWindow(true);
	m_RePlay.EnableWindow(true);
	Display(Mem.getMemStatus());
}




void CDynamicMemoryManagementDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent) {
	case 1:{
		OnBnClickedExecute();
		break;
	}
	case 2:{
		break;
	}

	}
	
	CDialogEx::OnTimer(nIDEvent);
}


//������̣��ֹ����룩��̬�仯��־
void CDynamicMemoryManagementDlg::getLog(){
	outFile << m_operatingMode.GetCurSel() << endl; // ��¼���̲�����ʽ
	outFile << deqProcess.size() << endl; // ��¼������Ŀ
	for (int i = 0; i < deqProcess.size(); ++i){
		// ��¼���̱�ʶ���Լ�ռ�ÿռ��С
		outFile << deqProcess[i].getPID() << " " << deqProcess[i].getPSize() << endl;
	}
}
//������̣��Զ���������̬�仯��־
void CDynamicMemoryManagementDlg::getLogForAuto(){
	outFile << m_operatingMode.GetCurSel() << endl; 
	outFile << deqProcess.size() << endl;
	for (int i = 0; i < deqProcess.size(); ++i){
		// ��¼���̱�ʶ��ռ�ÿռ��С���Լ�����ִ��ʱ��
		outFile << deqProcess[i].getPID() << " " << deqProcess[i].getPSize()
			<< " " << deqProcess[i].getPTime() << endl;
	}
}