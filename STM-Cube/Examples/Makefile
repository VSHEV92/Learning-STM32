
# check that example variable is set
ifeq ($(example),)
$(error Please, set "example" variable)
endif

PROJECT_NAME := $(notdir $(example))
PROJECT_DIR := $(abspath $(example))
PROJECT_SCRIPT := project_script.txt

IOC_FILE := $(abspath $(example)/$(PROJECT_NAME).ioc)

# check that .ioc file is set
ifeq ($(IOC_FILE),)
$(error .ioc file not exist under $(example) folder)
endif

PATCH_FILE := $(PROJECT_DIR)/$(PROJECT_NAME).patch
CUBE_PROJECT_PATH := $(PROJECT_DIR)/cube_project

CUBE_BUILD_OPTS += -nosplash 
CUBE_BUILD_OPTS += -application org.eclipse.cdt.managedbuilder.core.headlessbuild
CUBE_BUILD_OPTS += -data $(CUBE_PROJECT_PATH)
CUBE_BUILD_OPTS += -import $(CUBE_PROJECT_PATH)/$(PROJECT_NAME)
CUBE_BUILD_OPTS += -build $(PROJECT_NAME)/Release

all: clean create_project build_project program_project


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


build_project:
	stm32cubeide $(CUBE_BUILD_OPTS)


program_project:
	STM32_Programmer_CLI -c port=SWD -w $(CUBE_PROJECT_PATH)/$(PROJECT_NAME)/Release/$(PROJECT_NAME).elf 
	STM32_Programmer_CLI -c port=SWD -rst


.ONESHELL:
$(PROJECT_SCRIPT):
	cat <<EOF > $(PROJECT_DIR)/$(PROJECT_SCRIPT)
	config load $(abspath $(IOC_FILE))
	project path $(CUBE_PROJECT_PATH)
	project generate
	exit
	EOF


create_patch:
	cd $(CUBE_PROJECT_PATH);                        \
	git restore $(PROJECT_NAME)/.cproject;          \
	git diff > $(PATCH_FILE)


clean: 
	rm -rf $(PROJECT_DIR)/$(PROJECT_SCRIPT)
	rm -rf $(CUBE_PROJECT_PATH)


.PHONY: create_project build_project program_project clean



