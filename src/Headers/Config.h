#ifndef _C_RUBE_CONFIG_H_
#define _C_RUBE_CONFIG_H_

// default sensitivity values for mouse control
extern float CONFIG_defaultPitchSens;
extern float CONFIG_defaultYawSens;
extern float CONFIG_defaultZoomSens;

// default zoom limits
extern float CONFIG_minZoom;
extern float CONFIG_maxZoom;

// starting orbital radius
extern float CONFIG_defaultOrbitalRadius;

// speed in radians per second for how fast the slices move
extern float CONFIG_sliceRotationSpeed;

// the starting width of the window
extern int CONFIG_width;

// the starting height of the window
extern int CONFIG_height;

// number of random rotation to make when shuffling
extern int CONFIG_shuffleSize;

// scaling factor for the gui
extern float CONFIG_guiScale;

// Load configuration file
void LoadConfigFile (void);
void _ProcessContents (char *contents,int fileSize);

#endif