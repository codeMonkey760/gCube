#ifndef _C_RUBE_CONFIG_H_
#define _C_RUBE_CONFIG_H_

// default sensitivity values for mouse control
extern float defaultPitchSens;
extern float defaultYawSens;
extern float defaultZoomSens;

// default zoom limits
extern int minZoom;
extern int maxZoom;

// starting orbital radius
extern float defaultOrbitalRadius;

// speed in radians per second for how fast the slices move
extern float sliceRotationSpeed;

// the starting width of the window
extern int width;

// the starting height of the window
extern int height;

// number of random rotation to make when shuffling
extern int shuffleSize;

// scaling factor for the gui
extern float guiScale;

// Load configuration file
void LoadConfigFile (void);
void _ProcessContents (char *contents,int fileSize);

// If no config file was found write one
void _WriteDefaultConfigFile (void);

#endif