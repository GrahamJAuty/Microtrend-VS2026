#pragma once
/*************************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define PROGRESSINFO_MODE_GENERAL 2
/*************************************************************************/

class CProgressInfoDlg : public CDialog
{
public:
	CProgressInfoDlg();   
	~CProgressInfoDlg();

	bool CreateForGeneralUse();		
	void SetGeneralInfoText( const char* sz1, const char* sz2 );

	void SetPos ( __int64 nProgress, __int64 nTarget );
	void SetPos ( __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy );
	void SetPos ( CSSFile& file );
	
	//{{AFX_DATA(CProgressInfoDlg)
	enum { IDD = IDD_PROGRESS_INFO };
	CStatic	m_staticInfo2;
	CStatic	m_staticInfo;
	CProgressCtrl m_Progress;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProgressInfoDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	virtual void OnCancel(){};
	virtual void OnOK(){}; 
	void PumpMessages();

	//{{AFX_MSG(CProgressInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nMode;
	int m_nProgressPos;
	int m_nProgressType;
	int m_nProgressPart;
};

/*********************************************************************/
