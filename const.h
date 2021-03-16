#ifndef CONST_H
#define CONST_H

//Define the code for Barge-in and whishper
//#define CODE_SPY        "*22"
//#define CODE_BARGEIN    "*24"
#define CODE_BARGEIN    "*22"
#define CODE_WISHPER    "*23"
#define CODE_TXFR       "#"
#define CODE_CONF       "*34"
#define CODE_ADD        "3"
#define CODE_VM        "*26"



#define COLOUR_RED  "#FF4F4f"
#define COLOUR_GREEN  "#00Ffff"
#define COLOUR_YELLOW   "yellow"


#define CONTEXT_NORMAL  0
#define CONTEXT_TXFR    1
#define CONTEXT_CONF    2
#define CONTEXT_BARGEIN 3
#define CONTEXT_WHISPER 4

#define STATUS_NOTCONNECTED 0
#define STATUS_NOTINUSE     1
#define STATUS_RINGING      2
#define STATUS_TALKING      3


typedef struct
{
    char extn[64];
    int iType;
    int iStatus;
}T_EndpointStatus;


typedef struct
{
    int id;
    char szGroupName[64];
    int iGroupType;
}T_Group;

#define GRP_CONF 0
#define GRP_CHAT 1


typedef struct
{
    int id;
    char szName[64];
    char szGroup[64];
    int iMode;
    int iType;
    int iMedia;
    char szDate[32];
    int iModeratorLess;
    int iConfRoom;
    int iPin;
    int iRecConf;
}T_Schedule;

#define MEDIA_AUDIO 0
#define MEDIA_VIDEO 1
#define CONF_TYPE_OUT 0
#define CONF_TYPE_IN  1



#endif // CONST_H
