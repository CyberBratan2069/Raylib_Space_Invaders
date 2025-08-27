/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#pragma once
#include <raylib.h>

typedef struct Block {
    Vector2 position;
} Block;


Block* new_block(Vector2 position);
void drawBlock();

extern Block* block;
