
// MICDlg.h : header file
//

#pragma once

#include "Mmsystem.h"
#include <string>
#include <vector>

// CMICDlg dialog
class CMICDlg : public CDialogEx
{
// Construction
private:
	void zagrajmi();

	int aktualny_element;
	std::string title;
	int tempo;
	int oktawa;
	std::vector <float> freq;
	std::vector <int> BUFFER_SIZE;

public:
	CMICDlg(CWnd* pParent = NULL);	// standard constructor

public:
	HWAVEOUT outHande;


	WAVEHDR outBuffer;
	unsigned char *outBufAudio;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButmicOutopen();
	afx_msg void OnBnClickedButmicOutclose();
	afx_msg void OnBnClickedButmicOutplay();
	afx_msg LRESULT OnOutDone(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButmicOutstop();
	CString Path;
	afx_msg void OnEnChangeEdit1();
};

