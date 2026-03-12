#pragma once
#include <stdint.h>

#define MAX_NAMED_AXES 64
#define MAX_CHANNELS   4   // anpassen

typedef struct {
    uint8_t letter_code; // enum axis_letter_code
    uint8_t chan;        // 1..MAX_CHANNELS (Siemens "Zehnerstelle")
    uint8_t idx;         // 1..15 (Siemens "Einerstelle")
} axis_name_t;

typedef struct {
    uint16_t axis_id;  // packed
    int joint;         // 0..EMCMOT_MAX_JOINTS-1
    uint8_t defined;
} named_axis_t;

typedef struct {
    uint8_t owner_ch;   // 0=free, else 1..MAX_CHANNELS
    uint8_t lock_count; // reentrant acquire
} axis_own_t;

// Global registries (motion RT side)
extern named_axis_t g_named_axes[MAX_NAMED_AXES];
extern axis_own_t   g_axis_own[MAX_NAMED_AXES];

// joint owner derived each cycle (or on change)
extern uint8_t *g_joint_owner_ch;

// Helpers
uint16_t axis_pack(uint8_t letter_code, uint8_t chan, uint8_t idx);
int axis_lookup_index(uint16_t axis_id);               // returns idx in g_named_axes or -1
int axis_define(uint16_t axis_id, int joint);          // config time
int axis_acquire(uint16_t axis_id, uint8_t channel);   // runtime
int axis_release(uint16_t axis_id, uint8_t channel);   // runtime
void axis_rebuild_joint_owner_map(int numJoints);      // after define or ownership change
uint8_t axis_get_owner_ch(int axis_num);