// ToPDF.cpp : CToPDF ÇÃé¿ëï

#include "Stdafx.h"
#include "ToPDF.h"


// CToPDF

bool CToPDF::Eat(IDataObject *pDataObj) {
	HRESULT hr;
	FORMATETC fe = {
		CF_HDROP,
		NULL,
		DVASPECT_CONTENT,
		-1,
		TYMED_HGLOBAL
	};
	STGMEDIUM stgm = {0};
	m_tiffs.RemoveAll();
	if (SUCCEEDED(hr = pDataObj->GetData(&fe, &stgm))) {
		HDROP hdrop = reinterpret_cast<HDROP>(stgm.hGlobal);
		UINT cFiles = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
		for (UINT i = 0; i < cFiles; i++) {
			TCHAR szFile[MAX_PATH];
			UINT cch = DragQueryFile(hdrop, i, szFile, MAX_PATH);
			if (cch < MAX_PATH) {
				LPCTSTR pszExt = PathFindExtension(szFile);
				if (false
					|| _tcsicmp(pszExt, _T(".tif")) == 0
					|| _tcsicmp(pszExt, _T(".tiff")) == 0
				) {
					m_tiffs.Add(szFile);
				}
			}
		}
		ReleaseStgMedium(&stgm);
	}
	return !m_tiffs.IsEmpty();
}

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
			hInst = GetModuleHandle(NULL);
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

HRESULT CToPDF::Saveas(bool allowUI) {
	CString parms;
	for (size_t x = 0; x < m_tiffs.GetCount(); x++) {
		parms += _T(" \"");
		parms += m_tiffs[x];
		parms += _T("\"");
	}

	CString topdf = PUt::Combine(PUt::GetDir(PUt::GetModulefp()), _T("ToPDF.exe"));

	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi = {0};
	if (!CreateProcess(topdf, const_cast<LPTSTR>(static_cast<LPCTSTR>(parms)), NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		if (allowUI) MessageBox(NULL, _T("ãNìÆÇÕé∏îsÇµÇ‹ÇµÇΩÅBToPDF"), _T("PDFGenen"), MB_ICONEXCLAMATION);
		return E_FAIL;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return S_OK;
}

#if 0
HRESULT CToPDF::Saveas(bool allowUI) {
	OPENFILENAME ofn = {sizeof(OPENFILENAME)};

	TCHAR tcfp[1000] = {0};
	ofn.hInstance = _AtlBaseModule.GetModuleInstance(); // http://bytes.com/topic/net/answers/401577-_atlmodule-getmodulefilename
	ofn.lpstrFilter = _T("*.pdf\0*.pdf\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = tcfp;
	ofn.nMaxFile = 1000;
	ofn.Flags = OFN_ENABLESIZING|OFN_EXPLORER|OFN_HIDEREADONLY|OFN_LONGNAMES|OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = _T("pdf");
	if (GetSaveFileName(&ofn)) {
		CString tiffcp = PUt::Combine(PUt::GetDir(PUt::GetModulefp()), _T("tiffcp.exe"));
		CString tiff2pdf = PUt::Combine(PUt::GetDir(PUt::GetModulefp()), _T("tiff2pdf.exe"));

		CString fpTIFF = PUt::GetTempfp();
		{
			CString parms;
			for (size_t x = 0; x < m_tiffs.GetCount(); x++) {
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
			if (!CreateProcess(tiffcp, const_cast<LPTSTR>(static_cast<LPCTSTR>(parms)), NULL, NULL, false, 0, NULL, NULL, &si, &pi)) {
				if (allowUI) MessageBox(NULL, _T("ãNìÆÇÕé∏îsÇµÇ‹ÇµÇΩÅBtiffcp"), _T("PDFGenen"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}
			WaitForSingleObject(pi.hProcess, INFINITE);

			DWORD ec = 1;
			GetExitCodeProcess(pi.hProcess, &ec);

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			if (ec != 0) {
				if (allowUI) MessageBox(NULL, _T("ïœä∑Ç…é∏îsÇµÇ‹ÇµÇΩÅBtiffcp"), _T("PDFGenen"), MB_ICONEXCLAMATION);
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
			if (!CreateProcess(tiff2pdf, const_cast<LPTSTR>(static_cast<LPCTSTR>(parms)), NULL, NULL, false, 0, NULL, NULL, &si, &pi)) {
				if (allowUI) MessageBox(NULL, _T("ãNìÆÇÕé∏îsÇµÇ‹ÇµÇΩÅBtiff2pdf"), _T("PDFGenen"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}
			WaitForSingleObject(pi.hProcess, INFINITE);

			DWORD ec = 1;
			GetExitCodeProcess(pi.hProcess, &ec);

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			if (ec != 0) {
				if (allowUI) MessageBox(NULL, _T("ïœä∑Ç…é∏îsÇµÇ‹ÇµÇΩÅBtiff2pdf"), _T("PDFGenen"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}
		}
		SHChangeNotify(SHCNE_CREATE, SHCNF_PATH|SHCNF_FLUSH, tcfp, NULL);
		SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH|SHCNF_FLUSH, tcfp, NULL);
		if (allowUI) MessageBox(NULL, _T("ïœä∑ÇµÇ‹ÇµÇΩÅB"), _T("PDFGenen"), MB_ICONINFORMATION);
		return S_OK;
	}
	return S_FALSE;
}
#endif
