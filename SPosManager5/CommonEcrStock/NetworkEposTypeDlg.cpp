/**********************************************************************/
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "NetworkEposTypeDlg.h"
/**********************************************************************/

CNetworkEposTypeDlg::CNetworkEposTypeDlg( int nSPOSVersion, CNetworkSPOSVersionArray& arraySPOSVersion, CWnd* pParent)
: CDialog(CNetworkEposTypeDlg::IDD, pParent), m_arraySPOSVersion( arraySPOSVersion )
{
	m_nActiveTab = 0;
	m_nSPOSVersion = nSPOSVersion;
	AddTabDetails(0,14);
	AddTabDetails(10,14);
	AddTabDetails(20,14);
	AddTabDetails(30,14);
	AddTabDetails(40,14);
	AddTabDetails(50,6);
}

/**********************************************************************/

void CNetworkEposTypeDlg::AddTabDetails( int nStartIdx, int nListSize )
{
	CNetworkEposTabDetails info;
	info.m_nStartIdx = nStartIdx;
	info.m_nListSize = nListSize;
	m_arrayTabDetails.Add( info );
}

/**********************************************************************/

CNetworkEposTypeDlg::~CNetworkEposTypeDlg()
{
}

/**********************************************************************/

void CNetworkEposTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_TYPE, m_TabType);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CNetworkEposTypeDlg, CDialog)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TYPE, OnTcnSelchangeTabs)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CNetworkEposTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TabType.InsertItem(0, "SPos -> V4.1.827");
	m_TabType.InsertItem(1, "SPos -> V4.1.1216");
	m_TabType.InsertItem(2, "SPos -> V4.1.2012");
	m_TabType.InsertItem(3, "SPos -> V4.2.959");
	m_TabType.InsertItem(4, "SPos -> V4.3.37");
	m_TabType.InsertItem(5, "SPos -> V4.3.104");

	m_listData.SubclassDlgItem(IDC_LIST_TYPE, this);
	m_listData.InsertColumn(0, "Version", LVCFMT_LEFT, 100);
	m_listData.InsertColumn(1, "Features", LVCFMT_LEFT, 390);
	m_listData.InsertColumn(2, "CashRSP", LVCFMT_LEFT, 60);
	m_listData.ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0);
	m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_nArrayPos = 0;
	for (int n = 0; n < m_arraySPOSVersion.GetSize(); n++)
	{
		CNetworkSPOSVersionInfo infoVersion;
		m_arraySPOSVersion.GetAt(n, infoVersion);

		if (m_nSPOSVersion == infoVersion.m_nSPOSVersion)
		{
			m_nArrayPos = n;
			break;
		}
	}

	m_nActiveTab = 0;

	for (int nTab = m_arrayTabDetails.GetSize() - 1; nTab >= 0; nTab--)
	{
		if (m_nArrayPos >= m_arrayTabDetails[nTab].m_nStartIdx)
		{
			m_nActiveTab = nTab;
			break;
		}
	}

	m_TabType.SetCurSel(m_nActiveTab);
	AddList();

	m_staticInfo.SetWindowText("Compatible with SPOS up to V4.3.106 and CashRSP V44");

	return FALSE;
}

/**********************************************************************/

void CNetworkEposTypeDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST_TYPE )  
   {
	   m_listData.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CNetworkEposTypeDlg::OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	int nSel = m_listData.GetCurSel();

	if (nSel >= 0)
	{
		int nPos = m_arrayTabDetails[m_nActiveTab].m_nStartIdx + nSel;
		if ((nPos > 0) && (nPos < m_arraySPOSVersion.GetSize()))
		{
			m_nArrayPos = nPos;
		}
	}

	m_nActiveTab = m_TabType.GetCurSel();
	if ((m_nActiveTab < 0) || (m_nActiveTab >= m_arrayTabDetails.GetSize()))
	{
		m_nActiveTab = 0;
	}

	AddList();
}

/**********************************************************************/

void CNetworkEposTypeDlg::AddList()
{
	m_listData.DeleteAllItems();

	int nStartIdx = m_arrayTabDetails[m_nActiveTab].m_nStartIdx;

	for (int n = 0; n < m_arrayTabDetails[m_nActiveTab].m_nListSize; n++)
	{
		CNetworkSPOSVersionInfo infoType;
		m_arraySPOSVersion.GetAt(nStartIdx + n, infoType);

		m_listData.InsertItem(n, "");
		m_listData.SetItem(n, 0, LVIF_TEXT, infoType.m_strVersion, 0, 0, 0, NULL);
		m_listData.SetItem(n, 1, LVIF_TEXT, infoType.m_strFeatures, 0, 0, 0, NULL);
		m_listData.SetItem(n, 2, LVIF_TEXT, infoType.m_strCashRSP, 0, 0, 0, NULL);
	}

	int nListSel = m_nArrayPos - m_arrayTabDetails[m_nActiveTab].m_nStartIdx;
	if ((nListSel < 0) || (nListSel >= m_arrayTabDetails[m_nActiveTab].m_nListSize))
	{
		nListSel = -1;
	}

	POSITION pos = m_listData.GetFirstSelectedItemPosition();
	while (pos)
	{
		int iItem = m_listData.GetNextSelectedItem(pos);
		m_listData.SetItemState(iItem, 0, LVIS_SELECTED);
	}

	if (nListSel != -1)
	{
		m_listData.SetCurSel(nListSel);
		m_listData.SetFocus();
		m_listData.Invalidate();
	}
}

/**********************************************************************/

void CNetworkEposTypeDlg::OnOK() 
{
	int nSel = m_listData.GetCurSel();
	
	if ( nSel >= 0 )
	{
		int nPos = m_arrayTabDetails[ m_nActiveTab ].m_nStartIdx + nSel;
		if ( ( nPos >= 0 ) && ( nPos < m_arraySPOSVersion.GetSize() ) )
		{
			CNetworkSPOSVersionInfo infoType;
			m_arraySPOSVersion.GetAt( nPos, infoType );
			m_nSPOSVersion = infoType.m_nSPOSVersion;
		}
	}
	
	EndDialog( IDOK );
}

/**********************************************************************/
