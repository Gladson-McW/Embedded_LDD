savedcmd_/home/mcw_cbe/Embedded_LDD/usb_driver/usb_driver.mod := printf '%s\n'   usb_driver.o | awk '!x[$$0]++ { print("/home/mcw_cbe/Embedded_LDD/usb_driver/"$$0) }' > /home/mcw_cbe/Embedded_LDD/usb_driver/usb_driver.mod
