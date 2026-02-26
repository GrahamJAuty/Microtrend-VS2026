#pragma once

//*******************************************************************
// CPhotoIDListDlg dialog

class CPhotoIDListDlg : public CDialog
{
public:
	CPhotoIDListDlg(CWnd* pParent = NULL);   // standard constructor
//	virtual ~CPhotoIDListDlg();

// Dialog Data
	enum { IDD = IDD_PHOTOIDLIST };
	CButton	m_buttonDisplay;
	CButton	m_buttonPrint;
	CSSComboBox m_comboLabel;
	BOOL m_bSkipNoPhoto;
	CString	m_strLabelName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDisplay();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnSelchangeComboLabels();

	DECLARE_MESSAGE_MAP()

public:
	bool m_bPrintReqd;
};
