#pragma once
/*************************************************************************/
#include "Resource.h"
/*************************************************************************/
#define BACKUP_PROGRESS_BACKUP 1
#define BACKUP_PROGRESS_RESTORE 2
#define BACKUP_PROGRESS_DELETEFILE 3
#define BACKUP_PROGRESS_COUNTFILE 4
#define BACKUP_PROGRESS_BACKUPSOFAR 5
#define BACKUP_PROGRESS_RESTORESOFAR 6
#define BACKUP_PROGRESS_FINALISE 7
/*************************************************************************/

class CBackupProgressDlg : public CDialog
{
public:
	CBackupProgressDlg();   
	~CBackupProgressDlg();

	bool CreateOnMainWnd( bool bRestore );
		
	void SetText( int nType, __int64 nPart );
	void SetText( int nType, __int64 nPart, __int64 nTarget );
	void SetPos ( __int64 nProgress, __int64 nTarget );
	void SetPos ( __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy );
	void SetPos ( CSSFile& file );
	
	//{{AFX_DATA(CBackupProgressDlg)
	enum { IDD = IDD_BACKUP_PROGRESS };
	CStatic	m_staticInfo1A;
	CStatic	m_staticInfo1B;
	CProgressCtrl m_Progress1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBackupProgressDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	void SetText( CStatic& staticA, CStatic& staticB, CProgressCtrl& progress, int& nProgressType, __int64& nProgressPart, int nType, __int64 nPart, __int64 nTarget );
	void SetPos( CProgressCtrl& progress, int& nProgressPos, __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy );
	void SetPos( CProgressCtrl& progress, int& nProgressPos, __int64 nProgress, __int64 nTarget );
	void SetPos( CProgressCtrl& progress, int& nProgressPos, CSSFile& file );

protected:
	virtual void OnCancel(){};
	virtual void OnOK(){}; 
	void PumpMessages();

	//{{AFX_MSG(CBackupProgressDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nProgressPos1;
	int m_nProgressType1;
	__int64 m_nProgressPart1;
	bool m_bRestore;
};

/*********************************************************************/

