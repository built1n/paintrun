#include "compat.h"

struct section_t {
    int height; /* in pixels, 0 = void */
    color_t color; /* color */
};

struct coords_t {
    fixed_t x, y;
};

struct obstacle_t {
    struct coords_t position;
    enum { OBSTACLE_NONE } type; /* not implemented */
};

struct player_t {
    struct coords_t position;
    struct coords_t bounds;
    struct coords_t vel;
    color_t color;
};

struct game_ctx_t {
    struct section_t screen[LCD_WIDTH];
    fixed_t score;

    struct player_t player;

    int draw_position;
    enum { LAND, VOID } current_type;
    int current_height;
    int left_of_current;

    enum { RUNNING, PAUSED, OVER } status;
};

#define WIDTH_INCREMENT (LCD_WIDTH / 6)
#define HEIGHT_INCREMENT (LCD_HEIGHT / 6)

#define GAP_SMALL (WIDTH_INCREMENT * 2)
#define GAP_MED (WIDTH_INCREMENT * 3)
#define GAP_LARGE (WIDTH_INCREMENT * 4)

#define PLAYER_SIZE 40
#define MAX_SPEED FIXED(2)

#define PLAYER_INITX FIXED(LCD_WIDTH/4)
#define PLAYER_INITY FIXED(0)
