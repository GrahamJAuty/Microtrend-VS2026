#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CDatabaseValidationDlg : public CDialog
{
public:
	CDatabaseValidationDlg( bool bFullValidation, CWnd* pParent = NULL);

	//{{AFX_DATA(CDatabaseValidationDlg)
	enum { IDD = IDD_DATABASE_VALIDATION };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDatabaseValidationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	CStatic	m_staticInfo;
	CStatic	m_staticInfo2;
	CProgressCtrl m_Progress;
	//}}AFX_VIRTUAL

private:
	void SetInfoText( int nDbIdx, int nType );

protected:
	//{{AFX_MSG(CDatabaseValidationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg long RunValidation( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void PumpWaitingMessages();

private:
	void SetProgressPos ( __int64 nProgress, __int64 nTarget );
	void SetProgressPos ( __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy );
	void SetProgressPos ( CSSFile& file );

private:
	bool RunInternal( int nDbIdx );
	bool ValidateLocationPluFiles( int nNwkIdx );
	bool ValidatePluFile( CString& strFilename, __int64& nProgress, __int64 nTarget, int nContext, int nEntityNo1, int nEntityNo2 );
	
#ifdef STOCKMAN_SYSTEM
	bool ValidateStockFile( CString& strFilename, __int64& nProgress, __int64 nTarget, int nContext, int nEntityNo1, int nEntityNo2 );
#endif

private:
	void LogError( int nDbNo, int nSpNo, int nFileType, int nError, int nIndex, const char* szFilename );
	
private:
	bool m_bFullValidation;
	int m_nProgressType;
	int m_nProgressPart;
	int m_nProgressPos;

public:
	bool m_bValidAtStartup;
	bool m_bCancelAtStartup;
	CString m_strError;
};

/**********************************************************************/
#endif
/**********************************************************************/
