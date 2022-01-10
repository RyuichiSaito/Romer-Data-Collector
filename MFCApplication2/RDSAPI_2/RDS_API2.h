#pragma once

/// Default VC++ packing value is 8, overiding of this value to be compliant with other environment like VB6
#pragma pack(push,4)


#include <windows.h>
#include <afxdocob.h>
#include <string.h>
#include <Winuser.h>
#include "RDS_API_Tools2.h"

#ifndef MAKEDWORDUSINGBYTES
#define	MAKEDWORDUSINGBYTES(b0, b1, b2, b3)	((b0<<24) + (b1<<16) + (b2<<8) + (b3<<0))
#endif

/// Identifies current RDS API version
#define	RDS_API_VERSION_STRING	"0.3.7"
#define	RDS_API_VERSION_VALUE		MAKEDWORDUSINGBYTES(0,3,7,0)
#define	RDS_API_VERSION_DATE	"07/24/2008"


/// Definition of types
#ifndef byte
typedef unsigned char byte;
#endif

#ifndef word
typedef unsigned short word;
#endif

#ifndef dword
typedef unsigned long dword;
#endif

#ifndef qword
typedef unsigned __int64 qword;
#endif


#define RDS_MAX_AXIS		8
#define RDS_MSG_1 _T("RDS_SERVERAVAILABLE")
#define	RDS_TM_CHECK_STATE_ID			(WM_USER + 0x100)	
#define	RDS_TM_CHECK_STATE_TIMER	(500)

/// Available chunks. Thid party doesn't have to use directly this enum
enum eRDSChunkID
{
	// Chunks Service <-> Clients
	RDSCI_PING = 0,														// Used to known if link is not broken

	// Chunks Service -> Clients
	RDSCI_ARM = 1000,													// Information regarding arm
	RDSCI_PROBE,															// Information regarding probe
	RDSCI_POSITION,														// Calculated XYZ position, button states and so on
	RDSCI_SCANNER_HEADER,											// Not feed yet, extended data when a scanner is enabled
	RDSCI_SCANNER_POINTS,											// Not feed yet, extended data when a scanner is enabled
	RDSCI_SERVER_SHUTTING_DOWN,								// Service is shutting down

	// Chunks Clients -> Service
	RDSCI_CONNECT = 2000,											// Client wants to connect to service
	RDSCI_DISCONNECT,													// Client disconnects itself

	RDSCI_PLAY_MELODY,												// Ask to service to play a melody
	RDSCI_CONTROL_REMOTE_MOUSE,								// Client control remote mouse with this chunk
	RDSCI_EXECUTE_SCRIPT,											// Calibration/Certification script execution request
	RDSCI_DISPLAY_CONTROL_PANEL,							// Display control panel
	RDSCI_DISPLAY_ADVANCED_CONTROL_PANEL,			// Display specific arm control panel

	RDSCI_UPDATE_SETTING,											// Update setting value (i.e UI language)

	RDSCI_RESERVED1,
	RDSCI_RESERVED2,
	RDSCI_RESERVED3,
	RDSCI_RESERVED4,
	RDSCI_RESERVED5,
	RDSCI_RESERVED6,
	RDSCI_RESERVED7,
	RDSCI_RESERVED8,
	RDSCI_RESERVED9,
	RDSCI_RESERVED10,
	RDSCI_RESERVED11,

	RDSCI_END_OF_LIST,

};

/// Codes returned by RDS Service functions
enum eRDSReturnCode
{
	RDSRC_SUCCESS = 0,
	RDSRC_UNKNOWN_ERROR = 1,
	RDSRC_NOT_CONNECTED = 2,
	RDSRC_EXECUTION_FAILED = 3,
	RDSRC_INVALID_HANDLE = 4,
	RDSRC_INVALID_PARAMETER = 5,
};

/// This enum defines the arm event returned to RDS client. See sRDSPosition::eventType
enum eRDSEvent
{
	UNSYNCED_DATA = 0x8000000,							// Data is not reliable, use it just for information
	EVENT_SYNC = 1,													// Data is reliable
	EVENT_PROBE_1 = 2,											// Probe triggered its first event (TP-20 for instance)
	EVENT_PROBE_2 = 4,											// Probe triggered its second event
	EVENT_BEAM1_BLCK_FWD = 8,								// Beam probe: BEAM1 cut when going down
	EVENT_BEAM1_UNBLCK_FWD = 0x10,					// Beam probe: BEAM1 released when going down
	EVENT_BEAM1_BLCK_BCKWD = 0x20,					// Beam probe: BEAM1 cut when going up
	EVENT_BEAM1_UNBLCK_BCKWD = 0x40,				// Beam probe: BEAM1 cut when going up
	EVENT_BEAM2_BLCK_FWD = 0x80,						// Beam probe: BEAM2 cut when going down
	EVENT_BEAM2_UNBLCK_FWD = 0x0100,				// Beam probe: BEAM2 released when going down
	EVENT_BEAM2_BLCK_BCKWD = 0x0200,				// Beam probe: BEAM2 cut when going up
	EVENT_BEAM2_UNBLCK_BCKWD = 0x0400,			// Beam probe: BEAM2 cut when going up
	EVENT_OVER_LIMIT = 0x0800,							// Axis over limit
	EVENT_BUTTON_1_WENT_UP = 0x1000,				// Arm button 1 up
	EVENT_BUTTON_2_WENT_UP = 0x2000,				// Arm button 2 up
	EVENT_BUTTON_3_WENT_UP = 0x4000,				// Arm button 3 up
	EVENT_BUTTON_1_WENT_DOWN = 0x8000,			// Arm button 1 down
	EVENT_BUTTON_2_WENT_DOWN = 0x10000,			// Arm button 2 down
	EVENT_BUTTON_3_WENT_DOWN = 0x20000,			// Arm button 3 down
	EVENT_BUTTON_1_DOWN_LONG = 0x40000,			// Arm button 1 long down
	EVENT_BUTTON_2_DOWN_LONG = 0x80000,			// Arm button 2 long down
	EVENT_BUTTON_3_DOWN_LONG = 0x100000,		// Arm button 3 long down

	/// \note JPDL 07/24/2008 : EVENT_EASY flag usage has been rehabilitated
	EVENT_EASY = 0x200000,									// Third party must check this flag to know if it's a measure or not.
																					// The cases this flag is set depend on current probe type (sRDSProbe.ProbeType):
																					//		- Contact probe: EVENT_SYNC is set, EVENT_BUTTON_2_WENT_UP is set
																					//		- Trigger probe: EVENT_SYNC is set, EVENT_PROBE_1 is set
																					//		- External trigger: EVENT_SYNC is set, EVENT_EXTERNAL_TRIGGER is set
																					//		- Smart Ray scanner: EVENT_SYNC is set, EVENT_EXTERNAL_TRIGGER is set

	/// \note JPDL 05/20/2008 : New values
	EVENT_EXTERNAL_SWITCH_WENT_UP = 0x0400000,		// External switch up
	EVENT_EXTERNAL_SWITCH_WENT_DOWN = 0x0800000,	// External switch down
	EVENT_EXTERNAL_SWITCH_DOWN_LONG = 0x1000000,	// External switch long down

	EVENT_EXTERNAL_TRIGGER = 0x2000000,			// Extern trigger 
};


/// Internal RDS structure
struct sRDSChunk
{
	eRDSChunkID	ChunkID;									// See eRDSChunkID for a list of available chunks
	long				Parameter1;								// Optional parameter, use depends on chunk ID
	long				Parameter2;								// Optional parameter, use depends on chunk ID
	long				ChunkSize;								// Size of data coming after structure

	char				Reserved[16];							// Reserved for further usage

	void Init(void) { memset(this, 0, sizeof(*this)); }
};

/// sRDSClient identifies which client sent the request to service
struct sRDSClient
{
	HWND		hwnd;													// handle of client window which will receive back msgs from service.

	UINT		extendedData;									/// \note JPDL 06/19/2008 : Not used anymore
	BOOL		RemoteMouseOn;								/// \note JPDL 06/19/2008 : Not used anymore
	UINT		Sound;												/// \note JPDL 06/19/2008 : Not used anymore
	UINT		freq;													/// \note JPDL 06/19/2008 : Not used anymore
	UINT		dur;													/// \note JPDL 06/19/2008 : Not used anymore
	UINT		reserved;											/// \note JPDL 06/19/2008 : Not used anymore

	void Init(void) { memset(this, 0, sizeof(*this)); }
};

/// Structure sent by client to service to identify itself.
/// It is sent during connection to RDS Service. See RDSCI_CONNECT command
struct sRDSConnect
{
	UINT		RDSAPIVersion;								// RDS API version used by client
	uchar		AuthenticationValue[8];				// Encrypted authentication value describing which features client has access to
																				// If field is = {0,0,0,0,0,0,0,0} -> Client is unidentified
	UINT		ExtendedData;									// For internal use only
	char		Reserved[256];								// Reserved for further usage

	void Init(void) { memset(this, 0, sizeof(*this)); }
};

/// Used with RDSCI_EXECUTE_SCRIPT chunk
enum eRDSCalibMethod
{
	CM_UNKNOWN = 0,
	CM_SINGLE_POINT = 10,
	CM_SPHERE_1 = 20,
	CM_SPHERE_2,													// For now, only this method works with Multi-Gage arms
	CM_PLANE = 30,
	CM_T = 40,
};

/// Used with RDSCI_EXECUTE_SCRIPT chunk
enum eRDSScriptType
{
	ST_UNKNOWN = 0,
	ST_PROBE_CALIBRATION,
	ST_CERTIFICATION,
};

/// Used with RDSCI_EXECUTE_SCRIPT chunk
struct sRDSScript
{
	int								X, Y, Width, Height;
	eRDSScriptType		ScriptType;
	eRDSCalibMethod		Method;

	char	Reserved[256];							// Reserved for further usage

	void Init(void) { memset(this, 0, sizeof(*this)); }
};

/// Identifies type of probe plugged in arm. See sRDSProbe::ProbeType. 
/// \note JPDL 05/06/2008 : New probe types
enum eProbeType
{
	PT_UNKNOWN = 0,							// Unknown type
	PT_V = 1,										// "V" probe
	PT_CONTACT = 2,							// Contact probe
	PT_PLA = 3,									// Managed by Sigma arms only
	PT_RENISHAW = 4,						// Managed by Sigma arms only
	PT_UDT = 5,									// Managed by Sigma arms only
	PT_VERTEX = 6,							// Managed by Sigma arms only
	PT_ROTATIF = 7,							// Managed by Sigma arms only
	PT_VIDEO_PROBE_V2 = 8,			// Managed by Sigma arms only
	PT_VIDEO_PROBE_V3 = 9,			// Managed by Sigma arms only
	PT_TRIGGER_PROBE = 10,			// Trigger probe
	PT_EXTERNAL_TRIGGER = 11,		// External trigger probe
};

/// Identifies probe status, see sRDSProbe structure
enum eProbeStatusBits
{
	PSB_SELECTED = (1 << 0),						// = 1 if probe is currently selected
	PSB_FORCED = (1 << 1),							// = 1 if probe has been forced
	PSB_QUALIFIED = (2 << 1),						// = 1 if probe has been correctly calibrated and that calculation result is good enough
};

#define	LEN_PROBE_TYPE	40
#define	LEN_PROBE_NAME	80


/// This structure describes the probe currently plugged in the arm.
/// It's used with RDSCI_PROBE chunk. This structure is sent from service to client every time probe selection changes
/// Important: There are two particular cases to take in account:
///              - If user is calibrating a new probe. This one is considered as missing in third-party side.
///              Probe information will be sent again to third party at end of calibration.
///              - Calibration calculation result is too bad (> 1.0). No probe data sent to third-party. 
///              Probe must be recalibrated.

struct sRDSProbe
{
	double				BallDiameter;									// Probe ball diameter in mm
	double				Length;												// Probe length in mm
	qword					ID;														// Unique 64bits probe ID, comes from chipset embed in probe
	eProbeType		ProbeType;										// Probe type, see eProbeType enum for a list of available types
	char					TypeName[LEN_PROBE_TYPE + 1];		// Readable probe type
	char					Name[LEN_PROBE_NAME + 1];				// Probe name
	SYSTEMTIME		CreationDateTime;							// Says when probe has been created in arm specs
	SYSTEMTIME		UpdateDateTime;								// Indicates last time probe specs have been changed (i.e. probe calibration)
	float					CalculationResult;						// Result of probe calculation. Warning! It's not a result in millimeters.
	dword					StatusBits;										// It's a combination of eProbeStatusBits enum bits
	int						Index;												// Probe index: -1 if no probe selected, 0 for first probe, 1 for second probe and so on
	char					Reserved[248];								// Reserved for further usage

	void Init(void) { memset(this, 0, sizeof(*this)); BallDiameter = 0; Length = 0; CalculationResult = 0; Index = -1; }
};

// Every tube and encoder composing an arm embeds an identification chipset.
// Maximum number of components is: 10 encoders + 2 tubes
#define	MAX_COMPONENT_IDS	(10 + 2) 


/// Identifies arm status, see sRDSArm structure
/// \note JPDL 07/09/2008 : eArmStatus enum: AS_CONNECTED_WARNING had a bad value.
enum eArmStatus
{
	AS_NOT_CONNECTED = 0,								// Check ArmStatusText for more detail
	AS_CONNECTED = 1,
	AS_CONNECTED_WARNING = 2,						// Check ArmStatusText for more detail
};


/// Identify type of arm configured using RDS Control Panel
enum eRDSArmType
{
	RDSAT_NCAUSB = 0,										// NCA arm, USB link
	RDSAT_NCAWIFI,											// NCA arm, WiFi link
	RDSAT_SIGMAETHERNET,								// Sigma arm, Wired ethernet link
	RDSAT_SIGMAWIFI,										// Sigma arm, WiFi link
	RDSAT_INFINITEUSB,									// Infinite arm, USB link
	RDSAT_INFINITEWIFI,									// Infinite arm, WiFi link,
	RDSAT_NOARM,												// No arm configured
};


#define	LEN_ARM_STATUS_TEXT	100

/// This structure describes the arm currently connected to service.
/// Used with RDSCI_ARM chunk
/// This structure is sent from service to client every 500 ms
struct sRDSArm
{
	byte				HardwareVersion;								// Hardware version, v1 -> Multi-Gage
	byte				MajorFWVersion;									// Version of firmware used by arm motherboard
	byte				MinorFWVersion;									// Version of firmware used by arm motherboard

	dword				NbAxis;													// Number of axis composing the arm
	dword				Tube1Length;										// First tube length (in millimeters)
	dword				Tube2Length;										// Second tube length (in millimeters)

	byte				BatteryLevel;										// Level of battery: 0 to 100%
	byte				ExternalTemperature;						// Multiply correct value by 0.5 for �C temperature (+0.5�C to +85�C)
	byte				SpeakerVolume;									// Speaker volume: 0 to 100%
	char				SerialNumber[40];								// Readable arm serial number

	/// \note JPDL 03/13/2008 : New field
	eRDSArmType	ArmType;												// See eArmType enum for a list of available values

	/// \note JPDL 04/01/2008 : New fields
	dword				NbComponents;										// Number of components
	qword				Components[MAX_COMPONENT_IDS];	// Component IDs

	/// \note JPDL 04/02/2008 : New field
	eArmStatus	ArmStatus;											// See eArmStatus enum for a list of available values
																							// Check ArmStatusText if ArmStatus != AS_NO_ERROR

	/// \note JPDL 05/27/2008 : New field
	dword				ArmID;													// Arm serial number

	/// \note JPDL 06/27/2008 : New field
	char				ArmStatusText[LEN_ARM_STATUS_TEXT + 1];		// Contains arm status description

	char				Reserved[151];													// Reserved for further usage

	void Init(void) 
	{ 
		memset(this, 0, sizeof(*this)); 
	}
};


/// Can be used to access sRDSPosition::ProbeXYZ field
enum {
	RDS_X = 0,
	RDS_Y = 1,
	RDS_Z = 2,

	RDS_I = 0,
	RDS_J = 1,
	RDS_K = 2,
};



/// This structure is sent from service to client every time a position has been captured from arm
/// Used with RDSCI_POSITION chunk
/// Be sure to analyze EventType to know if event is valid (AND operation with EVENT_SYNC value)
struct sRDSPosition
{
	dword		EventType;											// Event type. Combination of eRDSEvent enum.
	dword		RecordTime;											// Not used yet

	double	ProbeXYZ[3];										// Probe XYZ position (probe ball center)
	double	ProbeMatrix[3][3];							// Probe matrix
	double	ArmXYZ[3];											// Arm XYZ position (probe attachment)
	double	ArmMatrix[3][3];								// Arm matrix

	double	Angles[RDS_MAX_AXIS];					// Calculated angles

	dword		VertexID;												// ID captured by Vertex probe
	dword		AnalogValues[3];								// Analog values (UDTX, UDTY, PLA)
	dword		NbLatches;											// Number of latches used to calculate XYZ position
	bool		ButtonsStates[3];								// Status of the 3 buttons

	bool		ExternalSwitchState;						// =TRUE if external switch is ON

	void Init(void)
	{
		memset(this, 0, sizeof(*this));
	}
};



/// Not feed yet, extended data when a scanner is enabled
struct sRDSScannerHeader
{
	dword		NbPoints;							// Number of sRDSScannerPoint structures following this header
	double	CameraVector[3];

	void Init(void)
	{
		memset(this, 0, sizeof(*this));
	}
};



/// Not feed yet, extended data when a scanner is enabled
struct sRDSScannerPoint
{
	double	XYZ[3];								// Calculated point
	double	XPicture, YPicture;		// Position in grabbed frame
	double	Level;								// Gray level
	byte		Reliable;							// <> 0 if point is reliable and if it can be used as is

	void Init(void)
	{
		memset(this, 0, sizeof(*this));
	}
};


/// This enum defines the list of predefines melodies RDS can play
enum eRDSSounds
{
	/// \note JPDL 06/23/2008 : RDS_CUSTOM_SOUND renamed to RDS_DEFAULT_SOUND
	RDS_DEFAULT_SOUND = 0,				// Default sound
	RDS_DATA_TAKEN_SOUND,					// Point has been captured
	RDS_ENTER_OVER_LIMIT_SOUND,		// An arm encoder is over limit
	RDS_EXIT_OVER_LIMIT_SOUND,		// Arm is no more over limit
	RDS_DATA_ERASED_SOUND,				// Point has been deleted
	RDS_LONG_BUTTON_SOUND,				// Long press done with an arm button
	RDS_CONNECTED_TO_ARM,					// Service is connected to arm
	RDS_PROBE_INSERTED,						// Probe has been plugged in
	RDS_PROBE_PULLED_OUT,					// Probe has been plugged out
	RDS_LOW_POWER_WARNING,				// Battery level is under low power threshold

	/// \note JPDL 06/23/2008 : New sound
	RDS_SUCCESS_SOUND,						// Operation successful
};


/// This structure is sent to service when client wants to play a custom sound or a predefined melody
/// Used with RDSCI_SOUND chunk
struct sRDSSound
{
	eRDSSounds		Sound;									// See eRDSSounds for a list of available melodies
	bool					bStored;								// If TRUE, Sound is the index of sound to play
																				// If FALSE, Frequency, Duration and Volume are used
	UINT					Frequency;
	UINT					Duration;
	UINT					Volume;

	char					Reserved[64];						// Reserved for further usage

	void Init(void) { memset(this, 0, sizeof(*this)); }
};



/// This enum defines the list of predefines melodies RDS can play
enum eRDSSetting
{
	RDSSET_UNKNOWN = 0,
	RDSSET_LANGUAGE = 1,														// UI language
	RDSSET_MANAGE_LONG_PRESS_ON_ARM_BUTTONS = 2,		/// \note JPDL 07/09/2008 : New setting
};


// List of available languages in RDS: These codes are based on ISO Standard 3166
// Use these values with sRDSSetting::ID set to RDSSET_LANGUAGE
#define	RDSLANG_USER_DEFAULT		""
#define	RDSLANG_FRENCH_FRANCE		"fr-FR"
#define	RDSLANG_ENGLISH_US			"en-US"


/// Used with RDSCI_UPDATE_SETTING chunk
struct sRDSSetting
{
	eRDSSetting	ID;							// Setting we want to retrieve or update
	char				Value[256 + 1];		// If ID=RDSSET_LANGUAGE, Value contains wanted language ID (i.e "fr-FR", "en-US")
															// If ID=RDSSET_MANAGE_LONG_PRESS_ON_ARM_BUTTONS, Value contains 0 or 1

	void Init(void) { memset(this, 0, sizeof(*this)); }
};



#define RDS_REMOTE_MOUSE_OFF	0
#define RDS_REMOTE_MOUSE_ON		1


/// Sends one to three structures to other partner and waits for result
#define RDS_SEND_STRUCTS																																\
	LRESULT	RDS_SendStructs(HWND hWnd, LPVOID pArg1, DWORD dwArg1Size,										\
													LPVOID pArg2=NULL, DWORD dwArg2Size=0,												\
													LPVOID pArg3=NULL, DWORD dwArg3Size=0)												\
	{																																											\
		DWORD	dwSize = dwArg1Size + dwArg2Size + dwArg3Size;																\
																																												\
		if(hWnd == 0)																																				\
			return RDSRC_INVALID_HANDLE;																											\
																																												\
		if(pArg1==NULL || dwArg1Size==0)																										\
			return RDSRC_INVALID_PARAMETER;																										\
																																												\
		char* pBuffer = new char[dwSize];																										\
		char* pCurrent = pBuffer; 																													\
																																												\
		memcpy(pCurrent, pArg1, dwArg1Size); 																								\
		pCurrent += dwArg1Size;																															\
																																												\
		if(pArg2)																																						\
		{																																										\
			memcpy(pCurrent, pArg2, dwArg2Size);																							\
			pCurrent += dwArg2Size;																														\
																																												\
			if(pArg3)																																					\
			{																																									\
				memcpy(pCurrent, pArg3, dwArg3Size);																						\
				pCurrent += dwArg3Size;																													\
			}																																									\
		}																																										\
																																												\
		COPYDATASTRUCT dStruct;																															\
		dStruct.dwData = 0;																																	\
		dStruct.cbData = dwSize;																														\
		dStruct.lpData = pBuffer;																														\
		LRESULT lresult = ::SendMessage(hWnd, WM_COPYDATA, (WPARAM)0, (LPARAM) &dStruct);		\
																																												\
		delete [] pBuffer;																																	\
																																												\
		if(lresult == FALSE)																																\
			return RDSRC_EXECUTION_FAILED;																										\
		else																																								\
			return RDSRC_SUCCESS;																															\
	}																																											


/// Sends a request to other partner and waits for result
#define RDS_SEND_REQUEST																																	\
	LRESULT	RDS_SendRequest(eRDSChunkID ChunkID, LPVOID pArg1=NULL, DWORD dwArg1Size=0,			\
													long lChunkParameter1=0, long lChunkParameter2=0)								\
	{																																												\
		sRDSChunk chunk;																																			\
		sRDSClient client;																																		\
																																													\
		chunk.Init();																																					\
		chunk.ChunkID = (eRDSChunkID)ChunkID;																									\
		chunk.Parameter1 = lChunkParameter1;																									\
		chunk.Parameter2 = lChunkParameter2;																									\
		chunk.ChunkSize = sizeof(client) + dwArg1Size;																				\
																																													\
		client.Init();																																				\
		client.hwnd = m_hWnd;																																	\
																																													\
		return RDS_SendStructs(m_hRDSWindow, &chunk, sizeof(chunk), &client, sizeof(client), pArg1, dwArg1Size);	\
	}


#define RDS_MSGS \
	ON_REGISTERED_MESSAGE(m_iMyPrivateWindowMessage1, OnServerAvailable) \
	ON_WM_COPYDATA() 


#define RDS_ADD_PRIVATE_MESSAGES \
	const UINT m_iMyPrivateWindowMessage1 = RegisterWindowMessage(RDS_MSG_1);

#define RDS_INIT_VARS																											\
	m_bServerFound = FALSE;																									\
	memset(&m_stConnectionDateTime, 0, sizeof(m_stConnectionDateTime));			\
	memset(&m_stLastMessageDateTime, 0, sizeof(m_stLastMessageDateTime));		\
	m_uiNumberOfMessagesReceived = 0;																				\
	m_hRDSWindow = 0;																												\
	m_uiExtendedData = 0;																										

#define RDS_INITIALIZE		\
	RDS_INIT_VARS;		\
	memset(&m_ucAuthenticationKey[0], 0, sizeof(m_ucAuthenticationKey));

/// \note JPDL 06/19/2008, RDS API v0.3.3 : New macro
#define RDS_INITIALIZE_EXT(AuthenticationKey)																									\
	RDS_INIT_VARS;																																							\
	memcpy(&m_ucAuthenticationKey[0], &AuthenticationKey[0], sizeof(m_ucAuthenticationKey));


#define RDS_ADD_VARIABLES \
	RDS_SEND_REQUEST	\
	RDS_SEND_STRUCTS	\
	LRESULT OnServerAvailable(WPARAM w, LPARAM l);\
	void		CheckState(void);\
	LRESULT RemoveFromRDS(void);\
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult); \
	BOOL		OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);\
	void RDS_SYNCED_DATA(sRDSPosition& p);\
	void RDS_UNSYNCED_DATA(sRDSPosition& p);\
	void RDS_PROBE_DATA(sRDSProbe& p);\
	void RDS_ARM_DATA(sRDSArm& a);\
	void RDS_SERVER_DOWN(void);\
	void RDS_SERVER_UP(void);\
	BOOL m_bServerFound;\
	HWND m_hRDSWindow;\
	BOOL m_bRemoteMouseIsOn;\
	SYSTEMTIME	m_stConnectionDateTime;	\
	SYSTEMTIME	m_stLastMessageDateTime;	\
	UINT				m_uiNumberOfMessagesReceived; \
	UINT				m_uiExtendedData; \
	uchar				m_ucAuthenticationKey[8]; \
public:


/// \note JPDL 06/25/2008 : Not used anymore, replaced with empty content for compatibility
#define RDS_ON_REMOVE_FROM_SERVER(ClassName)

/// \note JPDL 06/25/2008 : We execute directly request to avoid useless call to RemoveFromRDS
#define RDS_REMOVE_FROM_SERVER																									\
	if(m_bServerFound)																														\
	{																																							\
		KillTimer(RDS_TM_CHECK_STATE_ID);																						\
		RDS_SendRequest(RDSCI_DISCONNECT);																					\
																																								\
		m_bServerFound = FALSE;																											\
	}																																							


#define RDS_CALIBRATE_PROBE(x, y, width, height, method)												\
{																																								\
	if(m_bServerFound)																														\
	{																																							\
		sRDSScript		script;																												\
		script.Init();																															\
		script.ScriptType = ST_PROBE_CALIBRATION;																		\
		script.X = x; script.Y = y; script.Width = width; script.Height = height;		\
		script.Method = method;																											\
																																								\
		RDS_SendRequest(RDSCI_EXECUTE_SCRIPT, &script, sizeof(script));							\
	}																																							\
}																																							


#define RDS_SETTINGS																													\
{																																							\
	if(m_bServerFound)																													\
		RDS_SendRequest(RDSCI_DISPLAY_CONTROL_PANEL);															\
}																																							


#define RDS_ADVANCED_SETTINGS																									\
{																																							\
	if(m_bServerFound)																													\
		RDS_SendRequest(RDSCI_DISPLAY_ADVANCED_CONTROL_PANEL);										\
}																																							


#define RDS_REMOTE_MOUSE(x)																										\
{																																							\
	if(m_bServerFound)																													\
		RDS_SendRequest(RDSCI_CONTROL_REMOTE_MOUSE, NULL, 0, x);									\
}																																							


#define RDS_ON_SERVER_AVAILABLE(ClassName, RemoteMouse)										\
LRESULT ClassName::OnServerAvailable(WPARAM wParam, LPARAM lParam)				\
{																																					\
	if(m_bServerFound)																											\
		return RDSRC_SUCCESS;																									\
																																					\
	m_hRDSWindow = (HWND)lParam;																						\
																																					\
	sRDSConnect connect;																										\
	uchar				ucPlainValue[8];																						\
	cDES				des;																												\
	SYSTEMTIME	now;																												\
																																					\
	GetLocalTime(&now);																											\
																																					\
	ucPlainValue[0] = (byte)now.wDay;																				\
	ucPlainValue[1] = (byte)now.wMonth;																			\
	ucPlainValue[2] = (byte)now.wHour;																			\
	ucPlainValue[3] = (byte)now.wMinute;																		\
	ucPlainValue[4] = 0;																										\
	ucPlainValue[5] = 0;																										\
	ucPlainValue[6] = 0;																										\
	ucPlainValue[7] = 0;																										\
																																					\
	connect.Init();																													\
	des.SetKey(m_ucAuthenticationKey, cDES::eDESMode::DESM_ENCRYPTION);								\
	des.Compute(ucPlainValue, connect.AuthenticationValue);									\
	connect.ExtendedData = m_uiExtendedData;																\
	connect.RDSAPIVersion = RDS_API_VERSION_VALUE;													\
																																					\
	LRESULT rc = RDS_SendRequest(RDSCI_CONNECT, &connect, sizeof(connect));	\
	if(rc != RDSRC_SUCCESS)																									\
		return rc;																														\
																																					\
	m_bRemoteMouseIsOn = (bool)RemoteMouse;																	\
	RDS_REMOTE_MOUSE(RemoteMouse)																						\
	m_bServerFound = TRUE;																									\
	GetLocalTime(&m_stConnectionDateTime);																	\
																																					\
	RDS_SERVER_UP();																												\
																																					\
	SetTimer(RDS_TM_CHECK_STATE_ID, RDS_TM_CHECK_STATE_TIMER, NULL);				\
																																					\
	return RDSRC_SUCCESS;																										\
}																																					\
																																					\
void ClassName::CheckState(void)																					\
{																																					\
	if(m_bServerFound)																											\
	{																																				\
		if(::IsWindow(m_hRDSWindow) == FALSE)																	\
		{																																			\
			RDS_INIT_VARS;																											\
			RDS_SERVER_DOWN();																									\
		}																																			\
	}																																				\
}																																					\
																																					\
BOOL ClassName::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)	\
{																																												\
	if(message == WM_TIMER && wParam == RDS_TM_CHECK_STATE_ID)														\
		CheckState();																																				\
																																												\
	return __super::OnWndMsg(message, wParam, lParam, pResult);														\
}

#define RDS_PLAY_SOUND(Frequency, Duration, Volume)												\
{																																					\
	if(m_bServerFound)																											\
	{																																				\
		sRDSSound sound;																											\
		sound.Init();																													\
		sound.bStored = FALSE;																								\
		sound.Frequency = Frequency;																					\
		sound.Duration = Duration;																						\
		sound.Volume = Volume;																								\
																																					\
		RDS_SendRequest(RDSCI_PLAY_MELODY, &sound, sizeof(sound));						\
	}																																				\
}


#define RDS_MAKE_CANNED_SOUND(SoundIndex)																	\
{																																					\
	if(m_bServerFound)																											\
	{																																				\
		sRDSSound sound;																											\
		sound.Init();																													\
		sound.bStored = TRUE;																									\
		sound.Sound = SoundIndex;																							\
																																					\
		RDS_SendRequest(RDSCI_PLAY_MELODY, &sound, sizeof(sound));						\
	}																																				\
}

#define RDS_PLAY_MELODY(SoundIndex)																				\
	RDS_MAKE_CANNED_SOUND(SoundIndex);


/// \note JPDL 07/07/2008 : Added in RDS API v0.3.4
#define RDS_UPDATE_SETTING(id, value)																			\
{																																					\
	if(m_bServerFound)																											\
	{																																				\
		sRDSSetting setting;																									\
		setting.Init();																												\
		setting.ID = id;																											\
		strcpy(setting.Value, value);																					\
																																					\
		RDS_SendRequest(RDSCI_UPDATE_SETTING, &setting, sizeof(setting));			\
	}																																				\
}


#define RDS_ON_COPY_DATA(Class, Base)																			\
BOOL Class::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)				\
{ 																																				\
	if(pCopyDataStruct->cbData != 0) 																				\
	{ 																																			\
		char* p = new char[pCopyDataStruct->cbData];													\
		memcpy(p, pCopyDataStruct->lpData, pCopyDataStruct->cbData);					\
																																					\
		sRDSChunk chunk;																											\
		memcpy(&chunk, p, sizeof(chunk));																			\
																																					\
		GetLocalTime(&m_stLastMessageDateTime);																\
		m_uiNumberOfMessagesReceived++;																				\
																																					\
		switch(chunk.ChunkID)																									\
		{																																			\
			case RDSCI_POSITION:																								\
				sRDSPosition pos;																									\
				memcpy(&pos, &p[sizeof(chunk)], sizeof(pos));  										\
				if(pos.EventType&EVENT_SYNC) 																			\
					RDS_SYNCED_DATA(pos); 																					\
				else 																															\
					RDS_UNSYNCED_DATA(pos); 																				\
				break; 																														\
 																																					\
			case RDSCI_PROBE:																										\
				sRDSProbe probe;																									\
				memcpy(&probe, &p[sizeof(chunk)], sizeof(probe));									\
				RDS_PROBE_DATA(probe);																						\
				break;																														\
 																																					\
			case RDSCI_ARM:																											\
				sRDSArm arm;																											\
				memcpy(&arm, &p[sizeof(chunk)], sizeof(arm));											\
				RDS_ARM_DATA(arm);																								\
				break;																														\
 																																					\
			case RDSCI_SERVER_SHUTTING_DOWN:																		\
				RDS_INIT_VARS;																										\
				RDS_SERVER_DOWN();																								\
				break;																														\
		} 																																		\
		delete[] p;  																													\
	} 																																			\
																																					\
	return Base::OnCopyData(pWnd, pCopyDataStruct);													\
}																																					


#define RDS_ON_COPY_DATA_EASY(Class, Base)																\
BOOL Class::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)				\
{																																					\
	if(pCopyDataStruct->cbData != 0)																				\
	{																																				\
		char* p = new char[pCopyDataStruct->cbData];													\
		memcpy(p, pCopyDataStruct->lpData, pCopyDataStruct->cbData);					\
																																					\
		sRDSChunk chunk;																											\
		memcpy(&chunk, p, sizeof(chunk));																			\
																																					\
		GetLocalTime(&m_stLastMessageDateTime);																\
		m_uiNumberOfMessagesReceived++;																				\
																																					\
		switch(chunk.ChunkID)																										\
		{																																			\
			case RDSCI_POSITION:																								\
				sRDSPosition pos;																									\
				memcpy(&pos, &p[sizeof(chunk)], sizeof(pos));											\
				if(pos.EventType&EVENT_EASY)																			\
					RDS_SYNCED_DATA(pos);																						\
				else																															\
					RDS_UNSYNCED_DATA(pos);																					\
				break;																														\
																																					\
			case RDSCI_PROBE:																										\
				sRDSProbe probe;																									\
				memcpy(&probe, &p[sizeof(chunk)], sizeof(probe));									\
				RDS_PROBE_DATA(probe);																						\
				break;																														\
																																					\
			case RDSCI_ARM:																											\
				sRDSArm arm;																											\
				memcpy(&arm, &p[sizeof(chunk)], sizeof(arm));											\
				RDS_ARM_DATA(arm);																								\
				break;																														\
																																					\
			case RDSCI_SERVER_SHUTTING_DOWN:																		\
				RDS_INIT_VARS;																										\
				RDS_SERVER_DOWN();																								\
				break;																														\
		}																																			\
		delete[] p;																														\
	}																																				\
																																					\
	return Base::OnCopyData(pWnd, pCopyDataStruct);													\
}


/// \note JPDL, 03/09/2007 : For compatibility with other environment like VB6
#pragma pack(pop)
