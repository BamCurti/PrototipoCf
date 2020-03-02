################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: CAFETERA\ FAST\ LINE.axf

# Tool invocations
CAFETERA\ FAST\ LINE.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="CAFETERA FAST LINE.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -mcpu=cortex-m0plus -mthumb -T CAFETERA_FAST_LINE_Debug.ld -o "CAFETERA FAST LINE.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) "CAFETERA FAST LINE.axf"
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "CAFETERA FAST LINE.axf"; # arm-none-eabi-objcopy -v -O binary "CAFETERA FAST LINE.axf" "CAFETERA FAST LINE.bin" ; # checksum -p MKL25Z128xxx4 -d "CAFETERA FAST LINE.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
