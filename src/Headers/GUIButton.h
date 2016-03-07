#ifndef _C_RUBE_GUIBUTTON_H_
#define _C_RUBE_GUIBUTTON_H_

typedef void (*onClickFunc) (Cube*,int,bool);
typedef void (*onHighlightFunc) (void);

typedef struct {
    bool visible;
    bool enabled;
    bool depressed;
    bool highlighted;
    onClickFunc onClick;          // Button click callback
    onHighlightFunc onHighlight;  // Button highlight callback
    bool mouseDownInBounds;
    int sliceId;
    bool sliceForward;
    
    float posPercent[3];  //Position of the button's center in screen percentage
    float dim[3];         // Dimensions of the button in screen percent
    float rot;
    
    float tMtx[16];
    float texMtx[9];
    int texId;
    
    float ambientColor[3];
    float mixValue;
} GUIButton;

void InitButton (GUIButton *button);
void SetPRD (
    GUIButton *button, 
    float percentX, 
    float percentY, 
    float percentW, 
    float percentH, 
    float rotZ
);
void _RecalcTMtx (GUIButton *button);
bool inBounds (GUIButton *button, float percentX, float percentY);
void ButtonUpdate (GUIButton *button, float dt);
void ButtonOnMouseUp (GUIButton *button);
void ButtonOnMouseDown (GUIButton *button);
void ButtononMouseEnter (GUIButton *button);
void ButtononMouseExit (GUIButton *button);

#endif
