savedcmd_/home/mcw_cbe/Embedded_LDD/task_stat/task_stat.mod := printf '%s\n'   task_stat.o | awk '!x[$$0]++ { print("/home/mcw_cbe/Embedded_LDD/task_stat/"$$0) }' > /home/mcw_cbe/Embedded_LDD/task_stat/task_stat.mod
