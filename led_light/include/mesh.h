/**
 * @file mesh.h
 * @brief
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 0.1
 * @date 2019-03-06
 */

#ifndef _MESH_H_
#define _MESH_H_

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <settings/settings.h>

#include "node_composition.h"

#define BT_MESH_MODEL_LIGHT_LIGHTNESS_STATUS BT_MESH_MODEL_OP_2(0x82, 0x4E)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_STATUS BT_MESH_MODEL_OP_2(0x82, 0x52)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_STATUS BT_MESH_MODEL_OP_2(0x82, 0x54)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_STATUS BT_MESH_MODEL_OP_2(0x82, 0x56)
#define BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_STATUS BT_MESH_MODEL_OP_2(0x82, 0x58)


extern const struct bt_mesh_comp prov;

void bt_ready(int err);

#endif

