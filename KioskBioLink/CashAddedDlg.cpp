// CashAddedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SystemData.h"
#include "CardLink.h"
#include "CashAddedDlg.h"

// CCashAddedDlg dialog

#define	ID_TIMER 2

extern CSystemData			System;

//*******************************************************************

//IMPLEMENT_DYNAMIC(CCashAddedDlg, CDialog)

CCashAddedDlg::CCashAddedDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog()
	, m_dCashAdded(0)
{
	CDialog::Create(CCashAddedDlg::IDD, pParent);		// display dialog
	CenterWindow();

	m_bCancelRequested = FALSE;
}

CCashAddedDlg::~CCashAddedDlg()
{
	DestroyWindow();
}

void CCashAddedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CASHADDED, m_dCashAdded);
}

BEGIN_MESSAGE_MAP(CCashAddedDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//*******************************************************************
// CCashAddedDlg message handlers

BOOL CCashAddedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_CASHADDED,  SS_NUM_SDP,   11, "%.*f", System.m_nDPValue );

	StartTimer();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//**********************************************************************

void CCashAddedDlg::OnOK() 
{
	m_bCancelRequested = TRUE;						// set cancel has been pressed
}

//*******************************************************************

void CCashAddedDlg::StartTimer()
{
	m_hTimer = SetTimer( ID_TIMER, 500, NULL );
}

void CCashAddedDlg::StopTimer()
{
	if ( m_hTimer != 0 )
		KillTimer ( m_hTimer );

	m_hTimer = 0;
}

//**********************************************************************

void CCashAddedDlg::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == m_hTimer )
	{
		StopTimer();

		if ( m_bCancelRequested == TRUE )				// see if cancel was pressed during wait for card
		{
			CDialog::OnOK();
			return;
		}

		CheckForCash();

		StartTimer();
	}

	CDialog::OnTimer(nIDEvent);
}

void CCashAddedDlg::CheckForCash() 
{
	m_dCashAdded += 0.05;

	WaitLoop(20);
	Sleep(80);

	UpdateData(FALSE);
}
