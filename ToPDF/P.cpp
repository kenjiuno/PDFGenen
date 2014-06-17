
#include <afxwin.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <locale.h>

CWinApp theApp;

namespace PUt {
	CString Combine(CString dir, CString fn) {
		if (fn.Left(1) == _T("\\"))
			return fn;
		if (dir.Right(1) == _T("\\")) {
			return dir + fn;
		}
		return dir + _T("\\") + fn;
	}
	CString GetDir(CString dir) {
		int p = dir.ReverseFind(_T('\\'));
		if (p < 0)
			return CString();
		return dir.Left(p);
	}
	CString GetModulefp(HMODULE hInst = NULL) {
		TCHAR tc[MAX_PATH+1] = {0};
		if (hInst == NULL)
			hInst = _AtlBaseModule.GetModuleInstance();
		GetModuleFileName(hInst, tc, MAX_PATH);
		return tc;
	}
	CString GetTempfp(LPCTSTR pszPrefix = _T("tmp"), UINT nUnique = 0) {
		TCHAR tcdir[MAX_PATH+1] = {0};
		GetTempPath(MAX_PATH, tcdir);
		TCHAR tcfp[MAX_PATH+1] = {0};
		GetTempFileName(tcdir, pszPrefix, nUnique, tcfp);
		return tcfp;
	}
}

CStringArray m_tiffs;
const bool allowUI = true;

class CCLI : public CCommandLineInfo {
public:
	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast) {
		if (bFlag) return;
		m_tiffs.Add(pszParam);
	}
};

int _tmain() {
	if (!AfxWinInit(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW))
		return 1;
	setlocale(LC_ALL, ".ACP");
	_tprintf(_T("TIF/TIFF を PDF に変換 \n\n"));
	theApp.ParseCommandLine(CCLI());
	if (m_tiffs.IsEmpty()) {
		OPENFILENAME ofn = {sizeof(OPENFILENAME)};

		TCHAR tcfp[10000 +1] = {0};
		ofn.hInstance = _AtlBaseModule.GetModuleInstance(); // http://bytes.com/topic/net/answers/401577-_atlmodule-getmodulefilename
		ofn.lpstrFilter = _T("TIF/TIFF\0*.tif;*.tiff\0\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = tcfp;
		ofn.nMaxFile = 10000;
		ofn.lpstrTitle = _T("PDFに変換したいTIF/TIFFファイルを選択してください");
		ofn.Flags = OFN_ENABLESIZING|OFN_EXPLORER|OFN_HIDEREADONLY|OFN_LONGNAMES|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST |OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT;
		if (!GetOpenFileName(&ofn))
			return 1;
		LPCTSTR psz = tcfp;
		while (psz && *psz) {
			m_tiffs.Add(psz);
			psz = _tcschr(psz, 0);
			psz++;
		}
	}
	HRESULT hr;
	if (FAILED(hr = CoInitialize(NULL)))
		return 1;
	{
		OPENFILENAME ofn = {sizeof(OPENFILENAME)};

		TCHAR tcfp[1000] = {0};
		ofn.hInstance = _AtlBaseModule.GetModuleInstance(); // http://bytes.com/topic/net/answers/401577-_atlmodule-getmodulefilename
		ofn.lpstrFilter = _T("*.pdf\0*.pdf\0\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = tcfp;
		ofn.nMaxFile = 1000;
		ofn.lpstrTitle = _T("保存先のPDFファイルを選択してください");
		ofn.Flags = OFN_ENABLESIZING|OFN_EXPLORER|OFN_HIDEREADONLY|OFN_LONGNAMES|OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
		ofn.lpstrDefExt = _T("pdf");
		if (GetSaveFileName(&ofn)) {
			CString tiffcp = PUt::Combine(PUt::GetDir(PUt::GetModulefp()), _T("tiffcp.exe"));
			CString tiff2pdf = PUt::Combine(PUt::GetDir(PUt::GetModulefp()), _T("tiff2pdf.exe"));

			CString fpTIFF = PUt::GetTempfp();
			{
				CString parms;
				for (int x = 0; x < m_tiffs.GetCount(); x++) {
					parms += _T(" \"");
					parms += m_tiffs[x];
					parms += _T("\"");
				}
				{
					parms += _T(" \"");
					parms += fpTIFF;
					parms += _T("\"");
				}

				STARTUPINFO si = {sizeof(STARTUPINFO)};
				PROCESS_INFORMATION pi = {0};
				_tprintf(_T("TIF/TIFFを結合中... \n\n"));
				if (!CreateProcess(tiffcp, const_cast<LPTSTR>(static_cast<LPCTSTR>(parms)), NULL, NULL, false, 0, NULL, NULL, &si, &pi)) {
					if (allowUI) MessageBox(NULL, _T("起動は失敗しました。tiffcp"), _T("PDFGenen"), MB_ICONEXCLAMATION);
					return E_FAIL;
				}
				WaitForSingleObject(pi.hProcess, INFINITE);

				DWORD ec = 1;
				GetExitCodeProcess(pi.hProcess, &ec);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				if (ec != 0) {
					if (allowUI) MessageBox(NULL, _T("変換に失敗しました。tiffcp"), _T("PDFGenen"), MB_ICONEXCLAMATION);
					return E_FAIL;
				}
			}
			CString fpPDF = tcfp;
			{
				CString parms;
				{
					parms += _T(" -o \"");
					parms += fpPDF;
					parms += _T("\"");
				}
				{
					parms += _T(" \"");
					parms += fpTIFF;
					parms += _T("\"");
				}

				STARTUPINFO si = {sizeof(STARTUPINFO)};
				PROCESS_INFORMATION pi = {0};
				_tprintf(_T("TIF/TIFFを変換中... \n\n"));
				if (!CreateProcess(tiff2pdf, const_cast<LPTSTR>(static_cast<LPCTSTR>(parms)), NULL, NULL, false, 0, NULL, NULL, &si, &pi)) {
					if (allowUI) MessageBox(NULL, _T("起動は失敗しました。tiff2pdf"), _T("PDFGenen"), MB_ICONEXCLAMATION);
					return E_FAIL;
				}
				WaitForSingleObject(pi.hProcess, INFINITE);

				DWORD ec = 1;
				GetExitCodeProcess(pi.hProcess, &ec);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				if (ec != 0) {
					if (allowUI) MessageBox(NULL, _T("変換に失敗しました。tiff2pdf"), _T("PDFGenen"), MB_ICONEXCLAMATION);
					return E_FAIL;
				}
			}
			SHChangeNotify(SHCNE_CREATE, SHCNF_PATH|SHCNF_FLUSH, tcfp, NULL);
			SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH|SHCNF_FLUSH, tcfp, NULL);
			if (allowUI) MessageBox(NULL, _T("変換しました。"), _T("PDFGenen"), MB_ICONINFORMATION);
			return S_OK;
		}
	}
	CoUninitialize();
	return 0;
}