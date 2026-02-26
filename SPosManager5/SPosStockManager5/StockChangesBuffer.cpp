/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "StockChangesBuffer.h"
/**********************************************************************/

CStockChangesBuffer::CStockChangesBuffer()
{
	m_nStockIdx = -1;
	m_strStockCode = "";
}

/**********************************************************************/

void CStockChangesBuffer::SetStockCode( const char* szStockCode, bool bStockPlu )
{
	CString strStockCode = szStockCode;
	
	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
	{
		m_strStockCode = strStockCode;
		m_nStockIdx = nStockIdx;
		DataManager.Stock.GetAt ( m_nStockIdx, m_StockRecord );

		m_PluBuffer.Reset();
		m_StockPluBuffer.RemoveAll();
		m_NewPluNumbers.RemoveAll();

		CStockPluRecord StockPluRecord;
		StockPluRecord.SetStockCode ( m_strStockCode );

		if ( bStockPlu == TRUE )
		{
			int nStockPluIdx;
			if ( DataManager.StockPlu.FindStockCode ( m_strStockCode, nStockPluIdx ) == TRUE )
				DataManager.StockPlu.GetAt ( nStockPluIdx, StockPluRecord );

			m_StockPluBuffer.Add ( StockPluRecord );
		}
	}
	else
	{
		m_strStockCode = "";
		m_nStockIdx = -1;
	}
}
 
/**********************************************************************/

void CStockChangesBuffer::LogPluChange ( __int64 nPluNo )
{
	//NOTHING TO DO IF ALREADY LOGGED AS A NEW PLU
	for ( int nIndex = 0; nIndex < m_NewPluNumbers.GetSize(); nIndex++ )
		if ( nPluNo == m_NewPluNumbers.GetAt ( nIndex ) )
			return;
	
	//FIND OUT IF THE PLU IS DEFINED IN THE DATABASE
	int nOldPluIdx;
	if ( DataManager.Plu.FindPluByNumber ( nPluNo, nOldPluIdx ) == TRUE )
	{	
		//LOG THE PLU AS CHANGED IF IT IS NOT ALREADY LOGGED
		int nSavedPluIdx;
		if ( m_PluBuffer.FindPluByNumber ( nPluNo, nSavedPluIdx ) == FALSE )
		{
			CPluCSVRecord OldPluRecord;
			DataManager.Plu.GetAt ( nOldPluIdx, OldPluRecord );
			
			CRecipeCSVRecord OldRecipeRecord;
			DataManager.Plu.GetRecipe( nOldPluIdx, nPluNo, OldRecipeRecord );
			
			m_PluBuffer.InsertRecord ( OldPluRecord, OldRecipeRecord, FALSE );
		}
	}
	else
		m_NewPluNumbers.Add ( nPluNo );
}

/**********************************************************************/

void CStockChangesBuffer::LogStockPluChange( const char* szStockCode )
{
	CString strStockCode = szStockCode;

	int nStockPluIdx;
	if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
	{
		int nSavedStockPluIdx;
		if ( m_StockPluBuffer.FindStockCode ( strStockCode, nSavedStockPluIdx ) == FALSE )
		{
			CStockPluRecord OldStockPluRecord;
			DataManager.StockPlu.GetAt ( nStockPluIdx, OldStockPluRecord );
			m_StockPluBuffer.InsertAt ( nSavedStockPluIdx, OldStockPluRecord );
		}
	}
}

/**********************************************************************/

bool CStockChangesBuffer::CheckModified()
{
	if ( m_nStockIdx == -1 )
		return FALSE;

	//CHECK FOR CREATION OF NEW PLU RECORDS
	if ( m_NewPluNumbers.GetSize() > 0 )
		return TRUE;

	//CHECK FOR CHANGE TO STOCK RECORD
	CCSV csv ( ',', '"', FALSE, TRUE, TRUE );
	m_StockRecord.ConvertToCSV ( csv );
	CString strOldStock = csv.GetLine();
	CString strNewStock = DataManager.Stock.GetAt ( m_nStockIdx );

	if ( strOldStock != strNewStock )
		return TRUE;

	//CHECK FOR CHANGE TO PLU RECORD
	for ( int nIndex = 0; nIndex < m_PluBuffer.GetSize(); nIndex++ )
	{
		int nNewPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( m_PluBuffer.GetPluNoInt ( nIndex ), nNewPluIdx ) == TRUE )
		{
			CString strOldRecord = m_PluBuffer.GetAt ( nIndex );
			if ( strOldRecord != DataManager.Plu.GetAt ( nNewPluIdx ) )
				return TRUE;

			//GET THE NEW RECIPE
			CRecipeCSVRecord RecipeRecordNew;
			DataManager.Plu.GetRecipe( -1, DataManager.Plu.GetPluNoInt( nNewPluIdx ), RecipeRecordNew );

			//GET THE OLD RECIPE
			CRecipeCSVRecord RecipeRecordOld;
			m_PluBuffer.GetRecipe( -1, m_PluBuffer.GetPluNoInt( nIndex ), RecipeRecordOld );
		
			if ( RecipeRecordOld.GetSize() != RecipeRecordNew.GetSize() )
				return TRUE;

			for ( int nItemIdx = 0; nItemIdx < RecipeRecordOld.GetSize(); nItemIdx++ )
			{
				if ( RecipeRecordOld.GetStockQty( nItemIdx ) != RecipeRecordNew.GetStockQty( nItemIdx ) )
					return TRUE;

				CString strStockCodeOld = RecipeRecordOld.GetStockCode( nItemIdx );
				CString strStockCodeNew = RecipeRecordNew.GetStockCode( nItemIdx );

				if ( strStockCodeOld != strStockCodeNew )
					return TRUE;
			}
		}
	}

	//CHECK FOR CHANGE TO STOCK PLU RECORD
	for ( int nIndex = 0; nIndex < m_StockPluBuffer.GetSize(); nIndex++ )
	{
		CString strOldRecord = m_StockPluBuffer.GetAt ( nIndex );
		CCSV csv ( strOldRecord );
		CString strStockCode = csv.GetString(0); 

		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
		{
			if ( strOldRecord != DataManager.StockPlu.GetAt ( nStockPluIdx ) )
				return TRUE;
		}
	}

	return FALSE;
}

/**********************************************************************/

void CStockChangesBuffer::UndoChanges()
{
	if ( m_nStockIdx == -1 )
		return;

	//RESTORE ORIGINAL STOCK RECORD
	DataManager.Stock.SetAt ( m_nStockIdx, m_StockRecord );

	//DELETE NEWLY CREATED PLU RECORDS
	for ( int nIndex = 0; nIndex < m_NewPluNumbers.GetSize(); nIndex++ )
	{
		__int64 nPluNo = m_NewPluNumbers.GetAt ( nIndex );
		
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
			DataManager.Plu.RemoveAt ( nPluIdx );
	}

	//RESTORE ORIGINAL PLU RECORDS - OKAY TO USE PLU SET AT FUNCTION
	for ( int nIndex = 0; nIndex < m_PluBuffer.GetSize(); nIndex++ )
	{
		CPluCSVRecord PluRecord;
		m_PluBuffer.GetAt ( nIndex, PluRecord );

		CRecipeCSVRecord RecipeRecord;
		m_PluBuffer.GetRecipe( -1, PluRecord.GetPluNoInt(), RecipeRecord );

		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber ( PluRecord.GetPluNoInt(), nPluIdx ) == TRUE )
		{
			DataManager.Plu.SetAt ( nPluIdx, PluRecord );
			DataManager.Plu.SetRecipe ( -1, RecipeRecord );
		}
	}

	//RESTORE ORIGINAL STOCK PLU RECORDS
	for ( int nIndex = 0; nIndex < m_StockPluBuffer.GetSize(); nIndex++ )
	{
		CStockPluRecord StockPluRecord;
		m_StockPluBuffer.GetAt ( nIndex, StockPluRecord );

		CString strStockCode = StockPluRecord.GetStockCode();

		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
			DataManager.StockPlu.SetAt ( nStockPluIdx, StockPluRecord );
	}
}

/**********************************************************************/
