// adafruit_itsybitsy_m4.build.extra_flags=-D__SAMD51G19A__ -DADAFRUIT_ITSYBITSY_M4_EXPRESS -D__SAMD51__ {build.usb_flags} -D__FPU_PRESENT -DARM_MATH_CM4 -DCRYSTALLESS -mfloat-abi=hard -mfpu=fpv4-sp-d16
#ifdef ADAFRUIT_ITSYBITSY_M4_EXPRESS
  #undef DOTSTAR_LIB_ENABLED  // swap these two
  #define DOTSTAR_LIB_ENABLED // swap these two
#endif // #ifdef ADAFRUIT_ITSYBITSY_M4_EXPRESS

// #ifndef DOTSTAR_LIB_ENABLED
  // #define DOTSTAR_LIB_ENABLED // override
// #endif

// DOTSTAR
#ifdef DOTSTAR_LIB_ENABLED
  #ifndef HAS_DOTSTAR_LIB
    #define HAS_DOTSTAR_LIB
  #endif
#endif

#ifndef DOTSTAR_LIB_ENABLED
  #ifdef HAS_DOTSTAR_LIB
    #undef HAS_DOTSTAR_LIB
  #endif
#endif
