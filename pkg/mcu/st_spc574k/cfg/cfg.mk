# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2012  Evidence Srl
# 
# This file is part of ERIKA Enterprise.
# 
# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation, 
# (with a special exception described below).
# 
# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.
# 
# As a special exception, the copyright holders of this library give you
# permission to link this code with independent modules to produce an
# executable, regardless of the license terms of these independent
# modules, and to copy and distribute the resulting executable under
# terms of your choice, provided that you also meet, for each linked
# independent module, the terms and conditions of the license of that
# module.  An independent module is a module which is not derived from
# or based on this library.  If you modify this code, you may extend
# this exception to your version of the code, but you are not
# obligated to do so.  If you do not wish to do so, delete this
# exception statement from your version.
# 
# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.
# 
# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

## Author: 2014 Francesco Esposito.

ifeq ($(call iseeopt, EE_SPC574K), yes)

# Static ISR Vector Table
ifneq ($(or $(call iseeopt, EE_ISR_DYNAMIC_TABLE), $(call iseeopt,  EE_ISR_EXTERNAL_TABLE)), yes)
EE_SRCS += pkg/mcu/st_spc574k/src/ee_vtable.c
endif # EE_ISR_DYNAMIC_TABLE

# Enable System Timer
ifeq ($(call iseeopt, ENABLE_SYSTEM_TIMER), yes)
ifeq ($(call iseeopt, EE_SYSTEM_TIMER_DEVICE_STM), yes)
EE_SRCS += pkg/mcu/st_spc574k/src/ee_system_timer.c
endif # EE_SYSTEM_TIMER_DEVICE_STM
endif # ENABLE_SYSTEM_TIMER

endif # EE_SPC574K
