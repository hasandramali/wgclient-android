//
// YaPB - Counter-Strike Bot based on PODBot by Markus Klinge.
// Copyright © 2004-2022 YaPB Project <yapb@jeefo.net>.
//
// SPDX-License-Identifier: MIT
//

#pragma once

#include <crlib/basic.h>

#if defined (CR_HAS_SSE)
#  include CR_INTRIN_INCLUDE
#endif

CR_NAMESPACE_BEGIN

#if defined (CR_HAS_SSE)

namespace ssemath {
#include <crlib/ssemath/ssemath.h>
}

// simple wrapper for vector
class CR_ALIGN16 SimdVec3Wrap final {
private:
   __m128 _mm_dp_ps_2 (__m128 v1, __m128 v2) {
      const auto mul = _mm_mul_ps (v1, v2);
      const auto res = _mm_add_ps (_mm_shuffle_ps (v2, mul, _MM_SHUFFLE (1, 0, 0, 0)), mul);
      const auto ref = _mm_add_ps (_mm_shuffle_ps (mul, res, _MM_SHUFFLE (0, 3, 0, 0)), res);

      return _mm_shuffle_ps (ref, ref, _MM_SHUFFLE (2, 2, 2, 2));
   }

public:
#if defined (CR_CXX_MSVC)
#   pragma warning(push)
#   pragma warning(disable: 4201)
#endif
   union {
      __m128 m;

      struct {
         float x, y, z, w;
      };
   };
#if defined (CR_CXX_MSVC)
#   pragma warning(pop) 
#endif

   SimdVec3Wrap (const float &x, const float &y, const float &z) {
      m = _mm_set_ps (0.0f, z, y, x);
   }

   SimdVec3Wrap (const float &x, const float &y) {
      m = _mm_set_ps (0.0f, 0.0f, y, x);
   }

   SimdVec3Wrap (__m128 m) : m (m) {}

public:
   SimdVec3Wrap () = default;
   ~SimdVec3Wrap () = default;

public:
   SimdVec3Wrap normalize () {
      return { _mm_div_ps (m, _mm_sqrt_ps (_mm_dp_ps_2 (m, m))) };
   }

   void angleVectors (SimdVec3Wrap &sines, SimdVec3Wrap &cosines) {
      static constexpr float d2r[4] = {
         kDegreeToRadians, kDegreeToRadians,
         kDegreeToRadians, kDegreeToRadians
      };
      ssemath::sincos_ps (_mm_mul_ps (m, _mm_load_ps (d2r)), sines.m, cosines.m);
   }
};

#endif

CR_NAMESPACE_END
