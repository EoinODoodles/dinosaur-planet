.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword pausemenu_ctor
.dword pausemenu_dtor

# export table
.dword pausemenu_Update1
.dword pausemenu_Update2
.dword pausemenu_Draw
