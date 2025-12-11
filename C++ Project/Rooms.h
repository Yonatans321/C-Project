#pragma once
// number of rooms
static constexpr int NUM_ROOMS = 4;

// the width and height of each room
static constexpr int ROOM_WIDTH = 80;
static constexpr int ROOM_HEIGHT = 23;

// ============================================================================
// ROOM 0
// ============================================================================
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
    "W    W     W           W        W                                              W", // 14
    "W    W     W       * * *        W                                              W", // 15
    "W    W     W           W    1   W                                              W", // 16
    "W    W     W           WWWWWWWWWW                                              W", // 17
    "W    W     W      WWWW                                                         W", // 18
    "W    W     WWWWWWWW  W                         W                     ***  WWW  W", // 19
    "W    W               W                                              W   **    * W", // 20
    "W   ? \\          W    W                                             W ** KWWW  W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

// ============================================================================
// ROOM 1
// ============================================================================
static const char* const ROOM1[ROOM_HEIGHT] =
{
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W 1               ?                                                            W", // 1
    "W                                                                              W", // 2
    "W          WWWWWWWWWWWWWWWWWWWW                                                W", // 3
    "W        * *    WW            W                                                W", // 4
    "W          W     W            KW                                                W", // 5
    "W          W                  W                                                W", // 6
    "W          WWW**WWW  WWWWWWWWWW                                                W", // 7
    "W                  WW                                                          W", // 8
    "W                  WW                                                          W", // 9
    "W                  WW                                                          W", // 10
    "W                  WW                                                          W", // 11
    "W                  WW         WWWWWWWWWWWWWW                                   W", // 12
    "W                  WW         W            W                                   W", // 13
    "W                  WW         W  K  * W    W                                   W", // 14
    "W                             W            W                                   W", // 15
    "W                             WWWWWW**WWWWWW                                   W", // 16
    "W                                                                              W", // 17
    "W                                                                              W", // 18
    "W                                       WWWWWWWW                               W", // 19
    "W                                       W      W                               W", // 20
    "W                                       ?  2   W                               W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

// ============================================================================
// ROOM 2
// ============================================================================
static const char* const ROOM2[ROOM_HEIGHT] =
{
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W    2     W                 \\W                 \\W                           ?\\W", // 1
    "W          W        WW          W              WWWWW            WW             W", // 2
    "WWWWWWW?WWWWWWWWWWWW  W?WWWWWWWWWWWWWWWWW?WWWWW   WWWWWWWWWWWW  WWWWWWWW*WWWWWWW", // 3
    "W                  W                  WW                W                      W", // 4
    "WWWWWWW****WWWWWWW*W       WWW        W WWWWWWW***WWWWWWWWWWWW**WWWWWWWWWWWWWWWW", // 5
    "W                  W**WWWWW   WWWWWWWWW                 W                  W   W", // 6
    "W                  W                  W  WWWWWWWW WWWWWWWWW   W   WWW**WWWW  WW", // 7
    "WW****WWWWWWWWWWWWWW  WWWWW  WWWWWWWWWW  WWWWW  WWWWWWWWWW   WWWWWW**WWWWW  WWWW", // 8
    "WW*WWWWWWWWWWWWWWWWWWWWWWWW**WWWWWWWWWW**WWWWW**WWWWWWWWW***WWWWWWW**WWWWW  WWWW", // 9
    "W             ***                                                              W", // 10
    "W            WWWWWWWW                                                          W", // 11
    "W            WK W W  W                                                         W", // 12
    "W           WW   *   W                                                         W", // 13
    "W            WWW     W                                                         W", // 14
    "W            WW* WWWWW                                  W     W                W", // 15
    "W            WW**WWWWW                                  W     W                W", // 16
    "W                  WWW                              W               W          W", // 17
    "W                  WWW                              WW             WW          W", // 18
    "W        WWWWWWWWWWWWWWWWWWWWWW             W        WWWWWWWWWWWWWWW           W", // 19
    "W        W                    W                                                W", // 20
    "W        s         3         sss                                               W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

