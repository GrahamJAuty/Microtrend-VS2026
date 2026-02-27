/**********************************************************************/
#include "AutoBackupOptions.h"
/**********************************************************************/
#include "PropertySheetBackup.h"
/**********************************************************************/


CPropertySheetBackup::CPropertySheetBackup( int nMinutes, CWnd* pWndParent)
	 : CSSAutoShutdownPropertySheet(AutoShutdownHelper,"", pWndParent)
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	m_PropPageMode.SetPropertySheet( this );
	m_PropPageMode.SetMinutes( nMinutes );

	for (int nIndex = 0; nIndex < 7; nIndex++)
	{
		m_PageArray[nIndex].m_psp.dwFlags |= PSP_USETITLE;
	}
	
	AddPage( &m_PropPageMode );
	AddDayPages( nMinutes );

	SetTitle( "Custom Backup Schedule" );
	SetActivePage(1);
}

/**********************************************************************/

void CPropertySheetBackup::AddDayPages( int nMinutes )
{
	for (int n = GetPageCount() - 1; n > 0; n--)
	{
		RemovePage(n);
	}

	switch ( AutoBackupOptions.GetCustomMode() )
	{
	case BACKUP_MODE_MON_FRI:
		m_PageArray[0].m_psp.pszTitle = "Mon - Fri";
		m_PageArray[1].m_psp.pszTitle = "Sat";
		m_PageArray[2].m_psp.pszTitle = "Sun";
		m_PageArray[0].SetDayFlags( DAYFLAGS_MON_FRI );
		m_PageArray[1].SetDayFlags( DAYFLAGS_SAT );
		m_PageArray[2].SetDayFlags( DAYFLAGS_SUN );
		m_nPageCount = 3;
		break;

	case BACKUP_MODE_MON_SAT:
		m_PageArray[0].m_psp.pszTitle = "Mon - Sat";
		m_PageArray[1].m_psp.pszTitle = "Sun";
		m_PageArray[0].SetDayFlags( DAYFLAGS_MON_SAT );
		m_PageArray[1].SetDayFlags( DAYFLAGS_SUN );
		m_nPageCount = 2;
		break;

	case BACKUP_MODE_EVERYDAY:
		m_PageArray[0].m_psp.pszTitle = "Everyday";
		m_PageArray[0].SetDayFlags( DAYFLAGS_EVERYDAY );
		m_nPageCount = 1;
		break;

	case BACKUP_MODE_INDIVIDUAL:
	default:
		m_PageArray[0].m_psp.pszTitle = "Mon";
		m_PageArray[1].m_psp.pszTitle = "Tue";
		m_PageArray[2].m_psp.pszTitle = "Wed";
		m_PageArray[3].m_psp.pszTitle = "Thur";
		m_PageArray[4].m_psp.pszTitle = "Fri";
		m_PageArray[5].m_psp.pszTitle = "Sat";
		m_PageArray[6].m_psp.pszTitle = "Sun";
		m_PageArray[0].SetDayFlags( DAYFLAGS_MON );
		m_PageArray[1].SetDayFlags( DAYFLAGS_TUE );
		m_PageArray[2].SetDayFlags( DAYFLAGS_WED );
		m_PageArray[3].SetDayFlags( DAYFLAGS_THU );
		m_PageArray[4].SetDayFlags( DAYFLAGS_FRI );
		m_PageArray[5].SetDayFlags( DAYFLAGS_SAT );
		m_PageArray[6].SetDayFlags( DAYFLAGS_SUN );
		m_nPageCount = 7;
		break;
	}

	for ( int nIndex = 0; nIndex < m_nPageCount; nIndex++ )
	{
		m_PageArray[nIndex].SetMinutes( nMinutes );
		AddPage( &m_PageArray[nIndex] );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetBackup, CSSAutoShutdownPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetBackup)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
