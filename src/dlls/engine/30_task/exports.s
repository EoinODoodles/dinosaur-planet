.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword task_ctor
.dword task_dtor

# export table
.dword task_LoadRecentlyCompleted
.dword task_MarkTaskCompleted
.dword task_GetNumRecentlyCompleted
.dword task_GetRecentlyCompletedTaskText
.dword task_GetCompletionTaskText
.dword task_GetCompletionPercentage
