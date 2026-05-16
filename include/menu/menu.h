#pragma once

#include "../display/display.h"

#include "handler/home.h"
#include "handler/cut.h"

typedef struct {
    int32_t start_cut_pos;
    int32_t partial_cut_pos;
    int32_t full_cut_pos;
} menu_profile_t;

extern GCRYSLCD_MENU_DEFINE_PAGE(menu_main_page);

extern GCRYSLCD_MENU_DEFINE_PAGE(menu_about_page);

extern GCRYSLCD_MENU_DEFINE_PAGE(menu_start_page);

extern GCRYSLCD_MENU_DEFINE_PAGE(menu_pressets_page);

extern GCRYSLCD_MENU_DEFINE_PAGE(menu_settings_page);
extern GCRYSLCD_MENU_DEFINE_PAGE(menu_settings_wire_motor_page);
extern GCRYSLCD_MENU_DEFINE_PAGE(menu_settings_cut_motor_page);

extern bool menu_always_true;
extern bool menu_always_false;

extern int32_t menu_wire_motor_max_speed;
extern int32_t menu_wire_motor_accel;
extern int32_t menu_wire_motor_step_per_mm;

extern int32_t menu_cut_motor_max_speed;
extern int32_t menu_cut_motor_max_cut_speed;
extern int32_t menu_cut_motor_accel;

extern menu_profile_t menu_profile_0_5;
extern menu_profile_t menu_profile_1_0;
extern menu_profile_t menu_profile_1_5;
extern menu_profile_t menu_profile_2_0;
extern menu_profile_t menu_profile_2_5;
extern menu_profile_t menu_profile_3_0;
extern menu_profile_t menu_profile_3_5;
extern menu_profile_t menu_profile_4_0;
extern menu_profile_t menu_profile_4_5;
extern menu_profile_t menu_profile_5_0;

extern gcryslcd_menu_t menu;