/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include <stdlib.h>
#include "block.h"

Block* block = NULL;

Block* new_block(Vector2 position) {
    block = malloc(sizeof *block);
    if(!block) return NULL;
    block->position = position;
    return block;
}

void drawBlock() {
    if(!block) return;
    DrawRectangle(block->position.x, block->position.y, 3, 3, (Color){243, 216, 63, 255});
}

Rectangle hitboxBlock() {
    if(!block) return (Rectangle){0};

    Rectangle rectangle;
    rectangle.x      = block->position.x;
    rectangle.y      = block->position.y;
    rectangle.width  = 3;
    rectangle.height = 3;
    return rectangle;
}