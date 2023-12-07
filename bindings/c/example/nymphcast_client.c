/*
	nymphcast_client.c - Simple C client for Nymphcast.
	
	Revision 0
	
	Features:
			- 
			
	Notes:
			- 
		
	2023/12/01, Maya Posch
*/


#include "../src/nymphcast_client_c.h"

#include <stdio.h>


int main() {
	// Set up NymphCast library.
	if (!init_nymphCastClient()) { return 1; }
	
	// Obtain list of available NymphCast receivers (NCS).
	NC_NymphCastRemote* receivers;
	uint32_t recCount;
	if (!NC_findServers(receivers, &recCount)) { return 1; }
	
	// Print out list.
	printf("Receivers:\n#\tName\tIPv4\tIPv6\t\tPort\n");
	for (uint32_t i = 0; i < recCount; i++) {
		// Iterate through each entry and print out details.
		printf("%d:\t%s\t%s\t%s\t%d\n", i, receivers[i].name, 
						receivers[i].ipv4, receivers[i].ipv6, receivers[i].port);
	}
	
	// TODO: Obtain playback status of each receiver and print out.
	//NC_NymphPlaybackStatus stat = NC_playbackStatus(handle);
	
	// Obtain list of available NymphCast MediaServers (NCMS).
	NC_NymphCastRemote* shares;
	uint32_t shareCount;
	if (!NC_findShares(shares, &shareCount)) { return 1; }
	
	// Print out list.
	printf("MediaServers:\n#\tName\tIPv4\tIPv6\t\tPort\n");
	for (uint32_t i = 0; i < shareCount; i++) {
		// Iterate through each entry and print out details.
		printf("%d:\t%s\t%s\t%s\t%d\n", i, shares[i].name, 
						shares[i].ipv4, shares[i].ipv6, shares[i].port);
	}
	
	// Obtain number of files shared by each NCMS instance and print out.
	if (shareCount > 0) {
		NC_NymphMediaFile* files;
		uint32_t filecount;
		for (uint32_t j = 0; j < shareCount; j++) {
			if (!NC_getShares(shares[j], files, &filecount)) {
				return 1;
			}
			
			printf("\nNCMS: %s\n", shares[j].name);
			for (uint32_t k = 0; k < filecount; k++) {
				printf("\t%s\t%s\n", files[k].section, files[k].name);
			}
		}
	}
	
	// TODO: if at least one NCMS and one non-playing NCS instance exist, play back audio file
	// from an NCMS on the NCS.
	
	
	// Clean up.
	delete_nymphCastClient();
	
	return 0;
}
