//
// YaPB - Counter-Strike Bot based on PODBot by Markus Klinge.
// Copyright © 2004-2022 YaPB Project <yapb@jeefo.net>.
//
// SPDX-License-Identifier: MIT
//

#pragma once

#include <crlib/basic.h>

#define _USE_MATH_DEFINES
#include <math.h>

CR_NAMESPACE_BEGIN

constexpr float kFloatEpsilon = 0.01f;
constexpr float kFloatEqualEpsilon = 0.001f;
constexpr float kFloatCmpEpsilon = 1.192092896e-07f;
constexpr float kMathPi = 3.141592653589793f;
constexpr float kDegreeToRadians = kMathPi / 180.0f;
constexpr float kRadiansToDegree = 180.0f / kMathPi;

CR_NAMESPACE_END

#include <crlib/simd.h>

CR_NAMESPACE_BEGIN

#if defined (CR_HAS_SSE)
template <> inline float abs (const float &x) {
   return _mm_cvtss_f32 (ssemath::fabs_ps (_mm_load_ss (&x)));
}

template <> inline float min (const float &a, const float &b) {
   return _mm_cvtss_f32 (_mm_min_ss (_mm_load_ss (&a), _mm_load_ss (&b)));
}

template <> inline float max (const float &a, const float &b) {
   return _mm_cvtss_f32 (_mm_max_ss (_mm_load_ss (&a), _mm_load_ss (&b)));
}

template <> inline float clamp (const float &x, const float &a, const float &b) {
   return _mm_cvtss_f32 (_mm_min_ss (_mm_max_ss (_mm_load_ss (&x), _mm_load_ss (&a)), _mm_load_ss (&b)));
}
#endif

static inline bool fzero (const float e) {
   return cr::abs (e) < kFloatEpsilon;
}

static inline bool fequal (const float a, const float b) {
   return cr:: abs (a - b) < kFloatEqualEpsilon;
}

constexpr float rad2deg (const float r) {
   return r * kRadiansToDegree;
}

constexpr float deg2rad (const float d) {
   return d * kDegreeToRadians;
}

constexpr float modAngles (const float a) {
   return 360.0f / 65536.0f * (static_cast <int> (a * (65536.0f / 360.0f)) & 65535);
}

constexpr float normalizeAngles (const float a) {
   return 360.0f / 65536.0f * (static_cast <int> ((a + 180.0f) * (65536.0f / 360.0f)) & 65535) - 180.0f;
}

constexpr float anglesDifference (const float a, const float b) {
   return normalizeAngles (a - b);
}

inline float sinf (const float value) {
#if defined (CR_HAS_SSE)
   return _mm_cvtss_f32 (ssemath::sin_ps (_mm_load_ss (&value)));
#else
   return ::sinf (value);
#endif
}

inline float cosf (const float value) {
#if defined (CR_HAS_SSE)
   return _mm_cvtss_f32 (ssemath::cos_ps (_mm_load_ss (&value)));
#else
   return ::cosf (value);
#endif
}

inline float atan2f (const float y, const float x) {
#if defined (CR_HAS_SSE)
   return _mm_cvtss_f32 (ssemath::atan2_ps (_mm_load_ss (&y), _mm_load_ss (&x)));
#else
   return ::atan2f (y, x);
#endif
}

inline float powf (const float x, const float y) {
#if defined (CR_HAS_SSE)
   return _mm_cvtss_f32 (ssemath::pow_ps (_mm_load_ss (&x), _mm_load_ss (&y)));
#else
   return ::powf (x, y);
#endif
}

inline float sqrtf (const float value) {
#if defined (CR_HAS_SSE)
   return _mm_cvtss_f32 (_mm_sqrt_ss (_mm_load_ss (&value)));
#else
   return ::sqrtf (value);
#endif
}

inline float tanf (const float value) {
#if defined (CR_HAS_SSE)
   return _mm_cvtss_f32 (ssemath::tan_ps (_mm_load_ss (&value)));
#else
   return ::tanf (value);
#endif
}

inline float ceilf (const float value) {
#if defined (CR_HAS_SSE)
   return _mm_cvtss_f32 (ssemath::ceil_ps (_mm_load_ss (&value)));
#else
   return ::ceilf (value);
#endif
}

inline float log10 (const float value) {
#if defined (CR_HAS_SSE)
   return _mm_cvtss_f32 (ssemath::log10_ps (_mm_load_ss (&value)));
#else
   return ::log10f (value);
#endif
}

CR_NAMESPACE_END
