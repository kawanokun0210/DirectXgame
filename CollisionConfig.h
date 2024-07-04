#pragma once
#include<cstdint>

// Attr = Attribute -> 属性

// プレイヤー
const uint32_t kCollisionAttrPlayer = 0b1;
// 建物
const uint32_t kCollisionAttrBuilding = 0b1 << 1;
// アイテム
const uint32_t kCollisionAttrItem = 0b1 << 2;