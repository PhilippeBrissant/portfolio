#!make
SHELL := /bin/bash

filter = esp8266
board = esp8266:esp8266:nodemcuv2
port = COM8
sketch =

create-plugin-dir:
	- @if [ ! -d "plugins" ];then mkdir "plugins";fi
	- @if [ ! -d "plugins/arduino-cli" ];then mkdir "plugins/arduino-cli"; fi
	- @if [ ! -d "plugins/arduino-cli/bin" ];then mkdir "plugins/arduino-cli/bin"; fi

install-arduino-cli:
	- @echo "\n*** INSTALLING ARDUINO-CLI *** \n"
	- @if [ ! -f "plugins/arduino-cli/bin/arduino-cli" ];then\
	(curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=plugins/arduino-cli/bin sh);\
	else echo "There is already an arduino-cli installed. Skipping installation.";\
	fi
	- @echo "\nARDUINO-CLI VERSION"
	- @plugins/arduino-cli/bin/arduino-cli version

install-arduino-tools: create-plugin-dir install-arduino-cli
	- @echo "Arduino Tools setup completed!"

config-arduino-cli:
	- @echo "\n*** CONFIGURING ARDUINO_CLI ***\n"
	- @plugins/arduino-cli/bin/arduino-cli config init --overwrite
	- @plugins/arduino-cli/bin/arduino-cli config set logging.file ./plugins/arduino-cli/bin/arduino-cli.log
	- @plugins/arduino-cli/bin/arduino-cli config add board_manager.additional_urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
	- @plugins/arduino-cli/bin/arduino-cli config add board_manager.additional_urls https://dl.espressif.com/dl/package_esp32_index.json
	- @plugins/arduino-cli/bin/arduino-cli core update-index

install-esp-tools: install-arduino-tools config-arduino-cli
	- @echo "\n*** INSTALLING ESP TOOLS ***\n"
	- @plugins/arduino-cli/bin/arduino-cli core install esp8266:esp8266
	- @plugins/arduino-cli/bin/arduino-cli core install esp32:esp32
	- @echo "ESP Tools setup completed!"

get-fbqn:
	- @plugins/arduino-cli/bin/arduino-cli board listall | grep --ignore-case $(filter)

build: 
	- @echo "*** COMPILE ***"
	- @plugins/arduino-cli/bin/arduino-cli compile -b $(board) $(sketch)

upload: build
	- @echo "*** UPLOAD ***"
	- @plugins/arduino-cli/bin/arduino-cli upload -b $(board) -p $(port) $(sketch)
