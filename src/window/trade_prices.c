#include "trade_prices.h"

#include "empire/trade_prices.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/lang_text.h"
#include "graphics/panel.h"
#include "graphics/screen.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "window/advisors.h"

static void draw_background()
{
    graphics_in_dialog();

    graphics_shade_rect(33, 53, 574, 334, 0);
    outer_panel_draw(16, 144, 38, 11);
    lang_text_draw(54, 21, 26, 153, FONT_LARGE_BLACK);
    lang_text_draw(54, 22, 26, 228, FONT_NORMAL_BLACK);
    lang_text_draw(54, 23, 26, 253, FONT_NORMAL_BLACK);
    for (int i = 1; i < 16; i++) {
        int graphicOffset = i + resource_image_offset(i, RESOURCE_IMAGE_ICON);
        image_draw(image_group(GROUP_RESOURCE_ICONS) + graphicOffset,
            126 + 30 * i, 194);
        text_draw_number_centered(trade_price_buy(i),
            120 + 30 * i, 229, 30, FONT_SMALL_PLAIN);
        text_draw_number_centered(trade_price_sell(i),
            120 + 30 * i, 254, 30, FONT_SMALL_PLAIN);
    }
    lang_text_draw_centered(13, 1, 16, 296, 608, FONT_NORMAL_BLACK);

    graphics_reset_dialog();
}

static void handle_mouse(const mouse *m)
{
    if (m->right.went_up) {
        window_advisors_show();
    }
}

static int getTradePriceTooltipResource(struct TooltipContext *c)
{
    int xBase = screen_dialog_offset_x() + 124;
    int y = screen_dialog_offset_y() + 192;
    int xMouse = c->mouse_x;
    int yMouse = c->mouse_y;
    
    for (int i = 1; i < 16; i++) {
        int x = xBase + 30 * i;
        if (x <= xMouse && x + 24 > xMouse && y <= yMouse && y + 24 > yMouse) {
            return i;
        }
    }
    return 0;
}

static void get_tooltip(struct TooltipContext *c)
{
    int resource = getTradePriceTooltipResource(c);
    if (!resource) {
        return;
    }
    c->type = TooltipType_Button;
    c->textId = 131 + resource;
}

void window_trade_prices_show()
{
    window_type window = {
        Window_TradePricesDialog,
        draw_background,
        0,
        handle_mouse,
        get_tooltip
    };
    window_show(&window);
}