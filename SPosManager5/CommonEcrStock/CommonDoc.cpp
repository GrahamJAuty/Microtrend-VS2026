/**********************************************************************/
 
/**********************************************************************/
#include "CommonDoc.h"
/**********************************************************************/
IMPLEMENT_DYNCREATE(CCommonDoc, CDocument)
/**********************************************************************/

BEGIN_MESSAGE_MAP(CCommonDoc, CDocument)
	//{{AFX_MSG_MAP(CCommonDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

CCommonDoc::CCommonDoc()
{
}

/**********************************************************************/

CCommonDoc::~CCommonDoc()
{
}

/**********************************************************************/

BOOL CCommonDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CCommonDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/**********************************************************************/
#ifdef _DEBUG
void CCommonDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCommonDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
/**********************************************************************/

BOOL CCommonDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	return TRUE;
}

/**********************************************************************/
