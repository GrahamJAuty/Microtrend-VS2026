/**********************************************************************/
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "KeyboardHelpers.h"
#include "NodeTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CloneDatabaseDlg.h"
/**********************************************************************/

CCloneDatabaseDlg::CCloneDatabaseDlg( int nDbIdx, CDataPageFilter& Filter, CWnd* pParent)
	: CDialog(CCloneDatabaseDlg::IDD, pParent), m_Filter ( Filter )
{
	//{{AFX_DATA_INIT(CCloneDatabaseDlg)
	//}}AFX_DATA_INIT
	m_nSourceDbIdx = nDbIdx;
}

/**********************************************************************/

bool CCloneDatabaseDlg::IsRelevant()
{
	if ( dbDatabase.GetSize() < 2 )						return FALSE;
	if ( m_Filter.GetPluFlag() == SCOPE_DBASE )			return TRUE;
	if ( m_Filter.GetDeptFlag() == SCOPE_DBASE )		return TRUE;
	if ( m_Filter.GetEposGroupFlag() == SCOPE_DBASE )	return TRUE;
	if ( m_Filter.GetReportGroupFlag() == SCOPE_DBASE )	return TRUE;
	if ( m_Filter.GetConsolGroupFlag() == SCOPE_DBASE )	return TRUE;
	if ( m_Filter.GetTaxFlag() == SCOPE_DBASE )			return TRUE;
#ifdef STOCKMAN_SYSTEM
	if ( m_Filter.GetCategoryFlag() == SCOPE_DBASE )	return TRUE;
	if ( m_Filter.GetAllowanceFlag() == SCOPE_DBASE )	return TRUE;
#endif
	return FALSE;
}

/**********************************************************************/

void CCloneDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCloneDatabaseDlg)
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboDestination);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCloneDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CCloneDatabaseDlg)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCloneDatabaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listType.SubclassDlgItem(IDC_LIST, this);
	m_listType.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listType.InsertColumn(0, "", LVCFMT_LEFT, 250);
	m_listType.SetLockItemZeroFlag(FALSE);

#ifdef STOCKMAN_SYSTEM
	if (m_Filter.GetPluFlag() == SCOPE_DBASE || m_Filter.GetStockFlag() == SCOPE_DBASE)
	{
		CString str = "Plu, Stock, Barcode Links";

		if (EcrmanOptions.GetFeaturesMixMatchFlag())
		{
			str += ", Mix Match";
		}

		if (EcrmanOptions.GetFeaturesWalletStampRuleFlag())
		{
			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_SMP_V4:
				str += ", Purchase Controls";
				break;

			case LOYALTY_TYPE_LOY_V4:
			default:
				str += ", Stamp Offers";
				break;
			}
		}

		AddListItem( str, NODE_PLU );
	}
#else
	if ( m_Filter.GetPluFlag() == SCOPE_DBASE )
	{
		CString str = "Plu, Barcode Links";

		if (DealerFlags.GetGlobalPromotionFlag() == FALSE)
		{
			if (EcrmanOptions.GetFeaturesMixMatchFlag())
			{
				str += ", Mix Match";
			}

			if (EcrmanOptions.GetFeaturesWalletStampRuleFlag())
			{
				switch (SysInfo.GetLoyaltyOrSmartPayType())
				{
				case LOYALTY_TYPE_SMP_V4:
					str += ", Purchase Controls";
					break;

				case LOYALTY_TYPE_LOY_V4:
				default:
					str += ", Stamp Offers";
					break;
				}
			}
		}

		AddListItem ( str, NODE_PLU );
	}
#endif

	if ( m_Filter.GetDeptFlag() == SCOPE_DBASE )
	{
		AddListItem ( "Department", NODE_DEPARTMENT );
	}

	if ( m_Filter.GetEposGroupFlag() == SCOPE_DBASE )
	{
		AddListItem ( "Department Epos Group", NODE_DEPT_GROUP_EPOS );
	}

	if ( m_Filter.GetReportGroupFlag() == SCOPE_DBASE )
	{
		AddListItem ( "Department Report Group", NODE_DEPT_GROUP_REPORT );
	}

	if ( m_Filter.GetConsolGroupFlag() == SCOPE_DBASE )
	{
		AddListItem ( "Department Consolidation Group", NODE_DEPT_GROUP_CONSOL );
	}

	if ( m_Filter.GetTaxFlag() == SCOPE_DBASE )
	{
		AddListItem( EcrmanOptions.GetTaxString( "%T Rates" ), NODE_TAX );
	}

#ifdef STOCKMAN_SYSTEM
	if ( m_Filter.GetCategoryFlag() == SCOPE_DBASE )
	{
		AddListItem ( "Categories", NODE_CATEGORY );
	}

	if ( m_Filter.GetAllowanceFlag() == SCOPE_DBASE )
	{
		AddListItem ( "Allowances", NODE_ALLOWANCE );
	}
#endif

	m_LocationSelector.SetExcludeDbIdx ( m_nSourceDbIdx );
	
	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_DATABASE );
	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboDestination );
		
	CString strTitle;
	strTitle.Format ( "Clone Records from %s", dbDatabase.GetName( m_nSourceDbIdx ) );
	SetWindowText ( strTitle );
	
	return TRUE;  
}

/**********************************************************************/

void CCloneDatabaseDlg::AddListItem( const char* sz, int nItemData )
{
	CSSListTaggedSelectItem item;
	item.m_strText = sz;
	item.m_nData = nItemData;
	item.SetTagState(0);
	
	m_listType.AddItemToList( item );
}

/**********************************************************************/

void CCloneDatabaseDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listType.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CCloneDatabaseDlg::CopyOrRemove ( const char* szSource, const char* szDest  )
{
	CString strSource = szSource;
	CString strDest = szDest;

	if ( ::FileExists ( strSource ) == TRUE ) 
		CopyFile ( strSource, strDest, FALSE );
	else
		CFileRemover FileRemover ( strDest );
}

/**********************************************************************/

void CCloneDatabaseDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );

	int nDestDbIdx = LocSelEntity.GetDbIdx();
	
	CDWordArray arrayTypes;
	for ( int nIndex = 0; nIndex < m_listType.GetItemCount(); nIndex++ )
	{
		CSSListTaggedSelectItem item;
		m_listType.GetListItem( nIndex, item );

		if ( item.GetTagState() == 1 )
		{
			arrayTypes.Add ( item.m_nData );
		}
	}

	if ( arrayTypes.GetSize() == 0 )
	{
		Prompter.Error ( "You must select at least one type of data\nbefore you can perform this operation." );
		return;
	}

	if ( arrayTypes.GetAt(0) == NODE_PLU )
	{
#ifdef STOCKMAN_SYSTEM

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly ( nDestDbIdx, info, FALSE );
		
		if ( ( DataManager.Plu.GetSize() != 0 ) || ( DataManager.Stock.GetSize() != 0 ) )
		{
			CString strMsg = "The destination database already contains some plu\n";
			strMsg += "or stock records. You must remove these records\n";
			strMsg += "from the destination database before the clone operation\n";
			strMsg += "will be allowed to continue.";
			Prompter.Error ( strMsg );
			return;
		}
#else
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly ( nDestDbIdx, info, FALSE );
		
		if ( DataManager.Plu.GetSize() != 0 ) 
		{
			CString strMsg = "The destination database already contains some plu\n";
			strMsg += "records. You must remove these records from the\n";
			strMsg += "destination database before the clone operation will be\n";
			strMsg += "will be allowed to continue.";
			Prompter.Error ( strMsg );
			return;
		}
#endif
	}
	
	CString strTitle;
	strTitle.Format ( "Clone Database Records (%s to %s)", dbDatabase.GetName( m_nSourceDbIdx ), dbDatabase.GetName( nDestDbIdx ) );
	CStringArray arrayWarning;
	arrayWarning.Add ( strTitle );

	for (int nIndex = 0; nIndex < arrayTypes.GetSize(); nIndex++)
	{
		switch (arrayTypes.GetAt(nIndex))
		{
		case NODE_PLU:

#ifdef STOCKMAN_SYSTEM

			AddWarning(arrayWarning, "Plu records will be cloned.");
			AddWarning(arrayWarning, "Stock records will be cloned.");
			AddWarning(arrayWarning, "Barcode links will be cloned.");

			if (EcrmanOptions.GetFeaturesMixMatchFlag())
			{
				AddWarning(arrayWarning, "Mix Match, Offer and Promo records will be cloned.");
			}

			if (EcrmanOptions.GetFeaturesWalletStampRuleFlag())
			{
				switch (SysInfo.GetLoyaltyOrSmartPayType())
				{
				case LOYALTY_TYPE_SMP_V4:
					AddWarning(arrayWarning, "Purchase Control records will be cloned.");
					break;

				case LOYALTY_TYPE_LOY_V4:
				default:
					AddWarning(arrayWarning, "Stamp Offer records will be cloned.");
					break;
				}
			}

			AddWarning(arrayWarning, "Apparent stock levels will NOT be cloned.");

#else

			AddWarning(arrayWarning, "Plu records will be cloned.");
			AddWarning(arrayWarning, "Barcode links will be cloned.");

			if (EcrmanOptions.GetFeaturesMixMatchFlag())
			{
				AddWarning(arrayWarning, "Mix Match, Offer, Promo and Time Plan records will be cloned.");
			}

			if (EcrmanOptions.GetFeaturesWalletStampRuleFlag())
			{
				switch (SysInfo.GetLoyaltyOrSmartPayType())
				{
				case LOYALTY_TYPE_SMP_V4:
					AddWarning(arrayWarning, "Purchase Control records will be cloned.");
					break;

				case LOYALTY_TYPE_LOY_V4:
				default:
					AddWarning(arrayWarning, "Stamp Offer records will be cloned.");
					break;
				}
			}
			
			if (EcrmanOptions.GetFeaturesPluStockFlag())
			{
				AddWarning(arrayWarning, "Apparent stock levels will NOT be cloned.");
			}
#endif

			break;
	
		case NODE_DEPARTMENT:	
			AddWarning ( arrayWarning, "Department records will be cloned." );	
			break;
		
		case NODE_DEPT_GROUP_EPOS:	
			AddWarning ( arrayWarning, "Department Epos Group records will be cloned." );	
			break;

		case NODE_DEPT_GROUP_REPORT:	
			AddWarning ( arrayWarning, "Department Report Group records will be cloned." );	
			break;

		case NODE_DEPT_GROUP_CONSOL:	
			AddWarning ( arrayWarning, "Department Consolidation Group records will be cloned." );	
			break;
		
		case NODE_TAX:	
			AddWarning ( arrayWarning, EcrmanOptions.GetTaxString ( "%T Rates will be cloned." ) );	
			break;
		
#ifdef STOCKMAN_SYSTEM
		case NODE_CATEGORY:	
			AddWarning ( arrayWarning, "Category records will be cloned." );	
			break;

		case NODE_ALLOWANCE:	
			AddWarning ( arrayWarning, "Allowance records will be cloned." );	
			break;
		
#endif
		}
	}
			
	if (Prompter.Warning(arrayWarning) != IDYES)
	{
		return;
	}

	if (::ExistSubdirectory(dbDatabase.GetFolderPathData(nDestDbIdx)) == FALSE)
	{
		::MakeSubdirectory(dbDatabase.GetFolderPathData(nDestDbIdx));
	}

	CString strSource = "", strDest = "";
	for ( int nIndex = 0; nIndex < arrayTypes.GetSize(); nIndex++ )
	{
		switch ( arrayTypes.GetAt ( nIndex ) )
		{
		case NODE_PLU:
			strSource = DataManager.GetFilePathPlu ( m_nSourceDbIdx );
			strDest = DataManager.GetFilePathPlu ( nDestDbIdx );
			CopyOrRemove ( strSource, strDest );

			CKeyboardHelpers::RemovePluFile ( nDestDbIdx );

			{
				CFilenameUpdater FnUp1 ( SysFiles::Barcode, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::Barcode, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}

			if (DealerFlags.GetGlobalPromotionFlag() == FALSE)
			{
				if (EcrmanOptions.GetFeaturesMixMatchFlag())
				{
					{
						CFilenameUpdater FnUp1(SysFiles::MixMatch, m_nSourceDbIdx);
						CFilenameUpdater FnUp2(SysFiles::MixMatch, nDestDbIdx);
						CopyOrRemove(FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse());
					}

					{
						CFilenameUpdater FnUp1(SysFiles::Promotion, m_nSourceDbIdx);
						CFilenameUpdater FnUp2(SysFiles::Promotion, nDestDbIdx);
						CopyOrRemove(FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse());
					}

					{
						CFilenameUpdater FnUp1(SysFiles::TimePlan, m_nSourceDbIdx);
						CFilenameUpdater FnUp2(SysFiles::TimePlan, nDestDbIdx);
						CopyOrRemove(FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse());
					}

					{
						CFilenameUpdater FnUp1(SysFiles::TimePlanEntry, m_nSourceDbIdx);
						CFilenameUpdater FnUp2(SysFiles::TimePlanEntry, nDestDbIdx);
						CopyOrRemove(FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse());
					}
				}

				if (EcrmanOptions.GetFeaturesWalletStampRuleFlag())
				{
					switch (SysInfo.GetLoyaltyOrSmartPayType())
					{
					case LOYALTY_TYPE_SMP_V4:
					{
						CFilenameUpdater FnUp1(SysFiles::SmartPayPurchaseControl, m_nSourceDbIdx);
						CFilenameUpdater FnUp2(SysFiles::SmartPayPurchaseControl, nDestDbIdx);
						CopyOrRemove(FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse());
					}
					break;

					case LOYALTY_TYPE_LOY_V4:
					default:
					{
						CFilenameUpdater FnUp1(SysFiles::LoyaltyStampOffer, m_nSourceDbIdx);
						CFilenameUpdater FnUp2(SysFiles::LoyaltyStampOffer, nDestDbIdx);
						CopyOrRemove(FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse());
					}
					break;
					}
				}

				if (EcrmanOptions.GetFeaturesMixMatchFlag() || EcrmanOptions.GetFeaturesWalletStampRuleFlag())
				{
					{
						CFilenameUpdater FnUp1(SysFiles::OfferGroup, m_nSourceDbIdx);
						CFilenameUpdater FnUp2(SysFiles::OfferGroup, nDestDbIdx);
						CopyOrRemove(FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse());
					}

					{
						CFilenameUpdater FnUp1(SysFiles::OfferEntry, m_nSourceDbIdx);
						CFilenameUpdater FnUp2(SysFiles::OfferEntry, nDestDbIdx);
						CopyOrRemove(FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse());
					}
				}
			}
			
#ifdef STOCKMAN_SYSTEM
			{
				CFilenameUpdater FnUp1 ( SysFiles::StockDatabase, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::StockDatabase, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}

			{
				CFilenameUpdater FnUp1 ( SysFiles::RecipeDatabase, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::RecipeDatabase, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
#else
			{
				CWordArray arrayReadWrite, arrayReadOnly;
				arrayReadWrite.Add( NODE_PLU );
				arrayReadOnly.Add( NODE_MODIFIER );

				CDataManagerInfo info;
				if ( DataManager.OpenDatabaseReadWrite( nDestDbIdx, arrayReadWrite, arrayReadOnly, info ) == TRUE )
				{
					int nSize = DataManager.Plu.GetSize();
	
					StatusProgress.Lock( TRUE, "Copying PLU records" );
					for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
					{
						StatusProgress.SetPos ( nPluIdx, nSize );
						CPluCSVRecord PluRecord;
						DataManager.Plu.GetAt ( nPluIdx, PluRecord );
						PluRecord.SetApparentLevel(0.0);
						DataManager.Plu.SavePluRecord ( nPluIdx, PluRecord );
					}
					StatusProgress.Unlock();
	
					DataManager.WritePlu( info );
					DataManager.CloseDatabase();
				}	
			}
#endif	
		
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nDestDbIdx, info, FALSE );
				CCustomExportManagerPlu CustomExport;
			}
			
			break;

		case NODE_DEPARTMENT:
			{
				CFilenameUpdater FnUp1 ( SysFiles::Department, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::Department, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
			break;

		case NODE_DEPT_GROUP_EPOS:
			{
				CFilenameUpdater FnUp1 ( SysFiles::DepartmentEposGroup, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::DepartmentEposGroup, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
			break;

		case NODE_DEPT_GROUP_REPORT:
			{
				CFilenameUpdater FnUp1 ( SysFiles::DeptReportGroup, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::DeptReportGroup, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
			break;

		case NODE_DEPT_GROUP_CONSOL:
			{
				CFilenameUpdater FnUp1 ( SysFiles::DeptConsolGroup, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::DeptConsolGroup, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
			break;

		case NODE_TAX:
			{
				CFilenameUpdater FnUp1 ( SysFiles::TaxRates, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::TaxRates, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
			{
				CFilenameUpdater FnUp1 ( SysFiles::HistoricalTax, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::HistoricalTax, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
			break;
						
#ifdef STOCKMAN_SYSTEM
		case NODE_CATEGORY:
			{
				CFilenameUpdater FnUp1 ( SysFiles::Category, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::Category, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
			break;
				
		case NODE_ALLOWANCE:
			{
				CFilenameUpdater FnUp1 ( SysFiles::Allowance, m_nSourceDbIdx );
				CFilenameUpdater FnUp2 ( SysFiles::Allowance, nDestDbIdx );
				CopyOrRemove ( FnUp1.GetFilenameToUse(), FnUp2.GetFilenameToUse() );
			}
			break;	
#endif
		}
	}			
	
	Prompter.Info ( "Clone operation completed" );
}

/**********************************************************************/

void CCloneDatabaseDlg::AddWarning( CStringArray& arrayWarning, const char* sz )
{
	if (arrayWarning.GetSize() > 1)
	{
		arrayWarning.Add("");
	}

	arrayWarning.Add( sz );
}

/**********************************************************************/
