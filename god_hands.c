/*
 * Copyright (C) 2012-2014 Olivier Gayot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef uint8_t byte;
typedef int8_t rang_t;

typedef struct {
    byte value;
    rang_t rang;
} elem_t;

typedef struct {
    int len;
    elem_t *elem;
} map_t;

map_t *map_g;

/* resolve {{{ */

/* returns true if the map is empty, otherwise returns false */
static bool map_is_empty(void)
{
    for (int i = 0; i < map_g->len; i++) {
        if (map_g->elem[i].rang == -1) {
            return false;
        }
    }

    return true;
}

/* return the index of the next element */
static inline int get_next_case(int current)
{
    return (current + map_g->elem[current].value) % map_g->len;
}

/* return the index of the previous element */
static inline int get_prev_case(int current)
{
    int prev = current - map_g->elem[current].value;

    if (current < map_g->elem[current].value)
        prev += map_g->len;

    return prev;
}

static int perform_resolve_god_hands(int activated, int offset)
{
    int next, prev;

    /* this one is already activated, not possible captain */
    if (map_g->elem[activated].rang != -1) {
        return -1;
    }

    /* activate the current case */
    map_g->elem[activated].rang = offset;

    if (map_is_empty()) {
        /* we won */
        return 0;
    }

    next = get_next_case(activated);
    prev = get_prev_case(activated);

    if (perform_resolve_god_hands(next, offset + 1) == 0)
        return 0;

    if (perform_resolve_god_hands(prev, offset + 1) == 0)
        return 0;

    /* reset to default */
    map_g->elem[activated].rang = -1;
    return -1;
}

int solve_god_hands(map_t *map)
{
    for (int i = 0; i < map->len; i++) {
        map->elem[i].rang = -1;
    }

    map_g = map;

    for (int i = 0; i < map->len; i++) {
        if (perform_resolve_god_hands(i, 0) == 0) {
            return 0;
        }
    }

    return -1;
}

/* }}} */

static void display_solution(const map_t *map)
{
    for (int i = 0; i < map->len; i++) {
        /* run through the order */
        for (int j = 0; j < map->len; j++) {
            /* run through the elems */

            assert (map->elem[j].rang != -1);

            if (map->elem[j].rang == i) {
                printf("[%d] ", j);
                break;
            }
        }
    }
    putchar('\n');
}

int main(void)
{
    map_t map;
    char buffer[16];

    /* get the length of the map */
    for (;;) {
        if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO)) {
            fputs("size of the map: ", stdout);
        }

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0;
        }

        /* TODO replace with strtol to handle errors of type 42(.*) */
        map.len = atoi(buffer);

        if (map.len <= 0) {
            fprintf(stderr, "the map has an invalid size\n");

            if (!isatty(STDIN_FILENO))
                return -1;

            continue;
        }

        break;
    }

    map.elem = malloc(sizeof(elem_t) * map.len);

    for (int i = 0; i < map.len; i++) {
        if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
            printf("(%02d/%02d): ", i + 1, map.len);

        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0;
        }

        /* TODO check for errors (i.e. not numbers and negative values */
        map.elem[i].value = atoi(buffer);
    }

    if ((solve_god_hands(&map)) != 0) {
        fputs("cannot solve the paradox, kupo :-/\n", stderr);
    } else {
        fputs("we solved the paradox kupo! :-)\n", stdout);
        display_solution(&map);
    }

    return 0;
}
