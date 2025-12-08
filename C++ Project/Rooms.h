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
    "W                                                             s                W", // 1 
    "W          WWWWWWWWWW             WWWWWWWWWWWW                s                W", // 2
    "W          W        W             W          W                s                W", // 3
    "W                   W             W          W                s                W", // 4
    "W              ?    W             W          W                ssssssssssssssss W", // 5
    "W          W        W             W          W                                 W", // 6
    "W          WWWWWWWWWW             WWWW  WWWWWW                                 W", // 7
    "W                 *                                                            W", // 8
    "W                 *                                                            W", // 9
    "W        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW                          W", // 10
    "W            2   **                                                            W", // 11
    "W                *     *                                                       W", // 12
    "W                  WWWWWWWWWW                                                  W", // 13
    "W                  W        W                                                  W", // 14
    "W                   K  1    W                                                  W", // 15
    "W                  W        W                                                  W", // 16
    "W                  WWWWWWWWWW                                                  W", // 17
    "W                                                                              W", // 18
    "W                                                                              W", // 19
    "W                                                                              W", // 20
    "W     \\                                                                       W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

// ============================================================================
// ROOM 1
// ============================================================================
static const char* const ROOM1[ROOM_HEIGHT] =
{
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W 1                                                                            W", // 1
    "W                                                                              W", // 2
    "W          WWWWWWWWWWWWWWWWWWWW                                                W", // 3
    "W          W                  W                                                W", // 4
    "W          W        #         W                                                W", // 5
    "W          W                  W                                                W", // 6
    "W          WWW**WWW  WWWWWWWWWW                                                W", // 7
    "W                  WW                                                          W", // 8
    "W                  WW                                                          W", // 9
    "W                  WW                                                          W", // 10
    "W                  WW                                                          W", // 11
    "W                  WW         WWWWWWWWWWWWWW                                   W", // 12
    "W                  WW         W            W                                   W", // 13
    "W                  WW         W  K  * W                                        W", // 14
    "W                             W            W                                   W", // 15
    "W                             WWWWWW  WWWWWW                                   W", // 16
    "W                                                                              W", // 17
    "W                                                                              W", // 18
    "W                                       WWWWWWWW                               W", // 19
    "W                                       W      W                               W", // 20
    "W                                          2   W                               W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

// ============================================================================
// ROOM 2
// ============================================================================
static const char* const ROOM2[ROOM_HEIGHT] =
{
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W          W                  W                  W                             W", // 1
    "W          W                  W                  W                             W", // 2
    "WWWWWWW  WWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWWWWWWWWWWWWW  WWWWW", // 3
    "W                  W                  W                  W                     W", // 4
    "W                  W        ?         W        !         W                     W", // 5
    "W                  W                  W                  W                     W", // 6
    "WWWWWWW  WWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWWWWWWWWWWWWW  WWWWW", // 7
    "W                                                                              W", // 8
    "W            WWWWWWWW                                                          W", // 9
    "W            W      W                                                          W", // 10
    "W            W      W                                                          W", // 11
    "W            W      W                                                          W", // 12
    "W            W      W                                                          W", // 13
    "W            WW    WW                                                          W", // 14
    "W                  WW                                                          W", // 15
    "W                  WW                                                          W", // 16
    "W                  WW                                                          W", // 17
    "W                  WW                                                          W", // 18
    "W        WWWWWWWWWWWWWWWWWWWWWW                                                W", // 19
    "W        W                    W                                                W", // 20
    "W        K         3                                                           W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

