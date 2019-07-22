#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_ 

//========================DEBUG SWITCH===================
#define STREAM_DEBUG_FLAG 0
#define SOCKET_DEBUG_FLAG 0
//=======================================================

#define DEVICE_CFG_FILE "/customer/smartBD.xml"
#define UI_SURFACE_CFG_FILE "/customer/smartLayout.xml"
#define DEV_INFO_CFG_FILE "/customer/smartDevInfo.xml"


#define CALL_ID_LEN 8 //Çø+¶°+Â¥+·¿ 2+2+2+2 "01021102" "Room 02, 11th Floor, Uint 2, Group 1"
#define CALL_ID_MAX_LEN 16
#define TELPHONE_LEN 12
#define DEVICE_ADDR_MAX_LEN 64
#define IPADDR_MAX_LEN 16

#define LOCAL_LISTEN_PORT 10001

#define SEND_VIDEO_PORT 11001
#define RECV_VIDEO_PORT 11002

#define SEND_AUDIO_PORT 12001
#define RECV_AUDIO_PORT 12002
#define UDP_MAX_PACKSIZE 1460

#define HEADER_MAGIC_NUM 0x66668888

//DOOR CMD
#define DOOR_CALLROOM           (0x0001)
#define DOOR_CALLCENTER         (0x0002)
#define DOOR_MONTACK            (0x0003)
#define DOOR_BUSY               (0x0004)
#define DOOR_UNLOCKACK          (0x0005)
#define DOOR_GETTIME            (0x0006)
#define DOOR_HANGUP             (0x0007)
#define DOOR_ENDMONT            (0x0008)
#define DOOR_IDLE               (0x0009)

//ROOM CMD
#define ROOM_CALLROOM           (0x0401)
#define ROOM_STARTMONT          (0x0402)
#define ROOM_STOPMONT           (0x0403)
#define ROOM_IDLE               (0x0405)
#define ROOM_BUSY               (0x0406)
#define ROOM_HOLDON             (0x0407)
#define ROOM_HANGUP             (0x0408)
#define ROOM_UNLOCK             (0x0409)
#define ROOM_MONTACK            (0x040A)

typedef enum 
{
    E_ST_DEV_WALL = 1,
    E_ST_DEV_DOOR,
    E_ST_DEV_ROOM,
    E_ST_DEV_CENTER
} ST_DeviceType_E;

typedef enum
{
    E_DEVICE_BUSY = 1,
    E_DEVICE_IDLE,
    E_DEVICE_CALLROOM,
    E_DEVICE_CALLDOOR,
    E_DEVICE_TALKING,
    E_DEVICE_RING,
    E_DEVICE_MONITOR,
    E_DEVICE_MONITORED,
    E_DEVICE_CALLED,
} ST_Device_Sys_Status_e;

#endif //_APP_CONFIG_H_
