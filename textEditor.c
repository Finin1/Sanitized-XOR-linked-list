#define DEBUG_MODE 0
#define MAX_BUFFER_SIZE 10
#define WRITE 0
#define ERASE 1
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

#include "includes/XORLiLs.h"
#include <curses.h>
#include <stdio.h>

typedef unsigned op_type;
struct Buffer {
    struct XORNode *prev_node;
    struct XORNode *curr_node;
    struct EndPoints end_points;
};
struct BufferValue {
    char symbol;
    int position;
    op_type operation;
};

int buffer_size = 0;

void add_to_buffer(struct Buffer *buffer, char symbol, int position,
                   op_type operation) {
    int value_to_insert = operation + symbol * 10 + (position + 1) * 10000;

    if (buffer->curr_node != buffer->end_points.head) {
        clear_list(&(buffer->end_points.head));
        buffer->end_points.tail = NULL;
    }

    insert_first(&buffer->end_points, value_to_insert);
    buffer->curr_node = buffer->end_points.head;
    buffer->prev_node = NULL;
#if DEBUG_MODE
    print_list(buffer->head);
#endif
    buffer_size++;
}

struct BufferValue get_next_from_buffer(struct Buffer *buffer) {
    struct BufferValue removed_buffer_value;

    int removed_value = buffer->curr_node->data;
    struct XORNode *tmp;

    tmp = buffer->curr_node;
    go(buffer->prev_node, &buffer->curr_node);
    buffer->prev_node = tmp;

    removed_buffer_value.operation = removed_value % 10;
    removed_buffer_value.symbol = (removed_value / 10) % 1000;
    removed_buffer_value.position = (removed_value / 10000) - 1;

    buffer_size--;
    return removed_buffer_value;
}
struct BufferValue get_prev_from_buffer(struct Buffer *buffer) {
    struct BufferValue removed_buffer_value;

    struct XORNode *tmp;

    tmp = buffer->prev_node;
    go(buffer->curr_node, &buffer->prev_node);
    buffer->curr_node = tmp;

    int removed_value = buffer->curr_node->data;

    removed_buffer_value.operation = removed_value % 10;
    removed_buffer_value.symbol = (removed_value / 10) % 1000;
    removed_buffer_value.position = (removed_value / 10000) - 1;

    buffer_size--;
    return removed_buffer_value;
}

int main() {
    WINDOW *terminal_window = initscr();

    struct Buffer buffer;

    buffer.prev_node = NULL;
    buffer.curr_node = NULL;
    buffer.end_points.head = NULL;
    buffer.end_points.tail = NULL;

    move(5, 0);
    cbreak();
    noecho();
    keypad(terminal_window, 1);

    int character, curr_x_pos = 0, lastchar = 0;
    while ((character = getch()) != '\E') {
        int curr_x, curr_y;
        getyx(terminal_window, curr_y, curr_x);

        struct BufferValue buffer_data;
        switch (character) {
        case KEY_LEFT:
            curr_x_pos = max(0, curr_x_pos - 1);
            move(curr_y, curr_x_pos);
            refresh();
            break;
        case KEY_RIGHT:
            curr_x_pos = min(lastchar, curr_x_pos + 1);
            move(curr_y, curr_x_pos);
            refresh();
            break;
        case KEY_BACKSPACE:
            move(curr_y, max(0, curr_x - 1));
            character = inch();
            delch();
            lastchar = max(0, lastchar - 1);
            curr_x_pos = max(0, curr_x_pos - 1);
            add_to_buffer(&buffer, character, curr_x_pos, ERASE);
            refresh();
            break;
        case 10:
            break;
        case '`':
            // printf("Undo\n");
            if (!(buffer.curr_node)) {
                break;
            }
            buffer_data = get_next_from_buffer(&buffer);
            if (buffer_data.operation == WRITE) {
                move(curr_y, buffer_data.position);
                delch();
                curr_x_pos = buffer_data.position;
                lastchar--;
            } else if (buffer_data.operation == ERASE) {
                move(curr_y, buffer_data.position++);
                insch(buffer_data.symbol);
                move(curr_y, buffer_data.position);
                lastchar++;
                curr_x_pos = buffer_data.position;
            }
            break;
        case '~':
            if (!(buffer.prev_node)) {
                break;
            }
            buffer_data = get_prev_from_buffer(&buffer);
            if (buffer_data.operation == WRITE) {
                move(curr_y, buffer_data.position++);
                insch(buffer_data.symbol);
                curr_x_pos = buffer_data.position;
                move(curr_y, curr_x_pos);
                lastchar++;
            } else if (buffer_data.operation == ERASE) {
                move(curr_y, buffer_data.position);
                delch();
                move(curr_y, buffer_data.position);
                lastchar--;
                curr_x_pos = buffer_data.position;
            }
        default:
            if ((character >= 'a' && character <= 'z') ||
                (character >= 'A' && character <= 'Z') ||
                (character >= '0' && character <= '9') || character == ' ') {
                if (curr_x_pos < lastchar) {
                    insch(character);
                    move(curr_y, curr_x + 1);
                } else {
                    addch(character);
                }
                lastchar++;
                add_to_buffer(&buffer, character, curr_x_pos, WRITE);
                curr_x_pos++;
            }
        };
    }

    endwin();
    return 0;
}
