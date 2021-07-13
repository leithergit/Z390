#ifndef EVOLIS_AVA_PRINTERS_H
#define EVOLIS_AVA_PRINTERS_H

#include <stdint.h>
#include "evolis.h"
#define AVANSIA_IS_IBSY(asi) (asi.btHardwareStatus & 32) //!< Test the "I_Bsy" bit of `avansia_status_infos_t` struct.
#define AVANSIA_IS_IRDY(asi) (asi.btHardwareStatus & 16) //!< Test the "I_Rdy" bit of `avansia_status_infos_t` struct.
#define AVANSIA_IS_MPRT(asi) (asi.btHardwareStatus &  8) //!< Test the "M_Prt" bit of `avansia_status_infos_t` struct.
#define AVANSIA_IS_MERR(asi) (asi.btHardwareStatus &  4) //!< Test the "M_Err" bit of `avansia_status_infos_t` struct.
#define AVANSIA_IS_MBSY(asi) (asi.btHardwareStatus &  2) //!< Test the "M_Bsy" bit of `avansia_status_infos_t` struct.
#define AVANSIA_IS_MRDY(asi) (asi.btHardwareStatus &  1) //!< Test the "M_Rdy" bit of `avansia_status_infos_t` struct.

/**
 * List of print status returned by `avansia_print_status()` function.
 */
typedef enum avansia_print_status_e {
    /// No printing in progress.
    AVANSIA_PS_NONE,

    /// The printer waits for a card insertion.
    /// @see card_insert()
    /// @see EVOLIS_IT_MANUALNCMD
    AVANSIA_PS_STANDBY_INSERT,

    /// The printer is printing a card.
    AVANSIA_PS_PRINTING,

    /// The printer waits for a card ejection command.
    /// @see card_eject()
    /// @see EVOLIS_OT_STANDARD_STANDBY
    /// @see EVOLIS_OT_ERR_STANDBY
    AVANSIA_PS_STANDBY_EJECT,
} avansia_print_status_t;

/**
 * Available card orientations on ejection.
 */
typedef enum avansia_card_side_e {
    /// Default setting.
    AVANSIA_CS_DEFAULT = 0,

    /// Card will be ejected with front side up.
    AVANSIA_CS_FRONT_SIDE_UP = 1,

    /// Card will be ejected with back side up.
    AVANSIA_CS_FRONT_SIDE_DOWN = 2,
} avansia_card_side_t;

/**
 * A data structure that stores the printer error information.
 * Replaces `PR5600_PRINTER_SENSE` structure from the **EVLDAPI.dll** library.
 *
 * @see avansia_get_printer_status_infos
 * @see avansia_get_printer_status_infos2
 */
typedef struct avansia_sense_s {
    uint8_t    btErrorCode;                     //!< Stores a fixed value (11110000).
    uint8_t    btSegmentNumber;                 //!< Stores a fixed value (00000000).
    uint8_t    btSenseKey;                      //!< SenseKey
    uint8_t    btInfomation[4];                 //!< Indicates the difference between the byte length requested by the command and the transferred data length. This is a 4-byte big-endian value.
    uint8_t    btAdditionalSenseLength;         //!< Indicates the number of bytes of the additional sense data. Fixed to "0B" (hex).
    uint8_t    btCommandSpecificInformation[4]; //!< All bytes fixed to "0".
    uint8_t    btAdditionalSenseCode;           //!< Additional information about the error.
    uint8_t    btAdditionalSenseCodeQualifier;  //!< Additional information about the error.
    uint8_t    btFieldReplaceableUnitCode;      //!< Information about the faulty unit.
    uint8_t    btSenseKeySpecific[3];           //!< Fixed to "0".
    uint8_t    btAdditionalSenseBytes;          //!< Fixed to "0".
    uint8_t    btErrorClassCode;                //!< Error class code managed by the printer.
    uint8_t    btAdditionalErrorCode;           //!< Detailed error code managed by the printer.
    uint8_t    btErrorUnitCode;                 //!< A code that indicates the unit managing "btErrorClassCode" and "btAdditionalErrorCode".
} avansia_sense_t;

/**
 * A data structure that stores the printer's operational information.
 * Replaces the `PR5600_STATUS_INFO` structure from the **EVLDAPI.dll** library.
 *
 * @see avansia_get_printer_status_infos
 */
typedef struct avansia_status_infos_s {
    uint8_t     btHardwareStatus;               //!< Indicates the printer hardware status.
    uint8_t     btCardPosition;                 //!< Indicates the location of the card within the printer.
    uint8_t     btPrinterStatus;                //!< Indicates the printer's printing status.
    uint8_t     btEncoderStatus;                //!< Bits 0 to 3 indicate the operational status of the magnetic encoder unit while bits 4 and 7, the access status to the IC R/W.
    uint8_t     btMemoryStatus[2];              //!< Indicates the usage status of memory A (btMemoryStatus[0]) and memory B (btMemoryStatus[1]).
    uint8_t     btDataStatus[2];                //!< Indicates the print data reception status of memory A (btDataStatus[0]) and memory B (btDataStatus[1]).
    uint8_t     btOptionUnitStatus;             //!< Indicates special statuses of optional units.
    uint8_t     btTotalPrintNumber[3];          //!< Indicates the total of cards printed.
    uint8_t     btColorControlValue[3];         //!< Indicates the saturation adjustment values. Indexes are: "0" for Yellow, "1" for Magenta, and "2" for Cyan.
    char        acMagEncoderROMVersion[4];      //!< Indicates the magnetic encoder's ROM version.
    char        acHeatRollerROMVersion[4];      //!< Indicates the heat roller unit's ROM version.
    uint8_t     btICTagData[8];                 //!< Indicates the tag information of the card at the access position of IC R/W.
    uint8_t     btReserved;                     //!< Not used
} avansia_status_infos_t;

/**
 * It is the structure to store the extended operational information of the printer.
 * Replaces the `PR5600_STATUS_INFO2` structure from the **EVLDAPI.dll** library.
 *
 * @see avansia_get_printer_status_infos2
 */
typedef struct avansia_status_infos2_s {
    uint8_t     btHardwareStatus;               //!< Indicates the printer hardware status.
    uint8_t     btCardPosition;                 //!< Indicates the location of the card within the printer.
    uint8_t     btPrinterStatus;                //!< Indicates the printer's printing status.
    uint8_t     btEncoderStatus;                //!< Bits 0 to 3 indicate the operational status of the magnetic encoder unit while bits 4 and 7, the access status to the IC R/W.
    uint8_t     btMemoryStatus[2];              //!< Indicates the usage status of memory A (btMemoryStatus[0]) and memory B (btMemoryStatus[1]).
    uint8_t     btDataStatus[2];                //!< Indicates the print data reception status of memory A (btDataStatus[0]) and memory B (btDataStatus[1]).
    uint8_t     btOptionUnitStatus;             //!< Indicates special statuses of optional units.
    uint8_t     btTotalPrintNumber[3];          //!< Indicates the total of cards printed.
    uint8_t     btColorControlValue[3];         //!< Indicates the saturation adjustment values. Indexes are: "0" for Yellow, "1" for Magenta, and "2" for Cyan.
    char        acMagEncoderROMVersion[4];      //!< Indicates the magnetic encoder's ROM version.
    char        acHeatRollerROMVersion[4];      //!< Indicates the heat roller unit's ROM version.
    uint8_t     btICTagData[8];                 //!< Indicates the tag information of the card at the access position of IC R/W.
    uint8_t     btReserved[10];                 //!< Not used
    uint8_t     btStandbyStatus;                //!< It shows Standby Status of the printer.
    uint8_t     btRibbonRemainder;              //!< It shows rough residual quantity (%) of ink ribbon.
    uint8_t     btFilmRemainder;                //!< It shows rough residual quantity (%) of retransfer film.
    uint8_t     btReserved2[16];                //!< Not used
} avansia_status_infos2_t;

/**
 * A structure that hold `avansia_status_infos2_t` and `avansia_sense_t`. It
 * can be use when calling `evolis_status()` function with an Avansia printer.
 *
 * @see evolis_status()
 */
typedef struct avansia_status_s {
    avansia_status_infos2_t infos;
    avansia_sense_t sense;
} avansia_status_t;

/**
 * A data structure that stores the printer's system settings information.
 * Replaces `PR5600_SYSTEM_INFO` structure from the **EVLDAPI.dll** library.
 *
 * @see avansia_get_printer_system_infos
 */
typedef struct avansia_system_infos_s {
    uint8_t     btConfiguration[2];                         //!< Indicates the printer configuration information. Each bit indicates whether the option is connected (1) or not connected (0).
    uint8_t     btRibbonSetting;                            //!< Indicates the type of ribbon selected.
    uint8_t     btRibbonPanel;                              //!< Indicates the type of panel (type of data) that can be printed with the selected type of ribbon.
    uint8_t     btReserved1;                                //!< Not used.
    uint8_t     btMemoryMode;                               //!< Indicates the memory management method. Fixed to "02" (hex).
    uint8_t     btPrinterSetting;                           //!< Indicates settings related to the function for printing cards.
    uint8_t     btReserved2;                                //!< Not used.
    uint16_t    wdMaxHorizontalSizeImage;                   //!< Indicates the size of the area where image data can be printed.
    uint16_t    wdMaxVirticalSizeImage;                     //!< Indicates the size of the area where image data can be printed.
    uint16_t    wdMaxHorizontalSizeText;                    //!< Indicates the size of the area where text data can be printed.
    uint16_t    wdMaxVirticalSizeText;                      //!< Indicates the size of the area where text data can be printed.
    uint16_t    wdMaxHorizontalSizeOverlay;                 //!< Indicates the size of the area where protection layer data can be printed.
    uint16_t    wdMaxVirticalSizeOverlay;                   //!< Indicates the size of the area where protection layer data can be printed.
    uint16_t    wdStartHorizontalPositionImage;             //!< Indicates the starting position of the area where image data can be printed.
    uint16_t    wdStartVirticalPositionImage;               //!< Indicates the starting position of the area where image data can be printed.
    uint16_t    wdStartHorizontalPositionText;              //!< Indicates the starting position of the area where text data can be printed.
    uint16_t    wdStartVirticalPositionText;                //!< Indicates the starting position of the area where text data can be printed.
    uint16_t    wdStartHorizontalPositionOverlay;           //!< Indicates the starting position of the area where protection layer data can be printed.
    uint16_t    wdStartVirticalPositionOverlay;             //!< Indicates the starting position of the area where protection layer data can be printed.
    uint16_t    wdResolution;                               //!< Indicates the printer resolution in [dpi].
    char        acBaseRevision[4];                          //!< Indicates the base program version.
    char        acMainRevision[4];                          //!< Indicates the main program version.
    char        acSerialNumber[8];                          //!< Indicates the printer serial number.
    char        cPrinterID;                                 //!< Indicates the printer ID number.
} avansia_system_infos_t;

typedef struct avansia_ribbon_film_card_params_s {
    uint8_t btRibbonType;
    uint8_t btFilmType;
    uint8_t btRibbonNotice;
    uint8_t btFilmNotice;
    uint8_t btCardType;
    uint8_t btDecurlFront;
    uint8_t btDecurlBack;
    uint8_t btStdCardExit;
    uint8_t btErrCardExit;
    uint8_t btSetCardSide;
    avansia_card_side_t btCardEjectSide;
} avansia_ribbon_film_card_params_t;

/// Avansia: Get printer status.
EVOLIS_LIB int avansia_status(evolis_t* printer, avansia_status_t* status);

/// Avansia: Hardware reboot of the printer.
EVOLIS_LIB int avansia_hw_reset(evolis_t* printer, int timeout, char* timeouted);

/// Avansia: Simulate pressing the set button.
EVOLIS_LIB int avansia_press_set_button(evolis_t* printer);

/// Avansia: Get printing status.
EVOLIS_LIB int avansia_print_status(evolis_t* printer);

/// Avansia: Set card side up when ejecting.
EVOLIS_LIB int avansia_set_card_side(evolis_t* printer, avansia_card_side_t acs);

/// Avansia: Get printer status (Nisca complient).
EVOLIS_LIB int avansia_get_printer_status_infos(evolis_t* printer,
    avansia_status_infos_t* infos, avansia_sense_t* sense);

/// Avansia: Get extended printer status (Nisca complient).
EVOLIS_LIB int avansia_get_printer_status_infos2(evolis_t* printer,
    avansia_status_infos2_t* infos, size_t infosReqSize, size_t* infosRepSize,
    avansia_sense_t* sense, size_t senseReqSize, size_t* senseRepSize);

/// Avansia: Get printer settings.
EVOLIS_LIB int avansia_get_printer_system_infos(evolis_t* printer, avansia_system_infos_t* infos);

/// Avansia: Get ribbon, film and card params.
EVOLIS_LIB int avansia_get_ribbon_film_card_params(evolis_t* printer, avansia_ribbon_film_card_params_t* params);

/// Avansia: Set ribbon, film and card params.
EVOLIS_LIB int avansia_set_ribbon_film_card_params(evolis_t* printer, avansia_ribbon_film_card_params_t* params);

#endif // EVOLIS_AVA_PRINTERS_H
