#include "empire.h"

#include "core/calc.h"

#include "Data/Constants.h"
#include "Data/Empire.h"
#include "Data/Scenario.h"

enum {
    EMPIRE_WIDTH = 2000,
    EMPIRE_HEIGHT = 1000
};

static struct {
    int scroll_x;
    int scroll_y;
    int selected_object;
    int viewport_width;
    int viewport_height;
} data;

static void check_scroll_boundaries()
{
    int max_x = EMPIRE_WIDTH - data.viewport_width;
    int max_y = EMPIRE_HEIGHT - data.viewport_height;

    data.scroll_x = calc_bound(data.scroll_x, 0, max_x - 1);
    data.scroll_y = calc_bound(data.scroll_y, 0, max_y - 1);
}

void empire_init_scroll(int x, int y)
{
    data.scroll_x = x;
    data.scroll_y = y;
    data.viewport_width = EMPIRE_WIDTH;
    data.viewport_height = EMPIRE_HEIGHT;
}

void empire_set_viewport(int width, int height)
{
    data.viewport_width = width;
    data.viewport_height = height;
    check_scroll_boundaries();
}

void empire_adjust_scroll(int *x_offset, int *y_offset)
{
    *x_offset = *x_offset - data.scroll_x;
    *y_offset = *y_offset - data.scroll_y;
}

void empire_scroll_map(int direction)
{
    if (direction == Dir_8_None) {
        return;
    }
    switch (direction) {
        case Dir_0_Top:
            data.scroll_y -= 20;
            break;
        case Dir_1_TopRight:
            data.scroll_x += 20;
            data.scroll_y -= 20;
            break;
        case Dir_2_Right:
            data.scroll_x += 20;
            break;
        case Dir_3_BottomRight:
            data.scroll_x += 20;
            data.scroll_y += 20;
            break;
        case Dir_4_Bottom:
            data.scroll_y += 20;
            break;
        case Dir_5_BottomLeft:
            data.scroll_x -= 20;
            data.scroll_y += 20;
            break;
        case Dir_6_Left:
            data.scroll_x -= 20;
            break;
        case Dir_7_TopLeft:
            data.scroll_x -= 20;
            data.scroll_y -= 20;
            break;
    };
    check_scroll_boundaries();
}

int empire_selected_object()
{
    return data.selected_object;
}

void empire_clear_selected_object()
{
    data.selected_object = 0;
}

void empire_select_object(int x, int y)
{
    int map_x = x + data.scroll_x;
    int map_y = y + data.scroll_y;
    int min_dist = 10000;
    int obj_id = 0;
    for (int i = 0; i < MAX_EMPIRE_OBJECTS && Data_Empire_Objects[i].inUse; i++) {
        struct Data_Empire_Object *obj = &Data_Empire_Objects[i];
        int obj_x, obj_y;
        if (Data_Scenario.empireHasExpanded) {
            obj_x = obj->xExpanded;
            obj_y = obj->yExpanded;
        } else {
            obj_x = obj->x;
            obj_y = obj->y;
        }
        if (obj_x - 8 > map_x || obj_x + obj->width + 8 <= map_x) {
            continue;
        }
        if (obj_y - 8 > map_y || obj_y + obj->height + 8 <= map_y) {
            continue;
        }
        int dist = calc_maximum_distance(map_x, map_y, obj_x + obj->width / 2, obj_y + obj->height / 2);
        if (dist < min_dist) {
            min_dist = dist;
            obj_id = i + 1;
        }
    }
    data.selected_object = obj_id;
}

void empire_save_state(buffer *buf)
{
    buffer_write_i32(buf, data.scroll_x);
    buffer_write_i32(buf, data.scroll_y);
    buffer_write_i32(buf, data.selected_object);
}

void empire_load_state(buffer *buf)
{
    data.scroll_x = buffer_read_i32(buf);
    data.scroll_y = buffer_read_i32(buf);
    data.selected_object = buffer_read_i32(buf);
}