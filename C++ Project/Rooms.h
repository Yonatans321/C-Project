#pragma once
// number of rooms
static constexpr int NUM_ROOMS = 4;

// the width and height of each room
static constexpr int ROOM_WIDTH = 80;
static constexpr int ROOM_HEIGHT = 23;


// ROOM 0

static const char* const ROOM0[ROOM_HEIGHT] =
{//  01234567890123456789012345678901234567890123456789012345678901234567890123456789
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W    W     W                 s                                                 W", // 1 
    "W    W     W   WWWWWWWWWW    s          WWWWWWWWWWW                            W", // 2
    "W    W       ?  W       W    s          W          W                           W", // 3
    "W    W     W            W    s          W     \\    W                           W", // 4
    "W    W     WWW \\        W    s          W          W                           W", // 5
    "W    W     W   W        W    s          W          W                           W", // 6
    "W    W     W   WWWWWWWWWW    s          WWWWW?WWWWWW                           W", // 7
    "W    W     W                 s                                                 W", // 8
    "W    W     W                 s                                                 W", // 9
    "W    W     WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWsssssssssssssssssssss W", // 10
    "W    W     W                                                                   W", // 11
    "W    W     W                                                                   W", // 12
    "W    W     W           WWWWWWWWWW                                              W", // 13
    "W    W     W          * * *     W                                              W", // 14
    "W    W     W           W        W                                              W", // 15
    "W    W     W           W     1  W                                              W", // 16
    "W    W     W           WWWWWWWWWW                                              W", // 17
    "W    W     W      WWWW                                                         W", // 18
    "W    W     WWWWWWWW  W                         W                     W**  WWW  W", // 19
    "W    W               W                                              W    **   *W", // 20
    "W   ? \\          W    W                                             W ** KWWW  W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};


/// ROOM 1

static const char* const ROOM1[ROOM_HEIGHT] =
{
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W      1           W                                                           W", // 1
    "W                  W?WWWWWWWWWWWWW WWWWWWWWWWWWWWWWWWWWWWW WWWWWWWWWWWWWWWW WWWW", // 2
    "W          W       W        W  *  * W                      WWW                 W", // 3
    "W        WW WWWW   W        W\\ WW  WW    WWWWWWWWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWW", // 4
    "W        WW   \\W  W         WWWWWWW                   WWW                      W", // 5
    "W        WW*WWWWW*W         WWWWWWWWWWWWWWWWWWWWWWWWW WWWWWWWWWWWWWWWWWWWWWWW  W", // 6
    "W        W     *  W         W                          WW                      W", // 7
    "W        WW WWWWW W         W WWWWWWWWWWWWWWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWWWWW", // 8
    "W    WWWWWWWWWW   W         W             *               W?WWWWWWWWWWWWWWWWWWWW", // 9
    "W                  W        WWWWWWWWWWWWWWWWWWWWWWWWWWW\\ W  WWWWWWWWWWWWWWWWWWW", // 10
    "W**WWWWWWWWWWWWWWWWW        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW WWWWWWWWWWWWWWWWWWWW", // 11
    "W    W                      WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW WWWWWWWWWWWWWWWWWWWW", // 12
    "W    W    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW WWWWWWWWWWWWWWWWWWWW", // 13
    "W    W    W                    *    *            *            WWWWWWWWWWWWWWWWWW", // 14
    "W    W    W      WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW   WWWWWWWWWWWWWWWWWWWWWWWWWWW", // 15
    "W    W    W ** **K WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW***s       *                 W", // 16
    "W    W    W      WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW   WWWWWWWWWWWWWWWWWWWWWWW   W", // 17
    "W    W    WWWWWWWWWWWWWWWWWWWWWW    *      s      WWW     s                    W", // 18
    "W    W                              W      s              s                    W", // 19
    "WWWW?WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW     s              s                    W", // 20
    "W            *  *  *                       s      2       s                    W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

// ROOM 2

static const char* const ROOM2[ROOM_HEIGHT] =
{
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W    2     W                 \\ W                \\W                 W          \\W", // 1
    "W          W        WW          W              WWWWW             W      *      W", // 2
    "WWWWWWW?WWWWWWWWWWWW  W?WWWWWWWWWWWWWWWWW?WWWWW   WWWWWWWWWWWW  WWWWWWWW WWWWWWW", // 3
    "W                  W                  WWW               W                      W", // 4
    "WWWWWWW****WWWWWWW*W       WWW        W  WWWWWW***WWWWWWWWWWWW**WWWWWWWWWWWWWWWW", // 5
    "W                  W**WWWWW   WWWWWWWWW                 W                  W   W", // 6
    "W                  W                  W  WWWWWWWW WWWWWWWWW   W   WWW**WWWW   WW", // 7
    "WW****WWWWWWWWWWWWWW  WWWWW  WWWWWWWWWW  WWWWW  WWWWWWWWWW   WWWWWW**WWWWW  WWWW", // 8
    "WW*WWWWWWWWWWWWWWWWWWWWWWWW**WWWWWWWWWW**WWWWW**WWWWWWWWW***WWWWWWW**WWWWW  WWWW", // 9
    "W             ***                                                              W", // 10
    "W            WWWWWWWW                                                          W", // 11
    "W            WK   W  W                                                         W", // 12
    "W           WW       W                             WWWWWW     WWWWWW           W", // 13
    "W            WWW     W                                                         W", // 14
    "W            WW* WWWWW                                  W     W                W", // 15
    "W            WW**WWWWW                                  W     W                W", // 16
    "W                  WWW                             W               W           W", // 17
    "W                  WWW                             WW             WW           W", // 18
    "W        WWWWWWWWWWWWWWWWWWWWWW                     WWWWWWWWWWWWWWW            W", // 19
    "W        W       WWWWW        W                                                W", // 20
    "W        s       W 3 ?       sss                                               W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};


