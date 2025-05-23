/*
	nymphcast_client.h - Header file for the NymphCast client library.
	
	Revision 0
	
	Notes:
			-
			
	2019/10/26, Maya Posch
*/


#ifndef NYMPHCAST_CLIENT_H
#define NYMPHCAST_CLIENT_H


#include <string>
#include <fstream>
#include <functional>
#include <vector>

#include <nymph/nymph.h>


struct NymphCastRemote {
	std::string name;
	std::string ipv4;
	std::string ipv6;
	uint16_t port;
};


enum NymphRemoteStatus {
	NYMPH_PLAYBACK_STATUS_STOPPED = 1,
	NYMPH_PLAYBACK_STATUS_PLAYING = 2,
	NYMPH_PLAYBACK_STATUS_PAUSED = 3
};


struct NymphPlaybackStatus {
	NymphRemoteStatus status;
	bool error;
	bool stopped;
	bool playing;
	uint64_t duration;
	double position;
	uint8_t volume;
	bool subtitles_off;
	std::string title;
	std::string artist;
};


enum NymphMediaFileType {
	FILE_TYPE_AUDIO = 0,
	FILE_TYPE_VIDEO = 1,
	FILE_TYPE_IMAGE = 2,
	FILE_TYPE_PLAYLIST = 3
};

enum NymphSeekType {
	NYMPH_SEEK_TYPE_BYTES = 1,
	NYMPH_SEEK_TYPE_PERCENTAGE = 2
};


struct NymphMediaFile {
	NymphCastRemote mediaserver;
	uint32_t id;
	std::string name;
	std::string section;
	std::string rel_path;
	NymphMediaFileType type;
};


typedef std::function<void(std::string appId, std::string message)> AppMessageFunction;
typedef std::function<void(uint32_t handle, NymphPlaybackStatus status)> StatusUpdateFunction;


class NymphCastClient {
	std::string clientId = "NymphClient_21xb";
	std::ifstream source;
	
	AppMessageFunction appMessageFunction;
	StatusUpdateFunction statusUpdateFunction;
	
	void MediaReadCallback(uint32_t session, NymphMessage* msg, void* data);
	void MediaStopCallback(uint32_t session, NymphMessage* msg, void* data);
	void MediaSeekCallback(uint32_t session, NymphMessage* msg, void* data);
	void MediaStatusCallback(uint32_t session, NymphMessage* msg, void* data);
	void ReceiveFromAppCallback(uint32_t session, NymphMessage* msg, void* data);
	
public:
	NymphCastClient();
	~NymphCastClient();
	
	void setClientId(std::string id);
	void setApplicationCallback(AppMessageFunction function);
	void setStatusUpdateCallback(StatusUpdateFunction function);
	std::string getApplicationList(uint32_t handle);
	std::string sendApplicationMessage(uint32_t handle, std::string &appId, std::string &message, 
																				uint8_t format = 0);
	std::string loadResource(uint32_t handle, std::string &appId, std::string &name);

	std::vector<NymphCastRemote> findServers();
	std::vector<NymphCastRemote> findShares();
	bool connectServer(std::string ip, uint32_t port, uint32_t &handle);
	bool disconnectServer(uint32_t handle);
	
	std::vector<NymphMediaFile> getShares(NymphCastRemote mediaserver);
	uint8_t playShare(NymphMediaFile file, std::vector<NymphCastRemote> receivers);
	
	std::vector<NymphMediaFile> getReceiverShares(uint32_t handle);
	bool playReceiverShare(uint32_t handle, NymphMediaFile file);
	
	bool addSlaves(uint32_t handle, std::vector<NymphCastRemote> remotes);
	bool castFile(uint32_t handle, std::string filename);
	bool castUrl(uint32_t handle, std::string &url);
	
	uint8_t volumeSet(uint32_t handle, uint8_t volume);
	uint8_t volumeUp(uint32_t handle);
	uint8_t volumeDown(uint32_t handle);
	uint8_t volumeMute(uint32_t handle);
	
	uint8_t playbackStart(uint32_t handle);
	uint8_t playbackStop(uint32_t handle);
	uint8_t playbackPause(uint32_t handle);
	uint8_t playbackRewind(uint32_t handle);
	uint8_t playbackForward(uint32_t handle);
	uint8_t playbackSeek(uint32_t handle, NymphSeekType type, uint64_t value);
	NymphPlaybackStatus playbackStatus(uint32_t handle);
	
	uint8_t cycleSubtitles(uint32_t handle);
	uint8_t cycleAudio(uint32_t handle);
	uint8_t cycleVideo(uint32_t handle);
	uint8_t enableSubtitles(uint32_t handle, bool state);
};


#endif
