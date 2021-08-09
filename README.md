# NymphCast Client Library (libnymphcast) #

Libnymphcast is a library containing the core functionality for a [NymphCast](https://github.com/MayaPosch/NymphCast) client. This includes:

- Streaming media files to a remote NymphCast receiver.
- Sending a URL to a media file to a NymphCast receiver to initiate playback.
- Communication with remote NymphCast Apps.
- Multi-casting media content.
- Interact with [NymphCast MediaServers](https://github.com/MayaPosch/NymphCast-MediaServer).

## Binary releases ##

Binary releases of libnymphrpc are available for the following platforms:

**Alpine-based:** [libnymphcast](https://pkgs.alpinelinux.org/packages?name=libnymphcast&branch=edge)

## Compile from source ##

To compile libnymphcast from source, the following dependencies must be installed:

- [NymphRPC](https://github.com/MayaPosch/NymphRPC)
- LibPOCO (1.5+)

After this, the project can be compiled using a C++11 capable GCC compiler and make. 

After calling `make` in the root of the project folder, the library can be found in the `lib/` folder in a platform-specific sub-folder.

## Android target ##

In order to compile for Android platforms, ensure that the Clang-based cross-compiler is accessible on the system PATH, and that libPoco has been compiled & made available. The use of the [POCO-build](https://github.com/MayaPosch/Poco-build) project is recommended here.

With these dependencies in place, compiling for any of the specific Android platforms is done by adding any of the following behind the `make` command:

- **ANDROID=1** for targeting ARMv7-based (32-bit) Android.
- **ANDROID64=1** for targeting ARMv8-based (64-bit) Android.
- **ANDROIDX86=1** for targeting x86-based (32-bit) Android.

## Installation ##

On supported platforms (Linux-based), installation of the library can be performed using:

```
sudo make install
```


