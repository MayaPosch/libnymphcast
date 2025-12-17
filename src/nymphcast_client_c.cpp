/*
	nymphcast_client_c.cpp - Implementation of C99 binding for libnymphcast.
	
	Notes:
			- 
	
	2023/12/01, Maya Posch
*/

#include <nymphcast_client.h>	// C++ header.

extern "C" {
#include "nymphcast_client_c.h"	// C99 header.
}

extern "C" {
bool init_nymphCastClient();
bool delete_nymphCastClient();

void NC_setClientId(char* id, uint32_t len);
void NC_setLogLevel(NC_NymphLogLevels level);
void NC_setApplicationCallback(NC_AppMessageFunction function);
void NC_setStatusUpdateCallback(NC_StatusUpdateFunction function);
char* NC_getApplicationList(uint32_t handle, char* list, uint32_t* size);
char* NC_sendApplicationMessage(uint32_t handle, char* appId, char* message, uint8_t format,
								char* response, uint32_t* length);
bool NC_loadResource(uint32_t handle, char* appId, char* name, char* response, uint32_t* length);

bool NC_findServers(NC_NymphCastRemote** servers, uint32_t* count);
bool NC_findShares(NC_NymphCastRemote** servers, uint32_t* count);
bool NC_connectServer(char* ip, uint32_t port, uint32_t* handle);
bool NC_disconnectServer(uint32_t handle);

bool NC_getShares(NC_NymphCastRemote mediaserver, NC_NymphMediaFile** files, uint32_t* count);
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
}


// Globals
NymphCastClient client;


// Callbacks
NC_AppMessageFunction appMessageCB = 0;
void appMessageCallback(std::string appId, std::string message) {
	char* id = (char*) malloc(appId.size() + 1);
	memcpy(id, appId.c_str(), appId.size() + 1);
	char* msg = (char*) malloc(message.size() + 1);
	memcpy(msg, appId.c_str(), message.size() + 1);
	if (appMessageCB) {
		appMessageCB(id, msg);
	}
}


NC_StatusUpdateFunction statusUpdateCB = 0;
void statusUpdateCallback(uint32_t handle, NymphPlaybackStatus status) {
	NC_NymphPlaybackStatus st;
	st.status = (NymphRemoteStatus) status.status;
	st.error = status.error;
	st.stopped = status.stopped;
	st.playing = status.playing;
	st.duration = status.duration;
	st.position = status.position;
	st.volume = status.volume;
	st.subtitles_off = status.subtitles_off;
	st.title = (char*) malloc(status.title.size() + 1);
	memcpy(st.title, status.title.c_str(), status.title.size() + 1);
	st.artist = (char*) malloc(status.artist.size() + 1);
	memcpy(st.artist, status.artist.c_str(), status.artist.size() + 1);
	
	if (statusUpdateCB) {
		statusUpdateCB(handle, st);
	}
}
// ---


// --- INIT NYMPHCAST CLIENT ---
bool init_nymphCastClient() {
	// Nothing to do, client was initialised in global scope.
	
	// FIXME: Added to workaround Poco issue on Windows. Also see auto lib init disable in Makefile.
	Poco::Net::initializeNetwork();
	
	return true;
}


// --- DELETE NYMPHCAST CLIENT ---
bool delete_nymphCastClient() {
	// Nothing to do, as client lives in global scope.
	// FIXME: change client from stack to heap to enable init/delete?
	
	return true;
}


// --- SET CLIENT ID ---
void NC_setClientId(char* id, uint32_t len) {
	std::string str = std::string((const char*) id, (size_t) len);
	client.setClientId(str);
}

void NC_setLogLevel(NC_NymphLogLevels level) {
	client.setLogLevel((NymphLogLevels) level);
}


// --- SET APPLICATION CALLBACK ---
void NC_setApplicationCallback(NC_AppMessageFunction function) {
	// Set the internal callback, assign the provided function pointer to the internal reference.
	appMessageCB = function;
	client.setApplicationCallback(appMessageCallback);
}


// --- SET STATUS UPDATE CALLBACK ---
void NC_setStatusUpdateCallback(NC_StatusUpdateFunction function) {
	// Set the internal callback, assign the provided function pointer to the internal reference.
	statusUpdateCB = function;
	client.setStatusUpdateCallback(statusUpdateCallback);
}


// --- GET APPLICATION LIST ---
char* NC_getApplicationList(uint32_t handle, char* list, uint32_t size) {
	// Pass handle directly, create new buffer on heap and return it with the size in parameter.
	std::string res = client.getApplicationList(handle);
	list = (char*) malloc(res.size() + 1); 		// Remember null character.
	memcpy(list, res.c_str(), res.size() + 1);	// Also copy null character.
	size = res.size();
	return list;
}


// --- SEND APPLICATION MESSAGE ---
char* NC_sendApplicationMessage(uint32_t handle, char* appId, char* message, uint8_t format,
								char* response, uint32_t* length) {
	std::string id = std::string(appId);
	std::string msg = std::string(message);
	std::string res = client.sendApplicationMessage(handle, id, msg, format);
	response = (char*) malloc(res.size() + 1); 		// Remember null character.
	memcpy(response, res.c_str(), res.size() + 1);	// Also copy null character.
	*length = (uint32_t) res.size();
	return response;
}


// --- LOAD RESOURCE ---
bool NC_loadResource(uint32_t handle, char* appId, char* name, char* response, uint32_t* length) {
	std::string id = std::string(appId);
	std::string nm = std::string(name);
	std::string res = client.loadResource(handle, id, nm);
	if (res.empty()) { return false; }
	response = (char*) malloc(res.size() + 1); 		// Remember null character.
	memcpy(response, res.c_str(), res.size() + 1);	// Also copy null character.
	*length = res.size();
	
	return true;
}


// --- FIND SERVERS ---
bool NC_findServers(NC_NymphCastRemote** servers, uint32_t* count) {
	std::vector<NymphCastRemote> response;
	response = client.findServers();
	if (response.size() == 0) {
		servers = 0;
		*count = 0;
		return false;
	}
	
	// Allocate heap space for the new array.
	*count = (uint32_t) response.size();
	*servers = (NC_NymphCastRemote*) calloc(response.size(), sizeof(NC_NymphCastRemote));
	for (uint32_t i = 0; i < *count; i++) {
		// Copy data between the vector and array.
		(*servers)[i].name = (char*) malloc(response[i].name.size() + 1);
		memcpy((*servers)[i].name, response[i].name.c_str(), response[i].name.size() + 1);
		
		(*servers)[i].ipv4 = (char*) malloc(response[i].ipv4.size() + 1);
		memcpy((*servers)[i].ipv4, response[i].ipv4.c_str(), response[i].ipv4.size() + 1);
		
		(*servers)[i].ipv6 = (char*) malloc(response[i].ipv6.size() + 1);
		memcpy((*servers)[i].ipv6, response[i].ipv6.c_str(), response[i].ipv6.size() + 1);
		
		(*servers)[i].port = response[i].port;
	}
	
	return true;
}


// --- FIND SHARES ---
bool NC_findShares(NC_NymphCastRemote** servers, uint32_t* count) {
	std::vector<NymphCastRemote> response;
	response = client.findShares();
	if (response.size() == 0) {
		servers = 0;
		*count = 0;
		return false;
	}
	
	// Allocate heap space for the new array.
	*count = response.size();
	*servers = (NC_NymphCastRemote*) calloc(response.size(), sizeof(NC_NymphCastRemote));
	for (uint32_t i = 0; i < *count; i++) {
		// Copy data between the vector and array.
		(*servers)[i].name = (char*) malloc(response[i].name.size() + 1);
		memcpy((*servers)[i].name, response[i].name.c_str(), response[i].name.size() + 1);
		
		(*servers)[i].ipv4 = (char*) malloc(response[i].ipv4.size() + 1);
		memcpy((*servers)[i].ipv4, response[i].ipv4.c_str(), response[i].ipv4.size() + 1);
		
		(*servers)[i].ipv6 = (char*) malloc(response[i].ipv6.size() + 1);
		memcpy((*servers)[i].ipv6, response[i].ipv6.c_str(), response[i].ipv6.size() + 1);
		
		(*servers)[i].port = response[i].port;
	}
	
	return true;
}


// --- CONNECT SERVER ---
bool NC_connectServer(char* ip, uint32_t port, uint32_t* handle) {
	std::string host = std::string(ip);
	return client.connectServer(host, port, *handle);
}


// --- DISCONNECT SERVER ---
bool NC_disconnectServer(uint32_t handle) {
	return client.disconnectServer(handle);
}


// --- GET SHARES ---
bool NC_getShares(NC_NymphCastRemote mediaserver, NC_NymphMediaFile** files, uint32_t* count) {
	// Convert between the different data structures before and after the remote call.
	NymphCastRemote ms;
	ms.name = std::string(mediaserver.name);
	ms.ipv4 = std::string(mediaserver.ipv4);
	ms.ipv6 = std::string(mediaserver.ipv6);
	ms.port = mediaserver.port;
	std::vector<NymphMediaFile> list;
	list = client.getShares(ms);
	if (list.size() == 0) {
		files = 0;
		*count = 0;
		return false;
	}
	
	// Allocate heap space for the new array.
	*count = list.size();
	*files = (NC_NymphMediaFile*) calloc(list.size(), sizeof(NC_NymphMediaFile));
	for (uint32_t i = 0; i < *count; i++) {
		// Copy data between the vector and array.
		(*files)[i].mediaserver.name = (char*) malloc(list[i].mediaserver.name.size() + 1);
		memcpy((*files)[i].mediaserver.name, list[i].mediaserver.name.c_str(), 
										list[i].mediaserver.name.size() + 1);
		
		(*files)[i].mediaserver.ipv4 = (char*) malloc(list[i].mediaserver.ipv4.size() + 1);
		memcpy((*files)[i].mediaserver.ipv4, list[i].mediaserver.ipv4.c_str(), 
										list[i].mediaserver.ipv4.size() + 1);
		
		(*files)[i].mediaserver.ipv6 = (char*) malloc(list[i].mediaserver.ipv6.size() + 1);
		memcpy((*files)[i].mediaserver.ipv6, list[i].mediaserver.ipv6.c_str(), 
										list[i].mediaserver.ipv6.size() + 1);
										
		(*files)[i].mediaserver.port = list[i].mediaserver.port;
		
		(*files)[i].id = list[i].id;
		
		(*files)[i].name = (char*) malloc(list[i].name.size() + 1);
		memcpy((*files)[i].name, list[i].name.c_str(), list[i].name.size() + 1);
		
		(*files)[i].section = (char*) malloc(list[i].section.size() + 1);
		memcpy((*files)[i].section, list[i].section.c_str(), list[i].section.size() + 1);
		
		(*files)[i].type = list[i].type;
	}
	
	return true;
}


// --- PLAY SHARE ---
bool NC_playShare(NC_NymphMediaFile file, NC_NymphCastRemote* receivers, uint32_t count) {
	// Convert all receivers and the file structs before transmission.
	NymphMediaFile f;
	f.mediaserver.name = std::string(file.mediaserver.name);
	f.mediaserver.ipv4 = std::string(file.mediaserver.ipv4);
	f.mediaserver.ipv4 = std::string(file.mediaserver.ipv6);
	f.mediaserver.port = file.mediaserver.port;
	
	std::vector<NymphCastRemote> remotes;
	NymphCastRemote remote;
	for (uint32_t i = 0; i < count; i++) {
		remote.name = std::string(receivers[i].name);
		remote.ipv4 = std::string(receivers[i].ipv4);
		remote.ipv6 = std::string(receivers[i].ipv6);
		remote.port = receivers[i].port;
		
		remotes.push_back(remote);
	}
	
	return client.playShare(f, remotes);
}


// --- ADD SLAVES ---
bool NC_addSlaves(uint32_t handle, NC_NymphCastRemote* remotes, uint32_t count) {
	std::vector<NymphCastRemote> slaves;
	NymphCastRemote remote;
	for (uint32_t i = 0; i < count; i++) {
		remote.name = std::string(remotes[i].name);
		remote.ipv4 = std::string(remotes[i].ipv4);
		remote.ipv6 = std::string(remotes[i].ipv6);
		remote.port = remotes[i].port;
		
		slaves.push_back(remote);
	}
	
	return client.addSlaves(handle, slaves);
}


// --- CAST FILE ---
bool NC_castFile(uint32_t handle, char* filename) {
	std::string fn = std::string(filename);
	
	return client.castFile(handle, fn);
}


// --- CAST URL ---
bool NC_castUrl(uint32_t handle, char* url) {
	std::string uri = std::string(url);
	
	return client.castUrl(handle, uri);
}


// --- VOLUME SET ---
uint8_t NC_volumeSet(uint32_t handle, uint8_t volume) {
	return client.volumeSet(handle, volume);
}


// --- VOLUME UP ---
uint8_t NC_volumeUp(uint32_t handle) {
	return client.volumeUp(handle);
}


// --- VOLUME DOWN ---
uint8_t NC_volumeDown(uint32_t handle) {
	return client.volumeDown(handle);
}


// --- VOLUME MUTE ---
uint8_t NC_volumeMute(uint32_t handle) {
	return client.volumeMute(handle);
}


// --- PLAYBACK START ---
uint8_t NC_playbackStart(uint32_t handle) {
	return client.playbackStart(handle);
}


// --- PLAYBACK STOP ---
uint8_t NC_playbackStop(uint32_t handle) {
	return client.playbackStop(handle);
}


// --- PLAYBACK PAUSE ---
uint8_t NC_playbackPause(uint32_t handle) {
	return client.playbackPause(handle);
}


// --- PLAYBACK REWIND ---
uint8_t NC_playbackRewind(uint32_t handle) {
	return client.playbackRewind(handle);
}


// --- PLAYBACK FORWARD ---
uint8_t NC_playbackForward(uint32_t handle) {
	return client.playbackForward(handle);
}


// --- PLAYBACK SEEK ---
uint8_t NC_playbackSeek(uint32_t handle, NymphSeekType type, uint64_t value) {
	NymphSeekType t = (NymphSeekType) type;
	
	return client.playbackSeek(handle, t, value);
}


// --- PLAYBACK STATUS ---
NC_NymphPlaybackStatus NC_playbackStatus(uint32_t handle) {
	NymphPlaybackStatus res = client.playbackStatus(handle);
	
	NC_NymphPlaybackStatus status;
	status.status = (NymphRemoteStatus) res.status;
	status.error = res.error;
	status.stopped = res.stopped;
	status.playing = res.playing;
	status.duration = res.duration;
	status.position = res.position;
	status.volume = res.volume;
	status.subtitles_off = res.subtitles_off;
	status.title = (char*) malloc(res.title.size() + 1);
	memcpy(status.title, res.title.c_str(), res.title.size() + 1);
	status.artist = (char*) malloc(res.artist.size() + 1);
	memcpy(status.artist, res.artist.c_str(), res.artist.size() + 1);
	
	return status;
}


// --- CYCLE SUBTITLES ---
uint8_t NC_cycleSubtitles(uint32_t handle) {
	return client.cycleSubtitles(handle);
}


// --- CYCLE AUDIO
uint8_t NC_cycleAudio(uint32_t handle) {
	return client.cycleAudio(handle);
}


// --- CYCLE VIDEO ---
uint8_t NC_cycleVideo(uint32_t handle) {
	return client.cycleVideo(handle);
}


// --- ENABLE SUBTITLES ---
uint8_t NC_enableSubtitles(uint32_t handle, bool state) {
	return client.enableSubtitles(handle, state);
}
