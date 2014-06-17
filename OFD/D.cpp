
#include <afxwin.h>
#include <afxole.h>
#include <afxdlgs.h>

CWinApp theApp;

int _tmain() {
	if (!AfxWinInit(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW))
		return 1;
	if (!AfxOleInit())
		return 1;
	HRESULT hr;
	if (FAILED(hr = CoInitialize(NULL)))
		return 1;
	{
		CFileDialog wndDlg(true, NULL, _T("C:\\Proj\\PDFGenen\\DUMMY.tif"));
		wndDlg.DoModal();
	}
	CoUninitialize();
	return 0;
}
