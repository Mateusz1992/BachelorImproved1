################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Lib/subdir.mk
-include Dave/Generated/src/UART001/subdir.mk
-include Dave/Generated/src/SYSTM001/subdir.mk
-include Dave/Generated/src/RESET001/subdir.mk
-include Dave/Generated/src/NVIC002/subdir.mk
-include Dave/Generated/src/LIBS/subdir.mk
-include Dave/Generated/src/IO004/subdir.mk
-include Dave/Generated/src/I2C001/subdir.mk
-include Dave/Generated/src/DAVESupport/subdir.mk
-include Dave/Generated/src/CLK001/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
BachelorImproved1.hex \

SECONDARY_LIST += \
BachelorImproved1.lst \

SECONDARY_SIZE += \
BachelorImproved1.siz \


# All Target
all: BachelorImproved1.elf secondary-outputs

# Tool invocations
BachelorImproved1.elf: $(OBJS) ../BachelorImproved1.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM-GCC C Linker'
	"C:\DAVE-3.1.10\ARM-GCC/bin/arm-none-eabi-gcc" -T"../BachelorImproved1.ld" -nostartfiles -L"C:\DAVE-3.1.10\eclipse\/../CMSIS/Infineon/Lib" -L"C:\DAVE-3.1.10\eclipse\/../Examples/Lib" -L"C:\DAVE-3.1.10\eclipse\/../emWin/Start/GUI" -Wl,-Map,"BachelorImproved1.map" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "BachelorImproved1.elf" "@makefile.rsp" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

BachelorImproved1.hex: BachelorImproved1.elf
	@echo 'Invoking: ARM-GCC Create Flash Image'
	"C:\DAVE-3.1.10\ARM-GCC/bin/arm-none-eabi-objcopy" -O ihex "BachelorImproved1.elf"  "BachelorImproved1.hex"
	@echo 'Finished building: $@'
	@echo ' '

BachelorImproved1.lst: BachelorImproved1.elf
	@echo 'Invoking: ARM-GCC Create Listing'
	"C:\DAVE-3.1.10\ARM-GCC/bin/arm-none-eabi-objdump" -h -S "BachelorImproved1.elf" > "BachelorImproved1.lst"
	@echo 'Finished building: $@'
	@echo ' '

BachelorImproved1.siz: BachelorImproved1.elf
	@echo 'Invoking: ARM-GCC Print Size'
	"C:\DAVE-3.1.10\ARM-GCC/bin/arm-none-eabi-size"  --format=berkeley BachelorImproved1.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(SECONDARY_FLASH)$(S_DEPS)$(ARM_GCC_FOR_XMC_LINKER_OUTPUT_TYPE_OUTPUTS)$(SECONDARY_LIST) BachelorImproved1.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
