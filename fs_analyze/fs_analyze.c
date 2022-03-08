#include <stdio.h>
#include <getopt.h>

#include "ext4_analyze.h"

int main(int argc, char **argv) {
	int ch, ext4 = 0;
	int err = 0;
	
	struct option longopts[] = {
		{ "ext4",	no_argument,	NULL,	'e' },
		{ NULL,				0,		NULL,	0 }
	};

	while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
		switch (ch) {
			case 'e':
				ext4 = 1;
				break;
			default:
				printf("usage: %s --ext4 <block device>\n", argv[0]);
				return -1;
		}
	}
    
    if (!ext4) {
        printf("usage: %s --ext4 <block device>\n", argv[0]);
        return -1;
    }

	argc--;
	argv++;	
	
	err = ext4_analyze(argc, argv);
    
	return err;
}
