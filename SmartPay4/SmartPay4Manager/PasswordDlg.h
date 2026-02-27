/**********************************************************************/
#pragma once
/**********************************************************************/

class CPasswordDlg : public CSSAutoShutdownDialog
{
public:
	CPasswordDlg(CWnd* pParent = NULL);   
	virtual ~CPasswordDlg();

	enum { IDD = IDD_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()

public:
	CString m_strPassword;
};

/**********************************************************************/
