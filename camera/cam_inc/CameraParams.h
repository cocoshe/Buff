
#ifndef _MV_CAMERA_PARAMS_H_
#define _MV_CAMERA_PARAMS_H_

#include "PixelType.h"

// ??
#define MV_UNKNOW_DEVICE        0x00000000          // ????
#define MV_GIGE_DEVICE          0x00000001          // GigE?
#define MV_1394_DEVICE          0x00000002          // 1394-a/b ?
#define MV_USB_DEVICE           0x00000004          // USB3.0 ?
#define MV_CAMERALINK_DEVICE    0x00000008          // CameraLink?

typedef struct _MV_GIGE_DEVICE_INFO_
{
    unsigned int        nIpCfgOption;        
    unsigned int        nIpCfgCurrent;       //IP configuration:bit31-static bit30-dhcp bit29-lla          
    unsigned int        nCurrentIp;          //curtent ip          
    unsigned int        nCurrentSubNetMask;  //curtent subnet mask             
    unsigned int        nDefultGateWay;      //current gateway
    unsigned char       chManufacturerName[32];
    unsigned char       chModelName[32];
    unsigned char       chDeviceVersion[32];
    unsigned char       chManufacturerSpecificInfo[48];
    unsigned char       chSerialNumber[16];
    unsigned char       chUserDefinedName[16];      

    unsigned int        nNetExport;         // IP?

    unsigned int        nReserved[4];

}MV_GIGE_DEVICE_INFO;

#define INFO_MAX_BUFFER_SIZE 64
typedef struct _MV_USB3_DEVICE_INFO_
{
    unsigned char           CrtlInEndPoint;                        //?
    unsigned char           CrtlOutEndPoint;                       //?
    unsigned char           StreamEndPoint;                        //?
    unsigned char           EventEndPoint;                         //¼?
    unsigned short          idVendor;                              //?ID
    unsigned short          idProduct;                             //?ID
    unsigned int            nDeviceNumber;                         //?? 
    unsigned char           chDeviceGUID[INFO_MAX_BUFFER_SIZE];    //?GUID
    unsigned char           chVendorName[INFO_MAX_BUFFER_SIZE];    //?
    unsigned char           chModelName[INFO_MAX_BUFFER_SIZE];     //?
    unsigned char           chFamilyName[INFO_MAX_BUFFER_SIZE];    //
    unsigned char           chDeviceVersion[INFO_MAX_BUFFER_SIZE];  //??
    unsigned char           chManufacturerName[INFO_MAX_BUFFER_SIZE]; //
    unsigned char           chSerialNumber[INFO_MAX_BUFFER_SIZE];      //?
    unsigned char           chUserDefinedName[INFO_MAX_BUFFER_SIZE];  //û?
    unsigned int            nbcdUSB;                               //??USB?
    unsigned int            nReserved[3];                             //?
}MV_USB3_DEVICE_INFO;

// ??
typedef struct _MV_CC_DEVICE_INFO_
{
    // common info 
    unsigned short      nMajorVer;
    unsigned short      nMinorVer;
    unsigned int        nMacAddrHigh;               // MAC ?
    unsigned int        nMacAddrLow;

    unsigned int        nTLayerType;                // ???e.g. MV_GIGE_DEVICE

    unsigned int        nReserved[4];

    union
    {
        MV_GIGE_DEVICE_INFO stGigEInfo;
        MV_USB3_DEVICE_INFO stUsb3VInfo; 
        // more ...
    }SpecialInfo;

}MV_CC_DEVICE_INFO;

// ??
typedef struct _MV_NETTRANS_INFO_
{
    int64_t         nReviceDataSize;    // ???  [?StartGrabbingStopGrabbing?]
    int             nThrowFrameCount;   // ?
    unsigned int    nReserved[5];

}MV_NETTRANS_INFO;


// ????
#define MV_MAX_TLS_NUM          8
// ???
#define MV_MAX_DEVICE_NUM      256

// ???
typedef struct _MV_CC_DEVICE_INFO_LIST_
{
    unsigned int            nDeviceNum;                         // ?
    MV_CC_DEVICE_INFO*      pDeviceInfo[MV_MAX_DEVICE_NUM];     // ?256?

}MV_CC_DEVICE_INFO_LIST;


// ??
typedef struct _MV_FRAME_OUT_INFO_
{
    unsigned short      nWidth;             // ?
    unsigned short      nHeight;            // ?
    enum MvGvspPixelType     enPixelType;        // ??

    /*???*/
    unsigned int        nFrameNum;          // ?
    unsigned int        nDevTimeStampHigh;  // ?32?
    unsigned int        nDevTimeStampLow;   // ?32?
    unsigned int        nReserved0;         // 8??
    int64_t             nHostTimeStamp;     // ??

    unsigned int        nFrameLen;

    unsigned int        nReserved[3];       // 
}MV_FRAME_OUT_INFO;

// ??
typedef struct _MV_FRAME_OUT_INFO_EX_
{
    unsigned short      nWidth;             // ?
    unsigned short      nHeight;            // ?
    enum MvGvspPixelType     enPixelType;        // ??

    /*???*/
    unsigned int        nFrameNum;          // ?
    unsigned int        nDevTimeStampHigh;  // ?32?
    unsigned int        nDevTimeStampLow;   // ?32?
    unsigned int        nReserved0;         // 8??
    int64_t             nHostTimeStamp;     // ??

    unsigned int        nFrameLen;

    // ?chunk???
    // ????
    unsigned int        nSecondCount;
    unsigned int        nCycleCount;
    unsigned int        nCycleOffset;

    float               fGain;
    float               fExposureTime;
    unsigned int        nAverageBrightness;     //?

    // ?
    unsigned int        nRed;
    unsigned int        nGreen;
    unsigned int        nBlue;

    unsigned int        nFrameCounter;
    unsigned int        nTriggerIndex;      //

    //Line /
    unsigned int        nInput;        //
    unsigned int        nOutput;       //

    // ROI
    unsigned short      nOffsetX;
    unsigned short      nOffsetY;

    unsigned int        nReserved[41];       // 
}MV_FRAME_OUT_INFO_EX;

typedef struct _MV_DISPLAY_FRAME_INFO_
{
    void*                    hWnd;
    unsigned char*           pData;
    unsigned int             nDataLen;
    unsigned short           nWidth;             // ?
    unsigned short           nHeight;            // ?
    enum MvGvspPixelType     enPixelType;        // ??
    unsigned int             nRes[4];

}MV_DISPLAY_FRAME_INFO;

// ???
enum MV_SAVE_IAMGE_TYPE
{
    MV_Image_Undefined                 = 0,
    MV_Image_Bmp                       = 1,
    MV_Image_Jpeg                      = 2,
    MV_Image_Png                       = 3,     //?
    MV_Image_Tif                       = 4,     //?
};
// ??
typedef struct _MV_SAVE_IMAGE_PARAM_T_
{
    unsigned char*        pData;              // [IN]     ?
    unsigned int          nDataLen;           // [IN]     ??
    enum MvGvspPixelType       enPixelType;        // [IN]     ???
    unsigned short        nWidth;             // [IN]     ?
    unsigned short        nHeight;            // [IN]     ?

    unsigned char*        pImageBuffer;       // [OUT]    ??
    unsigned int          nImageLen;          // [OUT]    ???
    unsigned int          nBufferSize;        // [IN]     ??
    enum MV_SAVE_IAMGE_TYPE    enImageType;        // [IN]     ???

}MV_SAVE_IMAGE_PARAM;

// ??
typedef struct _MV_SAVE_IMAGE_PARAM_T_EX_
{
    unsigned char*      pData;              // [IN]     ?
    unsigned int        nDataLen;           // [IN]     ??
    enum MvGvspPixelType     enPixelType;        // [IN]     ???
    unsigned short      nWidth;             // [IN]     ?
    unsigned short      nHeight;            // [IN]     ?

    unsigned char*      pImageBuffer;       // [OUT]    ??
    unsigned int        nImageLen;          // [OUT]    ???
    unsigned int        nBufferSize;        // [IN]     ??
    enum MV_SAVE_IAMGE_TYPE  enImageType;        // [IN]     ???
    unsigned int        nJpgQuality;        // [IN]     , (50-99]

    // [IN]     Bayer???RGB24??  0- 1-? 2-Hamilton
    unsigned int        iMethodValue;
    unsigned int        nReserved[3];

}MV_SAVE_IMAGE_PARAM_EX;

// ???
typedef struct _MV_PIXEL_CONVERT_PARAM_T_
{
    unsigned short      nWidth;             // [IN]     ?
    unsigned short      nHeight;            // [IN]     ?

    enum MvGvspPixelType    enSrcPixelType;     // [IN]     ???
    unsigned char*      pSrcData;           // [IN]     ?
    unsigned int        nSrcDataLen;        // [IN]     ??

    enum MvGvspPixelType    enDstPixelType;     // [IN]     ???
    unsigned char*      pDstBuffer;         // [OUT]    ?
    unsigned int        nDstLen;            // [OUT]    ??
    unsigned int        nDstBufferSize;     // [IN]     ??
    unsigned int        nRes[4];
}MV_CC_PIXEL_CONVERT_PARAM;



// ???
typedef enum _MV_CAM_ACQUISITION_MODE_
{
    MV_ACQ_MODE_SINGLE      = 0,            // ???
    MV_ACQ_MODE_MUTLI       = 1,            // ???
    MV_ACQ_MODE_CONTINUOUS  = 2,            // ???

}MV_CAM_ACQUISITION_MODE;

// ??
typedef enum _MV_CAM_GAIN_MODE_
{
    MV_GAIN_MODE_OFF        = 0,            // ?
    MV_GAIN_MODE_ONCE       = 1,            // ?
    MV_GAIN_MODE_CONTINUOUS = 2,            // 

}MV_CAM_GAIN_MODE;

// ???
typedef enum _MV_CAM_EXPOSURE_MODE_
{
    MV_EXPOSURE_MODE_TIMED          = 0,            // Timed
    MV_EXPOSURE_MODE_TRIGGER_WIDTH  = 1,            // TriggerWidth
}MV_CAM_EXPOSURE_MODE;

// ????
typedef enum _MV_CAM_EXPOSURE_AUTO_MODE_
{
    MV_EXPOSURE_AUTO_MODE_OFF        = 0,            // ?
    MV_EXPOSURE_AUTO_MODE_ONCE       = 1,            // ?
    MV_EXPOSURE_AUTO_MODE_CONTINUOUS = 2,            // 

}MV_CAM_EXPOSURE_AUTO_MODE;

typedef enum _MV_CAM_TRIGGER_MODE_
{
    MV_TRIGGER_MODE_OFF         = 0,            // ?
    MV_TRIGGER_MODE_ON          = 1,            // 

}MV_CAM_TRIGGER_MODE;

typedef enum _MV_CAM_GAMMA_SELECTOR_
{
    MV_GAMMA_SELECTOR_USER      = 1,
    MV_GAMMA_SELECTOR_SRGB      = 2,

}MV_CAM_GAMMA_SELECTOR;

typedef enum _MV_CAM_BALANCEWHITE_AUTO_
{
    MV_BALANCEWHITE_AUTO_OFF            = 0,
    MV_BALANCEWHITE_AUTO_ONCE           = 2,
    MV_BALANCEWHITE_AUTO_CONTINUOUS     = 1,            // 

}MV_CAM_BALANCEWHITE_AUTO;

typedef enum _MV_CAM_TRIGGER_SOURCE_
{
    MV_TRIGGER_SOURCE_LINE0             = 0,
    MV_TRIGGER_SOURCE_LINE1             = 1,
    MV_TRIGGER_SOURCE_LINE2             = 2,
    MV_TRIGGER_SOURCE_LINE3             = 3,
    MV_TRIGGER_SOURCE_COUNTER0          = 4,

    MV_TRIGGER_SOURCE_SOFTWARE          = 7,
    MV_TRIGGER_SOURCE_FrequencyConverter= 8,

}MV_CAM_TRIGGER_SOURCE;


// GigEVision IP Configuration
#define MV_IP_CFG_STATIC        0x05000000
#define MV_IP_CFG_DHCP          0x06000000
#define MV_IP_CFG_LLA           0x04000000

// GigEVision Net Transfer Mode
#define MV_NET_TRANS_DRIVER     0x00000001
#define MV_NET_TRANS_SOCKET     0x00000002


// ?
#define MV_MATCH_TYPE_NET_DETECT             0x00000001      // ??
#define MV_MATCH_TYPE_USB_DETECT             0x00000002      // host?U3V??


// ??????
#define MV_MAX_XML_NODE_NUM_C       128

// ????
#define MV_MAX_XML_NODE_STRLEN_C    64

//?String???
#define MV_MAX_XML_STRVALUE_STRLEN_C 64

// ????
#define MV_MAX_XML_DISC_STRLEN_C    512

// ??
#define MV_MAX_XML_ENTRY_NUM        10

// ?
#define MV_MAX_XML_PARENTS_NUM      8

//ÿ?????
#define MV_MAX_XML_SYMBOLIC_STRLEN_C 64

#define MV_MAX_XML_SYMBOLIC_NUM      64

//??

// ?????
typedef struct _MV_ALL_MATCH_INFO_
{
    unsigned int    nType;          // ???e.g. MV_MATCH_TYPE_NET_DETECT
    void*           pInfo;          // ????
    unsigned int    nInfoSize;      // ???

}MV_ALL_MATCH_INFO;

//  ?????? MV_MATCH_TYPE_NET_DETECT
typedef struct _MV_MATCH_INFO_NET_DETECT_
{
    int64_t         nReviceDataSize;    // ???  [?StartGrabbingStopGrabbing?]
    int64_t         nLostPacketCount;   // ??
    unsigned int    nLostFrameCount;    // ?
    unsigned int    nReserved[5];          // 
}MV_MATCH_INFO_NET_DETECT;

// host?u3v????? MV_MATCH_TYPE_USB_DETECT
typedef struct _MV_MATCH_INFO_USB_DETECT_
{
    int64_t         nReviceDataSize;      // ???    [?OpenDevicceCloseDevice?]
    unsigned int    nRevicedFrameCount;   // ??
    unsigned int    nErrorFrameCount;     // ?
    unsigned int    nReserved[2];         // 
}MV_MATCH_INFO_USB_DETECT;

typedef struct _MV_IMAGE_BASIC_INFO_
{
    // width
    unsigned short      nWidthValue;
    unsigned short      nWidthMin;
    unsigned int        nWidthMax;
    unsigned int        nWidthInc;

    // height
    unsigned int        nHeightValue;
    unsigned int        nHeightMin;
    unsigned int        nHeightMax;
    unsigned int        nHeightInc;

    // framerate
    float               fFrameRateValue;
    float               fFrameRateMin;
    float               fFrameRateMax;

    // ??
    unsigned int        enPixelType;                // ???
    unsigned int        nSupportedPixelFmtNum;      // ????
    unsigned int        enPixelList[MV_MAX_XML_SYMBOLIC_NUM];
    unsigned int        nReserved[8];

}MV_IMAGE_BASIC_INFO;

//  ??
#define MV_EXCEPTION_DEV_DISCONNECT     0x00008001      // ??
#define MV_EXCEPTION_VERSION_CHECK      0x00008002      // SDK??


// ????
// ???APP?CCP?
#define MV_ACCESS_Exclusive                                         1
// ?5??????????
#define MV_ACCESS_ExclusiveWithSwitch                               2
// ??APP??
#define MV_ACCESS_Control                                           3
// ?5?????????
#define MV_ACCESS_ControlWithSwitch                                 4
// ??????
#define MV_ACCESS_ControlSwitchEnable                               5
// ?5????????????
#define MV_ACCESS_ControlSwitchEnableWithKey                        6
// ?????
#define MV_ACCESS_Monitor                                           7


/************************************************************************/
/* ?GenICamC??                                     */
/************************************************************************/

// ÿ????
enum MV_XML_InterfaceType
{
    IFT_IValue,         //!> IValue interface
    IFT_IBase,          //!> IBase interface
    IFT_IInteger,       //!> IInteger interface
    IFT_IBoolean,       //!> IBoolean interface
    IFT_ICommand,       //!> ICommand interface
    IFT_IFloat,         //!> IFloat interface
    IFT_IString,        //!> IString interface
    IFT_IRegister,      //!> IRegister interface
    IFT_ICategory,      //!> ICategory interface
    IFT_IEnumeration,   //!> IEnumeration interface
    IFT_IEnumEntry,     //!> IEnumEntry interface
    IFT_IPort,          //!> IPort interface
};

// ????
enum MV_XML_AccessMode
{
    AM_NI,          //!< Not implemented
    AM_NA,          //!< Not available
    AM_WO,          //!< Write Only
    AM_RO,          //!< Read Only
    AM_RW,          //!< Read and Write
    AM_Undefined,   //!< Object is not yet initialized
    AM_CycleDetect, //!< used internally for AccessMode cycle detection

};

enum MV_XML_Visibility
{
    V_Beginner      = 0,    //!< Always visible
    V_Expert        = 1,    //!< Visible for experts or Gurus
    V_Guru          = 2,    //!< Visible for Gurus
    V_Invisible     = 3,    //!< Not Visible
    V_Undefined     = 99    //!< Object is not yet initialized
};

typedef enum _MV_GIGE_EVENT_
{
    MV_EVENT_ExposureEnd                 = 1,           // ÿ??
    MV_EVENT_FrameStartOvertrigger       = 2,           // ??
    MV_EVENT_AcquisitionStartOvertrigger = 3,           // ??
    MV_EVENT_FrameStart                  = 4,           // ÿ???
    MV_EVENT_AcquisitionStart            = 5,           // ?????
    MV_EVENT_EventOverrun                = 6            // ¼
}MV_GIGE_EVENT;


//

// ?
typedef struct _MV_XML_NODE_FEATURE_
{
    enum MV_XML_InterfaceType    enType;                             // ?
    enum MV_XML_Visibility       enVisivility;                       //??
    char                    strDescription[MV_MAX_XML_DISC_STRLEN_C];//?     ????
    char                    strDisplayName[MV_MAX_XML_NODE_STRLEN_C];//?
    char                    strName[MV_MAX_XML_NODE_STRLEN_C];  // ?
    char                    strToolTip[MV_MAX_XML_DISC_STRLEN_C];  //?

    unsigned int            nReserved[4];
}MV_XML_NODE_FEATURE;

// ??
typedef struct _MV_XML_NODES_LIST_
{
    unsigned int            nNodeNum;       // ?
    MV_XML_NODE_FEATURE     stNodes[MV_MAX_XML_NODE_NUM_C];
}MV_XML_NODES_LIST;



typedef struct _MV_XML_FEATURE_Value_
{
    enum MV_XML_InterfaceType    enType;                             // ?
    char                    strDescription[MV_MAX_XML_DISC_STRLEN_C];//?     ????
    char                    strDisplayName[MV_MAX_XML_NODE_STRLEN_C];//?
    char                    strName[MV_MAX_XML_NODE_STRLEN_C];  // ?
    char                    strToolTip[MV_MAX_XML_DISC_STRLEN_C];  //?
    unsigned int            nReserved[4];
}MV_XML_FEATURE_Value;

typedef struct _MV_XML_FEATURE_Base_
{
    enum MV_XML_AccessMode   enAccessMode;       // ??
}MV_XML_FEATURE_Base;

typedef struct _MV_XML_FEATURE_Integer_
{
    char                strName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDisplayName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDescription[MV_MAX_XML_DISC_STRLEN_C];   // ????
    char                strToolTip[MV_MAX_XML_DISC_STRLEN_C];

    enum MV_XML_Visibility   enVisivility;                       //??
    enum MV_XML_AccessMode   enAccessMode;       // ??
    int                 bIsLocked;          // ?0-1-    ????
    int64_t             nValue;             // ??
    int64_t             nMinValue;          // ??
    int64_t             nMaxValue;          // ?
    int64_t             nIncrement;         // 

    unsigned int        nReserved[4];

}MV_XML_FEATURE_Integer;

typedef struct _MV_XML_FEATURE_Boolean_
{
    char                strName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDisplayName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDescription[MV_MAX_XML_DISC_STRLEN_C];   // ????
    char                strToolTip[MV_MAX_XML_DISC_STRLEN_C];

    enum MV_XML_Visibility   enVisivility;                       //??
    enum MV_XML_AccessMode   enAccessMode;       // ??
    int                 bIsLocked;          // ?0-1-    ????
    bool                bValue;             // ??

    unsigned int        nReserved[4];
}MV_XML_FEATURE_Boolean;

typedef struct _MV_XML_FEATURE_Command_
{
    char                strName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDisplayName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDescription[MV_MAX_XML_DISC_STRLEN_C];   // ????
    char                strToolTip[MV_MAX_XML_DISC_STRLEN_C];

    enum MV_XML_Visibility   enVisivility;                       //??
    enum MV_XML_AccessMode   enAccessMode;       // ??
    int                 bIsLocked;          // ?0-1-    ????

    unsigned int        nReserved[4];
}MV_XML_FEATURE_Command;

typedef struct _MV_XML_FEATURE_Float_
{
    char                strName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDisplayName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDescription[MV_MAX_XML_DISC_STRLEN_C];   // ????
    char                strToolTip[MV_MAX_XML_DISC_STRLEN_C];

    enum MV_XML_Visibility       enVisivility;                       //??
    enum MV_XML_AccessMode   enAccessMode;       // ??
    int                 bIsLocked;          // ?0-1-    ????
    double              dfValue;             // ??
    double              dfMinValue;          // ??
    double              dfMaxValue;          // ?
    double              dfIncrement;         // 

    unsigned int        nReserved[4];
}MV_XML_FEATURE_Float;

typedef struct _MV_XML_FEATURE_String_
{
    char                strName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDisplayName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDescription[MV_MAX_XML_DISC_STRLEN_C];   // ????
    char                strToolTip[MV_MAX_XML_DISC_STRLEN_C];

    enum MV_XML_Visibility       enVisivility;                       //??
    enum MV_XML_AccessMode   enAccessMode;       // ??
    int                 bIsLocked;          // ?0-1-    ????
    char                strValue[MV_MAX_XML_STRVALUE_STRLEN_C];// ??

    unsigned int        nReserved[4];
}MV_XML_FEATURE_String;

typedef struct _MV_XML_FEATURE_Register_
{
    char                strName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDisplayName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDescription[MV_MAX_XML_DISC_STRLEN_C];   // ????
    char                strToolTip[MV_MAX_XML_DISC_STRLEN_C];

    enum MV_XML_Visibility       enVisivility;                       //??
    enum MV_XML_AccessMode   enAccessMode;       // ??
    int                 bIsLocked;          // ?0-1-    ????
    int64_t             nAddrValue;             // ??

    unsigned int        nReserved[4];
}MV_XML_FEATURE_Register;

typedef struct _MV_XML_FEATURE_Category_
{
    char                    strDescription[MV_MAX_XML_DISC_STRLEN_C];//? ????
    char                    strDisplayName[MV_MAX_XML_NODE_STRLEN_C];//?
    char                    strName[MV_MAX_XML_NODE_STRLEN_C];  // ?
    char                    strToolTip[MV_MAX_XML_DISC_STRLEN_C];  //?

    enum MV_XML_Visibility       enVisivility;                       //??

    unsigned int            nReserved[4];
}MV_XML_FEATURE_Category;

typedef struct _MV_XML_FEATURE_EnumEntry_
{
    char                strName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDisplayName[MV_MAX_XML_NODE_STRLEN_C];
    char                strDescription[MV_MAX_XML_DISC_STRLEN_C];   // ????
    char                strToolTip[MV_MAX_XML_DISC_STRLEN_C];
    int                 bIsImplemented;
    int                 nParentsNum;
    MV_XML_NODE_FEATURE stParentsList[MV_MAX_XML_PARENTS_NUM];

    enum MV_XML_Visibility       enVisivility;                       //??
    int64_t             nValue;             // ??
    enum MV_XML_AccessMode   enAccessMode;       // ??
    int                 bIsLocked;          // ?0-1-    ????
    int                 nReserved[8];

}MV_XML_FEATURE_EnumEntry;


typedef struct _MV_XML_FEATURE_Enumeration_
{
    enum MV_XML_Visibility       enVisivility;                       //??
    char                    strDescription[MV_MAX_XML_DISC_STRLEN_C];//? ????
    char                    strDisplayName[MV_MAX_XML_NODE_STRLEN_C];//?
    char                    strName[MV_MAX_XML_NODE_STRLEN_C];  // ?
    char                    strToolTip[MV_MAX_XML_DISC_STRLEN_C];  //?

    int                 nSymbolicNum;          // Symbolic
    char                strCurrentSymbolic[MV_MAX_XML_SYMBOLIC_STRLEN_C];          // ?Symbolic
    char                strSymbolic[MV_MAX_XML_SYMBOLIC_NUM][MV_MAX_XML_SYMBOLIC_STRLEN_C];
    enum MV_XML_AccessMode   enAccessMode;       // ??
    int                 bIsLocked;          // ?0-1-    ????
    int64_t             nValue;             // ??

    unsigned int        nReserved[4];
}MV_XML_FEATURE_Enumeration;


typedef struct _MV_XML_FEATURE_Port_
{
    enum MV_XML_Visibility       enVisivility;                       //??
    char                    strDescription[MV_MAX_XML_DISC_STRLEN_C];//? ????
    char                    strDisplayName[MV_MAX_XML_NODE_STRLEN_C];//?
    char                    strName[MV_MAX_XML_NODE_STRLEN_C];  // ?
    char                    strToolTip[MV_MAX_XML_DISC_STRLEN_C];  //?

    enum MV_XML_AccessMode       enAccessMode;       // ??
    int                     bIsLocked;          // ?0-1-    ????

    unsigned int            nReserved[4];
}MV_XML_FEATURE_Port;



typedef struct _MV_XML_CAMERA_FEATURE_
{
    enum MV_XML_InterfaceType    enType;
    union
    {
        MV_XML_FEATURE_Integer      stIntegerFeature;
        MV_XML_FEATURE_Float        stFloatFeature;
        MV_XML_FEATURE_Enumeration  stEnumerationFeature;
        MV_XML_FEATURE_String       stStringFeature;
    }SpecialFeature;

}MV_XML_CAMERA_FEATURE;


typedef struct _MVCC_ENUMVALUE_T
{
    unsigned int    nCurValue;      // ??
    unsigned int    nSupportedNum;  // ???
    unsigned int    nSupportValue[MV_MAX_XML_SYMBOLIC_NUM];

    unsigned int    nReserved[4];
}MVCC_ENUMVALUE;

typedef struct _MVCC_INTVALUE_T
{
    unsigned int    nCurValue;      // ??
    unsigned int    nMax;
    unsigned int    nMin;
    unsigned int    nInc;

    unsigned int    nReserved[4];
}MVCC_INTVALUE;

typedef struct _MVCC_FLOATVALUE_T
{
    float           fCurValue;      // ??
    float           fMax;
    float           fMin;

    unsigned int    nReserved[4];
}MVCC_FLOATVALUE;

typedef struct _MVCC_STRINGVALUE_T
{
    char   chCurValue[256];      // ??

    unsigned int    nReserved[4];
}MVCC_STRINGVALUE;

#endif /* _MV_CAMERA_PARAMS_H_ */
