.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword RobotPatrol_ctor
.dword RobotPatrol_dtor

# export table
/*0*/ .dword RobotPatrol_obj_Setup
/*1*/ .dword RobotPatrol_obj_Control
/*2*/ .dword RobotPatrol_obj_Update
/*3*/ .dword RobotPatrol_obj_Print
/*4*/ .dword RobotPatrol_obj_Free
/*5*/ .dword RobotPatrol_obj_GetModelFlags
/*6*/ .dword RobotPatrol_obj_GetDataSize
