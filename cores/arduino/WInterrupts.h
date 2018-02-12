/*
 * Modified by RF Digital Corp. 2015.
 * www.RFDigital.com
 * 
 * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 */

/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _WIRING_INTERRUPTS_
#define _WIRING_INTERRUPTS_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*callback_t)(void);

void attachInterrupt(uint32_t pin, callback_t callback, uint32_t mode);
void detachInterrupt(uint32_t pin);

// prior non-compatible attachInterrupt() and detachInterrupt()
void dynamic_attachInterrupt(uint8_t IRQn, callback_t callback);
void dynamic_attachInterrupt_priority(uint8_t IRQn, callback_t callback, uint8_t priority);
void dynamic_detachInterrupt(uint8_t IRQn);

void attachPinInterrupt(uint32_t pin, pin_callback_t callback, uint32_t mode);
void detachPinInterrupt(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_INTERRUPTS_ */
