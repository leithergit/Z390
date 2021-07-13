#ifndef EVOLIS_Z390_LITHOGRAPHPRINTER_H
#define EVOLIS_Z390_LITHOGRAPHPRINTER_H

#include "Evolis_Z390_LithographPrinter_global.h"
#include "LithographPrinter.h"
#include "BasicOper.h"
#include "dcrf32.h"
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include "QEvolisPrinter.h"

//#include <QVector>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
LPVOID  CreateInstance(LPVOID lpReserve);
void    FreeInstance(LPVOID lpDev);
//void    InitZ390(void* env, void *androidContext);
//void    _init();
//void    _finit();
#ifdef __cplusplus
}
#endif


class EVOLIS_Z390_LITHOGRAPHPRINTER_EXPORT Evolis_Z390_LithographPrinter:public CLithographPrinter
{
public:
    Evolis_Z390_LithographPrinter(void);
    virtual ~Evolis_Z390_LithographPrinter(void);
    /** @ingroup CLithographPrinter Function declaration
          * @brief  打开设备
          * @param[in] pPort 端口号,U口设备：USB或usb；串口设备：COM+串口数，例如COM1；网口设备: IP
          * @param[in] pPortParam 端口参数，U口设备:  USB设备参数，VID\PID;串口设备：格式为波特率+奇偶检校位+起始位+终止位，例如：57600,n,8,1;网口设备：IP地址， 例如 172.0.0.1
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_Open(char *pPort, char *pPortParam, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  关闭设备
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_Close(char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  复位设备
          * @param[in] lTimeout 超时时间,单位ms
          * @param[in] nResetAction 1-复位卡保持在设备内；2-复位退卡到打印机取卡位置；3-复位吞卡到打印机回收箱(若硬件支持需实现)
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_Reset(long lTimeout, int nResetAction, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  获取固件版本以及动态库版本信息
          * @param[in] lTimeout 超时时间,单位ms
          * @param[out] pVersionInfo 固件版本信息
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_GetVersion(long lTimeout, char *pVersionInfo, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  允许进卡
          * @param[in] lTimeout 超时时间,单位ms
          * @param[in] nCheckMode 是否检磁,0-不检磁；1-检磁(若支持需实现,否则参数忽略)
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_EnableCard (long lTimeout, int nCheckMode, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  取消进卡
          * @param[in] lTimeout 超时时间,单位ms
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_DisableCard(long lTimeout,  char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  退卡到门口
          * @param[in] lTimeout 超时时间,单位ms
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_Eject(long lTimeout, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  吞卡到打印机回收箱
          * @param[in] lTimeout 超时时间,单位ms
          * @param[in] nBoxNo 吞卡卡箱号
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  读磁条(若支持需实现)
          * @param[in] lTimeout 超时时间,单位ms
          * @param[in] wTrackId 需要读取的磁道,0x0001-读1磁；0x0002-读2磁；0x0004-读3磁,当需要读多磁条时,异或即可,譬如读1、2、3磁条,则传入0x0007
          * @param[out] pTrackInfo 读出的磁条数据,具体见结构体定义
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_ReadTrack(long lTimeout, WORD wTrackId, Lithograph::LITHOGRAPHMAGINFO* pTrackInfo, char* pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  写磁条(若支持需实现)
          * @param[in] lTimeout 超时时间,单位ms
          * @param[in] wTrackId 需要写磁道,0x0001-读1磁；0x0002-读2磁；0x0004-读3磁,当需要写多磁条时,异或即可,譬如写1、2、3磁条,则传入0x0007
          * @param[in] pTrackInfo 需要写的磁条数据,结构体定义同读磁条接口
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_WriteTrack(long lTimeout, WORD wTrackId, Lithograph::LITHOGRAPHMAGINFO* pTrackInfo, char* pszRcCode) override;
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
        // 初始化读卡器 dc_init
        // 设置卡座 dc_setcpu
        // 卡上电 dc_CPUreset
        virtual int Print_IcPowerOn(long lTimeout, unsigned char* byOutAtr, int& nAtrlen, unsigned char* byUID, int& nUidLen, char* pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  IC卡下电(若支持需实现)
          * @param[in] lTimeout 超时时间,单位ms
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/

        // dc_exit
        virtual int Print_IcPowerOff(long lTimeout, char* pszRcCode) override;
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
        virtual int Print_IcExchange(long lTimeout, unsigned char* byIndata, int nInDataLen, unsigned char* pOutData, int& nOutLen, char* pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  从卡箱发卡(若支持需实现)
          * @param[in] lTimeout 超时时间,单位ms
          * @param[in] nBox 卡箱号
          * @param[in] nDispPos 发卡位置:1-发卡到读磁位置；2-发卡到接触IC卡位置;3-发卡到非接IC卡位置;4-发卡到打印位置
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  获取卡箱状态,包括发卡箱、回收箱、废卡箱等（若硬件支持需实现）
          * @param[in] lTimeout 超时时间,单位ms
          * @param[out] lpBoxInfo 卡箱状态信息结构体
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  获取打印机状态
          * @param[in] lTimeout 超时时间,单位ms
          * @param[out] lpStatus 打印机信息结构体,见定义
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  初始化打印,在打印图片、文字、条码前调用
          * @param[in] lTimeout 超时时间,单位ms
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        // 初始化打印，处理类库内部数据
        virtual int Print_InitPrint(long lTimeout, char *pszRcCode) override;
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
        // 传入图像参数，不作实际打印操作
        virtual int Print_PrintImage(long lTimeout, char *pImage, int nAngle, float fxPos,  float fyPos,  float fHeight,  float fWidth,  char *pszRcCode) override;
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
        // 传入文本参数，不作实际打印操作
        virtual int Print_PrintText(long lTimeout, char *pText, int nAngle, float fxPos,  float fyPos, char *pFontName, int nFontSize, int nFontStyle, int nColor, char *pszRcCode) override;
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

        virtual int Print_PrintBarcode(long lTimeout, int nAngle, char *pBarcodeType, float fxPos,  float fyPos, int nMultiplier, int nHeight, int nIsActive, char *pBarcodeText, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  提交任务,开始打印
          * @param[in] lTimeout 超时时间,单位ms
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/

        // 执行实际打印操作，卡片留在打印机内，打印过程结束后方可返回
        virtual int Print_StartPrint(long lTimeout, char *pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  翻转卡片
          * @param[in] lTimeout 超时时间,单位ms
          * @param[in] nAngle 旋转角度,一般只支持180度翻转卡片
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_Rotate(long lTimeout, int nAngle, char* pszRcCode) override;
        /** @ingroup CLithographPrinter Function declaration
          * @brief  关闭休眠
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_CloseSleepMode(long lTimeout, char* pszRcCode) override;

        int ReadBankCard(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);

        int ReadBankCard1(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);
        int ReadBankCard2(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);

        bool GetCardNumber(string strMsg,int nGPOIndex,int nRecsize,string &strCardNumber);

        bool FindCard(string &strBuffer,string &strNumber);

        bool RunApdu(DEVHANDLE dcHandle, string cmd,string &strOutmsg);
        /** @ingroup CLithographPrinter Function declaration
          * @brief  扩展命令
          * @param[in] pCommand 命令字符串
          * @param[in] lpCmdIn 输入数据
          * @param[in] lpCmdOut 输出数据
          * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
          * @return 0：成功；1：失败
         **/
        virtual int Print_ExtraCommand(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode) override;

		//下面是一个检查一个字符串中是否有'.'的函数，该函数将找到的'.'转化为'_'。
        inline void checkName(std::string& name, const std::string findvalue, const std::string replacevalue)
        {
            std::string::size_type startpos = 0;
            while (startpos != std::string::npos)
            {
                startpos = name.find(findvalue);   //找到'.'的位置
                if (startpos != std::string::npos) //std::string::npos表示没有找到该字符
                {
                    name.replace(startpos, 1, replacevalue); //实施替换，注意后面一定要用""引起来，表示字符串
                }
            }
        }
        inline int char2int(char input)
        {
            return input > 64 ? (input - 55) : (input - 48);
        }
        //ASCII码中将十进制数转换成对应的字符
        inline int int2char(char input)
        {
            return input > 9 ? (input + 55) : (input + 48);
        }

        int WriteCard1(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);
        int WriteCard2(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);
        int WriteCard3(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);
        int WriteCard4(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);
        int WriteCard5(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);
        int WriteCard6(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);
        int WriteCard7(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode);
		string CreateNode(std::string nodeName, std::string nodeVal);
		int fPubExtractXML(char* xmlStr, char* code, char* content, char* retMsg);
		int fXMLExtract(char* xmlstr, char* code, char* content, char* retMsg);
        string QueryNode_Text(void* pRootEle, const char* strNodeName);
        bool GetNodePointerByName(void* pRootEle, const char* strNodeName, void*& Node);
		string GetYYYYMMDD(char* pDate);

        bool WriteFile(DEVHANDLE dcHandle, string fileID, string tag, string val, string &strOutmsg);
        bool WriteCA(DEVHANDLE dcHandle, string fileID, string tag, string val,string &strOutmsg);
        string SM4Enc(string Key, string DATA);
        void hexstrxor(char* HexStr1, char* HexStr2, char* HexStr);
        string SM4_MAC(string RAM, string DATA, string CTK);
        string GetMAC(string random_32, string data, string key);
        int  String2Char(string tempstring);
        string& replace_all(string& str, const   string& old_value, const   string& new_value);
        // char, not int, is essential: otherwise gcc unpacks to vectors of int!  Huge slowdown.
        char ascii_toupper_char(char c)
        {
            return ('a' <= c && c <= 'z') ? c^0x20 : c;    // ^ autovectorizes to PXOR: runs on more ports than paddb
        }

        // gcc can only auto-vectorize loops when the number of iterations is known before the first iteration.  strlen gives us that
        size_t strtoupper(char *str)
        {
            size_t len = strlen(str);
            for (size_t i=0 ; i<len ; ++i) {
                str[i] = ascii_toupper_char(str[i]);  // gcc does the vector range check with psubusb / pcmpeqb instead of pcmpgtb
            }
            return len;
        }

        const char* getPlatformABI();
    private:

        int     m_hReader = 0;
		string  CardATR;
		CARDINFO m_CardInfo;
		NEWCARDINFO m_newInfos;
		string  cardInfo_temp;
		char    seeionId[128];
        string   m_strPicPath;
        char*   cpOutMsg = new char[1024];
        int     nOutMsgSize = 1024;


        /**
     * @brief  打开设备。
     * @par    说明：
     * 建立设备的通讯并且分配相应的资源，大部分功能接口都需要在此过程后才能进行，在不需要使用设备后，必须使用 ::dc_exit 去关闭设备的通讯和释放资源。
     * @param[in] port 端口号。
     * @n 0~99 - 表示串口模式（编号物理对应），编号0表示第一个串口合法设备，编号1表示第二个串口合法设备，以此类推。
     * @n 100~199 - 表示USB模式（编号逻辑对应），编号100表示第一个USB合法设备，编号101表示第二个USB合法设备，以此类推。
     * @n 200~299 - 表示PCSC模式（编号逻辑对应），编号200表示第一个PCSC合法设备，编号201表示第二个PCSC合法设备，以此类推。
     * @n 300~399 - 表示蓝牙模式（编号逻辑对应），编号300表示第一个蓝牙合法设备，编号301表示第二个蓝牙合法设备，以此类推。
     * @n 400~499 - 表示虚拟网络模式（编号逻辑对应），内部使用。
     * @n 500~599 - 表示内置SPI模式（编号逻辑对应），内部使用。
     * @param[in] baud 波特率，只针对串口模式有效。
     * @return <0表示失败，否则为设备标识符。
     */
        //using dc_init = void*  (*)(short port, int baud);
        //dc_init pdc_init = nullptr;
        /**
         * @brief  关闭设备。
         * @par    说明：
         * 关闭设备的通讯和释放资源。
         * @param[in] icdev 设备标识符。
         * @return <0表示失败，==0表示成功。
         */
        //using dc_exit = short (*)(void* icdev);
        //dc_exit pdc_exit = nullptr;
        /**
         * @brief  复位射频。
         * @par    说明：
         * 复位设备的射频，可以关闭，关闭并且启动。
         * @param[in] icdev 设备标识符。
         * @param[in] _Msec 为0表示关闭射频，否则为复位时间，单位为10毫秒，一般调用建议值为10。
         * @return <0表示失败，==0表示成功。
         */
        //using dc_reset = short (*)(HANDLE icdev, unsigned short _Msec);
        //dc_reset pdc_reset = nullptr;
        /**
         * @brief  接触式CPU卡复位。
         * @par    说明：
         * 对当前卡座CPU卡进行复位操作，此复位为冷复位。
         * @param[in] icdev 设备标识符。
         * @param[out] rlen 返回复位信息的长度。
         * @param[out] databuffer 返回的复位信息，请至少分配128个字节。
         * @return <0表示失败，==0表示成功。
         */
        //using dc_cpureset = short  (*)(HANDLE icdev, unsigned char* rlen, unsigned char* databuffer);
        //dc_cpureset pdc_cpureset = nullptr;
        /**
         * @brief  接触式CPU卡指令交互。
         * @par    说明：
         * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
         * @param[in] icdev 设备标识符。
         * @param[in] slen 发送数据的长度。
         * @param[in] sendbuffer 发送数据。
         * @param[out] rlen 返回数据的长度。
         * @param[out] databuffer 返回的数据。
         * @return <0表示失败，==0表示成功。
         */
        //using dc_cpuapduInt = short (*)(HANDLE icdev, unsigned int slen, unsigned char* sendbuffer, unsigned int* rlen, unsigned char* databuffer);
        //dc_cpuapduInt pdc_cpuapduInt = nullptr;
        /**
         * @brief  设置当前接触式卡座。
         * @par    说明：
         * 设置当前接触式卡座为指定卡座，用于多卡座切换卡操作。
         * @param[in] icdev 设备标识符。
         * @param[in] _Byte 卡座编号。
         * @n 0x0C - 附卡座/接触式CPU1卡座。
         * @n 0x0B - 接触式CPU2卡座。
         * @n 0x0D - SAM1卡座。
         * @n 0x0E - SAM2卡座。
         * @n 0x0F - SAM3卡座。
         * @n 0x11 - SAM4卡座。
         * @n 0x12 - SAM5卡座。
         * @n 0x13 - SAM6卡座/ESAM芯片。
         * @n 0x14 - SAM7卡座。
         * @n 0x15 - SAM8卡座。
         * @return <0表示失败，==0表示成功。
         */
        //using dc_setcpu = short (*)(HANDLE icdev, unsigned char _Byte);
        //dc_setcpu pdc_setcpu = nullptr;
        jobject jObj = nullptr;
        JNIEnv *pJniEnv = nullptr;

        volatile bool bThreadRun;
        QEvolisPrinter  *pEvolisPriner = nullptr;
        //QEvolisCtrl     *pEvolisCtrl = nullptr;


};

#endif // EVOLIS_Z390_LITHOGRAPHPRINTER_H
