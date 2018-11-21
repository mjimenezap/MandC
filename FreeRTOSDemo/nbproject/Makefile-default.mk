#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/FreeRTOSDemo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/FreeRTOSDemo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=DriverPicTrainer/config.c KernelFreeRTOS/tasks.c KernelFreeRTOS/croutine.c KernelFreeRTOS/list.c KernelFreeRTOS/queue.c KernelFreeRTOS/timers.c KernelFreeRTOS/event_groups.c KernelFreeRTOS/heap_1.c KernelFreeRTOS/port.c KernelFreeRTOS/portasm_dsPIC.S Serie.c Entradas.c Hora.c DemoMain.c sensor.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/DriverPicTrainer/config.o ${OBJECTDIR}/KernelFreeRTOS/tasks.o ${OBJECTDIR}/KernelFreeRTOS/croutine.o ${OBJECTDIR}/KernelFreeRTOS/list.o ${OBJECTDIR}/KernelFreeRTOS/queue.o ${OBJECTDIR}/KernelFreeRTOS/timers.o ${OBJECTDIR}/KernelFreeRTOS/event_groups.o ${OBJECTDIR}/KernelFreeRTOS/heap_1.o ${OBJECTDIR}/KernelFreeRTOS/port.o ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o ${OBJECTDIR}/Serie.o ${OBJECTDIR}/Entradas.o ${OBJECTDIR}/Hora.o ${OBJECTDIR}/DemoMain.o ${OBJECTDIR}/sensor.o
POSSIBLE_DEPFILES=${OBJECTDIR}/DriverPicTrainer/config.o.d ${OBJECTDIR}/KernelFreeRTOS/tasks.o.d ${OBJECTDIR}/KernelFreeRTOS/croutine.o.d ${OBJECTDIR}/KernelFreeRTOS/list.o.d ${OBJECTDIR}/KernelFreeRTOS/queue.o.d ${OBJECTDIR}/KernelFreeRTOS/timers.o.d ${OBJECTDIR}/KernelFreeRTOS/event_groups.o.d ${OBJECTDIR}/KernelFreeRTOS/heap_1.o.d ${OBJECTDIR}/KernelFreeRTOS/port.o.d ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.d ${OBJECTDIR}/Serie.o.d ${OBJECTDIR}/Entradas.o.d ${OBJECTDIR}/Hora.o.d ${OBJECTDIR}/DemoMain.o.d ${OBJECTDIR}/sensor.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/DriverPicTrainer/config.o ${OBJECTDIR}/KernelFreeRTOS/tasks.o ${OBJECTDIR}/KernelFreeRTOS/croutine.o ${OBJECTDIR}/KernelFreeRTOS/list.o ${OBJECTDIR}/KernelFreeRTOS/queue.o ${OBJECTDIR}/KernelFreeRTOS/timers.o ${OBJECTDIR}/KernelFreeRTOS/event_groups.o ${OBJECTDIR}/KernelFreeRTOS/heap_1.o ${OBJECTDIR}/KernelFreeRTOS/port.o ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o ${OBJECTDIR}/Serie.o ${OBJECTDIR}/Entradas.o ${OBJECTDIR}/Hora.o ${OBJECTDIR}/DemoMain.o ${OBJECTDIR}/sensor.o

# Source Files
SOURCEFILES=DriverPicTrainer/config.c KernelFreeRTOS/tasks.c KernelFreeRTOS/croutine.c KernelFreeRTOS/list.c KernelFreeRTOS/queue.c KernelFreeRTOS/timers.c KernelFreeRTOS/event_groups.c KernelFreeRTOS/heap_1.c KernelFreeRTOS/port.c KernelFreeRTOS/portasm_dsPIC.S Serie.c Entradas.c Hora.c DemoMain.c sensor.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/FreeRTOSDemo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ128MC802
MP_LINKER_FILE_OPTION=,--script=p33FJ128MC802.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/DriverPicTrainer/config.o: DriverPicTrainer/config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/DriverPicTrainer" 
	@${RM} ${OBJECTDIR}/DriverPicTrainer/config.o.d 
	@${RM} ${OBJECTDIR}/DriverPicTrainer/config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DriverPicTrainer/config.c  -o ${OBJECTDIR}/DriverPicTrainer/config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DriverPicTrainer/config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/DriverPicTrainer/config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/tasks.o: KernelFreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/tasks.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/tasks.c  -o ${OBJECTDIR}/KernelFreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/tasks.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/croutine.o: KernelFreeRTOS/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/croutine.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/croutine.c  -o ${OBJECTDIR}/KernelFreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/croutine.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/list.o: KernelFreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/list.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/list.c  -o ${OBJECTDIR}/KernelFreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/list.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/queue.o: KernelFreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/queue.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/queue.c  -o ${OBJECTDIR}/KernelFreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/queue.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/timers.o: KernelFreeRTOS/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/timers.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/timers.c  -o ${OBJECTDIR}/KernelFreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/timers.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/event_groups.o: KernelFreeRTOS/event_groups.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/event_groups.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/event_groups.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/event_groups.c  -o ${OBJECTDIR}/KernelFreeRTOS/event_groups.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/event_groups.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/event_groups.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/heap_1.o: KernelFreeRTOS/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/heap_1.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/heap_1.c  -o ${OBJECTDIR}/KernelFreeRTOS/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/heap_1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/heap_1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/port.o: KernelFreeRTOS/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/port.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/port.c  -o ${OBJECTDIR}/KernelFreeRTOS/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/port.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Serie.o: Serie.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Serie.o.d 
	@${RM} ${OBJECTDIR}/Serie.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Serie.c  -o ${OBJECTDIR}/Serie.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Serie.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/Serie.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Entradas.o: Entradas.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Entradas.o.d 
	@${RM} ${OBJECTDIR}/Entradas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Entradas.c  -o ${OBJECTDIR}/Entradas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Entradas.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/Entradas.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Hora.o: Hora.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Hora.o.d 
	@${RM} ${OBJECTDIR}/Hora.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Hora.c  -o ${OBJECTDIR}/Hora.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Hora.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/Hora.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/DemoMain.o: DemoMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DemoMain.o.d 
	@${RM} ${OBJECTDIR}/DemoMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DemoMain.c  -o ${OBJECTDIR}/DemoMain.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DemoMain.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/DemoMain.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/sensor.o: sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sensor.o.d 
	@${RM} ${OBJECTDIR}/sensor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sensor.c  -o ${OBJECTDIR}/sensor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sensor.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/sensor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/DriverPicTrainer/config.o: DriverPicTrainer/config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/DriverPicTrainer" 
	@${RM} ${OBJECTDIR}/DriverPicTrainer/config.o.d 
	@${RM} ${OBJECTDIR}/DriverPicTrainer/config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DriverPicTrainer/config.c  -o ${OBJECTDIR}/DriverPicTrainer/config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DriverPicTrainer/config.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/DriverPicTrainer/config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/tasks.o: KernelFreeRTOS/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/tasks.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/tasks.c  -o ${OBJECTDIR}/KernelFreeRTOS/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/tasks.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/croutine.o: KernelFreeRTOS/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/croutine.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/croutine.c  -o ${OBJECTDIR}/KernelFreeRTOS/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/croutine.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/list.o: KernelFreeRTOS/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/list.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/list.c  -o ${OBJECTDIR}/KernelFreeRTOS/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/list.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/queue.o: KernelFreeRTOS/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/queue.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/queue.c  -o ${OBJECTDIR}/KernelFreeRTOS/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/queue.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/timers.o: KernelFreeRTOS/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/timers.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/timers.c  -o ${OBJECTDIR}/KernelFreeRTOS/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/timers.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/event_groups.o: KernelFreeRTOS/event_groups.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/event_groups.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/event_groups.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/event_groups.c  -o ${OBJECTDIR}/KernelFreeRTOS/event_groups.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/event_groups.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/event_groups.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/heap_1.o: KernelFreeRTOS/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/heap_1.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/heap_1.c  -o ${OBJECTDIR}/KernelFreeRTOS/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/heap_1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/heap_1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/KernelFreeRTOS/port.o: KernelFreeRTOS/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/port.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  KernelFreeRTOS/port.c  -o ${OBJECTDIR}/KernelFreeRTOS/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/port.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Serie.o: Serie.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Serie.o.d 
	@${RM} ${OBJECTDIR}/Serie.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Serie.c  -o ${OBJECTDIR}/Serie.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Serie.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/Serie.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Entradas.o: Entradas.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Entradas.o.d 
	@${RM} ${OBJECTDIR}/Entradas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Entradas.c  -o ${OBJECTDIR}/Entradas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Entradas.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/Entradas.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Hora.o: Hora.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Hora.o.d 
	@${RM} ${OBJECTDIR}/Hora.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Hora.c  -o ${OBJECTDIR}/Hora.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Hora.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/Hora.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/DemoMain.o: DemoMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DemoMain.o.d 
	@${RM} ${OBJECTDIR}/DemoMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  DemoMain.c  -o ${OBJECTDIR}/DemoMain.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/DemoMain.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/DemoMain.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/sensor.o: sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sensor.o.d 
	@${RM} ${OBJECTDIR}/sensor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sensor.c  -o ${OBJECTDIR}/sensor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sensor.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O2 -I"./" -I"KernelFreeRTOS/include" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/sensor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o: KernelFreeRTOS/portasm_dsPIC.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  KernelFreeRTOS/portasm_dsPIC.S  -o ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -Wa,-MD,"${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.d" "${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o: KernelFreeRTOS/portasm_dsPIC.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/KernelFreeRTOS" 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.d 
	@${RM} ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  KernelFreeRTOS/portasm_dsPIC.S  -o ${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -Wa,-MD,"${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.d" "${OBJECTDIR}/KernelFreeRTOS/portasm_dsPIC.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/FreeRTOSDemo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/FreeRTOSDemo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=512,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/FreeRTOSDemo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/FreeRTOSDemo.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=512,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/FreeRTOSDemo.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
