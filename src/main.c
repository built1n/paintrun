#include "globals.h"

void draw(struct game_ctx_t *ctx)
{
    plat_set_background(LCD_RGBPACK(0, 0, 0));
    plat_clear();
    for(int i=0;i<ARRAYLEN(ctx->screen); ++i)
    {
        plat_set_foreground(ctx->screen[i].color);
        plat_vline(i, LCD_HEIGHT, LCD_HEIGHT - ctx->screen[i].height);
    }
    plat_update();
}

void scroll(struct game_ctx_t *ctx)
{
    ctx->screen[ctx->draw_position].height = ctx->current_height;
    ctx->screen[ctx->draw_position].color = LCD_RGBPACK(240,240,240);

    if((ctx->draw_position++) == ARRAYLEN(ctx->screen))
    {
        ctx->draw_position = ARRAYLEN(ctx->screen) - 1;
        /* scroll */
        memcpy(ctx->screen, ctx->screen + 1, sizeof(ctx->screen) - sizeof(ctx->screen[0]));
    }
    if(!ctx->left_of_current--)
    {
        /* generate next block */
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
}

void dash_main(void)
{
    struct game_ctx_t realctx;
    struct game_ctx_t *ctx = &realctx;
    memset(ctx, 0, sizeof(*ctx));

    plat_logf("init screen");

    for(ctx->draw_position = 0; ctx->draw_position < ARRAYLEN(ctx->screen)/3; ++ctx->draw_position)
    {
        ctx->screen[ctx->draw_position].height = LCD_HEIGHT/3;
        ctx->screen[ctx->draw_position].color = LCD_RGBPACK(240, 240, 240);
    }

    ctx->current_type = VOID;
    ctx->current_height = 0;
    ctx->left_of_current = GAP_MED;

    while(1)
    {
#ifdef PLAT_WANTS_YIELD
        plat_yield();
#endif
        scroll(ctx);
        draw(ctx);
        printf("%d\n", plat_pollaction());
        //plat_sleep(10);
    }
}
