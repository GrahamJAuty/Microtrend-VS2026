/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropertySheetOptions.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetOptions, CPropertySheet)
/**********************************************************************/

CPropertySheetOptions::CPropertySheetOptions( bool bSchedulerFinished, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_PageFeatures.SetMainWnd( pWndParent );

	m_psh.dwFlags |= PSH_NOAPPLYNOW;

#ifdef STOCKMAN_SYSTEM
	
	AddPage( &m_PagePlu );
	AddPage( &m_PageStock1 );
	AddPage( &m_PageStock2 );

	if ( SysInfo.GetMaxDatabases() > 1 )
		AddPage( &m_PageGlobals );

	AddPage( &m_PageFeatures );
	AddPage( &m_PageReports1 );
	AddPage( &m_PageReports2 );
	AddPage( &m_PageReports3 );
	AddPage( &m_PageBackup );
	AddPage( &m_PageExpire);

	if ( Sysset.IsBarmanSystem() == TRUE ) 
	{
		AddPage( &m_PageLabels1 );
		AddPage( &m_PageLabels2 );
	}

	AddPage( &m_PageEmail);
	
#else
	
	AddPage( &m_PagePlu );
	
	if ( SysInfo.GetMaxDatabases() > 1 )
		AddPage( &m_PageGlobals );

	AddPage( &m_PageFeatures );
	AddPage( &m_PageReports1 );
	AddPage( &m_PageReports2 );
	AddPage( &m_PageReports3 );
	AddPage( &m_PageBackup );
	AddPage( &m_PageExpire);

	if ( Sysset.IsBarmanSystem() == TRUE ) 
	{
		AddPage( &m_PageLabels1 );
		AddPage( &m_PageLabels2 );
	}

	AddPage( &m_PageEmail);
	
#endif

	SetTitle ( "Options" );
}

/**********************************************************************/

CPropertySheetOptions::~CPropertySheetOptions()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetOptions, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetOptions)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

