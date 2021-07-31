# NymphCast Client Library (libnymphcast) #

Libnymphcast is a library containing the core functionality for a [NymphCast](https://github.com/MayaPosch/NymphCast) client. This includes:

- Streaming media files to a remote NymphCast receiver.
- Sending a URL to a media file to a NymphCast receiver to initiate playback.
- Communication with remote NymphCast Apps.
- Multi-casting media content.
- Interact with [NymphCast MediaServers](https://github.com/MayaPosch/NymphCast-MediaServer).

## Compile from source ##

To compile libnymphcast from source, the following dependencies must be installed:

- [NymphRPC](https://github.com/MayaPosch/NymphRPC)
- LibPOCO (1.5+)

After this, the project can be compiled using a C++11 capable GCC compiler and make. 

After calling `make` in the root of the project folder, the library can be found in the `lib/` folder.
