/**********************************************************************/
#include "PosTrayFilenameTimeAttendDlg.h"
/**********************************************************************/

CPosTrayFilenameTimeAttendDlg::CPosTrayFilenameTimeAttendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPosTrayFilenameTimeAttendDlg::IDD, pParent)
{
	for (int nRepIdx = 0; nRepIdx < DataManagerNonDb.TimeAttendReportNames.GetSize(); nRepIdx++)
	{
		CTimeAttendReportNameCSVRecord record;
		DataManagerNonDb.TimeAttendReportNames.GetAt(nRepIdx, record);

		CellTypeInfo info;
		info.m_nType = CELL_TYPE_EDIT_TEXT;
		info.m_nSSEditType = 0xFFFF;
		info.m_nSSEditWidth = EPOS_MAXLEN_EXPORT_FILENAME;
		info.m_strSSEditFormat = "";
		info.m_nMinInt = 0;
		info.m_nMaxInt = 0;
		info.m_strCurrent = record.GetReportFilename();

		::TrimSpaces(info.m_strCurrent, FALSE);
		if (info.m_strCurrent == "")
		{
			info.m_strCurrent.Format("TA%3.3d", record.GetReportNo());
		}

		m_arrayCellTypes.Add(info);
	}
}

/**********************************************************************/

CPosTrayFilenameTimeAttendDlg::~CPosTrayFilenameTimeAttendDlg()
{
}

/**********************************************************************/

void CPosTrayFilenameTimeAttendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CPosTrayFilenameTimeAttendDlg, CDialog)
	ON_WM_MEASUREITEM()
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CPosTrayFilenameTimeAttendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText("PosTray Export Filenames (Time & Attendance)");

	m_listReports.SubclassDlgItem ( IDC_LIST, this );
	m_listReports.InsertColumn ( 0, "Report Name", LVCFMT_LEFT, 230 );
	m_listReports.InsertColumn ( 1, "Export Filename", LVCFMT_LEFT, 230 );
	m_listReports.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	for (int nRepIdx = 0; nRepIdx < DataManagerNonDb.TimeAttendReportNames.GetSize(); nRepIdx++)
	{
		CTimeAttendReportNameCSVRecord record;
		DataManagerNonDb.TimeAttendReportNames.GetAt(nRepIdx, record);

		CString strReportName = "";

		switch (record.GetReportNo())
		{
		case 1:
			strReportName = "Detailed report";
			break;

		case 2:
			strReportName = "Export (shifts only)";
			break;

		case 3:
			strReportName = "Export (with breaks)";
			break;

		case 4:
			strReportName = "Detailed report + Estimated Pay";
			break;

		case 5:
			strReportName = "Estimated Pay Summary";
			break;
		}

		CString strFilename = m_arrayCellTypes[ nRepIdx ].m_strCurrent;
		
		int nPos = m_listReports.GetItemCount();
		m_listReports.InsertItem ( nPos, "" );
		m_listReports.SetItem ( nPos, 0, LVIF_TEXT, strReportName, 0, 0, 0, NULL );
		m_listReports.SetItem ( nPos, 1, LVIF_TEXT, strFilename, 0, 0, 0, NULL );
	}

	return TRUE;  
}

/**********************************************************************/

void CPosTrayFilenameTimeAttendDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )    
   {	
	   m_listReports.AdjustRowHeight( lpMeasureItemStruct, 10 ); 	
   }
}

/**********************************************************************/

BOOL CPosTrayFilenameTimeAttendDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (GetFocus() == &m_listReports)
			{
				m_listReports.HandleReturn(1);
				pMsg->wParam = NULL;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

LRESULT CPosTrayFilenameTimeAttendDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam)
{
	m_listReports.AllowListScroll( TRUE );
	m_listReports.SetEditCell( -1, -1 );

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	
	if (wParam == IDC_LIST)
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;  
	
		int nRow = pDispInfo -> item.iItem;
		int nCol = pDispInfo -> item.iSubItem;

		if ( ( nRow >= 0 ) && ( nRow < m_arrayCellTypes.GetSize() ) )
		{
			CellTypeInfo info = m_arrayCellTypes.GetAt( nRow );

			switch( info.m_nType )
			{
			case CELL_TYPE_EDIT_TEXT:
				{
					if ( strText.GetLength() <= EPOS_MAXLEN_EXPORT_FILENAME )
					{
						::TrimSpaces( strText, FALSE );

						if ( strText == "" )
						{
							CTimeAttendReportNameCSVRecord Report;
							DataManagerNonDb.TimeAttendReportNames.GetAt( nRow, Report );
							strText.Format("TA%3.3d", Report.GetReportNo());
						}

						m_arrayCellTypes[nRow].m_strCurrent = strText;
					}
					
					m_listReports.SetItem ( nRow, 1, LVIF_TEXT, m_arrayCellTypes[nRow].m_strCurrent, 0, 0, 0, NULL );
				}
				break;
			}
		}
	}

	return 1;
}

/**********************************************************************/

LRESULT CPosTrayFilenameTimeAttendDlg::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nCol = pCellType -> m_nCol;
	int nRow = pCellType -> m_nRow;

	if ( nCol != 1 )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	if ( nRow < 0 || nRow >= m_arrayCellTypes.GetSize() )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	pCellType -> m_nType = m_arrayCellTypes[nRow].m_nType;
	pCellType -> m_nSSEditType = m_arrayCellTypes[nRow].m_nSSEditType;
	pCellType -> m_strSSEditFormat = m_arrayCellTypes[nRow].m_strSSEditFormat;
	pCellType -> m_nSSEditWidth = m_arrayCellTypes[nRow].m_nSSEditWidth;
	
	return 1;
}

/**********************************************************************/

void CPosTrayFilenameTimeAttendDlg::OnOK()
{
	for ( int nRepIdx = 0; nRepIdx < m_arrayCellTypes.GetSize(); nRepIdx++ )
	{	
		CTimeAttendReportNameCSVRecord record;
		DataManagerNonDb.TimeAttendReportNames.GetAt(nRepIdx, record);
		record.SetReportFilename( m_arrayCellTypes[nRepIdx].m_strCurrent );
		DataManagerNonDb.TimeAttendReportNames.SetAt(nRepIdx, record);
	}

	EndDialog( IDOK );
}

/**********************************************************************/
