
// BinConvertToolDlg.h: 头文件
//

#pragma once
#include <stdint.h>

#pragma pack(1)
typedef struct
{
	char hw_num[3];
	unsigned char fw_num[3];
	uint32_t fw_size;
	uint32_t fw_crc;
	uint32_t header_crc;
	char memo[6];
}bin_header_t;
typedef union 
{
	unsigned char temp[24];
	bin_header_t  header;
}header_union_t;
#pragma pack()

int32_t GetBinSize(char *filename);
void read_bin(char* path, uint8_t * buf, uint32_t size);

// uint32_t CalcCRC32(uint8_t * ptr, int startIndex, int len);
uint32_t CalcCRC32(uint8_t *ptr, uint32_t startIndex, uint32_t len,int32_t seed);
// CBinConvertToolDlg 对话框
class CBinConvertToolDlg : public CDialogEx
{
// 构造
public:
	CBinConvertToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINCONVERTTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpenbinfile();
	CString m_FileName;
	CString m_OnlyFileName;
	CString m_OnlyFileName2;
	CString m_Hareware_Num;
	CString m_Firmware_Num;
	CString m_Header_CRC;
	CString m_Body_CRC;
	CString m_FilePath;
	CString m_FilePath2;
	header_union_t  m_BinHeader;
	header_union_t  m_BinHeader2;
	afx_msg void OnBnClickedBtnAnalyseBin();
	afx_msg void OnBnClickedBtnGenBinIncludeHead();
	afx_msg void OnBnClickedBtnGenArrayList();
	int SaveBinToArray(unsigned char * p, uint32_t len);
	afx_msg void OnBnClickedBtnOta();
	CComboBox m_PortLIst;
	afx_msg void OnBnClickedButtonOpenbin2();
	afx_msg void OnBnClickedButtonTogether();
	CString m_FileName2;
	CString m_Hareware_Num2;
	CString m_Firmware_Num2;
	CString m_Header_CRC2;
	CString m_Body_CRC2;
};
