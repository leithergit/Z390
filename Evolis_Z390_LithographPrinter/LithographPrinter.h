#pragma once

#define DECLARE_HANDLE(x)  typedef WORD x

#ifndef CONST
#define CONST               const
#endif

#ifndef nullptr
#define nullptr 0
#endif

# if !defined(__LFSDBUS__) && !defined(__PISADBUS__)
#define FALSE   0
#define TRUE    1
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef char CHAR;
typedef wchar_t WCHAR; 
typedef unsigned int   DWORD;
typedef DWORD* LPDWORD,*PDWORD;
typedef short SHORT;
typedef unsigned short      WORD;
typedef WORD* LPWORD;
typedef unsigned long ULONG;
typedef unsigned char UCHAR;
typedef SHORT * LPSHORT;
typedef void* LPVOID;
typedef LPVOID * LPLPVOID;
typedef ULONG * LPULONG;
typedef USHORT * LPUSHORT;
typedef void *HANDLE;
typedef void VOID;
typedef VOID* PVOID;
typedef HANDLE HPROVIDER;
typedef ULONG REQUESTID;
typedef REQUESTID * LPREQUESTID;
typedef HANDLE HAPP;
typedef HAPP * LPHAPP;
typedef USHORT HSERVICE;
typedef HSERVICE * LPHSERVICE;
typedef long LONG;
typedef LONG HRESULT;
typedef LONG LRESULT;
typedef HRESULT * LPHRESULT;
typedef int BOOL;
typedef BOOL *LPBOOL,*PBOOL;

typedef CHAR *NPSTR, *LPSTR,*PSTR;
typedef unsigned char *LPBYTE,*PBYTE;
#ifdef __DBUSBUILD__
    typedef char* HWND;
#else
    typedef VOID* HWND;
#endif


typedef  CONST CHAR *LPCSTR, *PCSTR;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;
////////////////////////////////////////////////////////
typedef  WCHAR *NWPSTR, *LPWSTR;
typedef  CONST WCHAR *LPCWSTR, *PCWSTR;
typedef unsigned int UINT32, *PUINT32;

typedef HANDLE HPROVIDER;
typedef ULONG REQUESTID;
typedef REQUESTID * LPREQUESTID;
typedef HANDLE HAPP;
typedef HAPP * LPHAPP;
typedef USHORT HSERVICE;
typedef HSERVICE * LPHSERVICE;

DECLARE_HANDLE(HINSTANCE);

typedef HINSTANCE HMODULE;

typedef UINT WPARAM;
typedef LONG LPARAM;

///////////////////////////////////////////////////////////////定义常用结构体
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

#endif

namespace Lithograph
{
	typedef struct _LITHOGRAPHMAGINFO 
	{
		int nTrack1Len;		//TRACK1长度
		int nTrack2Len;		//TRACK2长度
		int nTrack3Len;		//TRACK3长度
		unsigned char track1_data[80];	//TRACK1数据
		unsigned char track2_data[80];	//TRACK2数据
		unsigned char track3_data[170];	//TRACK3数据
		WORD	wTrack1Method;			//TRACK1写磁方式,低抗0x0002,高抗0x0004、自动0x0008
		WORD	wTrack2Method;			//TRACK2写磁方式,低抗0x0002,高抗0x0004、自动0x0008
		WORD	wTrack3Method;			//TRACK3写磁方式,低抗0x0002,高抗0x0004、自动0x0008
	}LITHOGRAPHMAGINFO,*LPLITHOGRAPHMAGINFO;

	typedef struct _LITHOGRAPHBOXUNIT
	{
		unsigned char byType;			//卡箱类型 0:未知，1：发卡箱，2：回收箱
		unsigned char byBoxNumber;		//卡箱号
		unsigned char byStatus;			//卡箱状态,0：正常; 1：卡少; 2:无卡; 3：不可操作; 4：不存在; 5：高(快满)；6：满； 7：未知
		BOOL bHardwareSensor;	//是否支持故障状态传感器,TRUE:支持，FALSE：不支持
	}LITHOGRAPHBOXUNIT,*LPLITHOGRAPHBOXUNIT;

	typedef struct _LITHOGRAPHBOXINFO
	{
		int nCount;				//卡箱总个数(所有卡箱)
		LPLITHOGRAPHBOXUNIT lpplist;		//卡箱信息结构体指针
	}LITHOGRAPHBOXINFO,*LPLITHOGRAPHBOXINFO;

	typedef struct _LITHOGRAPHSTATUS
	{
		WORD fwDevice;			//打印机状态, 0-在线；1-忙；2-不在线；3-故障
		WORD fwMedia;			//介质状态，0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
		WORD fwToner;			//平印色带状态,0-FLLL;1-LOW;2-OUT;3-NOTSUPP;4-UNKNOW
	}LITHOGRAPHSTATUS,*LPLITHOGRAPHSTATUS;

	//复位
	enum ResetAction
	{
		RESET_HOLD = 1,
		RESET_EJECT,
		RESET_RETAIN,
	};

	enum CheckMag
	{
		NONCHECK_MAG = 0,
		CHECK_MAG,
	};

	enum TrackId
	{
		TRACK1 = 0x0001,
		TRACK2 = 0x0002,
		TRACK3 = 0x0003,
	};
	
	enum WriteMethod
	{
		LOCO = 0x0002,
		HICO = 0x0004,
		AUTO = 0x0008,
	};
	
	enum DispensePos
	{
		DISP_TO_TRACK_POS = 1,
		DISP_TO_CONTACT_POS = 2,
		DISP_TO_CONTACTLESS_POS = 3,
		DISP_TO_PRINT_POS = 4,
	};

	enum BoxType
	{
		UNKNOW_BOX = 0,
		SUPPLY_BOX = 1,
		RETAIN_BOX = 2,
	};

	enum BoxStatus
	{
		BOX_OK = 0,
		BOX_LOW,
		BOX_EMPTY,
		BOX_INOP,
		BOX_MISS,
		BOX_HIGH,
		BOX_FULL,
		BOX_UNKNOW,
	};
	
	enum PrinterStatus
	{
		DEVICE_ONLINE = 0,
		DEVICE_BUSY,
		DEVICE_OFFLINE,
		DEVICE_HWERR,
	};
	
	enum MediaStatus
	{
		NO_CARD = 0,
		CARD_GATE,
		CARD_INNER,
		CARD_IC_POS,
		CARD_RF_POS,
		CARD_JAM,
		CARD_UNKNOW
	};
	
	enum TonnerStatus
	{
		TONNER_FULL = 0,
		TONNER_LOW,
		TONNER_OUT,
		TONNER_NOTSUPP,
		TONNER_UNKNOW
	};

	enum FontStyle
	{
		FONT_NORMAL = 1,
		FONT_BLOD = 2,
		FONT_ITALIC = 4,
		FONT_BLACK = 8
	};

	enum CardType
	{
		CARD_IC,
		CARD_PSAM
	};
};


class CLithographPrinter
{
public:
	CLithographPrinter(void){};
	virtual ~CLithographPrinter(void){};
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  打开设备
	  * @param[in] pPort 端口号,U口设备：USB或usb；串口设备：COM+串口数，例如COM1；网口设备: IP
	  * @param[in] pPortParam 端口参数，U口设备:  USB设备参数，VID\PID;串口设备：格式为波特率+奇偶检校位+起始位+终止位，例如：57600,n,8,1;网口设备：IP地址， 例如 172.0.0.1
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_Open(char *pPort, char *pPortParam, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  关闭设备
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_Close(char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  复位设备
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] nResetAction 1-复位卡保持在设备内；2-复位退卡到打印机取卡位置；3-复位吞卡到打印机回收箱(若硬件支持需实现)
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_Reset(long lTimeout, int nResetAction, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  获取固件版本以及动态库版本信息
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] pVersionInfo 固件版本信息
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_GetVersion(long lTimeout, char *pVersionInfo, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  允许进卡
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] nCheckMode 是否检磁,0-不检磁；1-检磁(若支持需实现,否则参数忽略)
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_EnableCard (long lTimeout, int nCheckMode, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  取消进卡
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_DisableCard(long lTimeout,  char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  退卡到门口
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_Eject(long lTimeout, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  吞卡到打印机回收箱
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] nBoxNo 吞卡卡箱号
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  读磁条(若支持需实现)
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] wTrackId 需要读取的磁道,0x0001-读1磁；0x0002-读2磁；0x0004-读3磁,当需要读多磁条时,异或即可,譬如读1、2、3磁条,则传入0x0007
	  * @param[out] pTrackInfo 读出的磁条数据,具体见结构体定义
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_ReadTrack(long lTimeout, WORD wTrackId, Lithograph::LITHOGRAPHMAGINFO *pTrackInfo, char *pszRcCode) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  写磁条(若支持需实现)
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] wTrackId 需要写磁道,0x0001-读1磁；0x0002-读2磁；0x0004-读3磁,当需要写多磁条时,异或即可,譬如写1、2、3磁条,则传入0x0007 
	  * @param[in] pTrackInfo 需要写的磁条数据,结构体定义同读磁条接口
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_WriteTrack(long lTimeout, WORD wTrackId, Lithograph::LITHOGRAPHMAGINFO *pTrackInfo, char *pszRcCode) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  IC卡上电(若支持需实现)
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] byOutAtr ATR数据,HEX
	  * @param[out] nAtrlen ATR数据长度
	  * @param[out] byUID 当使用非接时需返回UID,否则返回空""
	  * @param[out] nUidLen 当使用非接时返回UID长度,否则长度为0
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_IcPowerOn(long lTimeout, unsigned char *byOutAtr, int &nAtrlen, unsigned char *byUID, int &nUidLen, char *pszRcCode) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  IC卡下电(若支持需实现)
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_IcPowerOff(long lTimeout, char *pszRcCode) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  IC卡数据交互(若支持需实现)
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] byIndata 输入指令HEX
	  * @param[in] nInDataLen 输入指令长度
	  * @param[out] pOutData 响应数据HEX
	  * @param[out] nOutLen 响应数据长度
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_IcExchange(long lTimeout, unsigned char *byIndata, int nInDataLen, unsigned char *pOutData, int &nOutLen, char *pszRcCode) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  从卡箱发卡(若支持需实现)
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] nBox 卡箱号
	  * @param[in] nDispPos 发卡位置:1-发卡到读磁位置；2-发卡到接触IC卡位置;3-发卡到非接IC卡位置;4-发卡到打印位置
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_Dispense(long lTimeout, int nBox, int nDispPos, char *pszRcCode) {return 0;};
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  获取卡箱状态,包括发卡箱、回收箱、废卡箱等（若硬件支持需实现）
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] lpBoxInfo 卡箱状态信息结构体
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO &lpBoxInfo, char *pszRcCode) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  获取打印机状态
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] lpStatus 打印机信息结构体,见定义
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  初始化打印,在打印图片、文字、条码前调用
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_InitPrint(long lTimeout, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  预打印图片
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] pImage 图片全路径
	  * @param[in] nAngle 图片顺时针旋转角度
	  * @param[in] fxPos 打印X坐标
	  * @param[in] fyPos 打印Y坐标
	  * @param[in] fHeight 打印高度
	  * @param[in] fWidth 打印宽度
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_PrintImage(long lTimeout, char *pImage, int nAngle, float fxPos,  float fyPos,  float fHeight,  float fWidth,  char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  预打印文字
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] pText 打印的内容
	  * @param[in] nAngle 顺时针旋转角度,用于打印水平或竖直文字
	  * @param[in] fxPos 打印X坐标
	  * @param[in] fyPos 打印Y坐标
	  * @param[in] pFontName 打印字体,例如"宋体"
	  * @param[in] nFontSize 字体大小
	  * @param[in] nFontStyle 字体风格：1-常规；2-粗体；4-斜体；8-黑体
	  * @param[in] nColor 打印字体颜色,RGB颜色值
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_PrintText(long lTimeout, char *pText, int nAngle, float fxPos,  float fyPos, char *pFontName, int nFontSize, int nFontStyle, int nColor, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  预打印条码
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] nAngle 条码旋转角度,用于竖直或水平打印条码
	  * @param[in] pBarcodeType 条码类型,譬如"c39"、"2/5"、"c128"、"c128c"、"ean13"等,若支持其他条码类型,请在说明文档中进行说明
	  * @param[in] fxPos 打印X坐标
	  * @param[in] fyPos 打印Y坐标
	  * @param[in] nMultiplier 倍频系数
	  * @param[in] nHeight 条码高度
	  * @param[in] nIsActive 数据值激活,0不激活,1激活
	  * @param[in] pBarcodeText 打印的条码数据
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_PrintBarcode(long lTimeout, int nAngle, char *pBarcodeType, float fxPos,  float fyPos, int nMultiplier, int nHeight, int nIsActive, char *pBarcodeText, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  提交任务,开始打印
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_StartPrint(long lTimeout, char *pszRcCode) = 0;
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  翻转卡片
	  * @param[in] lTimeout 超时时间,单位ms
	  * @param[in] nAngle 旋转角度,一般只支持180度翻转卡片
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_Rotate(long lTimeout, int nAngle, char *pszRcCode) {return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  关闭休眠
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_CloseSleepMode(long lTimeout, char *pszRcCode){return 0;}
	/** @ingroup CLithographPrinter Function declaration
	  * @brief  扩展命令
	  * @param[in] pCommand 命令字符串
	  * @param[in] lpCmdIn 输入数据
	  * @param[in] lpCmdOut 输出数据
	  * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
	  * @return 0：成功；1：失败
	 **/
	virtual int Print_ExtraCommand(long lTimeout, char *pCommand, LPVOID lpCmdIn, LPVOID &lpCmdOut, char *pszRcCode) {return 0;}
};

