
/**
 * @author W
 * @version Version 1.0.1
 * @date 2017-03-07
 * @copyright D&C
 */


#ifndef MOBILEDEVSLLIBRARY_BASICOPER_H
#define MOBILEDEVSLLIBRARY_BASICOPER_H


#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief 设置参数
* @attention 在调用跨地区库和银医通库之前先设置参数
*
* @param[in]   pt      0:libusb通讯, 1:串口通讯, 2:系统USB通讯
* @param[in]   spp        如果pt=1选择串口，此参数为串口路径
* @param[in]   baudRate    波特率一般都为115200
* @return <0表示失败 ==0成功
*/
int setInitParameter(unsigned char pt, const char * spp = nullptr, int br = 0);
/**
* @brief 设置剩上下文
* @attention 选择系统USB通讯时，应该调用此参数传入activity上下文
*
* @param[in]   androidContext   activity上下文
* @return
*/
void create_android_context(void *androidContext);
/**
* @brief 设置JVM
* @attention 选择系统USB通讯时，应该调用此参数传入Java虚拟机环境
*
* @param[in]   vm   JVM
* @return
*/
void create_JavaVM(JavaVM* vm);
/**
* @brief 设置键盘类型
* @attention 读卡器接P3、P30此参数type传1，M300JC传2
*
* @param[in]   type
* @return
*/
void setKeyboardType(int type);
/**
 * @brief 打开端口
 * @attention 此接口专门针对Android平台设计，使用C语言调用的用户，必须调用此接口进行初始化工作，而不是调用dcrf32.h中的 ::dc_init 方法，需要特别注意。
 *
 * @par    说明：建立设备的通讯并且分配相应的资源，大部分功能接口都需要在此过程后才能进行，在不需要使用设备后，必须使用 ::dc_exit 去关闭设备的通讯和释放资源。
 * @param[in]   ucType      0：libusb通讯, 1:串口通讯 2:系统USB通讯
 * @param[in]   path        串口路径，只针对串口模式有效。
 * @param[in]   baudRate    波特率，只针对串口模式有效。
 * @param[in]   androidContext   activity上下文（选择系统USB通讯时，应该调用此参数传入activity上下文）
 * @param[in]   vm   JVM     （选择系统USB通讯时，应该调用此参数传入Java虚拟机环境）
 * @return <0表示失败，否则为设备标识符 (请保存，后续的函数，有可能必须传入此值) 。
 */
short dc_Open(unsigned char ucType, const char* path = nullptr, int baudRate = 0, void *androidContext = nullptr, JavaVM* vm = nullptr, void (* setAuthorizationSuccess)(void) = nullptr);

/**
 *
 * @brief 关闭端口
 *
 * @par    说明：关闭设备的通讯和释放资源。
 * @return <0表示失败，==0表示成功。
 */
short dc_Close();
/**
 * @brief 获取SDK版本信息
 *
 * @par   说明：获取SDK开发包的版本信息，至少分配64个字节。
 * @attention 至少分配64个字节。
 * @param[in,out] pszVersion 版本信息
 * @return <0表示失败，==0表示成功。
 */
short dc_GetSdkVersion(char* pszVersion);



/**
 * @brief 获取身份证信息
 *
 * @par   说明：读取身份证数据，包含基础文字和相片内容
 * @param[in,out] pszName       姓名              至少分配64个字节
 * @param[in,out] pszSex        性别              至少分配8个字节
 * @param[in,out] pszNation     民族              至少分配16个字节
 * @param[in,out] pszBirthday   生日              至少分配64个字节
 * @param[in,out] pszAddress    住址              至少分配256个字节
 * @param[in,out] pszIdNum      证件号码           至少分配32个字节
 * @param[in,out] pszOrgan      签发机关           至少分配128个字节
 * @param[in,out] pszStartTime  生效开始日期       至少分配64个字节
 * @param[in,out] pszEndTime    生效结束日期       至少分配64个字节
 * @param[in,out] pucBmpBuffer  身份证头像元数据    固定分配38862个字节
 * @return <0表示失败，==0表示成功。
 */
short dc_GetIdRawBuffer(char *pszName,char *pszSex,char *pszNation,
                        char* pszBirthday,char *pszAddress,char *pszIdNum,
                        char *pszOrgan,char *pszStartTime,char *pszEndTime,
                        unsigned char *pucBmpBuffer);

/**
 * @brief  读身份证。
 * @par    说明：
 * 读取身份证的原始信息数据。
 * @param[in] icdev 设备标识符。
 * @param[in] type 类型。
 * @n 1 - 读取文字信息、相片信息和指纹信息。
 * @n 2 - 读取追加住址信息。
 * @n 3 - 读取文字信息、相片信息、指纹信息和追加住址信息。
 * @param[out] text_len 返回文字信息的长度。
 * @param[out] text 返回的文字信息   中国（姓名|性别|民族|生日|住址|身份证号码|发卡机关|生效起始日期|生效结束日期）；
 * 外国（姓名|性别|永久居留证号码|国籍或所在地区代码|中文姓名|证件签发日期|证件终止日期|出生日期|证件版本号|当次申请受理机关代码|证件类型标识|预留项）；
 * 港澳台（姓名|性别|预留项|出生日期|住址|公民身份号码|签发机关|有效期起始日期|有效期截止日期|通行证号码|签发次数|预留区|证件类型标识|预留区）
 * @param[out] photo_len 返回相片信息的长度。
 * @param[out] photo 返回的相片信息，请至少分配54 + 102 * 126 * 3 + 126 * 2个字节。
 * @param[out] fingerprint_len 返回指纹信息的长度。
 * @param[out] fingerprint 返回的指纹信息，请至少分配1024个字节。
 * @param[out] extra_len 返回追加住址信息的长度。
 * @param[out] extra 返回的追加住址信息。
 * @return <0表示失败，==0表示成功。
 */
short dc_SamAReadCardInfoString(int type, int *text_len, unsigned char *text, int *photo_len, unsigned char *photo, int *fingerprint_len, unsigned char *fingerprint, int *extra_len, unsigned char *extra);

int dc_StartBardCodeLoopReceive();
void dc_StopBardCodeLoopReceive();

void* dc_BardCodeReceive(void* arg);

int dc_UserDefinedTransData(unsigned char* pucCmdData, unsigned int uiCmdLen,
                            unsigned char* pucResData, unsigned int* puiResLen, unsigned int timeout);

extern unsigned int uiFlag;
extern pthread_t id;

/**
 * 使读卡器进入升级状态
 * @return 0：成功 ，其他：失败
 */
int dc_DfuAccess();

/**
 * 升级开始
 * @param[in] ucFileType 升级文件类型：0-应用程序；1-字库程序
 * @param[in] uiFileLen 升级文件总长度
 * @param[in,out] puiFirstPackgeLen 读卡器请求第一包的数据长度
 * @return 0：成功 ，其他：失败
 */
int dc_DfuBegin(unsigned char ucFileType,unsigned int uiFileLen,unsigned int* puiFirstPackgeLen);

/**
 * 升级过程
 * @param ucFileType 升级文件类型：0-应用程序；1-字库程序
 * @param uiThisTimePackgeLen 本次下发文件数据包长度
 * @param pucThisTimePackgeData 本次下发文件数据
 * @param puiNextTimeRequestPackgeLen 读卡器请求下次数据包长度
 * @return 0：成功 ，其他：失败
 */
int dc_DfuPocess(unsigned char ucFileType,unsigned int uiThisTimePackgeLen,unsigned char *pucThisTimePackgeData, unsigned int* puiNextTimeRequestPackgeLen);

int dc_WordStockBegin(unsigned char ucFileType, unsigned int uiFileLen, unsigned int * puiFirstPackgeLen);

int dc_WordStockPocess(unsigned char ucFileType, unsigned int packageNum, unsigned int uiFileLen, unsigned int uiThisTimePackgeLen, unsigned char *pucThisTimePackgeData);

#ifdef __cplusplus
}
#endif

#endif //MOBILEDEVSLLIBRARY_CMETHOD_H
