#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSRoomCSVArray.h"
/**********************************************************************/

class CPMSBookEditExtendDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSBookEditExtendDlg)

public:
	CPMSBookEditExtendDlg( int nRoomIdx, CWnd* pParent = NULL);   
	virtual ~CPMSBookEditExtendDlg();

public:
	void SetMaxNights( int n ){ m_nMaxNights = n; }

private:
	CStatic m_staticMaxNights;
	CEdit* GetEditNights();

public:
	enum { IDD = IDD_PMS_BOOK_EDIT_EXTEND };

public:
	virtual BOOL OnInitDialog();
		
private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	int m_nMaxNights;
	int m_nNights;

private:
	CPMSRoomCSVRecord m_Room;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
