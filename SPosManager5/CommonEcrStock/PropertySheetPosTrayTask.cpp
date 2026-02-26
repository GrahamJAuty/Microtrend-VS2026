/**********************************************************************/
#include "PropertySheetPosTrayTask.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPosTrayTask, CPropertySheet)
/**********************************************************************/

CPropertySheetPosTrayTask::CPropertySheetPosTrayTask( CPosTrayFileSet& FileSet, CPosTrayTask& task, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent), m_PosTrayFileSet( FileSet), m_Task( task )
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_Page1.SetEmailTask( &m_Task );
	m_Page1.SetPosTrayFileSet( &m_PosTrayFileSet );
	m_Page2.SetEmailTask( &m_Task );
	
	if (m_PosTrayFileSet.m_nTaskFileType != POSTRAY_TASKFILE_PROCESSTRAN_TASK)
	{
		AddPage(&m_Page1);
	}
	
	AddPage( &m_Page2 );

	switch( m_PosTrayFileSet.m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_PROCESSTRAN_TASK:
		SetTitle("Process Pending CashRSP Transactions");
		break;

	case POSTRAY_TASKFILE_TIMEATTEND_TASK:
		SetTitle("Configure Time & Attendance Report Task for PosTray");
		break;

	case POSTRAY_TASKFILE_HIST_TASK:
		SetTitle( "Configure Sales History Report Task for PosTray" );
		break;

	case POSTRAY_TASKFILE_STKLEVEL_TASK:
		SetTitle( "Configure Stock Level Report Task for PosTray" );
		break;

	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		SetTitle( "Configure CSV Sales Export Task for PosTray" );
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		SetTitle( "Configure CSV Payment Export Task for PosTray" );
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		SetTitle( "Configure CSV Void Export Task for PosTray" );
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		SetTitle( "Configure CSV Discount Export Task for PosTray" );
		break;

	case POSTRAY_TASKFILE_EPOS_TASK:
	default:
		SetTitle( "Configure EPOS Report Task for PosTray" );
		break;
	}
}

/**********************************************************************/

CPropertySheetPosTrayTask::~CPropertySheetPosTrayTask()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPosTrayTask, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPosTrayTask)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropertySheetPosTrayTask::OnInitDialog()
{
	CPropertySheet::OnInitDialog();

	if (POSTRAY_TASKFILE_EPOS_TASK != m_PosTrayFileSet.m_nTaskFileType)
	{
		RECT rc;

		// resize the sheet
		GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.bottom -= 30;
		MoveWindow(&rc);

		// resize the CTabCtrl
		CTabCtrl* pTab = GetTabControl();
		ASSERT(pTab);
		pTab->GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.bottom -= 30;
		pTab->MoveWindow(&rc);

		// resize the page
		CPropertyPage* pPage = GetActivePage();
		ASSERT(pPage);
		// store page size in m_PageRect
		pPage->GetWindowRect(&m_PageRect);
		ScreenToClient(&m_PageRect);
		m_PageRect.bottom -= 30;
		pPage->MoveWindow(&m_PageRect);

		// move the OK, Cancel, and Apply buttons
		CWnd* pWnd = GetDlgItem(IDOK);
		pWnd->GetWindowRect(&rc);
		rc.top -= 30;
		rc.bottom -= 30;
		ScreenToClient(&rc);
		pWnd->MoveWindow(&rc);

		pWnd = GetDlgItem(IDCANCEL);
		pWnd->GetWindowRect(&rc);
		rc.top -= 30;
		rc.bottom -= 30;
		ScreenToClient(&rc);
		pWnd->MoveWindow(&rc);
	}

	CenterWindow();
	return TRUE;
}

/**********************************************************************/
