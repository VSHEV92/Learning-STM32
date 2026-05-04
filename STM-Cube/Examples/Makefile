
# check that .ioc file is set
ifeq ($(IOC_FILE),)
$(error Please, set IOC_FILE variable to CubeMX .ioc file)
endif


PROJECT_NAME := $(patsubst %.ioc,%,$(notdir $(IOC_FILE)))
PROJECT_DIR := $(abspath $(dir $(IOC_FILE)))
PROJECT_SCRIPT := project_script.txt

PATCH_FILE := $(PROJECT_DIR)/$(PROJECT_NAME).patch
CUBE_PROJECT_PATH := $(PROJECT_DIR)/cube_project

all: clean create_project


create_project: $(PROJECT_SCRIPT)
	$(shell which STM32CubeMX) -q $(PROJECT_DIR)/$(PROJECT_SCRIPT)

	cd $(CUBE_PROJECT_PATH);    \
	git init;                   \
	git add .;                  \
	git commit -m 'init commit'

ifneq ("$(wildcard $(PATCH_FILE))","")
	cd $(CUBE_PROJECT_PATH);    \
	git apply $(PATCH_FILE)
endif


.ONESHELL:
$(PROJECT_SCRIPT):
	cat <<EOF > $(PROJECT_DIR)/$(PROJECT_SCRIPT)
	config load $(abspath $(IOC_FILE))
	project path $(CUBE_PROJECT_PATH)
	project generate
	exit
	EOF

# create patch file for origin CubeMX file update 
create_patch:
	cd $(CUBE_PROJECT_PATH);                        \
	git restore $(PROJECT_NAME)/.cproject;          \
	git diff > $(PATCH_FILE)


clean: 
	rm -rf $(PROJECT_DIR)/$(PROJECT_SCRIPT)
	rm -rf $(CUBE_PROJECT_PATH)

.PHONY: create_project clean



