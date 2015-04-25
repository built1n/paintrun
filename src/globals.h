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

struct game_ctx_t {
    struct section_t screen[LCD_WIDTH];
    struct coords_t player;
    struct coorts_t player_vel;
    fixed_t score;

    int draw_position;
    enum { LAND, VOID } current_type;
    int current_height;
    int left_of_current;
};

#define WIDTH_INCREMENT (LCD_WIDTH / 6)
#define HEIGHT_INCREMENT (LCD_HEIGHT / 6)

#define GAP_SMALL (WIDTH_INCREMENT * 2)
#define GAP_MED (WIDTH_INCREMENT * 3)
#define GAP_LARGE (WIDTH_INCREMENT * 4)
