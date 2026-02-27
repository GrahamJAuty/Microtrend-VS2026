#include "AtcArchiveDlg.h"


// CAtcArchiveDlg dialog

CAtcArchiveDlg::CAtcArchiveDlg( const char* szFilename, const char* szTitle, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CAtcArchiveDlg::IDD, pParent)
	, m_datelist(szFilename)
	, m_strTitle(szTitle)
{
	m_strSelDate = "";
	m_strFirstDate = "";
	ClearTotals();
}

void CAtcArchiveDlg::ClearTotals()
{
	m_nDayCount = 0;
	m_nLinesOut = 0;
	m_nDaysOut = 0;
	m_nEntries = 0;
}

CAtcArchiveDlg::~CAtcArchiveDlg()
{
}

void CAtcArchiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ATCARCHIVECOUNT, m_nDayCount);
	DDX_Text(pDX, IDC_EDIT_ATCARCHIVELINES, m_nLinesOut);
	DDX_Text(pDX, IDC_EDIT_ATCARCHIVEDATES, m_nDaysOut);
	DDX_Text(pDX, IDC_EDIT_ATCARCHIVEENTRIES, m_nEntries);
}

BEGIN_MESSAGE_MAP(CAtcArchiveDlg, CSSAutoShutdownDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
END_MESSAGE_MAP()


// CAtcArchiveDlg message handlers

BOOL CAtcArchiveDlg::CSSAutoShutdownPostInitDialog()
{
	m_listboxDate.SubclassDlgItem ( IDC_LIST1, this );
	m_listboxDate.InsertColumn ( 0, "Date", LVCFMT_LEFT, 72 );
	m_listboxDate.InsertColumn ( 1, "Entries", LVCFMT_RIGHT, 60 );

	ClearTotals();

	if ( ( m_nDayCount = m_datelist.GetSize() ) > 0 )
	{
		m_strFirstDate = m_datelist.GetAtDate(0);		// first date in list

		for ( int i = 0 ; i < m_nDayCount ; i++ )
		{
			CString strMess;
			strMess.Format ( "%s\t%d", m_datelist.GetAtDate(i), m_datelist.GetAtCount(i) );
			m_listboxDate.AddString ( strMess );

			m_nEntries += m_datelist.GetAtCount(i);
		}
	}
	UpdateData ( FALSE );

	return TRUE;
}

//*******************************************************************

void CAtcArchiveDlg::OnOK()
{
	if ( m_nDaysOut == 0 && m_nLinesOut == 0 )
		Prompter.Error( "No lines have been selected", m_strTitle );
	else
	{
		int nCurSel;
		if ( ( nCurSel = m_listboxDate.GetCurSel() ) != LB_ERR )
			m_strSelDate = m_datelist.GetAtDate(nCurSel);

		EndDialog(IDOK);
	}
}

void CAtcArchiveDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnOK();
	*pResult = 0;
}

void CAtcArchiveDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_nLinesOut = 0;
	m_nDaysOut = 0;

	int nCurSel;
	if ( ( nCurSel = m_listboxDate.GetCurSel() ) >= 0 )
	{
		m_nDaysOut = nCurSel + 1;
		for ( int i = 0 ; i <= nCurSel ; i++ )
			m_nLinesOut += m_datelist.GetAtCount(i);
	}

	UpdateData ( FALSE );	
	*pResult = 0;
}

//*******************************************************************

void CAtcArchiveDlg::RemoveEntries ( CSSDate& date )
{
	m_datelist.RemoveEntries ( date );
}

//*******************************************************************
