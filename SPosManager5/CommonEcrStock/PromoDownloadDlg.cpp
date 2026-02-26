/**********************************************************************/
 
/**********************************************************************/
#include "PromoDownloadDlg.h"
/**********************************************************************/

CPromoDownloadDlg::CPromoDownloadDlg(CWnd* pParent /*=NULL*/)
	: CReportDataTableDlg( pParent)
{
	m_nDownloadType = 1;
	m_strTitle = "Select Download Type";
	m_arrayReports.Add( "Promotions Only" );
	m_arrayReports.Add( "Full Mix Match" );
}

/**********************************************************************/

void CPromoDownloadDlg::HandleReport1()
{
	m_nDownloadType = 1;
	EndDialog( IDOK );
}

/**********************************************************************/

void CPromoDownloadDlg::HandleReport2()
{
	m_nDownloadType = 2;
	EndDialog( IDOK );
}

/**********************************************************************/
