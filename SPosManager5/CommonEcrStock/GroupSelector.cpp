/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
//#include "globalfunctions.h"
#include "GroupCSVArrayBase.h"
/**********************************************************************/
#include "GroupSelector.h"
/**********************************************************************/

CGroupSelector::CGroupSelector( CEdit& edit, CSSComboBox& combo, int nGroupType ) : m_EditBox ( edit ), m_ComboBox ( combo )
{
	m_nGroupType = nGroupType;
	m_nGroupFilter = 0;

	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		m_nGroupNoMin = ReportGroup::GroupNo.Min;
		m_nGroupNoMax = ReportGroup::GroupNo.Max;
		break;

	case NODE_DEPT_GROUP_CONSOL:
		m_nGroupNoMin = ConsolGroup::GroupNo.Min;
		m_nGroupNoMax = ConsolGroup::GroupNo.Max;
		break;

	case NODE_PAYMENT_GROUP:
		m_nGroupNoMin = PaymentGroup::GroupNo.Min;
		m_nGroupNoMax = PaymentGroup::GroupNo.Max;
		break;

	case NODE_ALLERGEN:
		m_nGroupNoMin = Allergen::AllergenNo.Min;
		m_nGroupNoMax = Allergen::AllergenNo.Max;
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		m_nGroupNoMin = EposGroup::GroupNo.Min;
		m_nGroupNoMax = EposGroup::GroupNo.Max;
		break;
	}
}

/**********************************************************************/

CGroupCSVArrayBase* CGroupSelector::GetGroupArray()
{
	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		return &DataManager.ReportGroup;

	case NODE_DEPT_GROUP_CONSOL:
		return &DataManager.ConsolGroup;

	case NODE_PAYMENT_GROUP:
		return &DataManager.PaymentGroup;

	case NODE_ALLERGEN:
		return &DataManagerNonDb.Allergen;

	case NODE_DEPT_GROUP_EPOS:
	default:
		return &DataManager.EposGroup;
	}
}

/**********************************************************************/

void CGroupSelector::FillGroupCombo( int nGroupFilter )
{
	m_ComboBox.ResetContent();

	CGroupCSVArrayBase* pGroupArray = GetGroupArray();
	
	for ( int nIndex = 0; nIndex < pGroupArray -> GetSize(); nIndex++ )
	{
		CBaseGroupInfo infoGroup;
		pGroupArray -> GetGroupInfo( nIndex, infoGroup );

		if ( 0 == infoGroup.m_nGroupNo )
			continue;

		if ( infoGroup.m_strReportText != infoGroup.m_strDefText )
		{
			int nInsertionPoint = m_ComboBox.AddString ( infoGroup.m_strReportText );
			m_ComboBox.SetItemData ( nInsertionPoint, ( WORD ) infoGroup.m_nGroupNo );
		}
	}

	for ( int nIndex = 0; nIndex < pGroupArray -> GetSize(); nIndex++ )
	{
		CBaseGroupInfo infoGroup;
		pGroupArray -> GetGroupInfo( nIndex, infoGroup );

		if ( 0 == infoGroup.m_nGroupNo )
			continue;

		if ( infoGroup.m_strReportText == infoGroup.m_strDefText )
		{
			int nInsertionPoint = m_ComboBox.InsertString ( -1, infoGroup.m_strReportText );
			m_ComboBox.SetItemData ( nInsertionPoint, ( WORD ) infoGroup.m_nGroupNo );
		}
	}

	int nGroupIdx;
	if ( pGroupArray -> FindGroupByNumber ( 0, nGroupIdx ) )
	{
		CBaseGroupInfo infoGroup;
		pGroupArray -> GetGroupInfo( nGroupIdx, infoGroup );
		m_ComboBox.InsertString (0, infoGroup.m_strReportText );
		m_ComboBox.SetItemData(0,0);
	}

	m_ComboBox.SetCurSel(0);
	m_nGroupFilter = m_ComboBox.GetItemData(0);

	bool bFound = FALSE;
	for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == (WORD) nGroupFilter )
		{
			m_ComboBox.SetCurSel ( nIndex );
			m_nGroupFilter = nGroupFilter;
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE ) 
	{
		m_ComboBox.InsertString ( 0, "Unknown" );
		m_ComboBox.SetItemData ( 0, nGroupFilter  );
		m_ComboBox.SetCurSel(0);
		m_nGroupFilter = nGroupFilter;
	}

	UpdateGroupEditBox();
}

/**********************************************************************/

void CGroupSelector::UpdateGroupCombo( int nNewGroupFilter)
{
	for ( int nIndex = 0; nIndex <= m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == ( WORD ) nNewGroupFilter )
		{
			m_ComboBox.SetCurSel ( nIndex );
			m_nGroupFilter = nNewGroupFilter;
			return;
		}
	}
}

/**********************************************************************/

void CGroupSelector::UpdateGroupEditBox()
{
	CString strGroupNo = "";
	if ( m_nGroupFilter >= m_nGroupNoMin && m_nGroupFilter <= m_nGroupNoMax )
		strGroupNo.Format ( "%d", m_nGroupFilter );

	m_EditBox.SetWindowText ( strGroupNo );
}

/**********************************************************************/

void CGroupSelector::SelectGroupFromCombo() 
{
	int nIndex = m_ComboBox.GetCurSel();
	
	if ( nIndex != CB_ERR )
	{
		int nNewGroupFilter = m_ComboBox.GetItemData ( nIndex );

		if ( nNewGroupFilter != m_nGroupFilter )
		{
			m_nGroupFilter = nNewGroupFilter;
			UpdateGroupEditBox();
		}
	}
}

/**********************************************************************/

void CGroupSelector::SelectGroupFromEditBox() 
{
	CString strGroupNo;
	m_EditBox.GetWindowText( strGroupNo );
	
	::TrimSpaces ( strGroupNo, FALSE );
	
	if ( strGroupNo == "" )
	{
		UpdateGroupEditBox();
		return;
	}

	int nNewGroupFilter = atoi ( strGroupNo );
	if ( nNewGroupFilter != m_nGroupFilter )
	{
		UpdateGroupCombo( nNewGroupFilter );
		UpdateGroupEditBox();
	}
}

/**********************************************************************/
