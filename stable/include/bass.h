/* BASS 0.8 C/C++ header file, copyright (c) 1999-2000 Ian Luck.
   Please report bugs/suggestions/etc... to bass@un4seen.com */

#ifndef BASS_H
#define BASS_H

#ifndef _LINUX
#include <wtypes.h>
#else
typedef int DWORD;
typedef int BOOL;
typedef int HWND;
#define CALLBACK
#define WINAPI
#endif
#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSDEF
#define BASSDEF(f) WINAPI f
#endif

typedef DWORD HMUSIC;		// MOD music handle
typedef DWORD HSAMPLE;		// sample handle
typedef DWORD HCHANNEL;		// playing sample's channel handle
typedef DWORD HSTREAM;		// sample stream handle
typedef DWORD HSYNC;		// synchronizer handle
typedef DWORD HDSP;			// DSP handle

// Error codes returned by BASS_GetErrorCode()
#define BASS_OK				0	// all is OK
#define BASS_ERROR_MEM		1	// memory error
#define BASS_ERROR_FILEOPEN	2	// can't open the file
#define BASS_ERROR_DRIVER	3	// can't find a free/valid driver
#define BASS_ERROR_BUFLOST	4	// the sample buffer was lost - please report this!
#define BASS_ERROR_HANDLE	5	// invalid handle
#define BASS_ERROR_FORMAT	6	// unsupported format
#define BASS_ERROR_POSITION	7	// invalid playback position
#define BASS_ERROR_INIT		8	// BASS_Init has not been successfully called
#define BASS_ERROR_START	9	// BASS_Start has not been successfully called
#define BASS_ERROR_INITCD	10	// can't initialize CD
#define BASS_ERROR_CDINIT	11	// BASS_CDInit has not been successfully called
#define BASS_ERROR_NOCD		12	// no CD in drive
#define BASS_ERROR_CDTRACK	13	// can't play the selected CD track
#define BASS_ERROR_ALREADY	14	// already initialized
#define BASS_ERROR_CDVOL	15	// CD has no volume control
#define BASS_ERROR_NOPAUSE	16	// not paused
#define BASS_ERROR_NOTAUDIO	17	// not an audio track
#define BASS_ERROR_NOCHAN	18	// can't get a free channel
#define BASS_ERROR_ILLTYPE	19	// an illegal type was specified
#define BASS_ERROR_ILLPARAM	20	// an illegal parameter was specified
#define BASS_ERROR_NO3D		21	// no 3D support
#define BASS_ERROR_NOEAX	22	// no EAX support
#define BASS_ERROR_DEVICE	23	// illegal device number
#define BASS_ERROR_NOPLAY	24	// not playing
#define BASS_ERROR_FREQ		25	// illegal sample rate
#define BASS_ERROR_NOA3D	26	// A3D.DLL is not installed
#define BASS_ERROR_NOTFILE	27	// the stream is not a file stream (WAV/MP3)
#define BASS_ERROR_NOHW		29	// no hardware voices available
#define BASS_ERROR_NOSYNC	30	// synchronizers have been disabled
#define BASS_ERROR_UNKNOWN	-1	// some other mystery error

// Device setup flags
#define BASS_DEVICE_8BITS	1	// use 8 bit resolution, else 16 bit
#define BASS_DEVICE_MONO	2	// use mono, else stereo
#define BASS_DEVICE_3D		4	// enable 3D functionality
/* If the BASS_DEVICE_3D flag is not specified when initilizing BASS,
then the 3D flags (BASS_SAMPLE_3D and BASS_MUSIC_3D) are ignored when
loading/creating a sample/stream/music. */
#define BASS_DEVICE_A3D		8	// enable A3D functionality
#define BASS_DEVICE_NOSYNC	16	// disable synchronizers

// DirectSound interfaces (for use with BASS_GetDSoundObject)
#define BASS_OBJECT_DS		1	// IDirectSound
#define BASS_OBJECT_DS3DL	2	// IDirectSound3DListener

typedef struct {
	DWORD size;		// size of this struct (set this before calling the function)
	DWORD flags;	// device capabilities (DSCAPS_xxx flags)
/* The following values are irrelevant if the device doesn't have hardware
support (DSCAPS_EMULDRIVER is specified in flags) */
	DWORD hwsize;	// size of total device hardware memory
	DWORD hwfree;	// size of free device hardware memory
	DWORD freesam;	// number of free sample slots in the hardware
	DWORD free3d;	// number of free 3D sample slots in the hardware
	DWORD minrate;	// min sample rate supported by the hardware
	DWORD maxrate;	// max sample rate supported by the hardware
	BOOL eax;		// device supports EAX? (always FALSE if BASS_DEVICE_3D was not used)
	DWORD a3d;		// A3D version (0=unsupported or BASS_DEVICE_A3D was not used)
	DWORD dsver;	// DirectSound version (use to check for DX5/7 functions)
} BASS_INFO;

// BASS_INFO flags (from DSOUND.H)
#define DSCAPS_CONTINUOUSRATE	0x00000010
/* supports all sample rates between min/maxrate */
#define DSCAPS_EMULDRIVER		0x00000020
/* device does NOT have hardware DirectSound support */
#define DSCAPS_CERTIFIED		0x00000040
/* device driver has been certified by Microsoft */
/* The following flags tell what type of samples are supported by HARDWARE
mixing, all these formats are supported by SOFTWARE mixing */
#define DSCAPS_SECONDARYMONO	0x00000100	// mono
#define DSCAPS_SECONDARYSTEREO	0x00000200	// stereo
#define DSCAPS_SECONDARY8BIT	0x00000400	// 8 bit
#define DSCAPS_SECONDARY16BIT	0x00000800	// 16 bit

// Volume sliding flags
#define BASS_SLIDE_DIGITAL	1	// slide digital master volume
#define BASS_SLIDE_CD		2	// slide CD volume

// Music flags
#define BASS_MUSIC_RAMP		1	// normal ramping
#define BASS_MUSIC_RAMPS	2	// sensitive ramping
/* Ramping doesn't take a lot of extra processing and improves
the sound quality by removing "clicks". Sensitive ramping will
leave sharp attacked samples, unlike normal ramping. */
#define BASS_MUSIC_LOOP		4	// loop music
#define BASS_MUSIC_FT2MOD	16	// play .MOD as FastTracker 2 does
#define BASS_MUSIC_PT1MOD	32	// play .MOD as ProTracker 1 does
#define BASS_MUSIC_MONO		64	// force mono mixing (less CPU usage)
#define BASS_MUSIC_3D		128	// enable 3D functionality
#define BASS_MUSIC_POSRESET	256	// stop all notes when moving position

// Sample info structure & flags
typedef struct {
	DWORD freq;		// default playback rate
	DWORD volume;	// default volume (0-100)
	int pan;		// default pan (-100=left, 0=middle, 100=right)
	DWORD flags;	// BASS_SAMPLE_xxx flags
	DWORD length;	// length (in samples, not bytes)
	DWORD max;		// maximum simultaneous playbacks
/* The following are the sample's default 3D attributes (if the sample
is 3D, BASS_SAMPLE_3D is in flags) see BASS_ChannelSet3DAttributes */
	DWORD mode3d;	// BASS_3DMODE_xxx mode
	float mindist;	// minimum distance
	float maxdist;	// maximum distance
	DWORD iangle;	// angle of inside projection cone
	DWORD oangle;	// angle of outside projection cone
	DWORD outvol;	// delta-volume outside the projection cone
/* The following are the defaults used if the sample uses the DirectX 7
voice allocation/management features. */
	DWORD vam;		// voice allocation/management flags (BASS_VAM_xxx)
	DWORD priority;	// priority (0=lowest, 0xffffffff=highest)
} BASS_SAMPLE;

#define BASS_SAMPLE_8BITS		1	// 8 bit, else 16 bit
#define BASS_SAMPLE_MONO		2	// mono, else stereo
#define BASS_SAMPLE_LOOP		4	// looped
#define BASS_SAMPLE_3D			8	// 3D functionality enabled
#define BASS_SAMPLE_SOFTWARE	16	// it's NOT using hardware mixing
#define BASS_SAMPLE_MUTEMAX		32	// muted at max distance (3D only)
#define BASS_SAMPLE_VAM			64	// uses the DX7 voice allocation & management
#define BASS_SAMPLE_OVER_VOL	0x10000	// override lowest volume
#define BASS_SAMPLE_OVER_POS	0x20000	// override longest playing
#define BASS_SAMPLE_OVER_DIST	0x30000 // override furthest from listener (3D only)

#define BASS_MP3_HALFRATE	0x10000 // reduced quality MP3 (half sample rate)
#define BASS_MP3_SETPOS		0x20000 // enable seeking on the MP3

// DX7 voice allocation flags
#define BASS_VAM_HARDWARE		1
/* Play the sample in hardware. If no hardware voices are available then
the "play" call will fail */
#define BASS_VAM_SOFTWARE		2
/* Play the sample in software (ie. non-accelerated). No other VAM flags
may be used together with this flag. */

// DX7 voice management flags
/* These flags enable hardware resource stealing... if the hardware has no
available voices, a currently playing buffer will be stopped to make room for
the new buffer. NOTE: only samples loaded/created with the BASS_SAMPLE_VAM
flag are considered for termination by the DX7 voice management. */
#define BASS_VAM_TERM_TIME		4
/* If there are no free hardware voices, the buffer to be terminated will be
the one with the least time left to play. */
#define BASS_VAM_TERM_DIST		8
/* If there are no free hardware voices, the buffer to be terminated will be
one that was loaded/created with the BASS_SAMPLE_MUTEMAX flag and is beyond
it's max distance. If there are no buffers that match this criteria, then the
"play" call will fail. */
#define BASS_VAM_TERM_PRIO		16
/* If there are no free hardware voices, the buffer to be terminated will be
the one with the lowest priority. */


// 3D vector (for 3D positions/velocities/orientations)
typedef struct {
	float x;	// +=right, -=left
	float y;	// +=up, -=down
	float z;	// +=front, -=behind
} BASS_3DVECTOR;

// 3D channel modes
#define BASS_3DMODE_NORMAL		0
/* normal 3D processing */
#define BASS_3DMODE_RELATIVE	1
/* The channel's 3D position (position/velocity/orientation) are relative to
the listener. When the listener's position/velocity/orientation is changed
with BASS_Set3DPosition, the channel's position relative to the listener does
not change. */
#define BASS_3DMODE_OFF			2
/* Turn off 3D processing on the channel, the sound will be played
in the center. */

// EAX environments, use with BASS_SetEAXParameters
enum
{
    EAX_ENVIRONMENT_GENERIC,
    EAX_ENVIRONMENT_PADDEDCELL,
    EAX_ENVIRONMENT_ROOM,
    EAX_ENVIRONMENT_BATHROOM,
    EAX_ENVIRONMENT_LIVINGROOM,
    EAX_ENVIRONMENT_STONEROOM,
    EAX_ENVIRONMENT_AUDITORIUM,
    EAX_ENVIRONMENT_CONCERTHALL,
    EAX_ENVIRONMENT_CAVE,
    EAX_ENVIRONMENT_ARENA,
    EAX_ENVIRONMENT_HANGAR,
    EAX_ENVIRONMENT_CARPETEDHALLWAY,
    EAX_ENVIRONMENT_HALLWAY,
    EAX_ENVIRONMENT_STONECORRIDOR,
    EAX_ENVIRONMENT_ALLEY,
    EAX_ENVIRONMENT_FOREST,
    EAX_ENVIRONMENT_CITY,
    EAX_ENVIRONMENT_MOUNTAINS,
    EAX_ENVIRONMENT_QUARRY,
    EAX_ENVIRONMENT_PLAIN,
    EAX_ENVIRONMENT_PARKINGLOT,
    EAX_ENVIRONMENT_SEWERPIPE,
    EAX_ENVIRONMENT_UNDERWATER,
    EAX_ENVIRONMENT_DRUGGED,
    EAX_ENVIRONMENT_DIZZY,
    EAX_ENVIRONMENT_PSYCHOTIC,

    EAX_ENVIRONMENT_COUNT			// total number of environments
};

// EAX presets, usage: BASS_SetEAXParameters(EAX_PRESET_xxx)
#define EAX_PRESET_GENERIC         EAX_ENVIRONMENT_GENERIC,0.5F,1.493F,0.5F
#define EAX_PRESET_PADDEDCELL      EAX_ENVIRONMENT_PADDEDCELL,0.25F,0.1F,0.0F
#define EAX_PRESET_ROOM            EAX_ENVIRONMENT_ROOM,0.417F,0.4F,0.666F
#define EAX_PRESET_BATHROOM        EAX_ENVIRONMENT_BATHROOM,0.653F,1.499F,0.166F
#define EAX_PRESET_LIVINGROOM      EAX_ENVIRONMENT_LIVINGROOM,0.208F,0.478F,0.0F
#define EAX_PRESET_STONEROOM       EAX_ENVIRONMENT_STONEROOM,0.5F,2.309F,0.888F
#define EAX_PRESET_AUDITORIUM      EAX_ENVIRONMENT_AUDITORIUM,0.403F,4.279F,0.5F
#define EAX_PRESET_CONCERTHALL     EAX_ENVIRONMENT_CONCERTHALL,0.5F,3.961F,0.5F
#define EAX_PRESET_CAVE            EAX_ENVIRONMENT_CAVE,0.5F,2.886F,1.304F
#define EAX_PRESET_ARENA           EAX_ENVIRONMENT_ARENA,0.361F,7.284F,0.332F
#define EAX_PRESET_HANGAR          EAX_ENVIRONMENT_HANGAR,0.5F,10.0F,0.3F
#define EAX_PRESET_CARPETEDHALLWAY EAX_ENVIRONMENT_CARPETEDHALLWAY,0.153F,0.259F,2.0F
#define EAX_PRESET_HALLWAY         EAX_ENVIRONMENT_HALLWAY,0.361F,1.493F,0.0F
#define EAX_PRESET_STONECORRIDOR   EAX_ENVIRONMENT_STONECORRIDOR,0.444F,2.697F,0.638F
#define EAX_PRESET_ALLEY           EAX_ENVIRONMENT_ALLEY,0.25F,1.752F,0.776F
#define EAX_PRESET_FOREST          EAX_ENVIRONMENT_FOREST,0.111F,3.145F,0.472F
#define EAX_PRESET_CITY            EAX_ENVIRONMENT_CITY,0.111F,2.767F,0.224F
#define EAX_PRESET_MOUNTAINS       EAX_ENVIRONMENT_MOUNTAINS,0.194F,7.841F,0.472F
#define EAX_PRESET_QUARRY          EAX_ENVIRONMENT_QUARRY,1.0F,1.499F,0.5F
#define EAX_PRESET_PLAIN           EAX_ENVIRONMENT_PLAIN,0.097F,2.767F,0.224F
#define EAX_PRESET_PARKINGLOT      EAX_ENVIRONMENT_PARKINGLOT,0.208F,1.652F,1.5F
#define EAX_PRESET_SEWERPIPE       EAX_ENVIRONMENT_SEWERPIPE,0.652F,2.886F,0.25F
#define EAX_PRESET_UNDERWATER      EAX_ENVIRONMENT_UNDERWATER,1.0F,1.499F,0.0F
#define EAX_PRESET_DRUGGED         EAX_ENVIRONMENT_DRUGGED,0.875F,8.392F,1.388F
#define EAX_PRESET_DIZZY           EAX_ENVIRONMENT_DIZZY,0.139F,17.234F,0.666F
#define EAX_PRESET_PSYCHOTIC       EAX_ENVIRONMENT_PSYCHOTIC,0.486F,7.563F,0.806F

// A3D resource manager modes
#define A3D_RESMODE_OFF				0	// off
#define A3D_RESMODE_NOTIFY			1	// notify when there are no free hardware channels
#define A3D_RESMODE_DYNAMIC     	2	// non-looping channels are managed
#define A3D_RESMODE_DYNAMIC_LOOPERS	3	// all (inc. looping) channels are managed (req A3D 1.2)

// software 3D mixing algorithm modes (used with BASS_Set3DAlgorithm)
#define BASS_3DALG_DEFAULT	0
/* default algorithm (currently translates to BASS_3DALG_OFF) */
#define BASS_3DALG_OFF		1
/* Uses normal left and right panning. The vertical axis is ignored except for
scaling of volume due to distance. Doppler shift and volume scaling are still
applied, but the 3D filtering is not performed. This is the most CPU efficient
software implementation, but provides no virtual 3D audio effect. Head Related
Transfer Function processing will not be done. Since only normal stereo panning
is used, a channel using this algorithm may be accelerated by a 2D hardware
voice if no free 3D hardware voices are available. */
#define BASS_3DALG_FULL		2
/* This algorithm gives the highest quality 3D audio effect, but uses more CPU.
Requires Windows 98 2nd Edition or Windows 2000 that uses WDM drivers, if this
mode is not available then BASS_3DALG_OFF will be used instead. */
#define BASS_3DALG_LIGHT	3
/* This algorithm gives a good 3D audio effect, and uses less CPU than the FULL
mode. Requires Windows 98 2nd Edition or Windows 2000 that uses WDM drivers, if
this mode is not available then BASS_3DALG_OFF will be used instead. */

typedef DWORD (CALLBACK STREAMPROC)(HSTREAM handle, void *buffer, DWORD length, DWORD user);
/* Stream callback function. NOTE: A stream function should obviously be as quick
as possible, other streams (and MOD musics) can't be mixed until it's finished.
handle : The stream that needs writing
buffer : Buffer to write the samples in
length : Number of bytes to write
user   : The 'user' parameter value given when calling BASS_StreamCreate
RETURN : Number of bytes written. If less than "length" then the
         stream is assumed to be at the end, and is stopped. */

/* Sync types (with BASS_ChannelSetSync() "param" and SYNCPROC "data"
definitions) & flags. */
#define BASS_SYNC_MUSICPOS	0
/* Sync when a music reaches a position.
param: LOWORD=order (0=first, -1=all) HIWORD=row (0=first, -1=all)
data : LOWORD=order HIWORD=row */
#define BASS_SYNC_MUSICINST	1
/* Sync when an instrument (sample for the non-instrument based formats)
is played in a music (not including retrigs).
param: LOWORD=instrument (1=first) HIWORD=note (0=c0...119=b9, -1=all)
data : LOWORD=note HIWORD=volume (0-64) */
#define BASS_SYNC_END		2
/* Sync when a music or file stream reaches the end.
param: not used
data : not used */
#define BASS_SYNC_MUSICFX	3
/* Sync when the "sync" effect (XM/MTM/MOD: E8x, IT/S3M: S0x) is used.
param: 0:data=pos, 1:data="x" value
data : param=0: LOWORD=order HIWORD=row, param=1: "x" value */
#define BASS_SYNC_MIXTIME	0x40000000	// FLAG: sync at mixtime, else at playtime
#define BASS_SYNC_ONETIME	0x80000000	// FLAG: sync only once, else continuously

typedef void (CALLBACK SYNCPROC)(HSYNC handle, DWORD channel, DWORD data, DWORD user);
/* Sync callback function. NOTE: a sync callback function should be very
quick (eg. just posting a message) as other syncs cannot be processed
until it has finished. If the sync is a "mixtime" sync, then other streams
and MOD musics can not be mixed until it's finished either.
handle : The sync that has occured
channel: Channel that the sync occured in
data   : Additional data associated with the sync's occurance
user   : The 'user' parameter given when calling BASS_ChannelSetSync */

typedef void (CALLBACK DSPPROC)(HSYNC handle, DWORD channel, void *buffer, DWORD length, DWORD user);
/* DSP callback function. NOTE: A DSP function should obviously be as quick as
possible... other DSP functions, streams and MOD musics can not be processed
until it's finished.
handle : The DSP handle
channel: Channel that the DSP is being applied to
buffer : Buffer to apply the DSP to
length : Number of bytes in the buffer
user   : The 'user' parameter given when calling BASS_ChannelSetDSP */

#define CDCHANNEL	0	// CD channel, for use with BASS_Channel functions

DWORD BASSDEF(BASS_GetVersion)();
/* Retrieve the version number of BASS that is loaded.
RETURN : The BASS version (LOWORD.HIWORD) */

BOOL BASSDEF(BASS_GetDeviceDescription)(int devnum, char **desc);
/* Get the text description of a device. This function can be used to
enumerate the available devices.
devnum : The device (0=first)
desc   : Pointer to store pointer to text description at */

void BASSDEF(BASS_SetBufferLength)(float length);
/* Set the amount that BASS mixes ahead new musics/streams. Changing
this setting does not affect musics/streams that have already been
loaded/created. Increasing the buffer length, decreases the chance of
the sound possibly breaking-up on slower computers, but also requires
more memory. The default length is 0.5 secs.
length : The buffer length in seconds (limited to 0.3-2.0s)

NOTE: This setting does not represent the latency (delay between playing
and hearing the sound). The latency depends on the drivers, the power of
the CPU, and the complexity of what's being mixed (eg. an IT using filters
requires more processing than a plain 4 channel MOD). So the buffer length
actually has no effect on the latency. */

void BASSDEF(BASS_SetGlobalVolumes)(int musvol, int samvol, int strvol);
/* Set the global music/sample/stream volume levels.
musvol : MOD music global volume level (0-100, -1=leave current)
samvol : Sample global volume level (0-100, -1=leave current)
strvol : Stream global volume level (0-100, -1=leave current) */

void BASSDEF(BASS_GetGlobalVolumes)(DWORD *musvol, DWORD *samvol, DWORD *strvol);
/* Retrive the global music/sample/stream volume levels.
musvol : MOD music global volume level (NULL=don't retrieve it)
samvol : Sample global volume level (NULL=don't retrieve it)
strvol : Stream global volume level (NULL=don't retrieve it) */

void BASSDEF(BASS_SetLogCurves)(BOOL volume, BOOL pan);
/* Make the volume/panning values translate to a logarithmic curve,
or a linear "curve" (the default).
volume : volume curve (FALSE=linear, TRUE=log)
pan    : panning curve (FALSE=linear, TRUE=log) */

void BASSDEF(BASS_Set3DAlgorithm)(DWORD algo);
/* Set the 3D algorithm for software mixed 3D channels (does not affect
hardware mixed channels). Changing the mode only affects subsequently
created or loaded samples/streams/musics, not those that already exist.
Requires DirectX 7 or above.
algo   : algorithm flag (BASS_3DALG_xxx) */

DWORD BASSDEF(BASS_ErrorGetCode)();
/* Get the BASS_ERROR_xxx error code. Use this function to get the
reason for an error. */


BOOL BASSDEF(BASS_Init)(int device, DWORD freq, DWORD flags, HWND win);
/* Initialize the digital output. This must be called before all following
BASS functions (except CD functions). The volume is initially set to 100
(the maximum), use BASS_SetVolume() to adjust it.
device : Device to use (0=first, -1=default, -2=no sound)
freq   : Output sample rate
flags  : BASS_DEVICE_xxx flags
win    : Owner window

NOTE: The "no sound" device (device=-2), allows loading and "playing"
of MOD musics only (all sample/stream functions and most other functions
fail). This is so that you can still use the MOD musics as synchronizers
when there is no soundcard present. When using device -2, you should still
set the other arguments as you would do normally. */

void BASSDEF(BASS_Free)();
/* Free all resources used by the digital output, including all musics
and samples. */

void *BASSDEF(BASS_GetDSoundObject)(DWORD object);
/* Retrieve a pointer to a DirectSound interface. This can be used by
advanced users to "plugin" external functionality.
object : The interface to retrieve (BASS_OBJECT_xxx)
RETURN : A pointer to the requested interface (NULL=error) */

void BASSDEF(BASS_GetInfo)(BASS_INFO *info);
/* Retrieve some information on the device being used.
info   : Pointer to store info at */

float BASSDEF(BASS_GetCPU)();
/* Get the current CPU usage of BASS. This includes the time taken to mix
the MOD musics and sample streams, and also the time taken by any user
DSP functions. It does not include plain sample mixing which is done by
the output device (hardware accelerated) or DirectSound (emulated). Audio
CD playback requires no CPU usage.
RETURN : The CPU usage percentage (floating-point) */

BOOL BASSDEF(BASS_Start)();
/* Start the digital output. */

BOOL BASSDEF(BASS_Stop)();
/* Stop the digital output, stopping all musics/samples/streams. */

BOOL BASSDEF(BASS_Pause)();
/* Stop the digital output, pausing all musics/samples/streams. Use
BASS_Start() to resume the digital output. */

BOOL BASSDEF(BASS_SetVolume)(DWORD volume);
/* Set the digital output master volume.
volume : Desired volume level (0-100) */

int BASSDEF(BASS_GetVolume)();
/* Get the digital output master volume.
RETURN : The volume level (0-100, -1=error) */

BOOL BASSDEF(BASS_Set3DFactors)(float distf, float rollf, float doppf);
/* Set the factors that affect the calculations of 3D sound.
distf  : Distance factor (0.0-10.0, 1.0=use meters, 0.3=use feet, <0.0=leave current)
         By default BASS measures distances in meters, you can change this
         setting if you are using a different unit of measurement.
roolf  : Rolloff factor, how fast the sound quietens with distance
         (0.0=no rolloff, 1.0=real world, 2.0=2x real... 10.0=max, <0.0=leave current)
doppf  : Doppler factor (0.0=no doppler, 1.0=real world, 2.0=2x real... 10.0=max, <0.0=leave current)
         The doppler effect is the way a sound appears to change frequency when it is
         moving towards or away from you. The listener and sound velocity settings are
         used to calculate this effect, this "doppf" value can be used to lessen or
         exaggerate the effect. */

BOOL BASSDEF(BASS_Get3DFactors)(float *distf, float *rollf, float *doppf);
/* Get the factors that affect the calculations of 3D sound.
distf  : Distance factor (NULL=don't get it)
roolf  : Rolloff factor (NULL=don't get it)
doppf  : Doppler factor (NULL=don't get it) */

BOOL BASSDEF(BASS_Set3DPosition)(BASS_3DVECTOR *pos, BASS_3DVECTOR *vel, BASS_3DVECTOR *front, BASS_3DVECTOR *top);
/* Set the position/velocity/orientation of the listener (ie. the player/viewer).
pos    : Position of the listener (NULL=leave current)
vel    : Listener's velocity, used to calculate doppler effect (NULL=leave current)
front  : Direction that listener's front is pointing (NULL=leave current)
top    : Direction that listener's top is pointing (NULL=leave current)
         NOTE: front & top must both be set in a single call */

BOOL BASSDEF(BASS_Get3DPosition)(BASS_3DVECTOR *pos, BASS_3DVECTOR *vel, BASS_3DVECTOR *front, BASS_3DVECTOR *top);
/* Get the position/velocity/orientation of the listener.
pos    : Position of the listener (NULL=don't get it)
vel    : Listener's velocity (NULL=don't get it)
front  : Direction that listener's front is pointing (NULL=don't get it)
top    : Direction that listener's top is pointing (NULL=don't get it)
         NOTE: front & top must both be retrieved in a single call */

BOOL BASSDEF(BASS_Apply3D)();
/* Apply changes made to the 3D system. This must be called to apply any changes
made with BASS_Set3DFactors, BASS_Set3DPosition, BASS_ChannelSet3DAttributes or
BASS_ChannelSet3DPosition. It improves performance to have DirectSound do all the
required recalculating at the same time like this, rather than recalculating after
every little change is made. NOTE: This is automatically called when starting a 3D
sample with BASS_SamplePlay3D/Ex. */

BOOL BASSDEF(BASS_SetEAXParameters)(int env, float vol, float decay, float damp);
/* Set the type of EAX environment and it's parameters. Obviously, EAX functions
have no effect if no EAX supporting device (ie. SB Live) is used.
env    : Reverb environment (EAX_ENVIRONMENT_xxx, -1=leave current)
vol    : Volume of the reverb (0.0=off, 1.0=max, <0.0=leave current)
decay  : Time in seconds it takes the reverb to diminish by 60dB (0.1-20.0, <0.0=leave current)
damp   : The damping, high or low frequencies decay faster (0.0=high decays quickest,
         1.0=low/high decay equally, 2.0=low decays quickest, <0.0=leave current) */

BOOL BASSDEF(BASS_GetEAXParameters)(DWORD *env, float *vol, float *decay, float *damp);
/* Get the current EAX parameters.
env    : Reverb environment (NULL=don't get it)
vol    : Reverb volume (NULL=don't get it)
decay  : Decay duration (NULL=don't get it)
damp   : The damping (NULL=don't get it) */

BOOL BASSDEF(BASS_SetA3DResManager)(DWORD mode);
/* Set the A3D resource management mode.
mode   : The mode (A3D_RESMODE_OFF=disable resource management,
         A3D_RESMODE_DYNAMIC=enable resource manager but looping channels are not managed,
         A3D_RESMODE_DYNAMIC_LOOPERS=enable resource manager including looping channels,
         A3D_RESMODE_NOTIFY=plays will fail when there are no available resources) */

DWORD BASSDEF(BASS_GetA3DResManager)();
/* Get the A3D resource management mode.
RETURN : The mode (0xffffffff=error) */

BOOL BASSDEF(BASS_SetA3DHFAbsorbtion)(float factor);
/* Set the A3D high frequency absorbtion factor.
factor  : Absorbtion factor (0.0=disabled, <1.0=diminished, 1.0=default,
          >1.0=exaggerated) */

BOOL BASSDEF(BASS_GetA3DHFAbsorbtion)(float *factor);
/* Retrieve the current A3D high frequency absorbtion factor.
factor  : The absorbtion factor */


HMUSIC BASSDEF(BASS_MusicLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
/* Load a music (MO3/XM/MOD/S3M/IT/MTM). The amplification and pan
seperation are initially set to 50, use BASS_MusicSetAmplify()
and BASS_MusicSetPanSep() to adjust them.
mem    : TRUE = Load music from memory
file   : Filename (mem=FALSE) or memory location (mem=TRUE)
offset : File offset to load the music from (only used if mem=FALSE)
length : Data length (only used if mem=FALSE, 0=use to end of file)
flags  : BASS_MUSIC_xxx flags
RETURN : The loaded music's handle (NULL=error) */

void BASSDEF(BASS_MusicFree)(HMUSIC handle);
/* Free a music's resources.
handle : Music handle */

char *BASSDEF(BASS_MusicGetName)(HMUSIC handle);
/* Retrieves a music's name.
handle : Music handle
RETURN : The music's name (NULL=error) */

DWORD BASSDEF(BASS_MusicGetLength)(HMUSIC handle);
/* Retrieves the length of a music in patterns (ie. how many "orders"
there are).
handle : Music handle
RETURN : The length of the music (0xFFFFFFFF=error) */

BOOL BASSDEF(BASS_MusicPlay)(HMUSIC handle);
/* Play a music. Playback continues from where it was last stopped/paused.
Multiple musics may be played simultaneously.
handle : Handle of music to play */

BOOL BASSDEF(BASS_MusicPlayEx)(HMUSIC handle, DWORD pos, int flags, BOOL reset);
/* Play a music, specifying start position and playback flags.
handle : Handle of music to play
pos    : Position to start playback from, LOWORD=order HIWORD=row
flags  : BASS_MUSIC_xxx flags. These flags overwrite the defaults
         specified when the music was loaded. (-1=use current flags)
reset  : TRUE = Stop all current playing notes and reset bpm/etc... */

BOOL BASSDEF(BASS_MusicSetAmplify)(HMUSIC handle, DWORD amp);
/* Set a music's amplification level.
handle : Music handle
amp    : Amplification level (0-100) */

BOOL BASSDEF(BASS_MusicSetPanSep)(HMUSIC handle, DWORD pan);
/* Set a music's pan seperation.
handle : Music handle
pan    : Pan seperation (0-100, 50=linear) */

BOOL BASSDEF(BASS_MusicSetPositionScaler)(HMUSIC handle, DWORD scale);
/* Set a music's "GetPosition" scaler
When you call BASS_ChannelGetPosition, the "row" (HIWORD) will be
scaled by this value. By using a higher scaler, you can get a more
precise position indication.
handle : Music handle
scale  : The scaler (1-256) */


HSAMPLE BASSDEF(BASS_SampleLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD max, DWORD flags);
/* Load a WAV sample. If you're loading a sample with 3D functionality,
then you should use BASS_GetInfo and BASS_SetInfo to set the default 3D
parameters. You can also use these two functions to set the sample's
default frequency/volume/pan/looping.
mem    : TRUE = Load sample from memory
file   : Filename (mem=FALSE) or memory location (mem=TRUE)
offset : File offset to load the sample from (only used if mem=FALSE)
length : Data length (only used if mem=FALSE, 0=use to end of file)
max    : Maximum number of simultaneous playbacks (1-65535)
flags  : BASS_SAMPLE_xxx flags (only the LOOP/3D/SOFTWARE/VAM/MUTEMAX/OVER_xxx flags are used)
RETURN : The loaded sample's handle (NULL=error) */

void* BASSDEF(BASS_SampleCreate)(DWORD length, DWORD freq, DWORD max, DWORD flags);
/* Create a sample. This function allows you to generate custom samples, or
load samples that are not in the WAV format. A pointer is returned to the
memory location at which you should write the sample's data. After writing
the data, call BASS_SampleCreateDone to get the new sample's handle.
length : The sample's length (in samples, NOT bytes)
freq   : default sample rate
max    : Maximum number of simultaneous playbacks (1-65535)
flags  : BASS_SAMPLE_xxx flags
RETURN : Memory location to write the sample's data (NULL=error) */

HSAMPLE BASSDEF(BASS_SampleCreateDone)();
/* Finished creating a new sample.
RETURN : The new sample's handle (NULL=error) */

void BASSDEF(BASS_SampleFree)(HSAMPLE handle);
/* Free a sample's resources.
handle : Sample handle */

BOOL BASSDEF(BASS_SampleGetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
/* Retrieve a sample's current default attributes.
handle : Sample handle
info   : Pointer to store sample info */

BOOL BASSDEF(BASS_SampleSetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
/* Set a sample's default attributes.
handle : Sample handle
info   : Sample info, only the freq/volume/pan/3D attributes and
         looping/override method flags are used */

HCHANNEL BASSDEF(BASS_SamplePlay)(HSAMPLE handle);
/* Play a sample, using the sample's default attributes.
handle : Handle of sample to play
RETURN : Handle of channel used to play the sample (NULL=error) */

HCHANNEL BASSDEF(BASS_SamplePlayEx)(HSAMPLE handle, DWORD start, int freq, int volume, int pan, BOOL loop);
/* Play a sample, using specified attributes.
handle : Handle of sample to play
start  : Playback start position (in samples, not bytes)
freq   : Playback rate (-1=default)
volume : Volume (-1=default, 0=silent, 100=max)
pan    : Pan position (-101=default, -100=left, 0=middle, 100=right)
loop   : TRUE = Loop sample (-1=default)
RETURN : Handle of channel used to play the sample (NULL=error) */

HCHANNEL BASSDEF(BASS_SamplePlay3D)(HSAMPLE handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
/* Play a 3D sample, setting it's 3D position, orientation and velocity.
handle : Handle of sample to play
pos    : position of the sound (NULL = x/y/z=0.0)
orient : orientation of the sound, this is irrelevant if it's an
         omnidirectional sound source (NULL = x/y/z=0.0)
vel    : velocity of the sound (NULL = x/y/z=0.0)
RETURN : Handle of channel used to play the sample (NULL=error) */

HCHANNEL BASSDEF(BASS_SamplePlay3DEx)(HSAMPLE handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel, DWORD start, int freq, int volume, BOOL loop);
/* Play a 3D sample, using specified attributes.
handle : Handle of sample to play
pos    : position of the sound (NULL = x/y/z=0.0)
orient : orientation of the sound, this is irrelevant if it's an
         omnidirectional sound source (NULL = x/y/z=0.0)
vel    : velocity of the sound (NULL = x/y/z=0.0)
start  : Playback start position (in samples, not bytes)
freq   : Playback rate (-1=default)
volume : Volume (-1=default, 0=silent, 100=max)
loop   : TRUE = Loop sample (-1=default)
RETURN : Handle of channel used to play the sample (NULL=error) */

BOOL BASSDEF(BASS_SampleStop)(HSAMPLE handle);
/* Stops all instances of a sample. For example, if a sample is playing
simultaneously 3 times, calling this function will stop all 3 of them,
which is obviously simpler than calling BASS_ChannelStop() 3 times.
handle : Handle of sample to stop */


HSTREAM BASSDEF(BASS_StreamCreate)(DWORD freq, DWORD flags, STREAMPROC *proc, DWORD user);
/* Create a user sample stream.
freq   : Stream playback rate (100-100000)
flags  : BASS_SAMPLE_xxx flags (only the 8BITS/MONO/3D flags are used)
proc   : User defined stream writing function
user   : The 'user' value passed to the callback function
RETURN : The created stream's handle (NULL=error) */

HSTREAM BASSDEF(BASS_StreamCreateFile)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
/* Create a sample stream from an MP3 or WAV file.
mem    : TRUE = Stream file from memory
file   : Filename (mem=FALSE) or memory location (mem=TRUE)
offset : File offset of the stream data
length : File length (0=use whole file if mem=FALSE)
flags  : BASS_SAMPLE_3D/BASS_SAMPLE_MONO/BASS_MP3_HALFRATE/BASS_MP3_SETPOS flags
RETURN : The created stream's handle (NULL=error) */

void BASSDEF(BASS_StreamFree)(HSTREAM handle);
/* Free a sample stream's resources.
handle : Stream handle */

DWORD BASSDEF(BASS_StreamGetLength)(HSTREAM handle);
/* Retrieves the playback length (in bytes) of a file stream. It's not always
possible to 100% accurately guess the length of a stream, so the length returned
may be only an approximation when using some WAV codecs.
handle : Stream handle
RETURN : The length (0xffffffff=error) */

DWORD BASSDEF(BASS_StreamGetBlockLength)(HSTREAM handle);
/* Retrieves the playback length (in bytes) of a block in a file stream.
handle : Stream handle
RETURN : The block length (0xffffffff=error) */

BOOL BASSDEF(BASS_StreamPlay)(HSTREAM handle, BOOL flush, DWORD flags);
/* Play a sample stream, optionally flushing the buffer first.
handle : Handle of stream to play
flush  : Flush buffer contents. If you stop a stream and then want to
         continue it from where it stopped, don't flush it. Flushing
         a file stream causes it to restart from the beginning.
flags  : BASS_SAMPLE_LOOP flag (only affects file streams) */


BOOL BASSDEF(BASS_CDInit)(char *drive);
/* Initialize the CD functions, must be called before any other CD
functions. The volume is initially set to 100 (the maximum), use
BASS_ChannelSetAttributes() to adjust it.
drive  : The CD drive, for example: "d:" (NULL=use default drive) */

void BASSDEF(BASS_CDFree)();
/* Free resources used by the CD */

BOOL BASSDEF(BASS_CDInDrive)();
/* Check if there is a CD in the drive. */

BOOL BASSDEF(BASS_CDPlay)(DWORD track, BOOL loop, BOOL wait);
/* Play a CD track.
track  : Track number to play (1=first)
loop   : TRUE = Loop the track
wait   : TRUE = don't return until playback has started (some drives
                will always wait anyway) */


/* A "channel" can be a playing sample (HCHANNEL), a MOD music (HMUSIC),
a sample stream (HSTREAM), or the CD (CDCHANNEL). The following
functions can be used with one or more of these channel types. */

BOOL BASSDEF(BASS_ChannelIsActive)(DWORD handle);
/* Check if a channel is active (playing).
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL) */

DWORD BASSDEF(BASS_ChannelGetFlags)(DWORD handle);
/* Get some info about a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM)
RETURN : BASS_SAMPLE_xxx flags (0xffffffff=error) */

BOOL BASSDEF(BASS_ChannelStop)(DWORD handle);
/* Stop a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL) */

BOOL BASSDEF(BASS_ChannelPause)(DWORD handle);
/* Pause a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL) */

BOOL BASSDEF(BASS_ChannelResume)(DWORD handle);
/* Resume a paused channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL) */

BOOL BASSDEF(BASS_ChannelSetAttributes)(DWORD handle, int freq, int volume, int pan);
/* Update a channel's attributes. The actual setting may not be exactly
as specified, depending on the accuracy of the device and drivers.
NOTE: Only the volume can be adjusted for the CD "channel", but not all
soundcards allow controlling of the CD volume level.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
freq   : Playback rate (100-100000, 0=original, -1=leave current)
volume : Volume (-1=leave current, 0=silent, 100=max)
pan    : Pan position (-101=current, -100=left, 0=middle, 100=right)
         panning has no effect on 3D channels */

BOOL BASSDEF(BASS_ChannelGetAttributes)(DWORD handle, DWORD *freq, DWORD *volume, int *pan);
/* Retrieve a channel's attributes. Only the volume is available for
the CD "channel" (if allowed by the soundcard/drivers).
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
freq   : Pointer to store playback rate (NULL=don't retrieve it)
volume : Pointer to store volume (NULL=don't retrieve it)
pan    : Pointer to store pan position (NULL=don't retrieve it) */

BOOL BASSDEF(BASS_ChannelSet3DAttributes)(DWORD handle, int mode, float min, float max, int iangle, int oangle, int outvol);
/* Set a channel's 3D attributes.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
mode   : BASS_3DMODE_xxx mode (-1=leave current setting)
min    : minimum distance, volume stops increasing within this distance (<0.0=leave current)
max    : maximum distance, volume stops decreasing past this distance (<0.0=leave current)
iangle : angle of inside projection cone in degrees (360=omnidirectional, -1=leave current)
oangle : angle of outside projection cone in degrees (-1=leave current)
         NOTE: iangle & oangle must both be set in a single call
outvol : delta-volume outside the projection cone (0=silent, 100=same as inside)
The iangle/oangle angles decide how wide the sound is projected around the
orientation angle. Within the inside angle the volume level is the channel
level as set with BASS_ChannelSetAttributes, from the inside to the outside
angles the volume gradually changes by the "outvol" setting. */

BOOL BASSDEF(BASS_ChannelGet3DAttributes)(DWORD handle, DWORD *mode, float *min, float *max, DWORD *iangle, DWORD *oangle, DWORD *outvol);
/* Retrieve a channel's 3D attributes.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
mode   : BASS_3DMODE_xxx mode (NULL=don't retrieve it)
min    : minumum distance (NULL=don't retrieve it)
max    : maximum distance (NULL=don't retrieve it)
iangle : angle of inside projection cone (NULL=don't retrieve it)
oangle : angle of outside projection cone (NULL=don't retrieve it)
         NOTE: iangle & oangle must both be retrieved in a single call
outvol : delta-volume outside the projection cone (NULL=don't retrieve it) */

BOOL BASSDEF(BASS_ChannelSet3DPosition)(DWORD handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
/* Update a channel's 3D position, orientation and velocity. The velocity
is only used to calculate the doppler effect.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
pos    : position of the sound (NULL=leave current)
orient : orientation of the sound, this is irrelevant if it's an
         omnidirectional sound source (NULL=leave current)
vel    : velocity of the sound (NULL=leave current) */

BOOL BASSDEF(BASS_ChannelGet3DPosition)(DWORD handle, BASS_3DVECTOR *pos, BASS_3DVECTOR *orient, BASS_3DVECTOR *vel);
/* Retrieve a channel's current 3D position, orientation and velocity.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
pos    : position of the sound (NULL=don't retrieve it)
orient : orientation of the sound, this is irrelevant if it's an
         omnidirectional sound source (NULL=don't retrieve it)
vel    : velocity of the sound (NULL=don't retrieve it) */

BOOL BASSDEF(BASS_ChannelSetPosition)(DWORD handle, DWORD pos);
/* Set the current playback position of a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
pos    : the position
    if HCHANNEL: position in bytes
    if HMUSIC: LOWORD=order HIWORD=row ... use MAKELONG(order,row)
    if HSTREAM: position in bytes, file streams (WAV/MP3) only (MP3s require BASS_MP3_SETPOS)
    if CDCHANNEL: position in milliseconds from start of track */

DWORD BASSDEF(BASS_ChannelGetPosition)(DWORD handle);
/* Get the current playback position of a channel.
handle : Channel handle (HCHANNEL/HMUSIC/HSTREAM, or CDCHANNEL)
RETURN : the position (0xffffffff=error)
	if HCHANNEL: position in bytes
	if HMUSIC: LOWORD=order HIWORD=row (see BASS_MusicSetPositionScaler)
	if HSTREAM: total bytes played since the stream was last flushed
	if CDCHANNEL: position in milliseconds from start of track */

DWORD BASSDEF(BASS_ChannelGetLevel)(DWORD handle);
/* Calculate a channel's current output level.
handle : Channel handle (HMUSIC/HSTREAM)
RETURN : LOWORD=left level (0-128) HIWORD=right level (0-128) (0xffffffff=error) */

DWORD BASSDEF(BASS_ChannelGetData)(DWORD handle, void *buffer, DWORD length);
/* Retrieves upto "length" bytes of the channel's current sample data. This is
useful if you wish to "visualize" the sound.
handle : Channel handle (HMUSIC/HSTREAM)
buffer : Location to write the sample data
length : Number of bytes wanted
RETURN : Number of bytes actually written to the buffer (0xffffffff=error) */

HSYNC BASSDEF(BASS_ChannelSetSync)(DWORD handle, DWORD type, DWORD param, SYNCPROC *proc, DWORD user);
/* Setup a sync on a channel. Multiple syncs may be used per channel.
handle : Channel handle
type   : Sync type (BASS_SYNC_xxx type & flags)
param  : Sync parameters (see the BASS_SYNC_xxx type description)
proc   : User defined callback function
user   : The 'user' value passed to the callback function
RETURN : Sync handle (NULL=error) */

BOOL BASSDEF(BASS_ChannelRemoveSync)(DWORD handle, HSYNC sync);
/* Remove a sync from a channel
handle : Channel handle
sync   : Handle of sync to remove */

HDSP BASSDEF(BASS_ChannelSetDSP)(DWORD handle, DSPPROC *proc, DWORD user);
/* Setup a user DSP function on a channel. When multiple DSP functions
are used on a channel, they are called in the order that they were added.
handle : Channel handle (HMUSIC/HSTREAM)
proc   : User defined callback function
user   : The 'user' value passed to the callback function
RETURN : DSP handle (NULL=error) */

BOOL BASSDEF(BASS_ChannelRemoveDSP)(DWORD handle, HDSP dsp);
/* Remove a DSP function from a channel
handle : Channel handle (HMUSIC/HSTREAM)
dsp    : Handle of DSP to remove */

BOOL BASSDEF(BASS_ChannelSetEAXMix)(DWORD handle, float mix);
/* Set the wet(reverb)/dry(no reverb) mix ratio on the channel. By default
the distance of the sound from the listener is used to calculate the mix.
NOTE: The channel must have 3D functionality enabled for the EAX environment
to have any affect on it.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
mix    : The ratio (0.0=reverb off, 1.0=max reverb, -1.0=let EAX calculate
         the reverb mix based on the distance) */

BOOL BASSDEF(BASS_ChannelGetEAXMix)(DWORD handle, float *mix);
/* Get the wet(reverb)/dry(no reverb) mix ratio on the channel.
handle : Channel handle (HCHANNEL/HSTREAM/HMUSIC)
mix    : Pointer to store the ratio at */

#ifdef __cplusplus
}
#endif

#endif
