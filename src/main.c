/****************************************************************************
 *     _____        _____ _   _ _______   _____  _    _ _   _
 *    |  __ \ /\   |_   _| \ | |__   __| |  __ \| |  | | \ | |
 *    | |__) /  \    | | |  \| |  | |    | |__) | |  | |  \| |
 *    |  ___/ /\ \   | | | . ` |  | |    |  _  /| |  | | . ` |
 *    | |  / ____ \ _| |_| |\  |  | |    | | \ \| |__| | |\  |
 *    |_| /_/    \_\_____|_| \_|  |_|    |_|  \_\\____/|_| \_|
 *
 * Copyright (C) 2015 Franklin Wei
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "globals.h"

void draw(struct game_ctx_t *ctx)
{
    plat_set_background(BACKGROUND_COLOR);
    plat_clear();
    for(int i = 0; i < ARRAYLEN(ctx->screen); ++i)
    {
        plat_set_foreground(ctx->screen[i].color);
        plat_vline(i, LCD_HEIGHT, LCD_HEIGHT - ctx->screen[i].height);
    }

    plat_set_foreground(ctx->player.color);

    plat_fillrect(ctx->player.position.x >> FRACBITS, ctx->player.position.y >> FRACBITS,
                  ctx->player.bounds.x >> FRACBITS, ctx->player.bounds.y >> FRACBITS);

    plat_update();
}

static void generate_new(struct game_ctx_t *ctx)
{
    ctx->current_type = (ctx->current_type == VOID) ? LAND : VOID;
    if(ctx->current_type == LAND)
    {
        ctx->current_height = RAND_RANGE(HEIGHT_INCREMENT, LCD_HEIGHT - HEIGHT_INCREMENT);
    }
    else
    {
        ctx->current_height = 0;
    }
    if(ctx->current_type == VOID)
        ctx->left_of_current = GAP_MED;
    else
    {
        switch(RAND_RANGE(0,2))
        {
        case 0:
            ctx->left_of_current = GAP_SMALL;
            break;
        case 1:
            ctx->left_of_current = GAP_MED;
            break;
        case 2:
            ctx->left_of_current = GAP_LARGE;
            break;
        default:
            /* panic */
            assert(0);
        }
    }
}

void scroll(struct game_ctx_t *ctx)
{
    ctx->screen[ctx->draw_position].height = ctx->current_height;
    ctx->screen[ctx->draw_position].color = LAND_COLOR;

    if((ctx->draw_position++) == ARRAYLEN(ctx->screen))
    {
        ctx->draw_position = ARRAYLEN(ctx->screen) - 1;
        /* scroll */
        memcpy(ctx->screen, ctx->screen + 1, sizeof(ctx->screen) - sizeof(ctx->screen[0]));
    }
    if(!ctx->left_of_current--)
    {
        /* generate next block */
        generate_new(ctx);
    }
}

void update_player(struct game_ctx_t *ctx)
{
    struct coords_t old = ctx->player.position;

    bool above_block = false;

    if((old.y + ctx->player.bounds.y)>> FRACBITS < LCD_HEIGHT - ctx->screen[(old.x + ctx->player.bounds.x) >> FRACBITS].height &&
       ctx->screen[(old.x + ctx->player.bounds.x) >> FRACBITS].height != 0)
        above_block = true;

    ctx->player.position.x += ctx->player.vel.x;
    ctx->player.position.y += ctx->player.vel.y;

    if(ctx->player.position.y <= 0)
    {
        ctx->player.vel.y = FIXED(0);
        ctx->player.position.y = FIXED(0);
    }

    if(((ctx->player.position.y + ctx->player.bounds.y) >> FRACBITS > LCD_HEIGHT - ctx->screen[(ctx->player.position.x + ctx->player.bounds.x) >> FRACBITS].height && !above_block)||
       (ctx->player.position.y + ctx->player.bounds.y) >> FRACBITS >= LCD_HEIGHT)
    {
        ctx->status = OVER;
        return;
    }

    /* check for collision with horizontal surfaces or apply gravity */
    if((ctx->player.position.y + ctx->player.bounds.y) >> FRACBITS >= LCD_HEIGHT - ctx->screen[(ctx->player.position.x + ctx->player.bounds.x) >> FRACBITS].height)
    {
        ctx->player.vel.y = FIXED(0);
        ctx->player.position.y = FIXED(LCD_HEIGHT - ctx->screen[(ctx->player.position.x + ctx->player.bounds.x)>> FRACBITS].height) - ctx->player.bounds.y;
        ctx->screen[(ctx->player.position.x + FP_DIV(ctx->player.bounds.x, FIXED(2))) >> FRACBITS].color = PAINT_COLOR;
    }
    else if((ctx->player.position.y + ctx->player.bounds.y) >> FRACBITS >= LCD_HEIGHT - ctx->screen[(ctx->player.position.x) >> FRACBITS].height)
    {
        ctx->player.vel.y = FIXED(0);
        ctx->player.position.y = FIXED(LCD_HEIGHT - ctx->screen[(ctx->player.position.x)>> FRACBITS].height) - ctx->player.bounds.y;
        ctx->screen[(ctx->player.position.x + FP_DIV(ctx->player.bounds.x, FIXED(2))) >> FRACBITS].color = PAINT_COLOR;
    }
    else
    {
        ctx->player.vel.y = MIN(ctx->player.vel.y + FP_DIV(FIXED(1),FIXED(100)), MAX_SPEED);
    }

}

static void init_world(struct game_ctx_t *ctx)
{
    memset(ctx, 0, sizeof(*ctx));

    /* first generate the initial block */
    for(ctx->draw_position = 0; ctx->draw_position < ARRAYLEN(ctx->screen)/3; ++ctx->draw_position)
    {
        ctx->screen[ctx->draw_position].height = LCD_HEIGHT/3;
        ctx->screen[ctx->draw_position].color = LAND_COLOR;
    }

    ctx->current_type = VOID;
    ctx->current_height = 0;
    ctx->left_of_current = GAP_MED;

    /* initialize the rest of the world */
    while(ctx->draw_position != ARRAYLEN(ctx->screen) - 1)
        scroll(ctx);
}

void about_screen(void)
{
    /* TODO */
    return;
}

void do_game(void)
{
    struct game_ctx_t realctx;
    struct game_ctx_t *ctx = &realctx;

    init_world(ctx);

    ctx->player.position.x = PLAYER_INITX;
    ctx->player.position.y = PLAYER_INITY;

    ctx->player.bounds.x = FIXED(PLAYER_SIZE);
    ctx->player.bounds.y = FIXED(PLAYER_SIZE);

    ctx->player.color = PLAYER_COLOR;

    long last_timestamp;

    while(ctx->status != OVER)
    {
        last_timestamp = plat_time();
#ifdef PLAT_WANTS_YIELD
        plat_yield();
#endif
        scroll(ctx);
        update_player(ctx);
        draw(ctx);
        enum keyaction_t key = plat_pollaction();
        if(key == ACTION_PAUSE)
        {
            plat_paused(ctx);
        }
        else if (key == ACTION_JUMP)
        {
            if(ctx->player.vel.y > -MAX_SPEED)
                ctx->player.vel.y -= FP_DIV(FIXED(1),FIXED(2));
        }
        long delta = plat_time() - last_timestamp;
        //printf("FPS: %d\n", (int)((double)1000 / ((delta == 0)?1:delta)));
    }

    plat_gameover(ctx);
}

void dash_main(void)
{
    /* main menu */
    while(1)
    {
        switch(plat_domenu())
        {
        case MENU_DOGAME:
            do_game();
            break;
        case MENU_ABOUT:
            about_screen();
            break;
        case MENU_QUIT:
            return;
        }
    }
}
