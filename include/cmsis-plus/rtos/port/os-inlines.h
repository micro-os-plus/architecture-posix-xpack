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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * If contains the implementation for all objects, using the
 * FreeRTOS API.
 *
 * This file is included in all src/os-*.cpp files.
 */

#ifndef CMSIS_PLUS_RTOS_PORT_OS_INLINES_H_
#define CMSIS_PLUS_RTOS_PORT_OS_INLINES_H_

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-app-config.h>
#include <cmsis-plus/rtos/os-c-decls.h>

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

#include <cmsis-plus/diag/trace.h>
#include <cmsis-plus/iso/malloc.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

extern "C" uint32_t signal_nesting;

namespace os
{
  namespace rtos
  {
    namespace port
    {
      // ----------------------------------------------------------------------

      namespace thread
      {
        class Context
        {
        public:

          // Used to avoid a complex casts below,
          // that might confuse the Eclipse formatter.
          typedef void
          (*func_t) (void);

          static void
          create (rtos::thread::Context* context, void* func, void* args);
        };

      } /* namespace thread */

      // ----------------------------------------------------------------------

      namespace scheduler
      {

        inline void
        __attribute__((always_inline))
        greeting (void)
        {
          struct utsname name;
          if (::uname (&name) != -1)
            {
              trace::printf ("POSIX synthetic, running on %s %s %s",
                             name.machine, name.sysname, name.release);
            }
          else
            {
              trace::printf ("POSIX synthetic");
            }

          trace::puts ("; non-preemptive.");
        }

        inline result_t
        __attribute__((always_inline))
        initialize (void)
        {
          signal_nesting = 0;

          return result::ok;
        }

        inline bool
        __attribute__((always_inline))
        in_handler_mode (void)
        {
          return (signal_nesting > 0);
        }

        inline void
        __attribute__((always_inline))
        lock (rtos::scheduler::status_t status)
        {
          ;
        }

        // Context functions must not be inlined (the compiler prevents this).
        void
        reschedule (bool save = true);

        inline void
        __attribute__((always_inline))
        _wait_for_interrupt (void)
        {
          trace::printf ("%s() \n", __func__);
          pause ();
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

        inline result_t
        __attribute__((always_inline))
        start (void)
        {
          rtos::scheduler::current_thread_ =
              rtos::scheduler::ready_threads_list_.remove_top ();

          ucontext_t* new_context =
              reinterpret_cast<ucontext_t*> (&(rtos::scheduler::current_thread_->context ().port_.ucontext));

          trace::printf ("%s() ctx %p %s\n", __func__, new_context,
                         rtos::scheduler::current_thread_->name ());

#if defined NDEBUG
          setcontext (new_context);
#else
          int res = setcontext (new_context);
          assert (res == 0);
#endif
          return result::ok;
        }

#pragma GCC diagnostic pop

      } /* namespace scheduler */

      namespace interrupts
      {
        class Critical_section
        {

        public:

          // Enter an IRQ critical section
          inline static rtos::interrupts::status_t
          __attribute__((always_inline))
          enter (void)
          {
            sigset_t set;
            sigemptyset (&set);
            sigaddset (&set, clock::signal_number);

            sigset_t old;
            sigprocmask (SIG_BLOCK, &set, &old);

            return old;
          }

          // Exit an IRQ critical section
          inline static void
          __attribute__((always_inline))
          exit (rtos::interrupts::status_t status)
          {
            sigprocmask (SIG_SETMASK, &status, nullptr);
          }
        };

        // ====================================================================

        class Uncritical_section
        {

        public:

          // Enter an IRQ uncritical section
          inline static rtos::interrupts::status_t
          __attribute__((always_inline))
          enter (void)
          {
            sigset_t set;
            sigemptyset (&set);
            sigaddset (&set, clock::signal_number);

            sigset_t old;
            sigprocmask (SIG_UNBLOCK, &set, &old);

            return old;
          }

          // Exit an IRQ critical section
          inline static void
          __attribute__((always_inline))
          exit (rtos::interrupts::status_t status)
          {
            sigprocmask (SIG_SETMASK, &status, nullptr);
          }
        };

      } /* namespace interrupts */

      class Thread
      {
      public:

        inline static void
        __attribute__((always_inline))
        clean (rtos::Thread* th)
        {
          // estd::free (th->context ().port_.ucontext.uc_stack.ss_sp);
        }

      };
      class Systick_clock
      {
      public:

        static void
        signal_handler (int signum);

        static void
        start (void);

      };

      // ======================================================================

      namespace this_thread
      {
        inline void
        __attribute__((always_inline))
        prepare_suspend (void)
        {
          ;
        }

        inline void
        __attribute__((always_inline))
        yield (void)
        {
          rtos::scheduler::current_thread_->resume ();
          scheduler::reschedule ();
        }

      } /* namespace this_thread */

    // ======================================================================

    } /* namespace port */
  } /* namespace rtos */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_PORT_OS_INLINES_H_ */