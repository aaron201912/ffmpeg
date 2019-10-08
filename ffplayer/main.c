#include <stdio.h>

#include "player.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Please provide a movie file, usage: \n");
        printf("example: ./Ssplayer ring.mp4 0\n");
        printf("0 : soft-decoding  1 : hard-decoding\n");
        return -1;
    }
    printf("Try playing %s ...\n", argv[1]);
    player_running(argv[1], argv[2]);

    return 0;
}
