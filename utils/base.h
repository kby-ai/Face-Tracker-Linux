#ifndef IDVERIFYDEMO_BASE_H
#define IDVERIFYDEMO_BASE_H

enum SDK_ERROR
{
	/**
	The API call returned with no error.
	*/
	SDK_SUCCESS = 0,

	SDK_MATCH_SUCCESS = 0,

	/**
	Activation key is invalid or unknown.
	*/
	SDK_ACTIVATION_SERIAL_UNKNOWN = 1,

	/**
	Activation key is expired.
	*/
	SDK_ACTIVATION_EXPIRED = 2,

	/**
	The API call without SDK initialization. You have to call initializeSDK()
	before calling this API.
	*/
	SDK_NOT_INITIALIZED = 3,

	/**
	initializeSDK() is called after SDK initialization.
	*/
	SDK_ALREADY_INITIALIZED = 4,

	/**
	There aren't resource file needed to initialize SDK in specified path.
	*/
	SDK_NO_RES_FILE = 5,
	/**
	No face detected in image.
	*/
	SDK_NOT_FACE_DETECTED = 6,

	/**
	match() is called without calling setIDCardPhoto() with no error.
	*/
	SDK_NONE_IDCARD_SET = 7,

	/**
	Parameters are corrupt.
	*/
	SDK_BAD_PARAMETER = 8,
	/*
	Inputed image is not ID photo.
	*/
	SDK_NOT_ID_PHOTO = 9,

	SDK_IMG_ERROR = 10,

	SDK_MATCH_FAILED = 11
};

enum FACE_SDK_ERROR
{
	/**
	The API call returned with no error.
	*/
	FACE_SDK_SUCCESS = 0,

	FACE_SDK_VERIFY_SUCCESS = 0,

	FACE_SDK_MATCH_SUCCESS = 0,

	FACE_SDK_LIVENESS_SUCCESS = 0,

	/**
	Activation key is invalid or unknown.
	*/
	FACE_SDK_ACTIVATION_SERIAL_UNKNOWN = 1,

	/**
	Activation key is expired.
	*/
	FACE_SDK_ACTIVATION_EXPIRED = 2,

	/**
	The API call without SDK initialization. You have to call initializeSDK()
	before calling this API.
	*/
	FACE_SDK_NOT_INITIALIZED = 3,

	/**
	initializeSDK() is called after SDK initialization.
	*/
	FACE_SDK_ALREADY_INITIALIZED = 4,

	/**
	There aren't resource file needed to initialize SDK in specified path.
	*/
	FACE_SDK_NO_RES_FILE = 5,
	/**
	No face detected in image.
	*/
	FACE_SDK_NOT_FACE_DETECTED = 6,

	/**
	match() is called without calling setIDCardPhoto() with no error.
	*/
	FACE_SDK_NOT_ENROLLED = 7,

	/**
	Parameters are corrupt.
	*/
	FACE_SDK_BAD_PARAMETER = 8,
	/*
	Inputed image is not ID photo.
	*/
	FACE_SDK_IMG_ERROR = 9,

	FACE_SDK_VERIFY_FAILED = 10,

	FACE_SDK_GROUP_ERROR = 11,
	
	FACE_SDK_DATABASE_ERROR = 12,

	FACE_SDK_PERSON_ERROR = 13,

	FACE_SDK_ERROR_PAN_OUT = 14,

	FACE_SDK_ERROR_BAT_IMAGE = 15,

	FACE_SDK_ERROR_MULTI_FACE = 16,

	FACE_SDK_MATCH_FAILED = 17,
	
	FACE_SDK_NONE_IDCARD_SET = 18,

	FACE_SDK_HWID_ERROR = 19,

	FACE_SDK_LIVENESS_FAIL = 20,

	FACE_SDK_LIVENESS_PROCESS = 21

};

#define ID_PHOTO_WIDTH	102
#define ID_PHOTO_HEIGHT	126

#define PROBE_IMAGE_MAX_WIDTH	1920
#define PROBE_IMAGE_MIN_WIDTH	1080

//DNN DICT VERSION
#define DT_IDVCD_1_0	0
#define DT_IDVCD15_1_3  1   //1:1
#define DT_E3_3_3       2   //1:N
#define DT_E3_4	        3   //1:N
#define DT_E3_3_0       4   //1:N Feature
#define DT_RN1_0        5   //1:N Feature
#define DICT_VERSION	DT_IDVCD_1_0 //DT_IDVCD_1_0

//ENGINE VERSION
#define	E_ORG			0
#define	E_FD_UP			1
#define	E_FD_MIX    	2

#define ENGINE_VERSION	E_FD_MIX

#if (ENGINE_VERSION == E_ORG)
    #define ID__ALL_DETECT_MODE						1
    #define ID__MIN_VALID_OVERLAPPED_REGION_COUNT	1
    #define ID__DELTA_CLASSIFIER_THRESHOLD			0.0f

    #define SCENE__ALL_DETECT_MODE					0
    #define SCENE__MIN_VALID_OVERLAPPED_REGION_COUNT	3
    #define SCENE__DELTA_CLASSIFIER_THRESHOLD			0.0f
#elif	(ENGINE_VERSION == E_FD_UP)
    #define ID__ALL_DETECT_MODE					1
    #define ID__MIN_VALID_OVERLAPPED_REGION_COUNT	1
    #define ID__DELTA_CLASSIFIER_THRESHOLD			3.0f

    #define SCENE__ALL_DETECT_MODE					1
    #define SCENE__MIN_VALID_OVERLAPPED_REGION_COUNT	1
    #define SCENE__DELTA_CLASSIFIER_THRESHOLD			3.0f
#elif (ENGINE_VERSION == E_FD_MIX)
    #define ID__ALL_DETECT_MODE						1
    #define ID__MIN_VALID_OVERLAPPED_REGION_COUNT	1
    #define ID__DELTA_CLASSIFIER_THRESHOLD			3.0f

    #define SCENE__ALL_DETECT_MODE					1
    #define SCENE__MIN_VALID_OVERLAPPED_REGION_COUNT	3
    #define SCENE__DELTA_CLASSIFIER_THRESHOLD			3.7f
#endif	//(ENGINE_VERSION == E_ORG)

#define MIN_FACE_WIDTH			40

#define PITCH_THRESHOLD							15
#define YAW_THRESHOLD							20
#define ROLL_THRESHOLD							20

#define MODEL_WEIGHT_1 "/kd1_e3_nir.tmp"
#define MODEL_WEIGHT_2 "/kd2_e3_nir.tmp"
#define MODEL_WEIGHT_3 "/kd3_e3_nir.bin"

/* Global variables*/
typedef struct _tagAlignParam
{
	int nFeatureSize;
	int nAlignWid;
	int nAlignHei;
	int nEyeMouthDis;
	int nEyeCenterXPos;
	int nEyeCenterYPos;
} AlignParam;

#define KDNN_SCALE	0.00390625

#define ID_SIZE_LIMIT			1		//0 -> limit, 1 -> all
#define FD_MIX_THRE_SIZE		480
#define USE_NORMAL_SCORE		1		//0 -> old, 1 -> new
//ResNet50_1.0
#define NORMAL_SRC_THREHOLD		82.7f
#define NORMAL_DST_THREHOLD		72.0f

#define FACE_SELECT_MODE		0		//0 -> max, 1 -> mix(minImageWidth > 480 ? max : (max + pos)

#define START_YEAR 2017
#define START_MONTH 7
#define START_DAY 20

//#define SAVE_LOG
#define DEBUG_EN 1

#endif //IDVERIFYDEMO_BASE_H
