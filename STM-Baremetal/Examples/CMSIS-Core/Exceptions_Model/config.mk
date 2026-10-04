
# Sources
SRC += $(EXAMPLE_DIR)/main.c
SRC += $(EXAMPLE_DIR)/startup.S

INC += -I $(THIRD_PARTY_DIR)/CMSIS_6/CMSIS/Core/Include
INC += -I $(THIRD_PARTY_DIR)/CMSIS_6/CMSIS/Core/Include/m-profile
#
# GDB commands
GDB_COMMANS += -ex "b main"

