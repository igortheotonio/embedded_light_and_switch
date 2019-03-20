/**
 * @file ic_bt.h
 * @Lucas Peixoto (lpdac@ic.ufal.br)
 * @brief
 * @version 0.1
 * @date 2018-12-25
 *
 * @copyright Copyright (c) 2018
 *
 */

#ifndef _IC_BT_H
#define _IC_BT_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/hci.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/mesh.h>
#include <settings/settings.h>

extern const struct bt_mesh_model_op generic_onoff_srv_op[];


// Generic OnOff Messages Types
#define BT_MESH_MODEL_OP_GENERIC_ONOFF_GET BT_MESH_MODEL_OP_2(0x82, 0x01)
#define BT_MESH_MODEL_OP_GENERIC_ONOFF_SET BT_MESH_MODEL_OP_2(0x82, 0x02)
#define BT_MESH_MODEL_OP_GENERIC_ONOFF_SET_UNACK BT_MESH_MODEL_OP_2(0x82, 0x03)
#define BT_MESH_MODEL_OP_GENERIC_ONOFF_STATUS BT_MESH_MODEL_OP_2(0x82, 0x04)


/**
 * @brief
 *
 * @param model
 * @param ctx
 * @param buf
 */
void generic_onoff_get(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                       struct net_buf_simple *buf);


/**
 * @brief
 *
 * @param model
 * @param ctx
 * @param buf
 */
void generic_onoff_set(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                       struct net_buf_simple *buf);


/**
 * @brief
 *
 * @param model
 * @param ctx
 * @param buf
 */
void generic_onoff_set_unack(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                             struct net_buf_simple *buf);

void generic_onoff_status(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx,
                          struct net_buf_simple *buf);
#endif
