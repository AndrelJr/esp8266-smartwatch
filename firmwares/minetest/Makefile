# Esptool firmware download options
PORT			?= /dev/ttyUSB0
BAUD			:= 1000000

# Directory structure
BUILDDIR		:= build
SRCDIR			:= src
OBJDIR			:= obj
LDSCRIPT		:= ldscript.ld
LIBESPHTTPD_DIR := libesphttpd

SRCS			:= $(wildcard  $(SRCDIR)/*.c)
OBJS			:= $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.c=.o)))
DEPS			:= $(addprefix $(OBJDIR),$(notdir $(SRCS:.c=.d)))

# SDK / Compiler settings
SDK_DIR			:= /opt/esp-open-sdk/esp_iot_sdk_v1.5.0
CC				:= xtensa-lx106-elf-gcc
LD				:= xtensa-lx106-elf-gcc
ESPTOOL			:= esptool.py
CFLAGS			:= -Os -fdata-sections -ffunction-sections -Wpointer-arith -Wall -Wno-parentheses -Wno-implicit-function-declaration -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -D__ets__ -DICACHE_FLASH
LDLIBS			:= -Wl,--start-group -lc -lgcc -lhal -lphy -lpp -lnet80211 -lwpa -lmain -llwip -lesphttpd -lwebpages-espfs -lcrypto -lm $(OBJS) -Wl,--end-group
LDFLAGS			:=  -nostdlib -Wl,--gc-sections -Wl,--no-check-sections -u call_user_start -Wl,-static -T $(LDSCRIPT)
INCLUDES		:= -I $(SDK_DIR)/include -I $(SDK_DIR)/include/json -I $(LIBESPHTTPD_DIR)/include -I $(SRCDIR)/

# Project settings
TARGET			:= handgelenk
FLASH_FREQ		:= 40m
FLASH_MODE		:= dio
FLASH_SIZE		:= 512
FLASH_SIZE_BIT	:= 4m
FLASH_OPT		:= --flash_freq $(FLASH_FREQ) --flash_mode $(FLASH_MODE) --flash_size $(FLASH_SIZE_BIT)

TARGET_ELF		:= $(BUILDDIR)/$(TARGET).elf
TARGET_FLASH	:= $(BUILDDIR)/$(TARGET)-0x00000.bin
TARGET_IROM		:= $(BUILDDIR)/$(TARGET)-0x40000.bin
MEM_LDSCRIPT	:= $(OBJDIR)/ldscript_memspecific.ld

LIBESPHTTPD		:= $(LIBESPHTTPD_DIR)/libesphttpd.a

all: $(OBJDIR) $(BUILDDIR) $(TARGET_FLASH) $(TARGET_IROM)
	@echo Compilation successful
	@echo Use \'make flash\' to flash firmware

$(TARGET_FLASH) $(TARGET_IROM): $(TARGET_ELF)
	$(ESPTOOL) elf2image $(FLASH_OPT) $^ --output $(BUILDDIR)/$(TARGET)-

$(TARGET_ELF): $(OBJS) $(LIBESPHTTPD) $(MEM_LDSCRIPT)
	$(LD) -L$(LIBESPHTTPD_DIR) -L$(SDK_DIR)/lib $(LDFLAGS) $(MEM_LDSCRIPT) $(LDLIBS) -o $@

$(MEM_LDSCRIPT):
	echo "MEMORY { irom0_0_seg : org = 0x40240000, len = "$$(printf "0x%X" $$(($(FLASH_SIZE)-0x4000)))" }"> $(MEM_LDSCRIPT)

$(LIBESPHTTPD):$(LIBESPHTTPD_DIR)
	$(MAKE) -C $(LIBESPHTTPD_DIR)
		
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

flash: all
	$(ESPTOOL) --port $(PORT) --baud $(BAUD) write_flash $(FLASH_OPT) 0x00000 $(TARGET_FLASH) 0x40000 $(TARGET_IROM)

.PHONY: clean

clean:
	$(RM) -r $(OBJDIR)
	$(RM) -r $(BUILDDIR)
	$(MAKE) -C $(LIBESPHTTPD_DIR) clean

-include $(DEPS)
