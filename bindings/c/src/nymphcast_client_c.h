/*
	nymphcast_client_c.h - Header file for the libnymphcast C99 binding.
	
	Revision 0
	
	Features:
			- C version of the C++-based libnymphcast API.
			- Requires C99.
			
	Notes:
			- Clients include this header & compile the .cpp source file as C++ prior to linking.
			
	2023/12/01, Maya Posch
*/


#ifndef NYMPHCAST_CLIENT_C_H
#define NYMPHCAST_CLIENT_C_H

#include <stdint.h>
#include <stdbool.h>


typedef struct NC_NymphCastRemote {
	char* name;
	char* ipv4;
	char* ipv6;
	uint16_t port;
} NC_NymphCastRemote;


#ifndef NYMPHCAST_CLIENT_H
typedef enum NymphRemoteStatus {
	NYMPH_PLAYBACK_STATUS_STOPPED = 1,
	NYMPH_PLAYBACK_STATUS_PLAYING = 2,
	NYMPH_PLAYBACK_STATUS_PAUSED = 3
} NymphRemoteStatus;
#endif


typedef struct NC_NymphPlaybackStatus {
	NymphRemoteStatus status;
	bool error;
	bool stopped;
	bool playing;
	uint64_t duration;
	double position;
	uint8_t volume;
	bool subtitles_off;
	char* title;
	char* artist;
} NC_NymphPlaybackStatus;


#ifndef NYMPHCAST_CLIENT_H
typedef enum NymphMediaFileType {
	FILE_TYPE_AUDIO = 0,
	FILE_TYPE_VIDEO = 1,
	FILE_TYPE_IMAGE = 2,
	FILE_TYPE_PLAYLIST = 3
} NymphMediaFileType;
#endif


#ifndef NYMPHCAST_CLIENT_H
typedef enum NymphSeekType {
	NYMPH_SEEK_TYPE_BYTES = 1,
	NYMPH_SEEK_TYPE_PERCENTAGE = 2
} NymphSeekType;
#endif


typedef struct NC_NymphMediaFile {
	NC_NymphCastRemote mediaserver;
	uint32_t id;
	char* name;
	char* section;
	NymphMediaFileType type;
} NC_NymphMediaFile;


typedef void (*NC_AppMessageFunction)(char*, char*);
typedef void (*NC_StatusUpdateFunction)(uint32_t, NC_NymphPlaybackStatus);


bool init_nymphCastClient();
bool delete_nymphCastClient();

void NC_setClientId(char* id, uint32_t len);
void NC_setApplicationCallback(NC_AppMessageFunction function);
void NC_setStatusUpdateCallback(NC_StatusUpdateFunction function);
char* NC_getApplicationList(uint32_t handle, char* list, uint32_t* size);
char* NC_sendApplicationMessage(uint32_t handle, char* appId, char* message, uint8_t format,
								char* response, uint32_t* length);
bool NC_loadResource(uint32_t handle, char* appId, char* name, char* response, uint32_t* length);

bool NC_findServers(NC_NymphCastRemote* servers, uint32_t* count);
bool NC_findShares(NC_NymphCastRemote* servers, uint32_t* count);
bool NC_connectServer(char* ip, uint32_t port, uint32_t* handle);
bool NC_disconnectServer(uint32_t handle);

bool NC_getShares(NC_NymphCastRemote mediaserver, NC_NymphMediaFile* files, uint32_t* count);
bool NC_playShare(NC_NymphMediaFile file, NC_NymphCastRemote* receivers, uint32_t count);

bool NC_addSlaves(uint32_t handle, NC_NymphCastRemote* remotes, uint32_t count);
bool NC_castFile(uint32_t handle, char* filename);
bool NC_castUrl(uint32_t handle, char* url);

uint8_t NC_volumeSet(uint32_t handle, uint8_t volume);
uint8_t NC_volumeUp(uint32_t handle);
uint8_t NC_volumeDown(uint32_t handle);
uint8_t NC_volumeMute(uint32_t handle);

uint8_t NC_playbackStart(uint32_t handle);
uint8_t NC_playbackStop(uint32_t handle);
uint8_t NC_playbackPause(uint32_t handle);
uint8_t NC_playbackRewind(uint32_t handle);
uint8_t NC_playbackForward(uint32_t handle);
uint8_t NC_playbackSeek(uint32_t handle, NymphSeekType type, uint64_t value);
NC_NymphPlaybackStatus NC_playbackStatus(uint32_t handle);

uint8_t NC_cycleSubtitles(uint32_t handle);
uint8_t NC_cycleAudio(uint32_t handle);
uint8_t NC_cycleVideo(uint32_t handle);
uint8_t NC_enableSubtitles(uint32_t handle, bool state);


#endif
