################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include u8g2/subdir.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
DevBoardTestBasic2.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
DevBoardTestBasic2.list \

OBJCOPY_HEX += \
DevBoardTestBasic2.hex \

OBJCOPY_BIN += \
DevBoardTestBasic2.bin \


# All Target
all: DevBoardTestBasic2.elf secondary-outputs

# Tool invocations
DevBoardTestBasic2.elf: $(OBJS) $(USER_OBJS) D:\work\STM32_Workspace\DevBoardTestBasic2\STM32F407VETX_FLASH.ld
	arm-none-eabi-gcc -o "DevBoardTestBasic2.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\work\STM32_Workspace\DevBoardTestBasic2\STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="DevBoardTestBasic2.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

DevBoardTestBasic2.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "DevBoardTestBasic2.list"
	@echo 'Finished building: $@'
	@echo ' '

DevBoardTestBasic2.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "DevBoardTestBasic2.hex"
	@echo 'Finished building: $@'
	@echo ' '

DevBoardTestBasic2.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "DevBoardTestBasic2.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
