/**********************************************************************/
#include "..\CommonEcrStock\BackgroundLayoutInfo.h"
#include "..\CommonEcrStock\ButtonActionHandler.h"
#include "..\CommonEcrStock\ButtonDefines.h"
#include "..\CommonEcrStock\ButtonEditDlg.h"
#include "..\CommonEcrStock\ButtonManager.h"
#include "..\CommonEcrStock\ButtonOptions.h"
#include "..\CommonEcrStock\ButtonViewHelpers.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "FontTable.h"
#include "..\CommonEcrStock\ListDataButtonListSelectDlg.h"
#include "..\SPosEcrManager5\MainFrmEcrman.h"
#include "..\SPosStockManager5\MainFrmStockman.h"
#include "MessageLogger.h"
#include "..\CommonEcrStock\PMSCalendarHelpers.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockmanView.h"
/**********************************************************************/
IMPLEMENT_DYNCREATE(CStockmanView, CView)
/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockmanView, CView)
	//{{AFX_MSG_MAP(CStockmanView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND( IDM_BUTTONBAR_OPTION1, OnButtonBarOption1)
	ON_COMMAND( IDM_BUTTONBAR_OPTION2, OnButtonBarOption2)
	ON_COMMAND( IDM_BUTTONBAR_OPTION3, OnButtonBarOption3)
	ON_COMMAND( IDM_BUTTONBAR_OPTION4, OnButtonBarOption4)
	ON_COMMAND( IDM_BUTTONBAR_OPTION5, OnButtonBarOption5)
	ON_COMMAND( IDM_BUTTONBAR_OPTION6, OnButtonBarOption6)
	ON_COMMAND( IDM_BUTTONBAR_OPTION7, OnButtonBarOption7)
	ON_COMMAND( IDM_BUTTONBAR_OPTION8, OnButtonBarOption8)
	ON_COMMAND( IDM_BUTTONBAR_OPTION9, OnButtonBarOption9)
	ON_COMMAND( IDM_BUTTONBAR_OPTION10, OnButtonBarOption10)
	ON_COMMAND( IDM_BUTTONBAR_OPTION11, OnButtonBarOption11)
	ON_COMMAND( IDM_BUTTONBAR_OPTION12, OnButtonBarOption12)
	ON_COMMAND( IDM_BUTTONBAR_OPTION13, OnButtonBarOption13)
	ON_COMMAND( IDM_BUTTONBAR_OPTION14, OnButtonBarOption14)
	ON_COMMAND( IDM_BUTTONBAR_OPTION15, OnButtonBarOption15)
	ON_COMMAND( IDM_BUTTONBAR_OPTION16, OnButtonBarOption16)
	ON_COMMAND( IDM_BUTTONBAR_OPTION17, OnButtonBarOption17)
	ON_COMMAND( IDM_BUTTONBAR_OPTION18, OnButtonBarOption18)
	ON_COMMAND( IDM_BUTTONBAR_OPTION19, OnButtonBarOption19)
	ON_COMMAND( IDM_BUTTONBAR_OPTION20, OnButtonBarOption20)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

CStockmanView::CStockmanView()
{
}

/**********************************************************************/

CStockmanView::~CStockmanView()
{
}

/**********************************************************************/

BOOL CStockmanView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/**********************************************************************/

void CStockmanView::OnDraw(CDC* pDC)
{
	if (BackgroundLayoutInfo.GotValidLayout() && (ButtonOptions.GetLayoutEnable()))
	{
		CRect rFrame;
		GetClientRect(rFrame);
		CButtonViewHelpers::DrawView(pDC, rFrame);
	}
	else
	{
		Sysset.DisplayWatermark(pDC, this);
	}
}

/**********************************************************************/
#ifdef _DEBUG
/**********************************************************************/

void CStockmanView::AssertValid() const
{
	CView::AssertValid();
}

void CStockmanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCommonDoc* CStockmanView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCommonDoc)));
	return (CCommonDoc*)m_pDocument;
}

/**********************************************************************/
#endif 
/**********************************************************************/

void CStockmanView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument() -> SetTitle ( "Stock Manager" );
	CMainFrameStockman* pMain = (CMainFrameStockman*) AfxGetMainWnd();

	CDC* pDC = GetDC();
	FontTable.BuildFontTable ( pDC );
	BackgroundLayoutInfo.Initialise( pDC );

	{
		CRect rcFrameWindow;
		pMain -> GetWindowRect( rcFrameWindow );

		CRect rcViewWindow;
		GetWindowRect( rcViewWindow );

		CRect rcViewClient;
		GetClientRect( rcViewClient );

		BackgroundLayoutInfo.SetFrameToViewAdjustX( rcFrameWindow.Width() - rcViewWindow.Width() );
		BackgroundLayoutInfo.SetFrameToViewAdjustY( rcFrameWindow.Height() - rcViewWindow.Height() );
		BackgroundLayoutInfo.SetNonClientAdjustX( rcViewWindow.Width() - rcViewClient.Width() );
		BackgroundLayoutInfo.SetNonClientAdjustY( rcViewWindow.Height() - rcViewClient.Height() );
	}

	if ( ButtonOptions.GetLayoutEnable() == TRUE )
	{
		switch( BackgroundLayoutInfo.LoadUpperImage() )
		{
		case BACKIMAGE_ERROR_LOAD:
			{
				CString strError = "";
				strError.Format( "Unable to load upper image file (UPPER.%s)", ButtonOptions.GetUpperImageType() );
				BackgroundLayoutInfo.AddError( strError );
			}
			break;
			
		case BACKIMAGE_ERROR_NODC:
			BackgroundLayoutInfo.AddError( "Unable to create upper image device context" );
			break;
		}

		switch( BackgroundLayoutInfo.LoadLowerImage() )
		{
		case BACKIMAGE_ERROR_LOAD:
			{
				CString strError = "";
				strError.Format( "Unable to load lower image file (LOWER.%s)", ButtonOptions.GetLowerImageType() );
				BackgroundLayoutInfo.AddError( strError );
			}
			break;
			
		case BACKIMAGE_ERROR_NODC:
			BackgroundLayoutInfo.AddError( "Unable to create lower image device context" );
			break;
		}

		if ( BackgroundLayoutInfo.ValidateLayout() == TRUE )
		{
			CRect rcMax(0, 0, BackgroundLayoutInfo.GetWindowSizeRequestX(), BackgroundLayoutInfo.GetWindowSizeRequestY() );
			pMain -> SetWindowPos( NULL, 0, 0, rcMax.Width(), rcMax.Height(), SWP_NOZORDER );
		}
	}
	
	ReleaseDC ( pDC );
}

/**********************************************************************/

BOOL CStockmanView::OnEraseBkgnd(CDC* pDC)
{
	if ( ( BackgroundLayoutInfo.GotValidLayout() == FALSE ) || ( ButtonOptions.GetLayoutEnable() == FALSE ) )
	{
		CRect rect;
		GetClientRect ( &rect );
		pDC->FillSolidRect( &rect, COLORREF ( EcrmanOptions.GetFeaturesBackgroundColour() ) );
	}
	return TRUE;
}

/**********************************************************************/

void CStockmanView::OnLButtonDown(UINT iFlags, CPoint point )
{
	if ( ( BackgroundLayoutInfo.GotValidLayout() == TRUE ) && ( ButtonOptions.GetLayoutEnable() == TRUE ) )
	{
		int nButtonIdx;
		CButtonInfo infoDisplay;
		if ( ButtonManager.CheckButtonClick( point, infoDisplay, nButtonIdx ) == TRUE )
		{
			switch( ButtonManager.GetButtonManagerMode() )
			{
			case BUTMGR_MODE_USER:
				switch( infoDisplay.GetButtonType() )
				{
				case BUTTON_TYPE_ACTION:
					ButtonActionHandler.Action( infoDisplay, this, GetParentFrame() );
					break;

				case BUTTON_TYPE_LIST:
					if ( ButtonManager.GotoList( infoDisplay ) == TRUE )
					{
						Invalidate();
					}
					break;

				case BUTTON_TYPE_BACK:
					if ( ButtonManager.GoBack() == TRUE )
					{
						Invalidate();
					}
					break;

				case BUTTON_TYPE_HOME:
					ButtonManager.GoHome();
					Invalidate();
					break;
				}
				break;

			case BUTMGR_MODE_EDIT_LIST:
				switch( infoDisplay.GetButtonType() )
				{
				case BUTTON_TYPE_ACTION:
				case BUTTON_TYPE_LIST:
				case BUTTON_TYPE_UNUSED:
					if ( ButtonManager.GetButtonEditDlg() != NULL )
					{
						ButtonManager.GetButtonEditDlg() -> MergeSettings( infoDisplay );
						ButtonManager.InvalidateImage( infoDisplay );
						ButtonManager.SetDisplayButton( nButtonIdx, infoDisplay );
						ButtonManager.SetLibraryButton( infoDisplay );
						Invalidate();
					}	
					break;

				case BUTTON_TYPE_LIST_SAVE:
					if ( ButtonManager.CheckButtonLibraryChanges() == FALSE )
						Prompter.Info( "No unsaved changes were detected" );
					else
					{
						if ( Prompter.YesNo( "Button lists have been modified - save changes", TRUE ) == IDYES )
						{
							ButtonManager.TempImageModeOff( TRUE );
							ButtonManager.WriteButtonActions();
							ButtonManager.TempImageModeOn();
							ButtonManager.RememberButtonLibrary();
						}
					}
					break;

				case BUTTON_TYPE_LIST_SELECT:
					{
						CListDataButtonListSelectDlg dlgSelect( ButtonManager.GetCurrentListNo(), this );
						
						if ( dlgSelect.DoModal() == IDOK )
						{
							int nSelect = dlgSelect.GetListSelection();

							if ( 0 == nSelect )
							{
								ButtonManager.GoHome();
								Invalidate();
							}
							else
							{
								infoDisplay.SetDetail1( nSelect );
								ButtonManager.JumpToList( infoDisplay );
								Invalidate();
							}
						}
					}
					break;

				case BUTTON_TYPE_LIST_NAME:
					{
						if ( ButtonManager.GetButtonEditDlg() != NULL )
						{
							delete( ButtonManager.GetButtonEditDlg() );
							ButtonManager.SetButtonEditDlg( NULL );
						}

						int nListNo = ButtonManager.GetCurrentListNo();

						CChangeListNameDlg dlgName( nListNo, this );
						if ( dlgName.DoModal() == IDOK )
						{
							ButtonManager.SetListName( nListNo, dlgName.m_strName );
							Invalidate();
						}
					}
					break;
				}
				break;
			}
		}
	}
}

/**********************************************************************/

void CStockmanView::OnRButtonDown(UINT iFlags, CPoint point )
{
	if ( ( BackgroundLayoutInfo.GotValidLayout() == TRUE ) && ( ButtonOptions.GetLayoutEnable() == TRUE ) )
	{
		int nButtonIdx;
		CButtonInfo infoDisplay;
		if ( ButtonManager.CheckButtonClick( point, infoDisplay, nButtonIdx ) == TRUE )
		{
			switch( ButtonManager.GetButtonManagerMode() )
			{
			case BUTMGR_MODE_EDIT_LIST:
				switch( infoDisplay.GetButtonType() )
				{
				case BUTTON_TYPE_ACTION:
				case BUTTON_TYPE_LIST:
				case BUTTON_TYPE_UNUSED:
					ButtonActionHandler.ShowContextMenuEditKey( this, nButtonIdx, infoDisplay );
					break;
				}
			}
		}
	}
}

/**********************************************************************/
void CStockmanView::OnButtonBarOption1()	{ ContextOption(1); }
void CStockmanView::OnButtonBarOption2()	{ ContextOption(2); }
void CStockmanView::OnButtonBarOption3()	{ ContextOption(3); }
void CStockmanView::OnButtonBarOption4()	{ ContextOption(4); }
void CStockmanView::OnButtonBarOption5()	{ ContextOption(5); }
void CStockmanView::OnButtonBarOption6()	{ ContextOption(6); }
void CStockmanView::OnButtonBarOption7()	{ ContextOption(7); }
void CStockmanView::OnButtonBarOption8()	{ ContextOption(8); }
void CStockmanView::OnButtonBarOption9()	{ ContextOption(9); }
void CStockmanView::OnButtonBarOption10()	{ ContextOption(10); }
void CStockmanView::OnButtonBarOption11()	{ ContextOption(11); }
void CStockmanView::OnButtonBarOption12()	{ ContextOption(12); }
void CStockmanView::OnButtonBarOption13()	{ ContextOption(13); }
void CStockmanView::OnButtonBarOption14()	{ ContextOption(14); }
void CStockmanView::OnButtonBarOption15()	{ ContextOption(15); }
void CStockmanView::OnButtonBarOption16()	{ ContextOption(16); }
void CStockmanView::OnButtonBarOption17()	{ ContextOption(17); }
void CStockmanView::OnButtonBarOption18()	{ ContextOption(18); }
void CStockmanView::OnButtonBarOption19()	{ ContextOption(19); }
void CStockmanView::OnButtonBarOption20()	{ ContextOption(20); }
/**********************************************************************/

void CStockmanView::ContextOption( int n )
{
	ButtonActionHandler.ContextOption( n );
}

/**********************************************************************/
