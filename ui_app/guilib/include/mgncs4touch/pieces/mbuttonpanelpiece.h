#ifndef _MGNCS_BTNPANELPIECE_H
#define _MGNCS_BTNPANELPIECE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _mButtonPanelPiece mButtonPanelPiece;
typedef struct _mButtonPanelPieceClass mButtonPanelPieceClass;

enum mButtonPanelPieceState{
    NCS_BPP_NORMAL = 0,
//    NCS_BPP_HILIGHT,
    NCS_BPP_PUSHED,
//    NCS_BPP_CAPTURED,
//    NCS_BPP_DISABLED
};

typedef struct _mStateTable {
    list_t list;
    mHotPiece *piece;
    int propertyId;
    DWORD propertyValue[2];
} mStateTable;

#define MSG_BTNPANELPIECE_STATE_CHANGED     (MSG_USER + 100)

//mShapeTransRoundPiece *backgroundPiece;
#define mButtonPanelPieceHeader(clss) \
	mPanelPieceHeader(clss) \
    enum mButtonPanelPieceState state;   \
    mColorTable gradientBackgroundColorTable[2]; \
    list_t stateTable;

struct _mButtonPanelPiece
{
	mButtonPanelPieceHeader(mButtonPanelPiece)
};

//mShapeTransRoundPiece* (*getBkgndPiece)(clss*);
#define mButtonPanelPieceClassHeader(clss, superCls) \
	mPanelPieceClassHeader(clss, superCls) \
    void (*setStateTable)(clss*, mHotPiece*, int, DWORD, DWORD); \
    void (*setGradientBackgroundColor)(clss*,\
        ARGB*, float*, unsigned int,         \
        ARGB*, float*, unsigned int);        \
    void (*clearGradientBackgroundColor)(clss*, BOOL, BOOL);     \
    void (*setStates)(clss*); \
    void (*useStateChange)(clss*, BOOL); \
    void (*setGradientBorderColor)(clss*,    \
            ARGB*, float*, unsigned int);

struct _mButtonPanelPieceClass
{
	mButtonPanelPieceClassHeader(mButtonPanelPiece, mPanelPiece)
};

MGNCS_EXPORT extern mButtonPanelPieceClass g_stmButtonPanelPieceCls;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGNCS_BTNPANELPIECE_H */
