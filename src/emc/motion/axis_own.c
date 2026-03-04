#include "axis_own.h"
#include <string.h>

named_axis_t g_named_axes[MAX_NAMED_AXES];
axis_own_t   g_axis_own[MAX_NAMED_AXES];

// you can place this in a static array in motion, size EMCMOT_MAX_JOINTS
uint8_t *g_joint_owner_ch = 0;

uint16_t axis_pack(uint8_t letter_code, uint8_t chan, uint8_t idx) {
    return (uint16_t)(((letter_code & 0x0F) << 12) | ((chan & 0x0F) << 8) | ((idx & 0x0F) << 4));
}

int axis_lookup_index(uint16_t axis_id) {
    for (int i=0; i<MAX_NAMED_AXES; i++) {
        if (g_named_axes[i].defined && g_named_axes[i].axis_id == axis_id) return i;
    }
    return -1;
}

int axis_define(uint16_t axis_id, int joint) {
    // prevent duplicates
    if (axis_lookup_index(axis_id) >= 0) return -2;

    // prevent two axes pointing to same joint? (recommended)
    for (int i=0; i<MAX_NAMED_AXES; i++) {
        if (g_named_axes[i].defined && g_named_axes[i].joint == joint) return -3;
    }

    for (int i=0; i<MAX_NAMED_AXES; i++) {
        if (!g_named_axes[i].defined) {
            g_named_axes[i].axis_id = axis_id;
            g_named_axes[i].joint   = joint;
            g_named_axes[i].defined = 1;
            g_axis_own[i].owner_ch  = 0;
            g_axis_own[i].lock_count= 0;
            return i;
        }
    }
    return -1; // full
}

int axis_acquire(uint16_t axis_id, uint8_t channel) {
    int i = axis_lookup_index(axis_id);
    if (i < 0) return -10; // unknown axis

    if (g_axis_own[i].owner_ch == 0) {
        g_axis_own[i].owner_ch = channel;
        g_axis_own[i].lock_count = 1;
        return 0;
    }
    if (g_axis_own[i].owner_ch == channel) {
        if (g_axis_own[i].lock_count < 255) g_axis_own[i].lock_count++;
        return 0;
    }
    return -11; // busy owned by other channel
}

int axis_release(uint16_t axis_id, uint8_t channel) {
    int i = axis_lookup_index(axis_id);
    if (i < 0) return -10;

    if (g_axis_own[i].owner_ch != channel) return -12; // not owner

    if (g_axis_own[i].lock_count > 1) {
        g_axis_own[i].lock_count--;
        return 0;
    }
    g_axis_own[i].owner_ch = 0;
    g_axis_own[i].lock_count = 0;
    return 0;
}

void axis_rebuild_joint_owner_map(int numJoints) {
    if (!g_joint_owner_ch) return;
    for (int j=0; j<numJoints; j++) g_joint_owner_ch[j] = 0;

    for (int i=0; i<MAX_NAMED_AXES; i++) {
        if (!g_named_axes[i].defined) continue;
        int j = g_named_axes[i].joint;
        if (j < 0 || j >= numJoints) continue;
        uint8_t owner = g_axis_own[i].owner_ch;
        if (owner) g_joint_owner_ch[j] = owner;
    }
}