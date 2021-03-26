
// BinConvertToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "BinConvertTool.h"
#include "BinConvertToolDlg.h"
#include "afxdialogex.h"
#include "serial\serial.h"
#include "serialport\SerialPort.h"
using namespace serial;
using namespace std;
using namespace itas109;

#pragma comment(lib,"serial.lib")

// Serial dutSerial("", 115200, Timeout::simpleTimeout(1000));
Serial dutSerial("", 115200, Timeout::simpleTimeout(200));

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DWPOLYNOMIAL   0X4C11DB7
// #define _CRT_SECURE_NO_WARNINGS
// CBinConvertToolDlg 对话框



CBinConvertToolDlg::CBinConvertToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BINCONVERTTOOL_DIALOG, pParent)
	, m_FileName(_T(""))
	, m_Hareware_Num(_T(""))
	, m_Firmware_Num(_T(""))
	, m_Header_CRC(_T(""))
	, m_Body_CRC(_T(""))
	, m_FileName2(_T(""))
	, m_Hareware_Num2(_T(""))
	, m_Firmware_Num2(_T(""))
	, m_Header_CRC2(_T(""))
	, m_Body_CRC2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBinConvertToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_FileName);
	DDX_Text(pDX, IDC_EDIT_HARDWARE_NUM, m_Hareware_Num);
	DDX_Text(pDX, IDC_EDIT_FIRMWARE_NUM, m_Firmware_Num);
	DDX_Text(pDX, IDC_EDIT_HEAD_CRC, m_Header_CRC);
	DDX_Text(pDX, IDC_EDIT_BODY_CRC, m_Body_CRC);

	DDX_Control(pDX, IDC_COMBO_PORT_LIST, m_PortLIst);
	DDX_Text(pDX, IDC_EDIT_FILENAME2, m_FileName2);
	DDX_Text(pDX, IDC_EDIT_HARDWARE_NUM2, m_Hareware_Num2);
	DDX_Text(pDX, IDC_EDIT_FIRMWARE_NUM2, m_Firmware_Num2);
	DDX_Text(pDX, IDC_EDIT_HEAD_CRC2, m_Header_CRC2);
	DDX_Text(pDX, IDC_EDIT_BODY_CRC2, m_Body_CRC2);
}

BEGIN_MESSAGE_MAP(CBinConvertToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENBINFILE, &CBinConvertToolDlg::OnBnClickedBtnOpenbinfile)
	ON_BN_CLICKED(IDC_BTN_ANALYSE_BIN, &CBinConvertToolDlg::OnBnClickedBtnAnalyseBin)
	ON_BN_CLICKED(IDC_BTN_GEN_BIN_INCLUDE_HEAD, &CBinConvertToolDlg::OnBnClickedBtnGenBinIncludeHead)
	ON_BN_CLICKED(IDC_BTN_GEN_ARRAY_LIST, &CBinConvertToolDlg::OnBnClickedBtnGenArrayList)
	ON_BN_CLICKED(IDC_BTN_OTA, &CBinConvertToolDlg::OnBnClickedBtnOta)
	ON_BN_CLICKED(IDC_BUTTON_OPENBIN2, &CBinConvertToolDlg::OnBnClickedButtonOpenbin2)
	ON_BN_CLICKED(IDC_BUTTON_TOGETHER, &CBinConvertToolDlg::OnBnClickedButtonTogether)
END_MESSAGE_MAP()


// CBinConvertToolDlg 消息处理程序

BOOL CBinConvertToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	m_Hareware_Num=_T("d.u.t");
	m_Firmware_Num=_T("2.0.0");
	m_FilePath=_T("");

	m_Hareware_Num2=_T("x.s.d");
	m_Firmware_Num2=_T("2.0.0");

	//Grap the Serial Port list
	CSerialPortInfo a;
	list<string> m_portsList = CSerialPortInfo::availablePorts();
	list<string>::iterator itor;
	TCHAR m_regKeyValue[255];
	for (itor = m_portsList.begin(); itor != m_portsList.end(); ++itor)
	{
#ifdef UNICODE
		int iLength;
		const char * _char = (*itor).c_str();
		iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, m_regKeyValue, iLength);
#else
		strcpy_s(m_regKeyValue, 255, (*itor).c_str());
#endif
		m_PortLIst.AddString(m_regKeyValue);
		// AfxMessageBox(m_regKeyValue);
		
	}
	m_PortLIst.SetCurSel(0);


	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBinConvertToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBinConvertToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBinConvertToolDlg::OnBnClickedBtnOpenbinfile()
{
	int32_t len;
	BOOL isOpen = TRUE;													   //是否打开(否则为保存)
	CString defaultDir = _T("D:\\");									   //默认打开的文件路径
	CString fileName = _T("");											   //默认打开的文件名
	CString filter = _T("文件 (*.bin; *.hex; *.txt)|*.bin;*.hex;*.txt||"); //文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	// openFileDlg.GetOFN().lpstrInitialDir = L"E:\\FileTest\\test.doc";
	INT_PTR result = openFileDlg.DoModal();
	// CString filePath = defaultDir + "\\test.doc";
	CString filePath;
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
		// AfxMessageBox(openFileDlg.GetFileName());
		// AfxMessageBox(openFileDlg.GetFolderPath());
		m_FilePath=openFileDlg.GetFolderPath();
		m_OnlyFileName=openFileDlg.GetFileName();
		m_FileName = filePath;

		len=GetBinSize((char *)m_FileName.GetBuffer(m_FileName.GetLength()));
		if(len>0)
			m_BinHeader.header.fw_size=len;
		// filter.Format(_T("the file size is %ld"),len);
		filter.Format("the file size is %d",len);
		AfxMessageBox(filter);
		UpdateData(FALSE);
	}
}

void CBinConvertToolDlg::OnBnClickedBtnAnalyseBin()
{
	// TODO: 在此添加控件通知处理程序代码
	// read the file header which length is 16bytes
	uint8_t header[16];
	uint32_t size;
	char * filename;
	uint32_t crc1,crc2;
	CString temp;
	//check the filename
	if(m_FileName.IsEmpty()==true)
	{
		AfxMessageBox(_T("the filename is empty, please input the file name"));
		return;
	}
	// assign the filename
	filename=(char *)m_FileName.GetBuffer(m_FileName.GetLength());

	// get the size of the file
	size=GetBinSize(filename);
	if(size<16)
	{
		AfxMessageBox(_T("the file is too small or the file do not contain the header"));
		return;
	}

	// read the header of the file
	read_bin(filename,header,16);

	// get the crc32 of the header
	crc1=CalcCRC32(header,0,14,0xffffffff);
    memcpy((void *)m_BinHeader.temp,header,16);
	crc2=m_BinHeader.header.header_crc;
	if(crc1!=crc2)
	{
		AfxMessageBox(_T("this bin do not include header"));
		return;
	}
	else
	{
		//dispaly the related value
		memcpy(m_BinHeader.temp, header, 16);

		// hardware number
		temp.Format("%c.%c.%c", m_BinHeader.header.hw_num[0], m_BinHeader.header.hw_num[1], m_BinHeader.header.hw_num[2]);
		m_Hareware_Num = temp;

		// firmwre number
		temp.Format("%d.%d.%d", m_BinHeader.header.fw_num[0], m_BinHeader.header.fw_num[1], m_BinHeader.header.fw_num[2]);
		m_Firmware_Num = temp;

		//body crc
		temp.Format("%08x", m_BinHeader.header.fw_crc);
		m_Body_CRC = temp;

		//header crc
		temp.Format("%08x", m_BinHeader.header.header_crc);
		m_Header_CRC = temp;

		// refresh window
		UpdateData(false);
	}
}


void CBinConvertToolDlg::OnBnClickedBtnGenBinIncludeHead()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	// get the hardware number
	char *p;
	char a, b, c;
	int m,n,k;
	uint32_t size;
	char * filename;
	uint32_t crc1,crc2;
	uint8_t *buf;
    CStdioFile newfile;
	bool rs;
    CString temp;
	int i=0;
	uint32_t size1;
	// uint8_t buf1[64];
	// int u=0;
	//p = (char *)m_Hareware_Num.GetBuffer(m_Hareware_Num.GetLength());
	p=(char *)m_Hareware_Num.GetBuffer();
	
	// char xx[10];
	// strncpy_s(xx,(char *)m_Hareware_Num.GetString(),m_Hareware_Num.GetLength());
	sscanf_s(p, "%c.%c.%c", (char *)&a,sizeof(char), (char *)&b,sizeof(char), (char *)&c,sizeof(char));
	// temp.Format("%c%c%c",a,b,c);
	// AfxMessageBox(temp);
	m_BinHeader.header.hw_num[0]=a; // fill the hardware number
	m_BinHeader.header.hw_num[1]=b;
	m_BinHeader.header.hw_num[2]=c;


	// get the firmware number
	p=(char *)m_Firmware_Num.GetBuffer();
	sscanf_s(p, "%d.%d.%d", (int *)&m,(int *)&n,(int *)&k);
	// temp.Format("%d%d%d",m,n,k);
	// AfxMessageBox(temp);
	m_BinHeader.header.fw_num[0]=m; // fill the firmware number
	m_BinHeader.header.fw_num[1]=n;
	m_BinHeader.header.fw_num[2]=k;


	// get the file size and crc32
	if(m_FileName.IsEmpty()==true)// check the filename
	{
		AfxMessageBox(_T("the filename is empty, please input the file name"));
		return;
	}
	
	filename=(char *)m_FileName.GetBuffer(m_FileName.GetLength());// assign the filename

	
	size=GetBinSize(filename);// get the size of the file
	if(size<16)
	{
		AfxMessageBox(_T("the file is too small or the file do not contain the header"));
		return;
	}
	// temp.Format("the file size is %d",size);
	// AfxMessageBox(temp);
	
    // adjust to 64 bytes
	size1 = size;
	size1+=63;
	size1>>=6;
	size1<<=6;
	m_BinHeader.header.fw_size=(uint32_t)size; //fill the header with bin size

	// adjust 4 bytes
	
	temp.Format("size1 is %d",size);
	AfxMessageBox(temp);

	// temp.Format("the file size is %d",size1);
	// AfxMessageBox(temp);
	buf=(uint8_t *)malloc(size1);
	memset(buf, 0x00, size1);
	read_bin(filename,buf,size);
	crc1=CalcCRC32(buf,0,size,0xffffffff); //get the crc of the bin file
	// crc1=CalcCRC32(buf,0,34624,0xffffffff); //get the crc of the bin file
	
    m_BinHeader.header.fw_crc=crc1;
	//m_Body_CRC.Format("%08X",crc1);
	temp.Format("%08x",crc1);
	m_Body_CRC=temp;
	AfxMessageBox(temp);


	crc2=CalcCRC32(m_BinHeader.temp,0,16,0xffffffff); // get the crc of the header
	m_BinHeader.header.header_crc = crc2;  // fill the header crc 
	temp.Format("%08x",crc2);
	m_Header_CRC=temp;
    

	// write header and bin to the new file
	temp="\\new_";
	temp=temp+m_OnlyFileName;
	temp=m_FilePath+temp;
	filename=(char *)temp.GetBuffer();
	rs = newfile.Open(filename, CStdioFile::modeCreate | CStdioFile::modeWrite |CStdioFile::typeBinary, NULL);
	if (rs == false)
	{
		AfxMessageBox("open file failed,please check");
	}
	else
	{
		newfile.Write((void *)m_BinHeader.temp,24);
		newfile.Write((void *)buf,size);
		newfile.Close();
	}
    UpdateData(FALSE);
	free(buf);
}


void CBinConvertToolDlg::OnBnClickedBtnGenArrayList()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	uint32_t size;
	unsigned char *p;
	char * filename;
	CString temp;
	if(m_FileName.IsEmpty()==true)
	{
		AfxMessageBox(_T("the file name is empty,please check"));
		return ;
	}
	else
	{
		AfxMessageBox("you can operate this file");
		filename=(char *)m_FileName.GetBuffer(m_FileName.GetLength());
		size=GetBinSize(filename);
		if(size==0)
		{
			AfxMessageBox(_T("the size of the file is 0,this bin file is empty"));
			return;
		}
		else
		{
			//p=(unsigned char *)malloc(size);
			p= new uint8_t[size];
			if(p==NULL)
			{
				AfxMessageBox(_T("allocate memory failed,please check"));
				return;

			}
			else
			{
				read_bin(filename,p,size);
				// temp.Format("%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x",p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15]);
				// AfxMessageBox(temp);
				SaveBinToArray(p,size);
				//free(p);
				delete [] p;
			}
			
		}
	}
}

int32_t GetBinSize(char *filename)
{     
    int32_t  siz = 0;     
    FILE  *fp ;
	errno_t err;
	err= fopen_s(&fp,filename, "rb");     
    if (err==0)   
    {        
        fseek(fp, 0, SEEK_END);        
        siz = ftell(fp);        
        fclose(fp);     
    }    
    // printf("\nfilename=%s,size=%d \n",filename,siz);	
    return siz;  
}

void read_bin(char* path, uint8_t * buf, uint32_t size)
{
    FILE* infile;
	errno_t err;
	err = fopen_s(&infile,path, "rb");
	if (err == 0)
	{
		fread(buf, sizeof(uint8_t), size, infile);
		fclose(infile);
	}
}


int CBinConvertToolDlg::SaveBinToArray(unsigned char * p, uint32_t len)
{
	// TODO: 在此处添加实现代码.
	CStdioFile newfile;
	char *filename;
	BOOL rs;
	uint32_t i;
	CString temp;
	CString path;
	if (m_FilePath.IsEmpty() == false)
	{
		path=m_FilePath;
		path += _T("\\bintoarray.txt");
		filename = (char *)path.GetBuffer(path.GetLength());
		AfxMessageBox(filename);
		rs = newfile.Open(filename, CStdioFile::modeCreate | CStdioFile::modeWrite, NULL);
		if (rs == false)
		{
			AfxMessageBox("open file failed,please check");
		}
		else
		{
			for (i = 0; i < len; i++)
			{
				temp.Format("0x%02X,", p[i]);
				newfile.WriteString(temp);
				if ((i & 0x0f) == 0x0f)
					newfile.WriteString(_T("\r\n"));
			}
			newfile.Close();
		}
	}
	return 0;
}


// uint32_t CalcCRC32(uint8_t * ptr, int startIndex, int len)
// {
//     uint32_t xbit;
//     int data;
//     uint32_t CRC = 0xFFFFFFFF; // init
//     for (int i = startIndex; i < startIndex + len; i += 4)
//     {
//         xbit = 1u << 31;

//         data = (ptr[i + 3]) | (ptr[i + 2] << 8) | (ptr[i + 1] << 16) | (ptr[i] << 24);
//         for (int bits = 0; bits < 32; bits++)
//         {
//             if ((CRC & 0x80000000) != 0)
//             {
//                 CRC <<= 1;
//                 CRC ^= DWPOLYNOMIAL;
//             }
//             else
//                 CRC <<= 1;
//             if ((data & xbit) != 0)
//                 CRC ^= DWPOLYNOMIAL;

//             xbit >>= 1;
//         }
//     }
//     return CRC;
// }

uint32_t CalcCRC32(uint8_t *ptr, uint32_t startIndex, uint32_t len,int32_t seed)
{
    uint32_t  xbit;
    uint32_t  data;
    uint32_t  crc ;//= 0xFFFFFFFF; // init
    int i;
    int bits;
    crc=seed;
    for (i = startIndex; i < startIndex + len; i += 4)
    {
        xbit = 1u << 31;


        data = (ptr[i + 3]) | (ptr[i + 2] << 8) | (ptr[i + 1] << 16) | (ptr[i] << 24);

        for (bits = 0; bits < 32; bits++)
        {
            if ((crc & 0x80000000) != 0)
            {
                crc <<= 1;
                crc ^= DWPOLYNOMIAL;
            }
            else
                crc <<= 1;

            if ((data & xbit) != 0)
                crc ^= DWPOLYNOMIAL;

            xbit >>= 1;
        }
    }
    return crc;
}

void CBinConvertToolDlg::OnBnClickedBtnOta()
{
	UpdateData(TRUE);

	// get the hardware number
	char *p;
	char a, b, c;
	int m,n,k;
	uint32_t size;
	char * filename;
	uint32_t crc1,crc2;
	uint8_t *buf;
    // CStdioFile newfile;
	bool rs;
    CString temp;
	int i;
	uint8_t xx[10];
	//p = (char *)m_Hareware_Num.GetBuffer(m_Hareware_Num.GetLength());
	p=(char *)m_Hareware_Num.GetBuffer();
	
	// char xx[10];
	// strncpy_s(xx,(char *)m_Hareware_Num.GetString(),m_Hareware_Num.GetLength());
	sscanf_s(p, "%c.%c.%c", (char *)&a,sizeof(char), (char *)&b,sizeof(char), (char *)&c,sizeof(char));
	// temp.Format("%c%c%c",a,b,c);
	// AfxMessageBox(temp);
	m_BinHeader.header.hw_num[0]=a; // fill the hardware number
	m_BinHeader.header.hw_num[1]=b;
	m_BinHeader.header.hw_num[2]=c;


	// get the firmware number
	p=(char *)m_Firmware_Num.GetBuffer();
	sscanf_s(p, "%d.%d.%d", (int *)&m,(int *)&n,(int *)&k);
	// temp.Format("%d%d%d",m,n,k);
	// AfxMessageBox(temp);
	m_BinHeader.header.fw_num[0]=m; // fill the firmware number
	m_BinHeader.header.fw_num[1]=n;
	m_BinHeader.header.fw_num[2]=k;


	// get the file size and crc32
	if(m_FileName.IsEmpty()==true)// check the filename
	{
		AfxMessageBox(_T("the filename is empty, please input the file name"));
		return;
	}
	
	filename=(char *)m_FileName.GetBuffer(m_FileName.GetLength());// assign the filename

	
	size=GetBinSize(filename);// get the size of the file
	if(size<16)
	{
		AfxMessageBox(_T("the file is too small or the file do not contain the header"));
		return;
	}
	m_BinHeader.header.fw_size=(uint16_t)size; //fill the header with bin size
	
	buf=(uint8_t *)malloc(size);
	read_bin(filename,buf,size);
	crc1=CalcCRC32(buf,0,size,0xffffffff); //get the crc of the bin file
	
    m_BinHeader.header.fw_crc=crc1;
	//m_Body_CRC.Format("%08X",crc1);
	temp.Format("%08x",crc1);
	m_Body_CRC=temp;

	crc2=CalcCRC32(m_BinHeader.temp,0,14,0xffffffff); // get the crc of the header
	m_BinHeader.header.header_crc = crc2;  // fill the header crc 
	temp.Format("%08x",crc2);
	m_Header_CRC=temp;
    

	// write header and bin to serial
	m_PortLIst.GetWindowText(temp);
	dutSerial.setPort(temp.GetBuffer(temp.GetLength()));
	if(dutSerial.isOpen()==false)
		dutSerial.open();
	dutSerial.setTimeout(serial::Timeout::max(), 100, 0, 100, 0); // set timeout is 100ms
	
	if(dutSerial.isOpen()==false)
	{
		temp="open "+ temp+ " failure";
	 	AfxMessageBox(temp);
		return;
	}

	xx[0]='o';
	xx[1]='t';
	xx[2]='a';
	dutSerial.write(xx,3);
	Sleep(5000);

	// write header
	dutSerial.write(m_BinHeader.temp,16);
	Sleep(100);

	// write bin
	for(i=0;i<size;i+=64)
	// for(i=0;i<128;i+=64)
	{
		if(i+64>size)
			dutSerial.write(buf+i,size-i);
		else
			dutSerial.write(buf+i,64);
		Sleep(100);
	}

    UpdateData(FALSE);
	free(buf);

	// TODO: 在此添加控件通知处理程序代码
	// char xx[]="my name is weijunfeng\n";
	// char str[256];
	// int len;
	// int i;
	
	
	// if (dutSerial.isOpen())
	// 	AfxMessageBox("open serial port successfully\n");
	// else
	// 	AfxMessageBox("open serial port failure\n");

	// //write data
	// for (i = 0; i < 10; i++)
	// {
	// 	dutSerial.write((uint8_t *)xx, strlen(xx));
	// 	Sleep(100);
	// }

	// // read data
	// dutSerial.flush();
	// memset(str,0,sizeof(str));
	// while (1)
	// {
	// 	len=dutSerial.read((uint8_t *)str,256);
	// 	if(len)
	// 	{
	// 		AfxMessageBox(str);
	// 		break;
	// 	}
	// }
	

}


void CBinConvertToolDlg::OnBnClickedButtonOpenbin2()
{
	int32_t len;
	BOOL isOpen = TRUE;													   //是否打开(否则为保存)
	CString defaultDir = _T("D:\\");									   //默认打开的文件路径
	CString fileName = _T("");											   //默认打开的文件名
	CString filter = _T("文件 (*.bin; *.hex; *.txt)|*.bin;*.hex;*.txt||"); //文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	// openFileDlg.GetOFN().lpstrInitialDir = L"E:\\FileTest\\test.doc";
	INT_PTR result = openFileDlg.DoModal();
	// CString filePath = defaultDir + "\\test.doc";
	CString filePath;
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
		// AfxMessageBox(openFileDlg.GetFileName());
		// AfxMessageBox(openFileDlg.GetFolderPath());
		m_FilePath2=openFileDlg.GetFolderPath();
		m_OnlyFileName2=openFileDlg.GetFileName();
		m_FileName2 = filePath;

		len=GetBinSize((char *)m_FileName2.GetBuffer(m_FileName2.GetLength()));
		if(len>0)
			m_BinHeader2.header.fw_size=len;
		// filter.Format(_T("the file size is %ld"),len);
		filter.Format("the file size is %d",len);
		AfxMessageBox(filter);
		UpdateData(FALSE);
	}
}


void CBinConvertToolDlg::OnBnClickedButtonTogether()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	// get the hardware number
	char *p;
	char a, b, c;
	int m,n,k;
	char a2, b2, c2;
	int m2,n2,k2;
	uint32_t size;
	uint32_t size2;
	char * filename;
	uint32_t crc1,crc2;
	uint8_t *buf;
	uint8_t *buf2;
    CStdioFile newfile;
	bool rs;
    CString temp;
	int i=0;
	uint32_t size1;
	uint32_t size12;
	// uint8_t buf1[64];
	// int u=0;
	
	// step1: get the hardware number
	p=(char *)m_Hareware_Num.GetBuffer();
	sscanf_s(p, "%c.%c.%c", (char *)&a,sizeof(char), (char *)&b,sizeof(char), (char *)&c,sizeof(char));
	// temp.Format("%c%c%c",a,b,c);
	// AfxMessageBox(temp);
	m_BinHeader.header.hw_num[0]=a; // fill the hardware number
	m_BinHeader.header.hw_num[1]=b;
	m_BinHeader.header.hw_num[2]=c;


	// step2: get the firmware number
	p=(char *)m_Firmware_Num.GetBuffer();
	sscanf_s(p, "%d.%d.%d", (int *)&m,(int *)&n,(int *)&k);
	// temp.Format("%d%d%d",m,n,k);
	// AfxMessageBox(temp);
	m_BinHeader.header.fw_num[0]=m; // fill the firmware number
	m_BinHeader.header.fw_num[1]=n;
	m_BinHeader.header.fw_num[2]=k;


	// step 3:get the file size 
	if(m_FileName.IsEmpty()==true)// check the filename
	{
		AfxMessageBox(_T("the filename is empty, please input the file name"));
		return;
	}
	
	filename=(char *)m_FileName.GetBuffer(m_FileName.GetLength());// assign the filename
	size=GetBinSize(filename);// get the size of the file
	if(size<16)
	{
		AfxMessageBox(_T("the file is too small or the file do not contain the header"));
		return;
	}
	// temp.Format("the file size is %d",size);
	// AfxMessageBox(temp);

	// adjust to 64 bytes
	size1 = size;
	size1+=63;
	size1>>=6;
	size1<<=6;

	m_BinHeader.header.fw_size=(uint32_t)size1; //fill the header with bin size

	// adjust 4 bytes
	// size1 = size;
	// size1 >>= 2;
	// size1 <<= 2;
	temp.Format("size1 is %d",size1);
	AfxMessageBox(temp);

    // step 4: get the bin 
	// temp.Format("the file size is %d",size1);
	// AfxMessageBox(temp);
	buf=(uint8_t *)malloc(size1);
	memset(buf, 0xff, size1);
	read_bin(filename,buf,size);

	// step 5: get the crc32 of the bin
	crc1=CalcCRC32(buf,0,size1,0xffffffff); //get the crc of the bin file
    m_BinHeader.header.fw_crc=crc1;
	//m_Body_CRC.Format("%08X",crc1);
	temp.Format("%08x",crc1);
	m_Body_CRC=temp;
	AfxMessageBox(temp);

	// step 6: get the crc32 of the header
	crc2=CalcCRC32(m_BinHeader.temp,0,16,0xffffffff); // get the crc of the header
	m_BinHeader.header.header_crc = crc2;  // fill the header crc 
	temp.Format("%08x",crc2);
	m_Header_CRC=temp;
    

//--------------------------------------------------------------------------------------------------------------------
// step1: get the hardware number
	p=(char *)m_Hareware_Num2.GetBuffer();
	sscanf_s(p, "%c.%c.%c", (char *)&a2,sizeof(char), (char *)&b2,sizeof(char), (char *)&c2,sizeof(char));
	// temp.Format("%c%c%c",a,b,c);
	// AfxMessageBox(temp);
	m_BinHeader2.header.hw_num[0]=a2; // fill the hardware number
	m_BinHeader2.header.hw_num[1]=b2;
	m_BinHeader2.header.hw_num[2]=c2;


	// step2: get the firmware number
	p=(char *)m_Firmware_Num2.GetBuffer();
	sscanf_s(p, "%d.%d.%d", (int *)&m2,(int *)&n2,(int *)&k2);
	// temp.Format("%d%d%d",m,n,k);
	// AfxMessageBox(temp);
	m_BinHeader2.header.fw_num[0]=m2; // fill the firmware number
	m_BinHeader2.header.fw_num[1]=n2;
	m_BinHeader2.header.fw_num[2]=k2;


	// step 3:get the file size 
	if(m_FileName2.IsEmpty()==true)// check the filename
	{
		AfxMessageBox(_T("the filename is empty, please input the file name"));
		return;
	}
	
	filename=(char *)m_FileName2.GetBuffer(m_FileName2.GetLength());// assign the filename
	size2=GetBinSize(filename);// get the size of the file
	if(size2<16)
	{
		AfxMessageBox(_T("the file is too small or the file do not contain the header"));
		return;
	}
	// temp.Format("the file size is %d",size);
	// AfxMessageBox(temp);

    // adjust to 64 bytes
	size12 = size2;
	size12+=63;
	size12>>=6;
	size12<<=6;
	m_BinHeader2.header.fw_size=(uint32_t)size2; //fill the header with bin size

	// adjust 4 bytes
	// size12 = size2;
	// size12 >>= 2;
	// size12 <<= 2;
	temp.Format("size1 is %d",size2);
	AfxMessageBox(temp);

    // step 4: get the bin 
	// temp.Format("the file size is %d",size1);
	// AfxMessageBox(temp);
	buf2=(uint8_t *)malloc(size12);
	memset(buf2,0xff, size12);
	read_bin(filename,buf2,size2);

	// step 5: get the crc32 of the bin
	crc1=CalcCRC32(buf2,0,size2,0xffffffff); //get the crc of the bin file
    m_BinHeader2.header.fw_crc=crc1;
	//m_Body_CRC.Format("%08X",crc1);
	temp.Format("%08x",crc1);
	m_Body_CRC2=temp;
	AfxMessageBox(temp);

	// step 6: get the crc32 of the header
	crc2=CalcCRC32(m_BinHeader2.temp,0,16,0xffffffff); // get the crc of the header
	m_BinHeader2.header.header_crc = crc2;  // fill the header crc 
	temp.Format("%08x",crc2);
	m_Header_CRC2=temp;


//-------------------------------------------------------------------------------------------------------------------------

	// write header and bin to the new file
	temp.Format("\\new_%c%c%c_MCU_V%d.%d.%d_LIB010101_%c%c%c_V%d.%d.%d.bin",a,b,c,m,n,k,a2,b2,c2,m2,n2,k2);
	// temp=temp+m_OnlyFileName;
	temp=m_FilePath+temp;
	filename=(char *)temp.GetBuffer();
	rs = newfile.Open(filename, CStdioFile::modeCreate | CStdioFile::modeWrite |CStdioFile::typeBinary, NULL);
	if (rs == false)
	{
		AfxMessageBox("open file failed,please check");
	}
	else
	{
		newfile.Write((void *)m_BinHeader.temp,24);
		newfile.Write((void *)m_BinHeader2.temp,24);
		newfile.Write((void *)buf,size1);
		newfile.Write((void *)buf2,size2);
		newfile.Close();
	}
    UpdateData(FALSE);
	free(buf);
	free(buf2);
}
