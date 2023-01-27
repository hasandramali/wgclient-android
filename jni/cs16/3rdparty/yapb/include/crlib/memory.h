//
// YaPB - Counter-Strike Bot based on PODBot by Markus Klinge.
// Copyright © 2004-2022 YaPB Project <yapb@jeefo.net>.
//
// SPDX-License-Identifier: MIT
//

#pragma once

#include <crlib/basic.h>
#include <crlib/movable.h>
#include <crlib/platform.h>

// provide placment new to avoid stdc++ <new> header
inline void *operator new (const size_t, void *ptr) noexcept {
   return ptr;
}

CR_NAMESPACE_BEGIN

// internal memory manager
class Memory final {
public:
   Memory () = default;
   ~Memory () = default;

public:
   template <typename T> static T *get (const size_t length = 1) {
      // supress gcc warnings
#if defined (CR_CXX_GCC)
      if (length > PTRDIFF_MAX) {
         plat.abort ();
      }
#endif
      auto memory = reinterpret_cast <T *> (malloc (cr::max <size_t> (1u, length * sizeof (T))));

      if (!memory) {
         plat.abort ();
      }
      return memory;
   }

   template <typename T> static void release (T *memory) {
      free (memory);
      memory = nullptr;
   }

public:
   template <typename T, typename ...Args> static void construct (T *memory, Args &&...args) {
      new (memory) T (cr::forward <Args> (args)...);
   }

   template <typename T> static void destruct (T *memory) {
      memory->~T ();
   }

   template <typename T> static void transfer (T *dest, T *src, size_t length) noexcept {
      for (size_t i = 0; i < length; ++i) {
         construct (&dest[i], cr::move (src[i]));
         destruct (&src[i]);
      }
   }
};

CR_NAMESPACE_END
