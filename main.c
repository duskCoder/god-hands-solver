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

static bool map_is_empty(void)
{
    for (int i = 0; i < map_g->len; i++) {
	if (map_g->elem[i].rang == -1) {
	    return false;
	}
    }

    return true;
}

static inline int get_next(int current)
{
    return (current + map_g->elem[current].value) % map_g->len;
}

static inline int get_prev(int current)
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

    next = get_next(activated);
    prev = get_prev(activated);

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

    if (isatty(0))
	fputs("size of the map: ", stdout);
    map.len = atoi(fgets(buffer, sizeof(buffer), stdin));

    map.elem = malloc(sizeof(elem_t) * map.len);

    for (int i = 0; i < map.len; i++) {
	if (isatty(0))
	    printf("(%02d/%02d): ", i + 1, map.len);
	fflush(stdout);
	map.elem[i].value = atoi(fgets(buffer, sizeof(buffer), stdin));
    }

    if ((solve_god_hands(&map)) != 0) {
	fputs("cannot resolve paradox kupo\n", stderr);
    } else {
	fputs("we resolved the paradox kupo\n", stdout);
	display_solution(&map);
    }

    return 0;
}
