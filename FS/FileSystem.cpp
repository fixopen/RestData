//
// Created by up duan on 16/2/2016.
//

#include "FileSystem.h"

//std::wstring SelectFile(HWND owner, std::wstring const& szFilter) {
//    std::wstring result;
//	OPENFILENAME opfn;
//	wchar_t strFilename[MAX_PATH] = {0};     //存放文件名
//	memset(&opfn, 0, sizeof(OPENFILENAME));  //初始化
//	opfn.lStructSize = sizeof(OPENFILENAME); //结构体大小
//	opfn.hwndOwner = owner;
//	opfn.lpstrFilter = szFilter.c_str();     //设置过滤
//	opfn.nFilterIndex = 1;                   //默认过滤器索引设为1
//	opfn.lpstrFile = strFilename;            //文件名的字段必须先把第一个字符设为0
//	opfn.nMaxFile = sizeof(strFilename);
//	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;  //设置标志位，检查目录或文件是否存在
//	//opfn.lpstrInitialDir = NULL;
//	// 显示对话框让用户选择文件
//	if (::GetOpenFileName(&opfn)) {
//		result = strFilename;
//	}
//	return result;
//}

//BOOL HDR_CreatePath(CString path){
//	TCHAR*  p = path.GetBuffer();
//	BOOL ret = FALSE;
//
//	int index=0;
//	int len = path.GetLength();
//	while(index<len){
//		if ('\\' == p[index]){
//			CString cp = path.Mid(0,index);
//			if (!HDR_CreateFolder(cp)){
//				CString temp;
//				temp.Format(_T("HDR_CreateFolder %d"),GetLastError());
//				AfxMessageBox(temp);
//				goto CREATE_PATH_RETURN;
//			}
//		}
//		index++;
//	}
//
//	//如果最后不是\\，则上述方法会漏调最后一个文件夹，故补充之
//	if ('\\' != p[len]){
//		if (!HDR_CreateFolder(path)){
//			goto CREATE_PATH_RETURN;
//		}
//	}
//
//	ret = TRUE;
//CREATE_PATH_RETURN:
//	path.ReleaseBuffer();
//	return ret;
//}

////目录是否存在的检查
//BOOL HDR_FolderExist(CString strPath)
//{
//	WIN32_FIND_DATA  wfd;
//	BOOL rValue = FALSE;
//	HANDLE hFind = FindFirstFile(strPath, &wfd);
//	if ((hFind != INVALID_HANDLE_VALUE)
//		&& (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//	{
//		rValue = TRUE;
//	}
//	FindClose(hFind);
//	return rValue;
//}

////文件存在性检查
//BOOL HDR_FileExist(CString strFileName)
//{
//	CFileFind fFind;
//	return fFind.FindFile(strFileName);
//}

////创建目录
//BOOL HDR_CreateFolder(CString strPath)
//{
//	if (HDR_FolderExist(strPath)){
//		return TRUE;
//	}
//	//AfxMessageBox(_T("HDR_CreateFolder"));
//	SECURITY_ATTRIBUTES attrib;
//	attrib.bInheritHandle = FALSE;
//	attrib.lpSecurityDescriptor = NULL;
//	attrib.nLength =sizeof(SECURITY_ATTRIBUTES);
//	//上面定义的属性可以省略。 直接return ::CreateDirectory( path, NULL); 即可
//	return ::CreateDirectory(strPath, &attrib);
//}

////取消路径最后的分隔符，如果存在
//CString HDR_RemoveFileSep(CString strPath){
//	if ('\\' == strPath.GetAt(strPath.GetLength())){
//		return strPath.Mid(0,strPath.GetLength()-1);
//	}else{
//		return strPath;
//	}
//}

////取得路径的最后一项的名称，可能是文件名（如果包含文件名）
////可能是文件夹名字（如果不包含文件名）
//CString HDR_GetPathLastItem(CString strPath){
//	TCHAR*  p = strPath.GetBuffer();
//	BOOL ret = FALSE;
//	strPath = HDR_RemoveFileSep(strPath);
//
//	int len = strPath.GetLength();
//	int index=len-1;
//
//	while(index>=0){
//		if ('\\' == p[index]){
//			break;
//		}
//		index --;
//	}
//	strPath.ReleaseBuffer();
//	return 	strPath.Mid(index+1,len-index);
//}

////调用Windows Shell打开一个文件
//HINSTANCE HDR_WindowShellOpen(HWND handle,CString fileName,CString *paramLine,CString *workDir)
//{
//	CString param = _T("");
//	CString work_dir = _T("");
//
//	if (NULL!=paramLine)
//	{
//		param = *paramLine;
//	}
//
//	if (NULL!=workDir)
//	{
//		work_dir = *work_dir;
//	}
//	return ShellExecute(handle,_T("open"),fileName,param,work_dir,SW_SHOWNORMAL);
//}

//long HDR_GetFileSize(CString fileName)
//{
//	BOOL                        fOk;
//	WIN32_FILE_ATTRIBUTE_DATA   fileInfo;
//
//	if (fileName.IsEmpty())
//		return -1;
//
//	fOk = GetFileAttributesEx(fileName, GetFileExInfoStandard, (void*)&fileInfo);
//	if (!fOk)
//		return -1;
////	assert(0 == fileInfo.nFileSizeHigh);
//	return (long)fileInfo.nFileSizeLow;
//}

//CString HDR_GetDateString(){
//	CTime tm=CTime::GetCurrentTime();
//	return tm.Format("%Y-%m-%d");
//}

//CString HDR_GetDateTimeString(){
//	CTime tm=CTime::GetCurrentTime();
//	return tm.Format("%Y-%m-%d %H:%M:%S");
//}

////confim:是否提示覆盖
//BOOL HDR_CopyFolder(CString dirFrom,CString dirTo, BOOL confim)
//{
//	int nLengthFrm = dirFrom.GetLength();
//	TCHAR *NewPathFrm = new TCHAR[nLengthFrm+2];
//	wcscpy(NewPathFrm,dirFrom);
//	NewPathFrm[nLengthFrm] = _T('\0');
//	NewPathFrm[nLengthFrm+1] = _T('\0');
//
//	int nLengthTo = dirTo.GetLength();
//	TCHAR *NewPathTo = new TCHAR[nLengthTo+2];
//	wcscpy(NewPathTo,dirTo);
//	NewPathTo[nLengthTo] = _T('\0');
//	NewPathTo[nLengthTo+1] = _T('\0');
//
//	SHFILEOPSTRUCT FileOp;
//	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
//	if(confim){
//		FileOp.fFlags =FOF_NOCONFIRMMKDIR  ; //FOF_NOCONFIRMATION 默认覆盖，不设置则出现是否覆盖提示,FOF_NOCONFIRMMKDIR,不出现创建文件夹对话框
//	}else{
//		FileOp.fFlags =FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION ; //FOF_NOCONFIRMATION 默认覆盖，不设置则出现是否覆盖提示,FOF_NOCONFIRMMKDIR,不出现创建文件夹对话框
//	}

//	FileOp.hNameMappings = NULL;
//	FileOp.hwnd = NULL;
//	FileOp.lpszProgressTitle = NULL;
//	FileOp.pFrom = NewPathFrm;
//	FileOp.pTo = NewPathTo;
//	FileOp.wFunc = FO_COPY;
//	int ret = SHFileOperation(&FileOp);
//	/*if(ret!=0)
//	{
//		CString result;
//		result.Format(_T("%d"),ret);
//		AfxMessageBox(result);
//	}*/
//	return ret == 0;
//}

//bool HDR_DeleteFile(TCHAR* lpszPath)
//{
//	SHFILEOPSTRUCT FileOp={0};
//	FileOp.fFlags = FOF_ALLOWUNDO |   //允许放回回收站
//		FOF_NOCONFIRMATION; //不出现确认对话框
//	FileOp.pFrom = lpszPath;
//	FileOp.pTo = NULL;      //一定要是NULL
//	FileOp.wFunc = FO_DELETE;    //删除操作
//	return SHFileOperation(&FileOp) == 0;
//}
