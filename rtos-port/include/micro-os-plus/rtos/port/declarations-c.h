/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MICRO_OS_PLUS_ARCHITECTURE_POSIX_RTOS_PORT_OS_C_DECLS_H_
#define MICRO_OS_PLUS_ARCHITECTURE_POSIX_RTOS_PORT_OS_C_DECLS_H_

// ----------------------------------------------------------------------------

/*
 * This file is included in `micro-os-plus/rtos/declarations-c.h` to
 * customise it with port specific declarations.
 *
 * These structures (which basically contain handlers)
 * are conditionally included in the system objects
 * when they are implemented using the port native objects.
 */

#include <stdint.h>

#if !defined(_XOPEN_SOURCE)
#error This port requires defining _XOPEN_SOURCE=700L globally
#endif
#include <ucontext.h>

#include <signal.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

// Must match port::clock::timestamp_t
typedef uint64_t micro_os_plus_port_clock_timestamp_t;

// Must match port::clock::duration_t
typedef uint32_t micro_os_plus_port_clock_duration_t;

// Must match port::clock::offset_t
typedef uint64_t micro_os_plus_port_clock_offset_t;

typedef struct
{
  ucontext_t ucontext; //
} micro_os_plus_port_thread_context_t;

typedef bool micro_os_plus_port_scheduler_state_t;

// Signal set (true if signal blocked)
typedef bool micro_os_plus_port_interrupts_state_t;

typedef uint64_t micro_os_plus_port_thread_stack_element_t;
typedef uint64_t micro_os_plus_port_thread_stack_allocation_element_t;

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_ARCHITECTURE_POSIX_RTOS_PORT_OS_C_DECLS_H_

// ----------------------------------------------------------------------------
