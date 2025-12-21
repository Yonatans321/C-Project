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
    "W   w      w                 s                                                 W", // 1 
    "W   w      w   wwwwwwwwww    s          wwwwwwwwwww                            W", // 2
    "W   w       ?   w       w    s          w          w                           W", // 3
    "W   w      w            w    s          w     \\    w                           W", // 4
    "W   w      www \\        w    s          w          w                           W", // 5
    "W   w      w   w        w    s          w          w                           W", // 6
    "W  @w      w   wwwwwwwwww    s          wwwww?wwwwww                           W", // 7
    "W   w      w                 s                                                 W", // 8
    "W   w      w                 s                                                 W", // 9
    "W   w      wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwssssssssssssssssssssssW", // 10
    "W   w      w                                                                   W", // 11
    "W   w      w                                                                   W", // 12
    "W   w      w           wwwwwwwwww                                              W", // 13
    "W   w      w            * *     w                          wwwwwwwwwwwwwwwwwww W", // 14
    "W   w      w           w        w                          wwwwwwwww wwwwwwwww W", // 15
    "W   w      w           w !   1  w                          wwwwwwwww wwwwwwwww*W", // 16
    "W   w      w           wwwwwwwwww                          wK       *wwwwwwwww*W", // 17
    "W   w      w      wwww                                     wwwwwwwww*wwwwwwwww W", // 18
    "W   w      wwwwwwww  w                         w           wwwwwww        **   W", // 19
    "W   w                w                                     wwwwwwwww wwwwwwwww w", // 20
    "W   ? \\          w    w                                   wwwwwwwww wwwwwwwww  W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

// ROOM 1

static const char* const ROOM1[ROOM_HEIGHT] =
{
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W      1           w                                                           W", // 1
    "W                  w?wwwwwwwwwwwww wwwwwwwwwwwwwwwwwwwwwww wwwwwwwwwwwwwwww wwwW", // 2
    "W          w       w        w     * w                      www                 W", // 3
    "W        ww wwww   w        w\\ ww  ww   wwwwwwwwwwwwwwwwwwwwwww wwwwwwwwwwwwwwwW", // 4
    "W        ww   \\w* w         wwwwwww@                  www                      W", // 5
    "W       www*wwwww*w         wwwwwwwwwwwwwwwwwwwwwwwww wwwwwwwwwwwwwwwwwwwwwww  W", // 6
    "W     ww     * *  w         w                          ww@                     W", // 7
    "W       www wwwww w         w wwwwwwwwwwwwwwwwwwwwwwwwwwwww  wwwwwwwwwwwwwwwwwwW", // 8
    "W    wwwwwwwwwwww w         w           * *               w?wwwwwwwwwwwwwwwwwwwW", // 9
    "W                www        wwwwwwwwwwwwwwwwwwwwwwwwwww\\ w   wwwwwwwwwwwwwwwwwwW", // 10
    "W*wwwwwwwwwwwwwwwwww        wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww wwwwwwwwwwwwwwwwwwwW", // 11
    "W*wwww                      wwwwwwwwwwwwwwwwwwwwwwwwwwwwwww wwwwwwwwwwwwwwwwwwwW", // 12
    "W    w    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww wwwwwwwwwwwwwwwwwwwW", // 13
    "W    w    w                    *                 *            wwwwwwwwwwwwwwwwwW", // 14
    "W    w    w      wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww   wwwwwwwwwwwwwwwwwwwwwwwwwwW", // 15
    "W    w    wwwwww*K wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww*s       **                W", // 16
    "W    w    w      wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww   wwwwwwwwwwwwwwwwwwwwwww   W", // 17
    "W    w    wwwwwwwwwwwwwwwwwwwwww   **      s      www     s                    W", // 18
    "W    w                              w      s              s                    W", // 19
    "Wwww?wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww     s  !           s                    W", // 20
    "W            *  *                          s      2       s                    W", // 21
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"  // 22
};

// ROOM 2

static const char* const ROOM2[ROOM_HEIGHT] =
{
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
    "W    2     W                 \\ W                \\W                 W          \\W", // 1
    "W          W        WW          W              WWWWW             W      *      W", // 2
    "WWWWWWW?WWWWWWWWWWWW  W?WWWWWWWWWWWWWWWWW?WWWWW   WWWWWWWWWWWW  WWWWWWWW WWWWWWW", // 3
    "W                  W                  W W               W  W                   W", // 4
    "WWWWWWW*WWWWWWWWWW*W       WWW        W WWWWWWWWW*WWWWWWWWW WWW*WWWWWWWWWWWWWWWW", // 5
    "W                  W**WWWWW   WWWWWWWWW                 W                  W   W", // 6
    "W                  W                  W WWWWWWWWW WWWWWWWWW   W   WWW**WWWW   WW", // 7
    "WW****WWWWWWWWWWWWWW  WWWWW  WWWWWWWWWW*WWWWWW  WWWWWWWWWW * WWWWWW**WWWWW  WWWW", // 8
    "WW*WWWWWWWWWWWWWWWWWWWWWWWW**WWWWWWWWWW*WWWWWW**WWWWWWWWWWW*WWWWWWW**WWWWW  WWWW", // 9
    "W             * *         W                                                    W", // 10
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


