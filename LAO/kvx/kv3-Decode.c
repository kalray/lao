/*
 * kv3/Decode.c
 *  (c) Copyright 2010-2018 Kalray SA.
 * Automatically generated from the Machine Description System (MDS).
 */

#include "MDT/MDT_.h"

#ifdef $XCC__h
#ifndef NDECODE
extern Opcode
Decode_Decoding_v1_double(const void *buffer);
#endif//NDECODE
#endif//$XCC__h

#ifndef NDECODE
Opcode
Decode_Decoding_v1_double(const void *buffer)
{
  const uint32_t *codeWords = (const uint32_t *)buffer;
  uint32_t codeWord_0 = codeWords[0];
  uint32_t codeWord_1 = codeWords[1];
  switch ((codeWord_0 >> 28) & 0x0000000f) {
  case 0x0000000a:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LBZ_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LBS_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LHZ_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LHS_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 25) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LV_speculate_registerA_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LV_column_speculate_registerAq_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_SB_upper27_lower10_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v1_SH_upper27_lower10_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_SW_upper27_lower10_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v1_SD_upper27_lower10_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SQ_upper27_lower10_registerZ_registerU_double;
            case 0x00000001:
              switch ((codeWord_0 >> 19) & 0x00000001) {
              case 0x00000000:
                return Opcode_v1_SO_upper27_lower10_registerZ_registerV_double;
              case 0x00000001:
                switch ((codeWord_0 >> 20) & 0x0000000f) {
                case 0x00000000:
                  return Opcode_v1_DZEROL_upper27_lower10_registerZ_double;
                default:
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            return Opcode_v1_SV_upper27_lower10_registerZ_registerE_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ALCLRW_registerW_upper27_lower10_registerZ_double;
          case 0x00000001:
            return Opcode_v1_ALCLRD_registerW_upper27_lower10_registerZ_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ALADDW_upper27_lower10_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v1_ALADDD_upper27_lower10_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LBZ_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LBS_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LHZ_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LHS_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 25) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LV_speculate_lsucond_registerY_registerA_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LV_column_speculate_lsucond_registerY_registerAq_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_SB_lsucond_registerY_offset27_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v1_SH_lsucond_registerY_offset27_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_SW_lsucond_registerY_offset27_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v1_SD_lsucond_registerY_offset27_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SQ_lsucond_registerY_offset27_registerZ_registerU_double;
            case 0x00000001:
              switch ((codeWord_0 >> 19) & 0x00000001) {
              case 0x00000000:
                return Opcode_v1_SO_lsucond_registerY_offset27_registerZ_registerV_double;
              case 0x00000001:
                switch ((codeWord_0 >> 20) & 0x0000000f) {
                case 0x00000000:
                  return Opcode_v1_DZEROL_lsucond_registerY_offset27_registerZ_double;
                default:
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            return Opcode_v1_SV_lsucond_registerY_offset27_registerZ_registerE_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ALCLRW_lsucond_registerY_registerW_offset27_registerZ_double;
          case 0x00000001:
            return Opcode_v1_ALCLRD_lsucond_registerY_registerW_offset27_registerZ_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ALADDW_lsucond_registerY_offset27_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v1_ALADDD_lsucond_registerY_offset27_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000b:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LWZ_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LWS_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LD_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_LQ_variant_registerM_upper27_lower10_registerZ_double;
          case 0x00000001:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_LO_variant_registerN_upper27_lower10_registerZ_double;
            case 0x00000001:
              switch ((codeWord_0 >> 20) & 0x0000000f) {
              case 0x00000000:
                return Opcode_v1_DTOUCHL_upper27_lower10_registerZ_double;
              case 0x00000001:
                return Opcode_v1_DINVALL_upper27_lower10_registerZ_double;
              case 0x00000005:
                return Opcode_v1_IINVALS_upper27_lower10_registerZ_double;
              default:
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x0000000e:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ACSWAPW_upper27_lower10_registerZ_registerU_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ACSWAPD_upper27_lower10_registerZ_registerU_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LWZ_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LWS_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_LD_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_LQ_variant_lsucond_registerY_registerM_offset27_registerZ_double;
          case 0x00000001:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_LO_variant_lsucond_registerY_registerN_offset27_registerZ_double;
            case 0x00000001:
              switch ((codeWord_0 >> 20) & 0x0000000f) {
              case 0x00000000:
                return Opcode_v1_DTOUCHL_lsucond_registerY_offset27_registerZ_double;
              case 0x00000001:
                return Opcode_v1_DINVALL_lsucond_registerY_offset27_registerZ_double;
              case 0x00000005:
                return Opcode_v1_IINVALS_lsucond_registerY_offset27_registerZ_double;
              default:
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x0000000e:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ACSWAPW_lsucond_registerY_offset27_registerZ_registerU_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ACSWAPD_lsucond_registerY_offset27_registerZ_registerU_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000c:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MADDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMAD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FADDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_FFMAWDP_registerM_registerZ_upper27_lower10_double;
          case 0x00000001:
            return Opcode_v1_FMULWDP_registerM_registerZ_upper27_lower10_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MADDWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMAWD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FADDWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_FFMAHWQ_registerM_registerZ_upper27_lower10_double;
          case 0x00000001:
            return Opcode_v1_FMULHWQ_registerM_registerZ_upper27_lower10_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MADDHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMAWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FADDHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_FFMSWDP_registerM_registerZ_upper27_lower10_double;
          case 0x00000001:
            return Opcode_v1_FMULWDC_registerM_registerZ_upper27_lower10_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMAHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FADDCWC_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_FFMSHWQ_registerM_registerZ_upper27_lower10_double;
          case 0x00000001:
            return Opcode_v1_FMULCWDC_registerM_registerZ_upper27_lower10_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MULD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMSD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FSBFD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000005:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MULWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMSWD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FSBFWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MULHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMSWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FSBFHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000007:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MULWC_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMSHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FSBFCWC_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000008:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MADDDT_registerM_registerZ_upper27_lower10_double;
          case 0x00000001:
            return Opcode_v1_MULDT_registerM_registerZ_upper27_lower10_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FMULD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMAHW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000009:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MADDUDT_registerM_registerZ_upper27_lower10_double;
          case 0x00000001:
            return Opcode_v1_MULUDT_registerM_registerZ_upper27_lower10_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FMULWD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMAW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000a:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MADDSUDT_registerM_registerZ_upper27_lower10_double;
          case 0x00000001:
            return Opcode_v1_MULSUDT_registerM_registerZ_upper27_lower10_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FMULWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMSHW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000b:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MADDUZDT_registerM_registerZ_upper27_lower10_double;
          case 0x00000001:
            return Opcode_v1_CMULDT_registerM_registerZ_upper27_lower10_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FMULHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FFMSW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000c:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_DOT2WD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FDOT2W_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FADDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000d:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_DOT2UWD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FDOT2WD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FSBFW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000e:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_DOT2SUWD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FMULWC_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FMULW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000f:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_DOT2W_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FMULCWC_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_FMULHW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000d:
    switch ((codeWord_0 >> 11) & 0x0000007f) {
    case 0x00000004:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000008:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_CRCBELMW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_CRCBELLW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_CRCLELMW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_CRCLELLW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MADDWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MADDUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MADDSUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MADDW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MSBFWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MSBFUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MSBFSUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MSBFW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MULWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MULUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MULSUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MULW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000e:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MAKE_registerW_extend6_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ADDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MIND_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MAXD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ABDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_SBFD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MINUD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MAXUD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ANDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_NANDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ORD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_NORD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000c:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_XORD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_NXORD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ANDND_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ORND_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_1 >> 29) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_ADDSD_registerW_registerZ_upper27_lower10_double;
        case 0x0000000f:
          return Opcode_v1_SBFSD_registerW_registerZ_upper27_lower10_double;
        default:
          return Opcode_v1_COMPD_comparison_registerW_registerZ_upper27_lower10_double;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_1 >> 29) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_SBMM8_registerW_registerZ_upper27_lower10_double;
        case 0x0000000f:
          return Opcode_v1_SBMMT8_registerW_registerZ_upper27_lower10_double;
        default:
          return Opcode_v1_CMOVED_scalarcond_registerZ_registerW_upper27_lower10_double;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000f:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_PCREL_registerW_extend6_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ADDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MINW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MAXW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ABDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_SBFW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MINUW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_MAXUW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ANDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_NANDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ORW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_NORW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000c:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_XORW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_NXORW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ANDNW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v1_ORNW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 12) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_ANDD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_NANDD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_ORD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_NORD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_XORD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_NXORD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_ANDND_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_ORND_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MINWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MAXWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_1 >> 0) & 0x07ffffff) {
            case 0x00000000:
              switch ((codeWord_0 >> 6) & 0x0000003f) {
              case 0x00000000:
                return Opcode_v1_ABSWP_registerW_registerZ_double;
              default:
                return Opcode_v1_ABDWP_splat32_registerW_registerZ_upper27_lower5_double;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v1_ABDWP_splat32_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_1 >> 0) & 0x07ffffff) {
            case 0x00000000:
              switch ((codeWord_0 >> 6) & 0x0000003f) {
              case 0x00000000:
                return Opcode_v1_NEGWP_registerW_registerZ_double;
              default:
                return Opcode_v1_SBFWP_splat32_registerW_registerZ_upper27_lower5_double;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v1_SBFWP_splat32_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MINUWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MAXUWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDCWC_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFCWC_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MINHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MAXHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_1 >> 0) & 0x07ffffff) {
            case 0x00000000:
              switch ((codeWord_0 >> 6) & 0x0000003f) {
              case 0x00000000:
                return Opcode_v1_ABSHQ_registerW_registerZ_double;
              default:
                return Opcode_v1_ABDHQ_splat32_registerW_registerZ_upper27_lower5_double;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v1_ABDHQ_splat32_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_1 >> 0) & 0x07ffffff) {
            case 0x00000000:
              switch ((codeWord_0 >> 6) & 0x0000003f) {
              case 0x00000000:
                return Opcode_v1_NEGHQ_registerW_registerZ_double;
              default:
                return Opcode_v1_SBFHQ_splat32_registerW_registerZ_upper27_lower5_double;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v1_SBFHQ_splat32_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MINUHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MAXUHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDCHCP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFCHCP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX2D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX2D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX4D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX4D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX8D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX8D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX16D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX16D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDWD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFWD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDUWD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFUWD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LANDD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LNANDD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LORD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LNORD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX2W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX2W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX4W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX4W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX8W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX8W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX16W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX16W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_AVGW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_AVGUW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_AVGRW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_AVGRUW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_LANDW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_LNANDW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_LORW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_LNORW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX2WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX2WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX4WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX4WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX8WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX8WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX16WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX16WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_AVGWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_AVGUWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_AVGRWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_AVGRUWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LANDWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LNANDWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LORWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LNORWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX2HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX2HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX4HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX4HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX8HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX8HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDX16HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFX16HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_AVGHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_AVGUHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_AVGRHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_AVGRUHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LANDHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LNANDHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LORHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LNORHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX2WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX2WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX4WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX4WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX8WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX8WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX16WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX16WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX2UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX2UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX4UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX4UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX8UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX8UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDX16UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFX16UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 11) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x0000000f) {
            case 0x0000000e:
              return Opcode_v1_ADDSW_registerW_registerZ_upper27_lower5_double;
            case 0x0000000f:
              return Opcode_v1_SBFSW_registerW_registerZ_upper27_lower5_double;
            default:
              return Opcode_v1_COMPW_comparison_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000c:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDCD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFCD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ADDCD_I_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SBFCD_I_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDSWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFSWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDSHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFSHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 11) & 0x0000001f) {
      case 0x00000001:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBMM8_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBMMT8_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 11) & 0x0000001f) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 27) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_FCOMPW_floatcomp_registerW_registerZ_upper27_lower5_double;
          case 0x00000001:
            return Opcode_v1_FCOMPD_floatcomp_registerW_registerZ_upper27_lower5_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  default:
    break;
  }
  return Opcode__UNDEF;
}
#endif//NDECODE

#ifdef $XCC__h
#ifndef NDECODE
extern Opcode
Decode_Decoding_v1_simple(const void *buffer);
#endif//NDECODE
#endif//$XCC__h

#ifndef NDECODE
Opcode
Decode_Decoding_v1_simple(const void *buffer)
{
  const uint32_t *codeWords = (const uint32_t *)buffer;
  uint32_t codeWord_0 = codeWords[0];
  switch ((codeWord_0 >> 28) & 0x00000007) {
  case 0x00000000:
    switch ((codeWord_0 >> 27) & 0x00000001) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_0 >> 18) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 6) & 0x00000fff) {
          case 0x00000000:
            switch ((codeWord_0 >> 20) & 0x0000000f) {
            case 0x00000000:
              return Opcode_v1_ERROP_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000001:
              switch ((codeWord_0 >> 0) & 0x0000003f) {
              case 0x00000000:
                switch ((codeWord_0 >> 7) & 0x0000001f) {
                case 0x0000001f:
                  return Opcode_v1_MOVEFO_registerN_registerBe_simple;
                default:
                  return Opcode_v1_ALIGNO_registerN_registerBe_registerCo_byteshift_simple;
                  break;
                }
                return Opcode__UNDEF;
              default:
                return Opcode_v1_ALIGNO_registerN_registerBe_registerCo_byteshift_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_ALIGNO_registerN_registerBe_registerCo_registerZ_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000001:
              switch ((codeWord_0 >> 0) & 0x0000003f) {
              case 0x00000000:
                switch ((codeWord_0 >> 7) & 0x0000001f) {
                case 0x0000001f:
                  return Opcode_v1_MOVEFO_registerN_registerBo_simple;
                default:
                  return Opcode_v1_ALIGNO_registerN_registerBo_registerCe_byteshift_simple;
                  break;
                }
                return Opcode__UNDEF;
              default:
                return Opcode_v1_ALIGNO_registerN_registerBo_registerCe_byteshift_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000001:
              return Opcode_v1_ALIGNO_registerN_registerBo_registerCe_registerZ_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 12) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 0) & 0x0000003f) {
            case 0x00000000:
              switch ((codeWord_0 >> 7) & 0x0000001f) {
              case 0x0000001f:
                return Opcode_v1_COPYV_registerA_registerBe_simple;
              default:
                return Opcode_v1_ALIGNV_registerA_registerBe_registerCo_byteshift_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v1_ALIGNV_registerA_registerBe_registerCo_byteshift_simple;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 0) & 0x0000003f) {
            case 0x00000000:
              switch ((codeWord_0 >> 7) & 0x0000001f) {
              case 0x0000001f:
                return Opcode_v1_COPYV_registerA_registerBo_simple;
              default:
                return Opcode_v1_ALIGNV_registerA_registerBo_registerCe_byteshift_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v1_ALIGNV_registerA_registerBo_registerCe_byteshift_simple;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 12) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_ALIGNV_registerA_registerBe_registerCo_registerZ_simple;
          case 0x00000001:
            return Opcode_v1_ALIGNV_registerA_registerBo_registerCe_registerZ_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 12) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MMA484BW_registerAp_registerBp_registerC_registerD_simple;
          case 0x00000001:
            return Opcode_v1_MMA484SUBW_registerAp_registerBp_registerC_registerD_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MMA484UBW_registerAp_registerBp_registerC_registerD_simple;
          case 0x00000001:
            return Opcode_v1_MMA484USBW_registerAp_registerBp_registerC_registerD_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 12) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_FMMA242HW0_registerA0_registerBp_registerC_registerD_simple;
          case 0x00000001:
            return Opcode_v1_FMMA242HW2_registerA2_registerBp_registerC_registerD_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_FMMA242HW1_registerA1_registerBp_registerC_registerD_simple;
          case 0x00000001:
            return Opcode_v1_FMMA242HW3_registerA3_registerBp_registerC_registerD_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 12) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MMA444HBD0_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000001:
            return Opcode_v1_MMA444HBD1_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000002:
            return Opcode_v1_MMA444HD_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000003:
            return Opcode_v1_MT44D_registerAq_registerBq_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MMA444UHBD0_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000001:
            return Opcode_v1_MMA444UHBD1_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000002:
            return Opcode_v1_MMA444UHD_registerAq_registerBq_registerC_registerD_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_0 >> 18) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MMA444SUHBD0_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000001:
            return Opcode_v1_MMA444SUHBD1_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000002:
            return Opcode_v1_MMA444SUHD_registerAq_registerBq_registerC_registerD_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_0 >> 18) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MMA444USHBD0_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000001:
            return Opcode_v1_MMA444USHBD1_registerAq_registerBq_registerC_registerD_simple;
          case 0x00000002:
            return Opcode_v1_MMA444USHD_registerAq_registerBq_registerC_registerD_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 12) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_CONVDHV0_roundint_saturate_registerAl_registerBq_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_CONVDHV1_roundint_saturate_registerAh_registerBq_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 12) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_CONVWBV0_roundint_saturate_registerAx_registerBq_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_CONVWBV1_roundint_saturate_registerAy_registerBq_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_CONVWBV2_roundint_saturate_registerAz_registerBq_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_CONVWBV3_roundint_saturate_registerAt_registerBq_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FSCALEWV_xrounding_silent2_rectify_registerA_registerB_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 12) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_FNARROW44WH_xrounding_silent2_registerA_registerBp_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 23) & 0x0000000f) {
      case 0x0000000e:
        return Opcode_v1_LOOPDO_registerZ_pcrel17_simple;
      case 0x0000000f:
        switch ((codeWord_0 >> 18) & 0x0000001f) {
        case 0x00000000:
          return Opcode_v1_TLBREAD_simple;
        case 0x00000001:
          return Opcode_v1_TLBPROBE_simple;
        case 0x00000002:
          return Opcode_v1_TLBWRITE_simple;
        case 0x00000003:
          return Opcode_v1_TLBDINVAL_simple;
        case 0x00000004:
          return Opcode_v1_TLBIINVAL_simple;
        case 0x00000008:
          return Opcode_v1_AWAIT_simple;
        case 0x00000009:
          return Opcode_v1_SLEEP_simple;
        case 0x0000000a:
          return Opcode_v1_STOP_simple;
        case 0x0000000b:
          return Opcode_v1_BARRIER_simple;
        case 0x0000000c:
          return Opcode_v1_WAITIT_registerZ_simple;
        case 0x0000000d:
          return Opcode_v1_SYNCGROUP_registerZ_simple;
        case 0x0000000e:
          return Opcode_v1_WFXL_systemT2_registerZ_simple;
        case 0x0000000f:
          return Opcode_v1_WFXM_systemT2_registerZ_simple;
        case 0x00000010:
          return Opcode_v1_SET_systemT3_registerZ_simple;
        case 0x00000011:
          return Opcode_v1_GET_registerZ_systemS2_simple;
        case 0x00000012:
          return Opcode_v1_RSWAP_registerZ_systemS4_simple;
        case 0x00000013:
          return Opcode_v1_IGET_registerZ_simple;
        case 0x00000014:
          return Opcode_v1_RET_simple;
        case 0x00000015:
          return Opcode_v1_RFE_simple;
        case 0x00000016:
          return Opcode_v1_IGOTO_registerZ_simple;
        case 0x00000017:
          return Opcode_v1_ICALL_registerZ_simple;
        case 0x00000018:
          return Opcode_v1_SCALL_sysnumber_simple;
        case 0x00000019:
          return Opcode_v1_SCALL_registerZ_simple;
        default:
          return Opcode_v1_CB_branchcond_registerZ_pcrel17_simple;
          break;
        }
        return Opcode__UNDEF;
      default:
        return Opcode_v1_CB_branchcond_registerZ_pcrel17_simple;
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000001:
    switch ((codeWord_0 >> 27) & 0x00000001) {
    case 0x00000000:
      return Opcode_v1_GOTO_pcrel27_simple;
    case 0x00000001:
      return Opcode_v1_CALL_pcrel27_simple;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000002:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_LBZ_variant_registerW_signed10_registerZ_simple;
      case 0x00000001:
        return Opcode_v1_LBS_variant_registerW_signed10_registerZ_simple;
      case 0x00000002:
        return Opcode_v1_LHZ_variant_registerW_signed10_registerZ_simple;
      case 0x00000003:
        return Opcode_v1_LHS_variant_registerW_signed10_registerZ_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 25) & 0x00000007) {
      case 0x00000000:
        return Opcode_v1_LV_speculate_registerA_signed10_registerZ_simple;
      case 0x00000001:
        return Opcode_v1_LV_column_speculate_registerAq_signed10_registerZ_simple;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_SB_signed10_registerZ_registerT_simple;
        case 0x00000001:
          return Opcode_v1_SH_signed10_registerZ_registerT_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_SW_signed10_registerZ_registerT_simple;
        case 0x00000001:
          return Opcode_v1_SD_signed10_registerZ_registerT_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_SQ_signed10_registerZ_registerU_simple;
          case 0x00000001:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SO_signed10_registerZ_registerV_simple;
            case 0x00000001:
              switch ((codeWord_0 >> 20) & 0x0000000f) {
              case 0x00000000:
                return Opcode_v1_DZEROL_signed10_registerZ_simple;
              default:
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          return Opcode_v1_SV_signed10_registerZ_registerE_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_ALCLRW_registerW_signed10_registerZ_simple;
        case 0x00000001:
          return Opcode_v1_ALCLRD_registerW_signed10_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_ALADDW_signed10_registerZ_registerT_simple;
        case 0x00000001:
          return Opcode_v1_ALADDD_signed10_registerZ_registerT_simple;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LBZ_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v1_LBZ_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LBS_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v1_LBS_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LHZ_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v1_LHZ_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LHS_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v1_LHS_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 25) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LV_speculate_doscale_registerA_registerY_registerZ_simple;
        default:
          return Opcode_v1_LV_speculate_lsucond_registerY_registerA_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LV_column_speculate_doscale_registerAq_registerY_registerZ_simple;
        default:
          return Opcode_v1_LV_column_speculate_lsucond_registerY_registerAq_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_SB_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v1_SB_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_SH_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v1_SH_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_SW_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v1_SW_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_SD_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v1_SD_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 13) & 0x00000007) {
            case 0x00000007:
              return Opcode_v1_SQ_doscale_registerY_registerZ_registerU_simple;
            default:
              return Opcode_v1_SQ_lsucond_registerY_registerZ_registerU_simple;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 13) & 0x00000007) {
              case 0x00000007:
                return Opcode_v1_SO_doscale_registerY_registerZ_registerV_simple;
              default:
                return Opcode_v1_SO_lsucond_registerY_registerZ_registerV_simple;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000001:
              switch ((codeWord_0 >> 20) & 0x0000000f) {
              case 0x00000000:
                switch ((codeWord_0 >> 12) & 0x0000000f) {
                case 0x0000000e:
                  return Opcode_v1_DZEROL_registerY_registerZ_simple;
                default:
                  return Opcode_v1_DZEROL_lsucond_registerY_registerZ_simple;
                  break;
                }
                return Opcode__UNDEF;
              default:
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_SV_doscale_registerY_registerZ_registerE_simple;
          default:
            return Opcode_v1_SV_lsucond_registerY_registerZ_registerE_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_ALCLRW_doscale_registerW_registerY_registerZ_simple;
          default:
            return Opcode_v1_ALCLRW_lsucond_registerY_registerW_registerZ_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_ALCLRD_doscale_registerW_registerY_registerZ_simple;
          default:
            return Opcode_v1_ALCLRD_lsucond_registerY_registerW_registerZ_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_ALADDW_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v1_ALADDW_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_ALADDD_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v1_ALADDD_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000003:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_LWZ_variant_registerW_signed10_registerZ_simple;
      case 0x00000001:
        return Opcode_v1_LWS_variant_registerW_signed10_registerZ_simple;
      case 0x00000002:
        return Opcode_v1_LD_variant_registerW_signed10_registerZ_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_LQ_variant_registerM_signed10_registerZ_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 19) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_LO_variant_registerN_signed10_registerZ_simple;
          case 0x00000001:
            switch ((codeWord_0 >> 20) & 0x0000000f) {
            case 0x00000000:
              return Opcode_v1_DTOUCHL_signed10_registerZ_simple;
            case 0x00000001:
              return Opcode_v1_DINVALL_signed10_registerZ_simple;
            case 0x00000005:
              return Opcode_v1_IINVALS_signed10_registerZ_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x0000000e:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_ACSWAPW_signed10_registerZ_registerU_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 19) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_COPYO_registerN_registerR_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_ACSWAPD_signed10_registerZ_registerU_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 19) & 0x0000001f) {
          case 0x00000011:
            return Opcode_v1_DINVAL_simple;
          case 0x00000013:
            return Opcode_v1_IINVAL_simple;
          case 0x00000019:
            return Opcode_v1_FENCE_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LWZ_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v1_LWZ_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LWS_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v1_LWS_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v1_LD_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v1_LD_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_LQ_variant_doscale_registerM_registerY_registerZ_simple;
          default:
            return Opcode_v1_LQ_variant_lsucond_registerY_registerM_registerZ_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 19) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 13) & 0x00000007) {
            case 0x00000007:
              return Opcode_v1_LO_variant_doscale_registerN_registerY_registerZ_simple;
            default:
              return Opcode_v1_LO_variant_lsucond_registerY_registerN_registerZ_simple;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 20) & 0x0000000f) {
            case 0x00000000:
              switch ((codeWord_0 >> 12) & 0x0000000f) {
              case 0x0000000e:
                return Opcode_v1_DTOUCHL_registerY_registerZ_simple;
              default:
                return Opcode_v1_DTOUCHL_lsucond_registerY_registerZ_simple;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000001:
              switch ((codeWord_0 >> 12) & 0x0000000f) {
              case 0x0000000e:
                return Opcode_v1_DINVALL_registerY_registerZ_simple;
              default:
                return Opcode_v1_DINVALL_lsucond_registerY_registerZ_simple;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000005:
              switch ((codeWord_0 >> 12) & 0x0000000f) {
              case 0x0000000e:
                return Opcode_v1_IINVALS_registerY_registerZ_simple;
              default:
                return Opcode_v1_IINVALS_lsucond_registerY_registerZ_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x0000000e:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_ACSWAPW_doscale_registerY_registerZ_registerU_simple;
          default:
            return Opcode_v1_ACSWAPW_lsucond_registerY_registerZ_registerU_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v1_ACSWAPD_doscale_registerY_registerZ_registerU_simple;
          default:
            return Opcode_v1_ACSWAPD_lsucond_registerY_registerZ_registerU_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000004:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_MADDD_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FFMAD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FADDD_registerW_registerZ_signed10_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FFMAWDP_registerM_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v1_FMULWDP_registerM_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_MADDWP_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FFMAWD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FADDWP_registerW_registerZ_signed10_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FFMAHWQ_registerM_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v1_FMULHWQ_registerM_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_MADDHQ_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FFMAWP_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FADDHQ_registerW_registerZ_signed10_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FFMSWDP_registerM_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v1_FMULWDC_registerM_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000001:
        return Opcode_v1_FFMAHQ_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FADDCWC_registerW_registerZ_signed10_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FFMSHWQ_registerM_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v1_FMULCWDC_registerM_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_MULD_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FFMSD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FSBFD_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FLOATD_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000005:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_MULWP_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FFMSWD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FSBFWP_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FLOATUD_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_MULHQ_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FFMSWP_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FSBFHQ_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FIXEDD_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000007:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_MULWC_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FFMSHQ_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FSBFCWC_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FIXEDUD_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000008:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_MADDDT_registerM_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v1_MULDT_registerM_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FFMAHW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FLOATW_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000009:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_MADDUDT_registerM_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v1_MULUDT_registerM_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULWD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FFMAW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FLOATUW_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000a:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_MADDSUDT_registerM_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v1_MULSUDT_registerM_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULWP_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FFMSHW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FIXEDW_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000b:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_MADDUZDT_registerM_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v1_CMULDT_registerM_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULHQ_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FFMSW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FIXEDUW_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000c:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_DOT2WD_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FDOT2W_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FADDW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FLOATWP_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000d:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_DOT2UWD_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FDOT2WD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FSBFW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FLOATUWP_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000e:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_DOT2SUWD_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FMULWC_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FMULW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FIXEDWP_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000f:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_DOT2W_registerW_registerZ_signed10_simple;
      case 0x00000001:
        return Opcode_v1_FMULCWC_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_FMULHW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_FIXEDUWP_rounding_silent_registerW_registerZ_unsigned6_simple;
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000005:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MADDD_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_MULWDP_registerM_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_DOT2WDP_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v1_MADDWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MADDHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FFMAD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FADDD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FFMAWDP_rounding_silent_registerM_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_FMULWDP_rounding_silent_registerM_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MADDWP_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_MULUWDP_registerM_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_DOT2UWDP_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v1_MADDUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MADDUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FFMAWD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FADDWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FFMAHWQ_rounding_silent_registerM_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_FMULHWQ_rounding_silent_registerM_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MADDHQ_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_MULSUWDP_registerM_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_DOT2SUWDP_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v1_MADDSUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MADDSUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FFMAWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FADDHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FFMSWDP_rounding_silent_registerM_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_FMULWDC_rounding_silent_registerM_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000001:
          return Opcode_v1_MM212W_registerM_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_DOT2WZP_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v1_MADDW_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FFMAHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FADDCWC_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FFMSHWQ_rounding_silent_registerM_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_FMULCWDC_rounding_silent_registerM_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MSBFD_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_MULD_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_MULWQ_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v1_MSBFWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MSBFHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FFMSD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FSBFD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FMMA212W_rounding_silent_registerM_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_FMM212W_rounding_silent_registerM_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000005:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MSBFWP_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_MULWP_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_MULCWC_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MSBFUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MSBFUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FFMSWD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FSBFWP_rounding_silent_registerW_registerZ_registerY_simple;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MSBFHQ_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_MULHQ_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_MULWDC_registerM_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MSBFSUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MSBFSUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FFMSWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FSBFHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FMMS212W_rounding_silent_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000007:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000001:
          return Opcode_v1_MULWC_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_MULCWDC_registerM_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MSBFW_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FFMSHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FSBFCWC_rounding_silent_registerW_registerZ_registerY_simple;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000008:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MADDWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MADDDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v1_MULDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v1_CRCBELMW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MULWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MULHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FFMAHW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000001:
          return Opcode_v1_FADDWQ_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000009:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MADDUWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MADDUDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v1_MULUDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v1_CRCBELLW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MULUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MULUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULWD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FFMAW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000001:
          return Opcode_v1_FADDCWCP_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000a:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MADDSUWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MADDSUDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v1_MULSUDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v1_CRCLELMW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MULSUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_MULSUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FFMSHW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000001:
          return Opcode_v1_FSBFWQ_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000b:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MMA212W_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MADDUZDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v1_CMULDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v1_CRCLELLW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MULW_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FFMSW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000001:
          return Opcode_v1_FSBFCWCP_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000c:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MSBFWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MSBFDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v1_CMULXDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v1_DOT2WD_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FDOT2W_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FADDW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FADDDP_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v1_FDOT2WDP_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000d:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MSBFUWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MSBFUDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v1_CMULGLXDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v1_DOT2UWD_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FDOT2WD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FSBFW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FADDCDC_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v1_FDOT2WZP_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000e:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MSBFSUWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MSBFSUDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v1_CMULGMXDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v1_DOT2SUWD_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULWC_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FMULW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FSBFDP_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v1_FMULWQ_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000f:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_MMS212W_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v1_MSBFUZDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v1_CMULGHXDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v1_DOT2W_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_COPYQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v1_FMULCWC_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v1_FMULHW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FSBFCDC_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000006:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        return Opcode_v1_MAKE_registerW_signed16_simple;
      case 0x00000001:
        return Opcode_v1_ADDD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_MIND_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_MAXD_registerW_registerZ_signed10_simple;
      case 0x00000004:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v1_ABSD_registerW_registerZ_simple;
        default:
          return Opcode_v1_ABDD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v1_NEGD_registerW_registerZ_simple;
        default:
          return Opcode_v1_SBFD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        return Opcode_v1_MINUD_registerW_registerZ_signed10_simple;
      case 0x00000007:
        return Opcode_v1_MAXUD_registerW_registerZ_signed10_simple;
      case 0x00000008:
        return Opcode_v1_ANDD_registerW_registerZ_signed10_simple;
      case 0x00000009:
        return Opcode_v1_NANDD_registerW_registerZ_signed10_simple;
      case 0x0000000a:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v1_COPYD_registerW_registerZ_simple;
        default:
          return Opcode_v1_ORD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        return Opcode_v1_NORD_registerW_registerZ_signed10_simple;
      case 0x0000000c:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000003ff:
          return Opcode_v1_NOTD_registerW_registerZ_simple;
        default:
          return Opcode_v1_XORD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        return Opcode_v1_NXORD_registerW_registerZ_signed10_simple;
      case 0x0000000e:
        return Opcode_v1_ANDND_registerW_registerZ_signed10_simple;
      case 0x0000000f:
        return Opcode_v1_ORND_registerW_registerZ_signed10_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x0000000e:
        return Opcode_v1_ADDSD_registerW_registerZ_signed10_simple;
      case 0x0000000f:
        return Opcode_v1_SBFSD_registerW_registerZ_signed10_simple;
      default:
        return Opcode_v1_COMPD_comparison_registerW_registerZ_signed10_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x0000000e:
        return Opcode_v1_SBMM8_registerW_registerZ_signed10_simple;
      case 0x0000000f:
        return Opcode_v1_SBMMT8_registerW_registerZ_signed10_simple;
      default:
        return Opcode_v1_CMOVED_scalarcond_registerZ_registerW_signed10_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        return Opcode_v1_INSF_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
      case 0x00000001:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000003c0:
          switch ((codeWord_0 >> 24) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ZXHD_registerW_registerZ_simple;
          default:
            return Opcode_v1_EXTFZ_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          return Opcode_v1_EXTFZ_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000001c0:
          switch ((codeWord_0 >> 24) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SXBD_registerW_registerZ_simple;
          default:
            return Opcode_v1_EXTFS_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x000003c0:
          switch ((codeWord_0 >> 24) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SXHD_registerW_registerZ_simple;
          case 0x00000001:
            return Opcode_v1_SXWD_registerW_registerZ_simple;
          default:
            return Opcode_v1_EXTFS_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          return Opcode_v1_EXTFS_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        return Opcode_v1_CLRF_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000007:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        return Opcode_v1_PCREL_registerW_signed16_simple;
      case 0x00000001:
        return Opcode_v1_ADDW_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v1_MINW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v1_MAXW_registerW_registerZ_signed10_simple;
      case 0x00000004:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v1_ABSW_registerW_registerZ_simple;
        default:
          return Opcode_v1_ABDW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v1_NEGW_registerW_registerZ_simple;
        default:
          return Opcode_v1_SBFW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        return Opcode_v1_MINUW_registerW_registerZ_signed10_simple;
      case 0x00000007:
        return Opcode_v1_MAXUW_registerW_registerZ_signed10_simple;
      case 0x00000008:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000000ff:
          return Opcode_v1_ZXBD_registerW_registerZ_simple;
        case 0x000003ff:
          return Opcode_v1_ZXWD_registerW_registerZ_simple;
        default:
          return Opcode_v1_ANDW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        return Opcode_v1_NANDW_registerW_registerZ_signed10_simple;
      case 0x0000000a:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v1_COPYW_registerW_registerZ_simple;
        default:
          return Opcode_v1_ORW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        return Opcode_v1_NORW_registerW_registerZ_signed10_simple;
      case 0x0000000c:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000003ff:
          return Opcode_v1_NOTW_registerW_registerZ_simple;
        default:
          return Opcode_v1_XORW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        return Opcode_v1_NXORW_registerW_registerZ_signed10_simple;
      case 0x0000000e:
        return Opcode_v1_ANDNW_registerW_registerZ_signed10_simple;
      case 0x0000000f:
        return Opcode_v1_ORNW_registerW_registerZ_signed10_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 12) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_STSUD_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_ADDD_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_MIND_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MAXD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ABDD_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFD_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_MINUD_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_MAXUD_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v1_ANDD_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_NANDD_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_ORD_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_NORD_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_XORD_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_NXORD_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_ANDND_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_ORND_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_STSUW_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_ADDW_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_MINW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MAXW_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ABDW_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFW_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_MINUW_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_MAXUW_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v1_ANDW_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_NANDW_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_ORW_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_NORW_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_XORW_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_NXORW_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_ANDNW_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_ORNW_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000001:
          return Opcode_v1_ADDWP_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_MINWP_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MAXWP_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ABDWP_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFWP_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_MINUWP_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_MAXUWP_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_ADDCWC_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_SBFCWC_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000001:
          return Opcode_v1_ADDHQ_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_MINHQ_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_MAXHQ_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ABDHQ_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFHQ_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_MINUHQ_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_MAXUHQ_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_ADDCHCP_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_SBFCHCP_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_ADDX2D_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_SBFX2D_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_ADDX4D_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_SBFX4D_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ADDX8D_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFX8D_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_ADDX16D_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_SBFX16D_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v1_ADDWD_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_SBFWD_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_ADDUWD_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_SBFUWD_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_LANDD_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_LNANDD_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_LORD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_LNORD_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_ADDX2W_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_SBFX2W_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_ADDX4W_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_SBFX4W_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ADDX8W_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFX8W_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_ADDX16W_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_SBFX16W_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v1_AVGW_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_AVGUW_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_AVGRW_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_AVGRUW_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_LANDW_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_LNANDW_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_LORW_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_LNORW_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_ADDX2WP_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_SBFX2WP_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_ADDX4WP_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_SBFX4WP_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ADDX8WP_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFX8WP_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_ADDX16WP_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_SBFX16WP_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v1_AVGWP_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_AVGUWP_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_AVGRWP_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_AVGRUWP_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_LANDWP_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_LNANDWP_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_LORWP_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_LNORWP_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_ADDX2HQ_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_SBFX2HQ_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_ADDX4HQ_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_SBFX4HQ_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ADDX8HQ_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFX8HQ_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_ADDX16HQ_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_SBFX16HQ_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v1_AVGHQ_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_AVGUHQ_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_AVGRHQ_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_AVGRUHQ_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_LANDHQ_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_LNANDHQ_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_LORHQ_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_LNORHQ_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_ADDX2WD_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_SBFX2WD_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_ADDX4WD_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_SBFX4WD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_ADDX8WD_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_SBFX8WD_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_ADDX16WD_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_SBFX16WD_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v1_ADDX2UWD_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_SBFX2UWD_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_ADDX4UWD_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_SBFX4UWD_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_ADDX8UWD_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v1_SBFX8UWD_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_ADDX16UWD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_SBFX16UWD_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_ADDSD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_SBFSD_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v1_COMPD_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_ADDSW_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_SBFSW_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v1_COMPW_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000c:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_ADDCD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_SBFCD_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v1_COMPWD_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_ADDCD_I_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_SBFCD_I_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v1_COMPUWD_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_ADDSWP_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_SBFSWP_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v1_COMPNWP_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_ADDSHQ_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_SBFSHQ_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v1_COMPNHQ_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 12) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v1_SBMM8_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_SBMMT8_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v1_CMOVED_scalarcond_registerZ_registerW_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 27) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_CMOVEWP_simdcond_registerZ_registerW_registerY_simple;
        case 0x00000001:
          return Opcode_v1_CMOVEHQ_simdcond_registerZ_registerW_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_CLZD_registerW_registerZ_simple;
        case 0x00000001:
          return Opcode_v1_CLSD_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v1_CBSD_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v1_CTZD_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v1_SRSD_registerW_registerZ_unsigned6_simple;
        case 0x00000009:
          return Opcode_v1_SLLD_registerW_registerZ_unsigned6_simple;
        case 0x0000000a:
          return Opcode_v1_SRAD_registerW_registerZ_unsigned6_simple;
        case 0x0000000b:
          return Opcode_v1_SRLD_registerW_registerZ_unsigned6_simple;
        case 0x0000000c:
          return Opcode_v1_SLSD_registerW_registerZ_unsigned6_simple;
        case 0x0000000e:
          switch ((codeWord_0 >> 6) & 0x0000003f) {
          case 0x00000010:
            return Opcode_v1_SATDH_registerW_registerZ_simple;
          case 0x00000020:
            return Opcode_v1_SATDW_registerW_registerZ_simple;
          default:
            return Opcode_v1_SATD_registerW_registerZ_unsigned6_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_CLZW_registerW_registerZ_simple;
        case 0x00000001:
          return Opcode_v1_CLSW_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v1_CBSW_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v1_CTZW_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v1_SRSW_registerW_registerZ_unsigned6_simple;
        case 0x00000009:
          return Opcode_v1_SLLW_registerW_registerZ_unsigned6_simple;
        case 0x0000000a:
          return Opcode_v1_SRAW_registerW_registerZ_unsigned6_simple;
        case 0x0000000b:
          return Opcode_v1_SRLW_registerW_registerZ_unsigned6_simple;
        case 0x0000000c:
          return Opcode_v1_SLSW_registerW_registerZ_unsigned6_simple;
        case 0x0000000e:
          return Opcode_v1_ROLW_registerW_registerZ_unsigned6_simple;
        case 0x0000000f:
          return Opcode_v1_RORW_registerW_registerZ_unsigned6_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_CLZWP_registerW_registerZ_simple;
        case 0x00000001:
          return Opcode_v1_CLSWP_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v1_CBSWP_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v1_CTZWP_registerW_registerZ_simple;
        case 0x00000006:
          return Opcode_v1_SXLHWP_registerW_registerZ_simple;
        case 0x00000007:
          return Opcode_v1_SXMHWP_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v1_SRSWPS_registerW_registerZ_unsigned6_simple;
        case 0x00000009:
          return Opcode_v1_SLLWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000a:
          return Opcode_v1_SRAWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000b:
          return Opcode_v1_SRLWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000c:
          return Opcode_v1_SLSWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000e:
          return Opcode_v1_ROLWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000f:
          return Opcode_v1_RORWPS_registerW_registerZ_unsigned6_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000006:
          return Opcode_v1_SXLBHQ_registerW_registerZ_simple;
        case 0x00000007:
          return Opcode_v1_SXMBHQ_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v1_SRSHQS_registerW_registerZ_unsigned6_simple;
        case 0x00000009:
          return Opcode_v1_SLLHQS_registerW_registerZ_unsigned6_simple;
        case 0x0000000a:
          return Opcode_v1_SRAHQS_registerW_registerZ_unsigned6_simple;
        case 0x0000000b:
          return Opcode_v1_SRLHQS_registerW_registerZ_unsigned6_simple;
        case 0x0000000c:
          return Opcode_v1_SLSHQS_registerW_registerZ_unsigned6_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v1_SRSD_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_SLLD_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_SRAD_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_SRLD_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_SLSD_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_SATD_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v1_SRSW_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_SLLW_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_SRAW_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_SRLW_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_SLSW_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_ROLW_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_RORW_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v1_SRSWPS_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_SLLWPS_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_SRAWPS_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_SRLWPS_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_SLSWPS_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v1_ROLWPS_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v1_RORWPS_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v1_SRSHQS_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v1_SLLHQS_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v1_SRAHQS_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v1_SRLHQS_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v1_SLSHQS_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000f:
          return Opcode_v1_MOVETQ_registerAl_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000f:
          return Opcode_v1_MOVETQ_registerAh_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 12) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_0 >> 27) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FCOMPW_floatcomp_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_FCOMPD_floatcomp_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 27) & 0x00000001) {
        case 0x00000000:
          return Opcode_v1_FCOMPNWP_floatcomp_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_FCOMPNHQ_floatcomp_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_FNEGD_registerW_registerZ_simple;
        case 0x00000001:
          return Opcode_v1_FABSD_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v1_FNEGW_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v1_FABSW_registerW_registerZ_simple;
        case 0x00000004:
          return Opcode_v1_FNEGWP_registerW_registerZ_simple;
        case 0x00000005:
          return Opcode_v1_FABSWP_registerW_registerZ_simple;
        case 0x00000006:
          return Opcode_v1_FNEGHQ_registerW_registerZ_simple;
        case 0x00000007:
          return Opcode_v1_FABSHQ_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v1_FSRSRD_registerW_registerZ_simple;
        case 0x0000000a:
          return Opcode_v1_FSRSRW_registerW_registerZ_simple;
        case 0x0000000c:
          return Opcode_v1_FSRSRWP_registerW_registerZ_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v1_FWIDENLWD_silent2_registerW_registerZ_simple;
        case 0x00000009:
          return Opcode_v1_FWIDENMWD_silent2_registerW_registerZ_simple;
        case 0x0000000a:
          return Opcode_v1_FWIDENLHW_silent2_registerW_registerZ_simple;
        case 0x0000000b:
          return Opcode_v1_FWIDENMHW_silent2_registerW_registerZ_simple;
        case 0x0000000c:
          return Opcode_v1_FWIDENLHWP_silent2_registerW_registerZ_simple;
        case 0x0000000d:
          return Opcode_v1_FWIDENMHWP_silent2_registerW_registerZ_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_FSRECD_silent2_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v1_FSRECW_silent2_registerW_registerZ_simple;
        case 0x00000004:
          return Opcode_v1_FSRECWP_silent2_registerW_registerZ_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_FSDIVD_silent2_registerW_registerP_simple;
        case 0x00000001:
          return Opcode_v1_FCDIVD_silent2_registerW_registerP_simple;
        case 0x00000002:
          return Opcode_v1_FSDIVW_silent2_registerW_registerP_simple;
        case 0x00000003:
          return Opcode_v1_FCDIVW_silent2_registerW_registerP_simple;
        case 0x00000004:
          return Opcode_v1_FSDIVWP_silent2_registerW_registerP_simple;
        case 0x00000005:
          return Opcode_v1_FCDIVWP_silent2_registerW_registerP_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000002:
          return Opcode_v1_FRECW_rounding2_silent2_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v1_FRSRW_rounding2_silent2_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v1_FNARROWDW_rounding2_silent2_registerW_registerZ_simple;
        case 0x0000000a:
          return Opcode_v1_FNARROWWH_rounding2_silent2_registerW_registerZ_simple;
        case 0x0000000c:
          return Opcode_v1_FNARROWDWP_rounding2_silent2_registerW_registerP_simple;
        case 0x0000000e:
          return Opcode_v1_FNARROWWHQ_rounding2_silent2_registerW_registerP_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v1_FMIND_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v1_FMAXD_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v1_FMINW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v1_FMAXW_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v1_FMINWP_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v1_FMAXWP_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v1_FMINHQ_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v1_FMAXHQ_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000f:
          return Opcode_v1_NOP_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
    break;
  }
  return Opcode__UNDEF;
}
#endif//NDECODE

#ifdef $XCC__h
#ifndef NDECODE
extern Opcode
Decode_Decoding_v1_triple(const void *buffer);
#endif//NDECODE
#endif//$XCC__h

#ifndef NDECODE
Opcode
Decode_Decoding_v1_triple(const void *buffer)
{
  const uint32_t *codeWords = (const uint32_t *)buffer;
  uint32_t codeWord_0 = codeWords[0];
  uint32_t codeWord_1 = codeWords[1];
  uint32_t codeWord_2 = codeWords[2];
  switch ((codeWord_0 >> 28) & 0x0000000f) {
  case 0x0000000a:
    switch ((codeWord_1 >> 29) & 0x00000007) {
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LBZ_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LBS_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LHZ_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LHS_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 25) & 0x00000007) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LV_speculate_registerA_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LV_column_speculate_registerAq_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SB_extend27_upper27_lower10_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v1_SH_extend27_upper27_lower10_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SW_extend27_upper27_lower10_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v1_SD_extend27_upper27_lower10_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 18) & 0x00000001) {
              case 0x00000000:
                return Opcode_v1_SQ_extend27_upper27_lower10_registerZ_registerU_triple;
              case 0x00000001:
                switch ((codeWord_0 >> 19) & 0x00000001) {
                case 0x00000000:
                  return Opcode_v1_SO_extend27_upper27_lower10_registerZ_registerV_triple;
                case 0x00000001:
                  switch ((codeWord_0 >> 20) & 0x0000000f) {
                  case 0x00000000:
                    return Opcode_v1_DZEROL_extend27_upper27_lower10_registerZ_triple;
                  default:
                    break;
                  }
                  return Opcode__UNDEF;
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000001:
              return Opcode_v1_SV_extend27_upper27_lower10_registerZ_registerE_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ALCLRW_registerW_extend27_upper27_lower10_registerZ_triple;
            case 0x00000001:
              return Opcode_v1_ALCLRD_registerW_extend27_upper27_lower10_registerZ_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ALADDW_extend27_upper27_lower10_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v1_ALADDD_extend27_upper27_lower10_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LBZ_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LBS_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LHZ_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LHS_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 25) & 0x00000007) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LV_speculate_lsucond_registerY_registerA_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LV_column_speculate_lsucond_registerY_registerAq_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SB_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v1_SH_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_SW_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v1_SD_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 18) & 0x00000001) {
              case 0x00000000:
                return Opcode_v1_SQ_lsucond_registerY_extend27_offset27_registerZ_registerU_triple;
              case 0x00000001:
                switch ((codeWord_0 >> 19) & 0x00000001) {
                case 0x00000000:
                  return Opcode_v1_SO_lsucond_registerY_extend27_offset27_registerZ_registerV_triple;
                case 0x00000001:
                  switch ((codeWord_0 >> 20) & 0x0000000f) {
                  case 0x00000000:
                    return Opcode_v1_DZEROL_lsucond_registerY_extend27_offset27_registerZ_triple;
                  default:
                    break;
                  }
                  return Opcode__UNDEF;
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000001:
              return Opcode_v1_SV_lsucond_registerY_extend27_offset27_registerZ_registerE_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ALCLRW_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
            case 0x00000001:
              return Opcode_v1_ALCLRD_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ALADDW_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v1_ALADDD_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000b:
    switch ((codeWord_1 >> 29) & 0x00000007) {
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LWZ_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LWS_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LD_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_LQ_variant_registerM_extend27_upper27_lower10_registerZ_triple;
            case 0x00000001:
              switch ((codeWord_0 >> 19) & 0x00000001) {
              case 0x00000000:
                return Opcode_v1_LO_variant_registerN_extend27_upper27_lower10_registerZ_triple;
              case 0x00000001:
                switch ((codeWord_0 >> 20) & 0x0000000f) {
                case 0x00000000:
                  return Opcode_v1_DTOUCHL_extend27_upper27_lower10_registerZ_triple;
                case 0x00000001:
                  return Opcode_v1_DINVALL_extend27_upper27_lower10_registerZ_triple;
                case 0x00000005:
                  return Opcode_v1_IINVALS_extend27_upper27_lower10_registerZ_triple;
                default:
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ACSWAPW_extend27_upper27_lower10_registerZ_registerU_triple;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ACSWAPD_extend27_upper27_lower10_registerZ_registerU_triple;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LWZ_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LWS_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_LD_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_LQ_variant_lsucond_registerY_registerM_extend27_offset27_registerZ_triple;
            case 0x00000001:
              switch ((codeWord_0 >> 19) & 0x00000001) {
              case 0x00000000:
                return Opcode_v1_LO_variant_lsucond_registerY_registerN_extend27_offset27_registerZ_triple;
              case 0x00000001:
                switch ((codeWord_0 >> 20) & 0x0000000f) {
                case 0x00000000:
                  return Opcode_v1_DTOUCHL_lsucond_registerY_extend27_offset27_registerZ_triple;
                case 0x00000001:
                  return Opcode_v1_DINVALL_lsucond_registerY_extend27_offset27_registerZ_triple;
                case 0x00000005:
                  return Opcode_v1_IINVALS_lsucond_registerY_extend27_offset27_registerZ_triple;
                default:
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ACSWAPW_lsucond_registerY_extend27_offset27_registerZ_registerU_triple;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_ACSWAPD_lsucond_registerY_extend27_offset27_registerZ_registerU_triple;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000c:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MADDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMAD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FADDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_FFMAWDP_registerM_registerZ_extend27_upper27_lower10_triple;
            case 0x00000001:
              return Opcode_v1_FMULWDP_registerM_registerZ_extend27_upper27_lower10_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MADDWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMAWD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FADDWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_FFMAHWQ_registerM_registerZ_extend27_upper27_lower10_triple;
            case 0x00000001:
              return Opcode_v1_FMULHWQ_registerM_registerZ_extend27_upper27_lower10_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MADDHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMAWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FADDHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_FFMSWDP_registerM_registerZ_extend27_upper27_lower10_triple;
            case 0x00000001:
              return Opcode_v1_FMULWDC_registerM_registerZ_extend27_upper27_lower10_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMAHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FADDCWC_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_FFMSHWQ_registerM_registerZ_extend27_upper27_lower10_triple;
            case 0x00000001:
              return Opcode_v1_FMULCWDC_registerM_registerZ_extend27_upper27_lower10_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000004:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MULD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMSD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FSBFD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000005:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MULWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMSWD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FSBFWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MULHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMSWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FSBFHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000007:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MULWC_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMSHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FSBFCWC_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000008:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_MADDDT_registerM_registerZ_extend27_upper27_lower10_triple;
            case 0x00000001:
              return Opcode_v1_MULDT_registerM_registerZ_extend27_upper27_lower10_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FMULD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMAHW_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000009:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_MADDUDT_registerM_registerZ_extend27_upper27_lower10_triple;
            case 0x00000001:
              return Opcode_v1_MULUDT_registerM_registerZ_extend27_upper27_lower10_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FMULWD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMAW_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000a:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_MADDSUDT_registerM_registerZ_extend27_upper27_lower10_triple;
            case 0x00000001:
              return Opcode_v1_MULSUDT_registerM_registerZ_extend27_upper27_lower10_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FMULWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMSHW_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000b:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v1_MADDUZDT_registerM_registerZ_extend27_upper27_lower10_triple;
            case 0x00000001:
              return Opcode_v1_CMULDT_registerM_registerZ_extend27_upper27_lower10_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FMULHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FFMSW_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000c:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_DOT2WD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FDOT2W_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FADDW_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000d:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_DOT2UWD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FDOT2WD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FSBFW_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000e:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_DOT2SUWD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FMULWC_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FMULW_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000f:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_DOT2W_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FMULCWC_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_FMULHW_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000e:
    switch ((codeWord_1 >> 29) & 0x00000007) {
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MAKE_registerW_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ADDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MIND_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MAXD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ABDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_SBFD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MINUD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_MAXUD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ANDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_NANDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ORD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_NORD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_XORD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_NXORD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ANDND_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_ORND_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_2 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x0000000f) {
          case 0x0000000e:
            return Opcode_v1_ADDSD_registerW_registerZ_extend27_upper27_lower10_triple;
          case 0x0000000f:
            return Opcode_v1_SBFSD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            return Opcode_v1_COMPD_comparison_registerW_registerZ_extend27_upper27_lower10_triple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_2 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x0000000f) {
          case 0x0000000e:
            return Opcode_v1_SBMM8_registerW_registerZ_extend27_upper27_lower10_triple;
          case 0x0000000f:
            return Opcode_v1_SBMMT8_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            return Opcode_v1_CMOVED_scalarcond_registerZ_registerW_extend27_upper27_lower10_triple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000f:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v1_PCREL_registerW_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  default:
    break;
  }
  return Opcode__UNDEF;
}
#endif//NDECODE

#ifdef $XCC__h
#ifndef NDECODE
extern Opcode
Decode_Decoding_v2_double(const void *buffer);
#endif//NDECODE
#endif//$XCC__h

#ifndef NDECODE
Opcode
Decode_Decoding_v2_double(const void *buffer)
{
  const uint32_t *codeWords = (const uint32_t *)buffer;
  uint32_t codeWord_0 = codeWords[0];
  uint32_t codeWord_1 = codeWords[1];
  switch ((codeWord_0 >> 28) & 0x0000000f) {
  case 0x0000000a:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LBZ_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LBS_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LHZ_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LHS_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 25) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LV_speculate_registerA_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LV_column_speculate_registerAq_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_SB_upper27_lower10_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v2_SH_upper27_lower10_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_SW_upper27_lower10_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v2_SD_upper27_lower10_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SQ_upper27_lower10_registerZ_registerU_double;
            case 0x00000001:
              switch ((codeWord_0 >> 19) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_SO_upper27_lower10_registerZ_registerV_double;
              default:
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            return Opcode_v2_SV_upper27_lower10_registerZ_registerE_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_ALCLRW_registerW_upper27_lower10_registerZ_double;
          case 0x00000001:
            return Opcode_v2_ALCLRD_registerW_upper27_lower10_registerZ_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_ALADDW_upper27_lower10_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v2_ALADDD_upper27_lower10_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LBZ_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LBS_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LHZ_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LHS_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 25) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LV_speculate_lsucond_registerY_registerA_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LV_column_speculate_lsucond_registerY_registerAq_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_SB_lsucond_registerY_offset27_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v2_SH_lsucond_registerY_offset27_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_SW_lsucond_registerY_offset27_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v2_SD_lsucond_registerY_offset27_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SQ_lsucond_registerY_offset27_registerZ_registerU_double;
            case 0x00000001:
              switch ((codeWord_0 >> 19) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_SO_lsucond_registerY_offset27_registerZ_registerV_double;
              default:
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            return Opcode_v2_SV_lsucond_registerY_offset27_registerZ_registerE_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_ALCLRW_lsucond_registerY_registerW_offset27_registerZ_double;
          case 0x00000001:
            return Opcode_v2_ALCLRD_lsucond_registerY_registerW_offset27_registerZ_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_ALADDW_lsucond_registerY_offset27_registerZ_registerT_double;
          case 0x00000001:
            return Opcode_v2_ALADDD_lsucond_registerY_offset27_registerZ_registerT_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000b:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LWZ_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LWS_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LD_variant_registerW_upper27_lower10_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_LQ_variant_registerM_upper27_lower10_registerZ_double;
          case 0x00000001:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_LO_variant_registerN_upper27_lower10_registerZ_double;
            case 0x00000001:
              switch ((codeWord_0 >> 20) & 0x0000000f) {
              case 0x00000000:
                return Opcode_v2_DTOUCHL_cachelev_upper27_lower10_registerZ_double;
              case 0x00000001:
                return Opcode_v2_DINVALL_cachelev_upper27_lower10_registerZ_double;
              case 0x00000005:
                return Opcode_v2_IINVALS_cachelev_upper27_lower10_registerZ_double;
              default:
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ACSWAPW_boolcas_upper27_lower10_registerZ_registerU_double;
            case 0x00000001:
              return Opcode_v2_ACSWAPD_boolcas_upper27_lower10_registerZ_registerU_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LWZ_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LWS_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_LD_variant_lsucond_registerY_registerW_offset27_registerZ_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_LQ_variant_lsucond_registerY_registerM_offset27_registerZ_double;
          case 0x00000001:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_LO_variant_lsucond_registerY_registerN_offset27_registerZ_double;
            case 0x00000001:
              switch ((codeWord_0 >> 20) & 0x0000000f) {
              case 0x00000000:
                return Opcode_v2_DTOUCHL_cachelev_lsucond_registerY_offset27_registerZ_double;
              case 0x00000001:
                return Opcode_v2_DINVALL_cachelev_lsucond_registerY_offset27_registerZ_double;
              case 0x00000005:
                return Opcode_v2_IINVALS_cachelev_lsucond_registerY_offset27_registerZ_double;
              default:
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ACSWAPW_boolcas_lsucond_registerY_offset27_registerZ_registerU_double;
            case 0x00000001:
              return Opcode_v2_ACSWAPD_boolcas_lsucond_registerY_offset27_registerZ_registerU_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000c:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000005:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULWP_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULHQ_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000007:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULWC_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000008:
      switch ((codeWord_0 >> 16) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDDT_registerM_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULDT_registerM_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000009:
      switch ((codeWord_0 >> 16) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDUDT_registerM_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULUDT_registerM_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000a:
      switch ((codeWord_0 >> 16) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDSUDT_registerM_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULSUDT_registerM_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000b:
      switch ((codeWord_0 >> 16) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDUZDT_registerM_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_CMULDT_registerM_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000c:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_DOT2WD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000d:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_DOT2UWD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000e:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_DOT2SUWD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000f:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_DOT2W_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000d:
    switch ((codeWord_0 >> 11) & 0x0000007f) {
    case 0x00000004:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000008:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_CRCBELMW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_CRCBELLW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_CRCLELMW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_CRCLELLW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDSUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MADDW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MSBFWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MSBFUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MSBFSUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MSBFW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULSUWD_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MULW_registerW_registerZ_upper27_lower5_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000e:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MAKE_registerW_extend6_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ADDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MIND_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MAXD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ABDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_SBFD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MINUD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MAXUD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ANDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_NANDD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ORD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_NORD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000c:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_XORD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_NXORD_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ANDND_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ORND_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_1 >> 29) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_ADDSD_registerW_registerZ_upper27_lower10_double;
        case 0x0000000f:
          return Opcode_v2_SBFSD_registerW_registerZ_upper27_lower10_double;
        default:
          return Opcode_v2_COMPD_comparison_registerW_registerZ_upper27_lower10_double;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_1 >> 29) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_SBMM8_registerW_registerZ_upper27_lower10_double;
        case 0x0000000f:
          return Opcode_v2_SBMMT8_registerW_registerZ_upper27_lower10_double;
        default:
          return Opcode_v2_CMOVED_scalarcond_registerZ_registerW_upper27_lower10_double;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000f:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_PCREL_registerW_extend6_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ADDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MINW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MAXW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ABDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_SBFW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MINUW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_MAXUW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ANDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_NANDW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ORW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_NORW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000c:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_XORW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_NXORW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ANDNW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          return Opcode_v2_ORNW_registerW_registerZ_upper27_lower10_double;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 12) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_ANDD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_NANDD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_ORD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_NORD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_XORD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_NXORD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_ANDND_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_ORND_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MINWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MAXWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_1 >> 0) & 0x07ffffff) {
            case 0x00000000:
              switch ((codeWord_0 >> 6) & 0x0000003f) {
              case 0x00000000:
                return Opcode_v2_ABSWP_registerW_registerZ_double;
              default:
                return Opcode_v2_ABDWP_splat32_registerW_registerZ_upper27_lower5_double;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v2_ABDWP_splat32_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_1 >> 0) & 0x07ffffff) {
            case 0x00000000:
              switch ((codeWord_0 >> 6) & 0x0000003f) {
              case 0x00000000:
                return Opcode_v2_NEGWP_registerW_registerZ_double;
              default:
                return Opcode_v2_SBFWP_splat32_registerW_registerZ_upper27_lower5_double;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v2_SBFWP_splat32_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MINUWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MAXUWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDCWC_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFCWC_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MINHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MAXHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_1 >> 0) & 0x07ffffff) {
            case 0x00000000:
              switch ((codeWord_0 >> 6) & 0x0000003f) {
              case 0x00000000:
                return Opcode_v2_ABSHQ_registerW_registerZ_double;
              default:
                return Opcode_v2_ABDHQ_splat32_registerW_registerZ_upper27_lower5_double;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v2_ABDHQ_splat32_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_1 >> 0) & 0x07ffffff) {
            case 0x00000000:
              switch ((codeWord_0 >> 6) & 0x0000003f) {
              case 0x00000000:
                return Opcode_v2_NEGHQ_registerW_registerZ_double;
              default:
                return Opcode_v2_SBFHQ_splat32_registerW_registerZ_upper27_lower5_double;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v2_SBFHQ_splat32_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MINUHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MAXUHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDCHCP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFCHCP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX2D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX2D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX4D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX4D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX8D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX8D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX16D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX16D_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDWD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFWD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDUWD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFUWD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LANDD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LNANDD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LORD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LNORD_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX2W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX2W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX4W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX4W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX8W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX8W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX16W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX16W_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_AVGW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_AVGUW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_AVGRW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_AVGRUW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_LANDW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_LNANDW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_LORW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_LNORW_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX2WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX2WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX4WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX4WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX8WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX8WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX16WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX16WP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_AVGWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_AVGUWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_AVGRWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_AVGRUWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LANDWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LNANDWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LORWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LNORWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX2HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX2HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX4HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX4HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX8HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX8HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDX16HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFX16HQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_AVGHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_AVGUHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_AVGRHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_AVGRUHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LANDHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LNANDHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LORHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LNORHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX2WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX2WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX4WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX4WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX8WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX8WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX16WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX16WD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX2UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX2UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX4UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX4UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX8UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX8UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDX16UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFX16UWD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 11) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x0000000f) {
            case 0x0000000e:
              return Opcode_v2_ADDSW_registerW_registerZ_upper27_lower5_double;
            case 0x0000000f:
              return Opcode_v2_SBFSW_registerW_registerZ_upper27_lower5_double;
            default:
              return Opcode_v2_COMPW_comparison_registerW_registerZ_upper27_lower5_double;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000c:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDCD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFCD_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ADDCD_I_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SBFCD_I_registerW_registerZ_upper27_lower5_double;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDSWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFSWP_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDSHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFSHQ_splat32_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 11) & 0x0000001f) {
      case 0x00000001:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBMM8_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_1 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBMMT8_registerW_registerZ_upper27_lower5_double;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 11) & 0x0000001f) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 27) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FCOMPW_floatcomp_registerW_registerZ_upper27_lower5_double;
          case 0x00000001:
            return Opcode_v2_FCOMPD_floatcomp_registerW_registerZ_upper27_lower5_double;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  default:
    break;
  }
  return Opcode__UNDEF;
}
#endif//NDECODE

#ifdef $XCC__h
#ifndef NDECODE
extern Opcode
Decode_Decoding_v2_simple(const void *buffer);
#endif//NDECODE
#endif//$XCC__h

#ifndef NDECODE
Opcode
Decode_Decoding_v2_simple(const void *buffer)
{
  const uint32_t *codeWords = (const uint32_t *)buffer;
  uint32_t codeWord_0 = codeWords[0];
  switch ((codeWord_0 >> 28) & 0x00000007) {
  case 0x00000000:
    switch ((codeWord_0 >> 27) & 0x00000001) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_0 >> 18) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 6) & 0x00000fff) {
          case 0x00000000:
            switch ((codeWord_0 >> 20) & 0x0000000f) {
            case 0x00000000:
              return Opcode_v2_ERROP_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 20) & 0x0000000f) {
            case 0x00000000:
              return Opcode_v2_BREAK_brknumber_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000001:
              switch ((codeWord_0 >> 0) & 0x0000003f) {
              case 0x00000000:
                switch ((codeWord_0 >> 7) & 0x0000001f) {
                case 0x0000001f:
                  return Opcode_v2_MOVEFO_registerN_registerBe_simple;
                default:
                  return Opcode_v2_ALIGNO_registerN_registerBe_registerCo_byteshift_simple;
                  break;
                }
                return Opcode__UNDEF;
              default:
                return Opcode_v2_ALIGNO_registerN_registerBe_registerCo_byteshift_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_ALIGNO_registerN_registerBe_registerCo_registerZ_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_0 >> 6) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000001:
              switch ((codeWord_0 >> 0) & 0x0000003f) {
              case 0x00000000:
                switch ((codeWord_0 >> 7) & 0x0000001f) {
                case 0x0000001f:
                  return Opcode_v2_MOVEFO_registerN_registerBo_simple;
                default:
                  return Opcode_v2_ALIGNO_registerN_registerBo_registerCe_byteshift_simple;
                  break;
                }
                return Opcode__UNDEF;
              default:
                return Opcode_v2_ALIGNO_registerN_registerBo_registerCe_byteshift_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000001:
              return Opcode_v2_ALIGNO_registerN_registerBo_registerCe_registerZ_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 12) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 0) & 0x0000003f) {
            case 0x00000000:
              switch ((codeWord_0 >> 7) & 0x0000001f) {
              case 0x0000001f:
                return Opcode_v2_COPYV_registerA_registerBe_simple;
              default:
                return Opcode_v2_ALIGNV_registerA_registerBe_registerCo_byteshift_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v2_ALIGNV_registerA_registerBe_registerCo_byteshift_simple;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 0) & 0x0000003f) {
            case 0x00000000:
              switch ((codeWord_0 >> 7) & 0x0000001f) {
              case 0x0000001f:
                return Opcode_v2_COPYV_registerA_registerBo_simple;
              default:
                return Opcode_v2_ALIGNV_registerA_registerBo_registerCe_byteshift_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              return Opcode_v2_ALIGNV_registerA_registerBo_registerCe_byteshift_simple;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 12) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_ALIGNV_registerA_registerBe_registerCo_registerZ_simple;
          case 0x00000001:
            return Opcode_v2_ALIGNV_registerA_registerBo_registerCe_registerZ_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 0) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 18) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_MMA4164BW_registerAp_registerBp_registerDp_simple;
              case 0x00000001:
                return Opcode_v2_MMA4164SUBW_registerAp_registerBp_registerDp_simple;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000001:
              switch ((codeWord_0 >> 18) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_MMA4164UBW_registerAp_registerBp_registerDp_simple;
              case 0x00000001:
                return Opcode_v2_MMA4164USBW_registerAp_registerBp_registerDp_simple;
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MMA444HD_registerAq_registerB_registerD_simple;
          case 0x00000001:
            return Opcode_v2_MMA444UHD_registerAq_registerB_registerD_simple;
          case 0x00000002:
            return Opcode_v2_MMA444SUHD_registerAq_registerB_registerD_simple;
          case 0x00000003:
            return Opcode_v2_MMA444USHD_registerAq_registerB_registerD_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_0 >> 0) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 12) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 18) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_FMMA482HW0_xrounding_silent2_registerAe_registerBp_registerDp_simple;
              case 0x00000001:
                return Opcode_v2_FMMA482HW1_xrounding_silent2_registerAo_registerBp_registerDp_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FSCALEWV_xrounding_silent2_rectify_registerA_registerB_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 12) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FNARROW44WH_xrounding_silent2_registerA_registerBp_simple;
          case 0x00000001:
            switch ((codeWord_0 >> 11) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 13) & 0x00000001) {
              case 0x00000000:
                switch ((codeWord_0 >> 8) & 0x00000007) {
                case 0x00000007:
                  switch ((codeWord_0 >> 18) & 0x00000003) {
                  case 0x00000000:
                    return Opcode_v2_MT44D_registerAq_registerBq_simple;
                  default:
                    return Opcode_v2_CONVDHV0_roundint_saturate_registerAl_registerBq_simple;
                    break;
                  }
                  return Opcode__UNDEF;
                default:
                  return Opcode_v2_CONVDHV0_roundint_saturate_registerAl_registerBq_simple;
                  break;
                }
                return Opcode__UNDEF;
              case 0x00000001:
                return Opcode_v2_CONVDHV1_roundint_saturate_registerAh_registerBq_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 23) & 0x0000000f) {
      case 0x0000000e:
        return Opcode_v2_LOOPDO_registerZ_pcrel17_simple;
      case 0x0000000f:
        switch ((codeWord_0 >> 18) & 0x0000001f) {
        case 0x00000000:
          return Opcode_v2_TLBREAD_simple;
        case 0x00000001:
          return Opcode_v2_TLBPROBE_simple;
        case 0x00000002:
          return Opcode_v2_TLBWRITE_simple;
        case 0x00000003:
          return Opcode_v2_TLBDINVAL_simple;
        case 0x00000004:
          return Opcode_v2_TLBIINVAL_simple;
        case 0x00000008:
          return Opcode_v2_AWAIT_simple;
        case 0x00000009:
          return Opcode_v2_SLEEP_simple;
        case 0x0000000a:
          return Opcode_v2_STOP_simple;
        case 0x0000000b:
          return Opcode_v2_BARRIER_simple;
        case 0x0000000c:
          return Opcode_v2_WAITIT_registerZ_simple;
        case 0x0000000d:
          return Opcode_v2_SYNCGROUP_registerZ_simple;
        case 0x0000000e:
          return Opcode_v2_WFXL_systemT2_registerZ_simple;
        case 0x0000000f:
          return Opcode_v2_WFXM_systemT2_registerZ_simple;
        case 0x00000010:
          return Opcode_v2_SET_systemT3_registerZ_simple;
        case 0x00000011:
          return Opcode_v2_GET_registerZ_systemS2_simple;
        case 0x00000012:
          return Opcode_v2_RSWAP_registerZ_systemS4_simple;
        case 0x00000013:
          return Opcode_v2_IGET_registerZ_simple;
        case 0x00000014:
          return Opcode_v2_RET_simple;
        case 0x00000015:
          return Opcode_v2_RFE_simple;
        case 0x00000016:
          return Opcode_v2_IGOTO_registerZ_simple;
        case 0x00000017:
          return Opcode_v2_ICALL_registerZ_simple;
        case 0x00000018:
          return Opcode_v2_SCALL_sysnumber_simple;
        case 0x00000019:
          return Opcode_v2_SCALL_registerZ_simple;
        default:
          return Opcode_v2_CB_branchcond_registerZ_pcrel17_simple;
          break;
        }
        return Opcode__UNDEF;
      default:
        return Opcode_v2_CB_branchcond_registerZ_pcrel17_simple;
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000001:
    switch ((codeWord_0 >> 27) & 0x00000001) {
    case 0x00000000:
      return Opcode_v2_GOTO_pcrel27_simple;
    case 0x00000001:
      return Opcode_v2_CALL_pcrel27_simple;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000002:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        return Opcode_v2_LBZ_variant_registerW_signed10_registerZ_simple;
      case 0x00000001:
        return Opcode_v2_LBS_variant_registerW_signed10_registerZ_simple;
      case 0x00000002:
        return Opcode_v2_LHZ_variant_registerW_signed10_registerZ_simple;
      case 0x00000003:
        return Opcode_v2_LHS_variant_registerW_signed10_registerZ_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 25) & 0x00000007) {
      case 0x00000000:
        return Opcode_v2_LV_speculate_registerA_signed10_registerZ_simple;
      case 0x00000001:
        return Opcode_v2_LV_column_speculate_registerAq_signed10_registerZ_simple;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_SB_signed10_registerZ_registerT_simple;
        case 0x00000001:
          return Opcode_v2_SH_signed10_registerZ_registerT_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_SW_signed10_registerZ_registerT_simple;
        case 0x00000001:
          return Opcode_v2_SD_signed10_registerZ_registerT_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_SQ_signed10_registerZ_registerU_simple;
          case 0x00000001:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SO_signed10_registerZ_registerV_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          return Opcode_v2_SV_signed10_registerZ_registerE_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_ALCLRW_registerW_signed10_registerZ_simple;
        case 0x00000001:
          return Opcode_v2_ALCLRD_registerW_signed10_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_ALADDW_signed10_registerZ_registerT_simple;
        case 0x00000001:
          return Opcode_v2_ALADDD_signed10_registerZ_registerT_simple;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LBZ_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v2_LBZ_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LBS_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v2_LBS_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LHZ_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v2_LHZ_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LHS_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v2_LHS_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 25) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LV_speculate_doscale_registerA_registerY_registerZ_simple;
        default:
          return Opcode_v2_LV_speculate_lsucond_registerY_registerA_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LV_column_speculate_doscale_registerAq_registerY_registerZ_simple;
        default:
          return Opcode_v2_LV_column_speculate_lsucond_registerY_registerAq_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_SB_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v2_SB_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_SH_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v2_SH_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_SW_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v2_SW_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_SD_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v2_SD_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 13) & 0x00000007) {
            case 0x00000007:
              return Opcode_v2_SQ_doscale_registerY_registerZ_registerU_simple;
            default:
              return Opcode_v2_SQ_lsucond_registerY_registerZ_registerU_simple;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 13) & 0x00000007) {
              case 0x00000007:
                return Opcode_v2_SO_doscale_registerY_registerZ_registerV_simple;
              default:
                return Opcode_v2_SO_lsucond_registerY_registerZ_registerV_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_SV_doscale_registerY_registerZ_registerE_simple;
          default:
            return Opcode_v2_SV_lsucond_registerY_registerZ_registerE_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_ALCLRW_doscale_registerW_registerY_registerZ_simple;
          default:
            return Opcode_v2_ALCLRW_lsucond_registerY_registerW_registerZ_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_ALCLRD_doscale_registerW_registerY_registerZ_simple;
          default:
            return Opcode_v2_ALCLRD_lsucond_registerY_registerW_registerZ_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_ALADDW_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v2_ALADDW_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_ALADDD_doscale_registerY_registerZ_registerT_simple;
          default:
            return Opcode_v2_ALADDD_lsucond_registerY_registerZ_registerT_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000003:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        return Opcode_v2_LWZ_variant_registerW_signed10_registerZ_simple;
      case 0x00000001:
        return Opcode_v2_LWS_variant_registerW_signed10_registerZ_simple;
      case 0x00000002:
        return Opcode_v2_LD_variant_registerW_signed10_registerZ_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_LQ_variant_registerM_signed10_registerZ_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 19) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_LO_variant_registerN_signed10_registerZ_simple;
          case 0x00000001:
            switch ((codeWord_0 >> 20) & 0x0000000f) {
            case 0x00000000:
              return Opcode_v2_DTOUCHL_cachelev_signed10_registerZ_simple;
            case 0x00000001:
              return Opcode_v2_DINVALL_cachelev_signed10_registerZ_simple;
            case 0x00000005:
              return Opcode_v2_IINVALS_cachelev_signed10_registerZ_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_ACSWAPW_boolcas_signed10_registerZ_registerU_simple;
          case 0x00000001:
            return Opcode_v2_ACSWAPD_boolcas_signed10_registerZ_registerU_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 24) & 0x00000003) {
          case 0x00000002:
            switch ((codeWord_0 >> 19) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_COPYO_registerN_registerR_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
          case 0x00000003:
            switch ((codeWord_0 >> 19) & 0x0000001f) {
            case 0x00000011:
              return Opcode_v2_DINVAL_simple;
            case 0x00000013:
              return Opcode_v2_IINVAL_simple;
            case 0x00000019:
              return Opcode_v2_FENCE_simple;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LWZ_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v2_LWZ_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LWS_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v2_LWS_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 13) & 0x00000007) {
        case 0x00000007:
          return Opcode_v2_LD_variant_doscale_registerW_registerY_registerZ_simple;
        default:
          return Opcode_v2_LD_variant_lsucond_registerY_registerW_registerZ_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 13) & 0x00000007) {
          case 0x00000007:
            return Opcode_v2_LQ_variant_doscale_registerM_registerY_registerZ_simple;
          default:
            return Opcode_v2_LQ_variant_lsucond_registerY_registerM_registerZ_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 19) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 13) & 0x00000007) {
            case 0x00000007:
              return Opcode_v2_LO_variant_doscale_registerN_registerY_registerZ_simple;
            default:
              return Opcode_v2_LO_variant_lsucond_registerY_registerN_registerZ_simple;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 20) & 0x0000000f) {
            case 0x00000000:
              switch ((codeWord_0 >> 12) & 0x0000000f) {
              case 0x0000000e:
                return Opcode_v2_DTOUCHL_cachelev_registerY_registerZ_simple;
              default:
                return Opcode_v2_DTOUCHL_cachelev_lsucond_registerY_registerZ_simple;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000001:
              switch ((codeWord_0 >> 12) & 0x0000000f) {
              case 0x0000000e:
                return Opcode_v2_DINVALL_cachelev_registerY_registerZ_simple;
              default:
                return Opcode_v2_DINVALL_cachelev_lsucond_registerY_registerZ_simple;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000005:
              switch ((codeWord_0 >> 12) & 0x0000000f) {
              case 0x0000000e:
                return Opcode_v2_IINVALS_cachelev_registerY_registerZ_simple;
              default:
                return Opcode_v2_IINVALS_cachelev_lsucond_registerY_registerZ_simple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 13) & 0x00000007) {
            case 0x00000007:
              return Opcode_v2_ACSWAPW_boolcas_doscale_registerY_registerZ_registerU_simple;
            default:
              return Opcode_v2_ACSWAPW_boolcas_lsucond_registerY_registerZ_registerU_simple;
              break;
            }
            return Opcode__UNDEF;
          case 0x00000001:
            switch ((codeWord_0 >> 13) & 0x00000007) {
            case 0x00000007:
              return Opcode_v2_ACSWAPD_boolcas_doscale_registerY_registerZ_registerU_simple;
            default:
              return Opcode_v2_ACSWAPD_boolcas_lsucond_registerY_registerZ_registerU_simple;
              break;
            }
            return Opcode__UNDEF;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000004:
    switch ((codeWord_0 >> 25) & 0x00000007) {
    case 0x00000000:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_MADDD_registerW_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v2_MADDWP_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FMULWC_conjugate_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FMULWCP_conjugate_rounding_silent_registerM_registerP_registerO_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_MADDHQ_registerW_registerZ_signed10_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFDMAW_rounding_silent_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_FFDMSW_rounding_silent_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FFDMAWP_rounding_silent_registerW_registerP_registerO_simple;
          case 0x00000001:
            return Opcode_v2_FFDMSWP_rounding_silent_registerW_registerP_registerO_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_FFDMAWQ_rounding_silent_registerM_registerQ_registerR_simple;
            case 0x00000001:
              return Opcode_v2_FFDMSWQ_rounding_silent_registerM_registerQ_registerR_simple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_MULD_registerW_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v2_MULWP_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFDMDAW_rounding_silent_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_FFDMSAW_rounding_silent_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FFDMDAWP_rounding_silent_registerW_registerP_registerO_simple;
          case 0x00000001:
            return Opcode_v2_FFDMSAWP_rounding_silent_registerW_registerP_registerO_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_FFDMDAWQ_rounding_silent_registerM_registerQ_registerR_simple;
            case 0x00000001:
              return Opcode_v2_FFDMSAWQ_rounding_silent_registerM_registerQ_registerR_simple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FLOATD_rounding_silent_registerW_registerZ_unsigned6_simple;
        case 0x00000001:
          return Opcode_v2_FLOATUD_rounding_silent_registerW_registerZ_unsigned6_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_MULHQ_registerW_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v2_MULWC_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFDMDSW_rounding_silent_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_FFDMASW_rounding_silent_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FFDMDSWP_rounding_silent_registerW_registerP_registerO_simple;
          case 0x00000001:
            return Opcode_v2_FFDMASWP_rounding_silent_registerW_registerP_registerO_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_FFDMDSWQ_rounding_silent_registerM_registerQ_registerR_simple;
            case 0x00000001:
              return Opcode_v2_FFDMASWQ_rounding_silent_registerM_registerQ_registerR_simple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FIXEDD_rounding_silent_registerW_registerZ_unsigned6_simple;
        case 0x00000001:
          return Opcode_v2_FIXEDUD_rounding_silent_registerW_registerZ_unsigned6_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MADDDT_registerM_registerZ_signed10_simple;
          case 0x00000001:
            return Opcode_v2_MADDUDT_registerM_registerZ_signed10_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MULDT_registerM_registerZ_signed10_simple;
          case 0x00000001:
            return Opcode_v2_MULUDT_registerM_registerZ_signed10_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMAWC_conjugate_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FFMAWCP_conjugate_rounding_silent_registerM_registerP_registerO_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FLOATW_rounding_silent_registerW_registerZ_unsigned6_simple;
        case 0x00000001:
          return Opcode_v2_FLOATUW_rounding_silent_registerW_registerZ_unsigned6_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000005:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MADDSUDT_registerM_registerZ_signed10_simple;
          case 0x00000001:
            return Opcode_v2_MADDUZDT_registerM_registerZ_signed10_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MULSUDT_registerM_registerZ_signed10_simple;
          case 0x00000001:
            return Opcode_v2_CMULDT_registerM_registerZ_signed10_simple;
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMSWC_conjugate_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FFMSWCP_conjugate_rounding_silent_registerM_registerP_registerO_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FIXEDW_rounding_silent_registerW_registerZ_unsigned6_simple;
        case 0x00000001:
          return Opcode_v2_FIXEDUW_rounding_silent_registerW_registerZ_unsigned6_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_DOT2WD_registerW_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v2_DOT2UWD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FMM212W_rounding_silent_registerM_registerZ_registerY_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FMM222W_multransm_multransl_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FLOATWP_rounding_silent_registerW_registerZ_unsigned6_simple;
        case 0x00000001:
          return Opcode_v2_FLOATUWP_rounding_silent_registerW_registerZ_unsigned6_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000007:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_DOT2SUWD_registerW_registerZ_signed10_simple;
        case 0x00000001:
          return Opcode_v2_DOT2W_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_FMMA212W_rounding_silent_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_FMMS212W_rounding_silent_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FMMA222W_multransm_multransl_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v2_FMMS222W_multransm_multransl_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FIXEDWP_rounding_silent_registerW_registerZ_unsigned6_simple;
        case 0x00000001:
          return Opcode_v2_FIXEDUWP_rounding_silent_registerW_registerZ_unsigned6_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000005:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MADDD_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_MULWDP_registerM_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_DOT2WDP_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v2_MADDWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MADDHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMAD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FADDD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFMAWDP_rounding_silent_registerM_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_FMULWDP_rounding_silent_registerM_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MADDWP_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_MULUWDP_registerM_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_DOT2UWDP_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v2_MADDUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MADDUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMAWD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FADDWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFMAHWQ_rounding_silent_registerM_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_FMULHWQ_rounding_silent_registerM_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MADDHQ_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_MULSUWDP_registerM_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_DOT2SUWDP_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v2_MADDSUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MADDSUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMAWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FADDHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFMSWDP_rounding_silent_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000001:
          return Opcode_v2_MM212W_registerM_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_DOT2WZP_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v2_MADDW_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMAHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FADDCWC_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFMSHWQ_rounding_silent_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MSBFD_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_MULD_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_MULWQ_registerM_registerP_registerO_simple;
        case 0x00000003:
          return Opcode_v2_MSBFWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MSBFHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMSD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FSBFD_rounding_silent_registerW_registerZ_registerY_simple;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000005:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MSBFWP_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_MULWP_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_MULCWC_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MSBFUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MSBFUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMSWD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FSBFWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000001:
          return Opcode_v2_FMULHO_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MSBFHQ_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_MULHQ_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_MULWDC_registerM_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MSBFSUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MSBFSUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMSWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FSBFHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000001:
          return Opcode_v2_FADDHO_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000007:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000001:
          return Opcode_v2_MULWC_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_MULCWDC_registerM_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MSBFW_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FFMSHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FSBFCWC_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000001:
          return Opcode_v2_FSBFHO_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000008:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MADDWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MADDDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_MULDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v2_CRCBELMW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MULWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MULHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FMULD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FFMAHW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFMAWQ_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v2_FADDWQ_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000009:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MADDUWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MADDUDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_MULUDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v2_CRCBELLW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MULUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MULUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FMULWD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FFMAW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFMSWQ_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v2_FADDCWCP_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000a:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MADDSUWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MADDSUDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_MULSUDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v2_CRCLELMW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MULSUWD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_MULSUHWQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FMULWP_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FFMSHW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFMAHO_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v2_FSBFWQ_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000b:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MMA212W_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MADDUZDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_CMULDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v2_CRCLELLW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MULW_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FMULHQ_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FFMSW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FFMSHO_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v2_FSBFCWCP_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000c:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MSBFWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MSBFDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_CMULXDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v2_DOT2WD_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FDOT2W_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FADDW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FADDDP_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v2_FDOT2WDP_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000d:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MSBFUWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MSBFUDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_CMULGLXDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v2_DOT2UWD_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        return Opcode_v2_FDOT2WD_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000002:
        return Opcode_v2_FSBFW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FADDCDC_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v2_FDOT2WZP_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000e:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MSBFSUWDP_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MSBFSUDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_CMULGMXDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v2_DOT2SUWD_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        return Opcode_v2_FMULW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FSBFDP_rounding_silent_registerM_registerP_registerO_simple;
        case 0x00000001:
          return Opcode_v2_FMULWQ_rounding_silent_registerM_registerP_registerO_simple;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000f:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 12) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_MMS212W_registerM_registerZ_registerY_simple;
        case 0x00000001:
          switch ((codeWord_0 >> 18) & 0x00000001) {
          case 0x00000000:
            return Opcode_v2_MSBFUZDT_registerM_registerZ_registerY_simple;
          case 0x00000001:
            return Opcode_v2_CMULGHXDT_registerM_registerZ_registerY_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          return Opcode_v2_DOT2W_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_COPYQ_registerM_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        return Opcode_v2_FMULHW_rounding_silent_registerW_registerZ_registerY_simple;
      case 0x00000003:
        switch ((codeWord_0 >> 18) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FSBFCDC_rounding_silent_registerM_registerP_registerO_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000006:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        return Opcode_v2_MAKE_registerW_signed16_simple;
      case 0x00000001:
        return Opcode_v2_ADDD_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v2_MIND_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v2_MAXD_registerW_registerZ_signed10_simple;
      case 0x00000004:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v2_ABSD_registerW_registerZ_simple;
        default:
          return Opcode_v2_ABDD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v2_NEGD_registerW_registerZ_simple;
        default:
          return Opcode_v2_SBFD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        return Opcode_v2_MINUD_registerW_registerZ_signed10_simple;
      case 0x00000007:
        return Opcode_v2_MAXUD_registerW_registerZ_signed10_simple;
      case 0x00000008:
        return Opcode_v2_ANDD_registerW_registerZ_signed10_simple;
      case 0x00000009:
        return Opcode_v2_NANDD_registerW_registerZ_signed10_simple;
      case 0x0000000a:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v2_COPYD_registerW_registerZ_simple;
        default:
          return Opcode_v2_ORD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        return Opcode_v2_NORD_registerW_registerZ_signed10_simple;
      case 0x0000000c:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000003ff:
          return Opcode_v2_NOTD_registerW_registerZ_simple;
        default:
          return Opcode_v2_XORD_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        return Opcode_v2_NXORD_registerW_registerZ_signed10_simple;
      case 0x0000000e:
        return Opcode_v2_ANDND_registerW_registerZ_signed10_simple;
      case 0x0000000f:
        return Opcode_v2_ORND_registerW_registerZ_signed10_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x0000000e:
        return Opcode_v2_ADDSD_registerW_registerZ_signed10_simple;
      case 0x0000000f:
        return Opcode_v2_SBFSD_registerW_registerZ_signed10_simple;
      default:
        return Opcode_v2_COMPD_comparison_registerW_registerZ_signed10_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x0000000e:
        return Opcode_v2_SBMM8_registerW_registerZ_signed10_simple;
      case 0x0000000f:
        return Opcode_v2_SBMMT8_registerW_registerZ_signed10_simple;
      default:
        return Opcode_v2_CMOVED_scalarcond_registerZ_registerW_signed10_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 26) & 0x00000003) {
      case 0x00000000:
        return Opcode_v2_INSF_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
      case 0x00000001:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000003c0:
          switch ((codeWord_0 >> 24) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ZXHD_registerW_registerZ_simple;
          default:
            return Opcode_v2_EXTFZ_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          return Opcode_v2_EXTFZ_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000001c0:
          switch ((codeWord_0 >> 24) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SXBD_registerW_registerZ_simple;
          default:
            return Opcode_v2_EXTFS_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x000003c0:
          switch ((codeWord_0 >> 24) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SXHD_registerW_registerZ_simple;
          case 0x00000001:
            return Opcode_v2_SXWD_registerW_registerZ_simple;
          default:
            return Opcode_v2_EXTFS_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          return Opcode_v2_EXTFS_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        return Opcode_v2_CLRF_registerW_registerZ_stopbit2_stopbit4_startbit_simple;
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
  case 0x00000007:
    switch ((codeWord_0 >> 16) & 0x00000003) {
    case 0x00000000:
      switch ((codeWord_0 >> 24) & 0x0000000f) {
      case 0x00000000:
        return Opcode_v2_PCREL_registerW_signed16_simple;
      case 0x00000001:
        return Opcode_v2_ADDW_registerW_registerZ_signed10_simple;
      case 0x00000002:
        return Opcode_v2_MINW_registerW_registerZ_signed10_simple;
      case 0x00000003:
        return Opcode_v2_MAXW_registerW_registerZ_signed10_simple;
      case 0x00000004:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v2_ABSW_registerW_registerZ_simple;
        default:
          return Opcode_v2_ABDW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v2_NEGW_registerW_registerZ_simple;
        default:
          return Opcode_v2_SBFW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        return Opcode_v2_MINUW_registerW_registerZ_signed10_simple;
      case 0x00000007:
        return Opcode_v2_MAXUW_registerW_registerZ_signed10_simple;
      case 0x00000008:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000000ff:
          return Opcode_v2_ZXBD_registerW_registerZ_simple;
        case 0x000003ff:
          return Opcode_v2_ZXWD_registerW_registerZ_simple;
        default:
          return Opcode_v2_ANDW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        return Opcode_v2_NANDW_registerW_registerZ_signed10_simple;
      case 0x0000000a:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x00000000:
          return Opcode_v2_COPYW_registerW_registerZ_simple;
        default:
          return Opcode_v2_ORW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        return Opcode_v2_NORW_registerW_registerZ_signed10_simple;
      case 0x0000000c:
        switch ((codeWord_0 >> 6) & 0x000003ff) {
        case 0x000003ff:
          return Opcode_v2_NOTW_registerW_registerZ_simple;
        default:
          return Opcode_v2_XORW_registerW_registerZ_signed10_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        return Opcode_v2_NXORW_registerW_registerZ_signed10_simple;
      case 0x0000000e:
        return Opcode_v2_ANDNW_registerW_registerZ_signed10_simple;
      case 0x0000000f:
        return Opcode_v2_ORNW_registerW_registerZ_signed10_simple;
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_0 >> 12) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_STSUD_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_ADDD_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_MIND_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MAXD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ABDD_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFD_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_MINUD_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_MAXUD_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v2_ANDD_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_NANDD_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_ORD_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_NORD_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_XORD_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_NXORD_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_ANDND_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_ORND_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_STSUW_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_ADDW_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_MINW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MAXW_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ABDW_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFW_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_MINUW_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_MAXUW_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v2_ANDW_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_NANDW_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_ORW_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_NORW_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_XORW_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_NXORW_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_ANDNW_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_ORNW_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000001:
          return Opcode_v2_ADDWP_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_MINWP_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MAXWP_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ABDWP_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFWP_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_MINUWP_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_MAXUWP_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_ADDCWC_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_SBFCWC_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000001:
          return Opcode_v2_ADDHQ_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_MINHQ_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_MAXHQ_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ABDHQ_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFHQ_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_MINUHQ_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_MAXUHQ_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_ADDCHCP_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_SBFCHCP_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_ADDX2D_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_SBFX2D_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_ADDX4D_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_SBFX4D_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ADDX8D_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFX8D_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_ADDX16D_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_SBFX16D_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v2_ADDWD_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_SBFWD_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_ADDUWD_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_SBFUWD_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_LANDD_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_LNANDD_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_LORD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_LNORD_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_ADDX2W_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_SBFX2W_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_ADDX4W_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_SBFX4W_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ADDX8W_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFX8W_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_ADDX16W_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_SBFX16W_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v2_AVGW_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_AVGUW_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_AVGRW_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_AVGRUW_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_LANDW_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_LNANDW_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_LORW_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_LNORW_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_ADDX2WP_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_SBFX2WP_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_ADDX4WP_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_SBFX4WP_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ADDX8WP_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFX8WP_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_ADDX16WP_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_SBFX16WP_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v2_AVGWP_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_AVGUWP_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_AVGRWP_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_AVGRUWP_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_LANDWP_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_LNANDWP_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_LORWP_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_LNORWP_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_ADDX2HQ_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_SBFX2HQ_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_ADDX4HQ_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_SBFX4HQ_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ADDX8HQ_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFX8HQ_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_ADDX16HQ_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_SBFX16HQ_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v2_AVGHQ_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_AVGUHQ_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_AVGRHQ_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_AVGRUHQ_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_LANDHQ_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_LNANDHQ_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_LORHQ_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_LNORHQ_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_ADDX2WD_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_SBFX2WD_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_ADDX4WD_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_SBFX4WD_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_ADDX8WD_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_SBFX8WD_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_ADDX16WD_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_SBFX16WD_registerW_registerZ_registerY_simple;
        case 0x00000008:
          return Opcode_v2_ADDX2UWD_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_SBFX2UWD_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_ADDX4UWD_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_SBFX4UWD_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_ADDX8UWD_registerW_registerZ_registerY_simple;
        case 0x0000000d:
          return Opcode_v2_SBFX8UWD_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_ADDX16UWD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SBFX16UWD_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000a:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_ADDSD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SBFSD_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v2_COMPD_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000b:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_ADDSW_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SBFSW_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v2_COMPW_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000c:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_ADDCD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SBFCD_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v2_COMPWD_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000d:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_ADDCD_I_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SBFCD_I_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v2_COMPUWD_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_ADDSWP_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SBFSWP_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v2_COMPNWP_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_ADDSHQ_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SBFSHQ_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v2_COMPNHQ_comparison_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_0 >> 12) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000e:
          return Opcode_v2_SBMM8_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SBMMT8_registerW_registerZ_registerY_simple;
        default:
          return Opcode_v2_CMOVED_scalarcond_registerZ_registerW_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 27) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_CMOVEWP_simdcond_registerZ_registerW_registerY_simple;
        case 0x00000001:
          return Opcode_v2_CMOVEHQ_simdcond_registerZ_registerW_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_CLZD_registerW_registerZ_simple;
        case 0x00000001:
          return Opcode_v2_CLSD_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v2_CBSD_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v2_CTZD_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v2_SRSD_registerW_registerZ_unsigned6_simple;
        case 0x00000009:
          return Opcode_v2_SLLD_registerW_registerZ_unsigned6_simple;
        case 0x0000000a:
          return Opcode_v2_SRAD_registerW_registerZ_unsigned6_simple;
        case 0x0000000b:
          return Opcode_v2_SRLD_registerW_registerZ_unsigned6_simple;
        case 0x0000000c:
          return Opcode_v2_SLSD_registerW_registerZ_unsigned6_simple;
        case 0x0000000e:
          switch ((codeWord_0 >> 6) & 0x0000003f) {
          case 0x00000010:
            return Opcode_v2_SATDH_registerW_registerZ_simple;
          case 0x00000020:
            return Opcode_v2_SATDW_registerW_registerZ_simple;
          default:
            return Opcode_v2_SATD_registerW_registerZ_unsigned6_simple;
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_0 >> 6) & 0x0000003f) {
          case 0x00000010:
            return Opcode_v2_SATUDH_registerW_registerZ_simple;
          case 0x00000020:
            return Opcode_v2_SATUDW_registerW_registerZ_simple;
          default:
            return Opcode_v2_SATUD_registerW_registerZ_unsigned6_simple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_CLZW_registerW_registerZ_simple;
        case 0x00000001:
          return Opcode_v2_CLSW_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v2_CBSW_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v2_CTZW_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v2_SRSW_registerW_registerZ_unsigned6_simple;
        case 0x00000009:
          return Opcode_v2_SLLW_registerW_registerZ_unsigned6_simple;
        case 0x0000000a:
          return Opcode_v2_SRAW_registerW_registerZ_unsigned6_simple;
        case 0x0000000b:
          return Opcode_v2_SRLW_registerW_registerZ_unsigned6_simple;
        case 0x0000000c:
          return Opcode_v2_SLSW_registerW_registerZ_unsigned6_simple;
        case 0x0000000e:
          return Opcode_v2_ROLW_registerW_registerZ_unsigned6_simple;
        case 0x0000000f:
          return Opcode_v2_RORW_registerW_registerZ_unsigned6_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_CLZWP_registerW_registerZ_simple;
        case 0x00000001:
          return Opcode_v2_CLSWP_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v2_CBSWP_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v2_CTZWP_registerW_registerZ_simple;
        case 0x00000006:
          return Opcode_v2_SXLHWP_registerW_registerZ_simple;
        case 0x00000007:
          return Opcode_v2_SXMHWP_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v2_SRSWPS_registerW_registerZ_unsigned6_simple;
        case 0x00000009:
          return Opcode_v2_SLLWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000a:
          return Opcode_v2_SRAWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000b:
          return Opcode_v2_SRLWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000c:
          return Opcode_v2_SLSWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000e:
          return Opcode_v2_ROLWPS_registerW_registerZ_unsigned6_simple;
        case 0x0000000f:
          return Opcode_v2_RORWPS_registerW_registerZ_unsigned6_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000006:
          return Opcode_v2_SXLBHQ_registerW_registerZ_simple;
        case 0x00000007:
          return Opcode_v2_SXMBHQ_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v2_SRSHQS_registerW_registerZ_unsigned6_simple;
        case 0x00000009:
          return Opcode_v2_SLLHQS_registerW_registerZ_unsigned6_simple;
        case 0x0000000a:
          return Opcode_v2_SRAHQS_registerW_registerZ_unsigned6_simple;
        case 0x0000000b:
          return Opcode_v2_SRLHQS_registerW_registerZ_unsigned6_simple;
        case 0x0000000c:
          return Opcode_v2_SLSHQS_registerW_registerZ_unsigned6_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v2_SRSD_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_SLLD_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_SRAD_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_SRLD_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_SLSD_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_SATD_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_SATUD_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000007:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v2_SRSW_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_SLLW_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_SRAW_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_SRLW_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_SLSW_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_ROLW_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_RORW_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v2_SRSWPS_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_SLLWPS_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_SRAWPS_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_SRLWPS_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_SLSWPS_registerW_registerZ_registerY_simple;
        case 0x0000000e:
          return Opcode_v2_ROLWPS_registerW_registerZ_registerY_simple;
        case 0x0000000f:
          return Opcode_v2_RORWPS_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000009:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v2_SRSHQS_registerW_registerZ_registerY_simple;
        case 0x00000009:
          return Opcode_v2_SLLHQS_registerW_registerZ_registerY_simple;
        case 0x0000000a:
          return Opcode_v2_SRAHQS_registerW_registerZ_registerY_simple;
        case 0x0000000b:
          return Opcode_v2_SRLHQS_registerW_registerZ_registerY_simple;
        case 0x0000000c:
          return Opcode_v2_SLSHQS_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000e:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000f:
          return Opcode_v2_MOVETQ_registerAl_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x0000000f:
          return Opcode_v2_MOVETQ_registerAh_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000003:
      switch ((codeWord_0 >> 12) & 0x0000000f) {
      case 0x00000000:
        switch ((codeWord_0 >> 27) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FCOMPW_floatcomp_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_FCOMPD_floatcomp_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 27) & 0x00000001) {
        case 0x00000000:
          return Opcode_v2_FCOMPNWP_floatcomp_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_FCOMPNHQ_floatcomp_registerW_registerZ_registerY_simple;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_FNEGD_registerW_registerZ_simple;
        case 0x00000001:
          return Opcode_v2_FABSD_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v2_FNEGW_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v2_FABSW_registerW_registerZ_simple;
        case 0x00000004:
          return Opcode_v2_FNEGWP_registerW_registerZ_simple;
        case 0x00000005:
          return Opcode_v2_FABSWP_registerW_registerZ_simple;
        case 0x00000006:
          return Opcode_v2_FNEGHQ_registerW_registerZ_simple;
        case 0x00000007:
          return Opcode_v2_FABSHQ_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v2_FSRSRD_registerW_registerZ_simple;
        case 0x0000000a:
          return Opcode_v2_FSRSRW_registerW_registerZ_simple;
        case 0x0000000c:
          return Opcode_v2_FSRSRWP_registerW_registerZ_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000008:
          return Opcode_v2_FWIDENLWD_silent2_registerW_registerZ_simple;
        case 0x00000009:
          return Opcode_v2_FWIDENMWD_silent2_registerW_registerZ_simple;
        case 0x0000000a:
          return Opcode_v2_FWIDENLHW_silent2_registerW_registerZ_simple;
        case 0x0000000b:
          return Opcode_v2_FWIDENMHW_silent2_registerW_registerZ_simple;
        case 0x0000000c:
          return Opcode_v2_FWIDENLHWP_silent2_registerW_registerZ_simple;
        case 0x0000000d:
          return Opcode_v2_FWIDENMHWP_silent2_registerW_registerZ_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_FSRECD_silent2_registerW_registerZ_simple;
        case 0x00000002:
          return Opcode_v2_FSRECW_silent2_registerW_registerZ_simple;
        case 0x00000004:
          return Opcode_v2_FSRECWP_silent2_registerW_registerZ_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000005:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_FSDIVD_silent2_registerW_registerP_simple;
        case 0x00000001:
          return Opcode_v2_FCDIVD_silent2_registerW_registerP_simple;
        case 0x00000002:
          return Opcode_v2_FSDIVW_silent2_registerW_registerP_simple;
        case 0x00000003:
          return Opcode_v2_FCDIVW_silent2_registerW_registerP_simple;
        case 0x00000004:
          return Opcode_v2_FSDIVWP_silent2_registerW_registerP_simple;
        case 0x00000005:
          return Opcode_v2_FCDIVWP_silent2_registerW_registerP_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000006:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000002:
          return Opcode_v2_FRECW_rounding2_silent2_registerW_registerZ_simple;
        case 0x00000003:
          return Opcode_v2_FRSRW_rounding2_silent2_registerW_registerZ_simple;
        case 0x00000008:
          return Opcode_v2_FNARROWDW_rounding2_silent2_registerW_registerZ_simple;
        case 0x0000000a:
          return Opcode_v2_FNARROWWH_rounding2_silent2_registerW_registerZ_simple;
        case 0x0000000c:
          return Opcode_v2_FNARROWDWP_rounding2_silent2_registerW_registerP_simple;
        case 0x0000000e:
          return Opcode_v2_FNARROWWHQ_rounding2_silent2_registerW_registerP_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000008:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          return Opcode_v2_FMIND_registerW_registerZ_registerY_simple;
        case 0x00000001:
          return Opcode_v2_FMAXD_registerW_registerZ_registerY_simple;
        case 0x00000002:
          return Opcode_v2_FMINW_registerW_registerZ_registerY_simple;
        case 0x00000003:
          return Opcode_v2_FMAXW_registerW_registerZ_registerY_simple;
        case 0x00000004:
          return Opcode_v2_FMINWP_registerW_registerZ_registerY_simple;
        case 0x00000005:
          return Opcode_v2_FMAXWP_registerW_registerZ_registerY_simple;
        case 0x00000006:
          return Opcode_v2_FMINHQ_registerW_registerZ_registerY_simple;
        case 0x00000007:
          return Opcode_v2_FMAXHQ_registerW_registerZ_registerY_simple;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x0000000f:
        switch ((codeWord_0 >> 25) & 0x00000007) {
        case 0x00000000:
          return Opcode_v2_SENDV_channel_registerA_simple;
        case 0x00000001:
          return Opcode_v2_RECVV_channel_registerA_simple;
        case 0x00000007:
          switch ((codeWord_0 >> 24) & 0x00000001) {
          case 0x00000001:
            return Opcode_v2_NOP_simple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
      break;
    }
    return Opcode__UNDEF;
    break;
  }
  return Opcode__UNDEF;
}
#endif//NDECODE

#ifdef $XCC__h
#ifndef NDECODE
extern Opcode
Decode_Decoding_v2_triple(const void *buffer);
#endif//NDECODE
#endif//$XCC__h

#ifndef NDECODE
Opcode
Decode_Decoding_v2_triple(const void *buffer)
{
  const uint32_t *codeWords = (const uint32_t *)buffer;
  uint32_t codeWord_0 = codeWords[0];
  uint32_t codeWord_1 = codeWords[1];
  uint32_t codeWord_2 = codeWords[2];
  switch ((codeWord_0 >> 28) & 0x0000000f) {
  case 0x0000000a:
    switch ((codeWord_1 >> 29) & 0x00000007) {
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LBZ_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LBS_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LHZ_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LHS_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_0 >> 25) & 0x00000007) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LV_speculate_registerA_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LV_column_speculate_registerAq_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SB_extend27_upper27_lower10_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v2_SH_extend27_upper27_lower10_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SW_extend27_upper27_lower10_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v2_SD_extend27_upper27_lower10_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 18) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_SQ_extend27_upper27_lower10_registerZ_registerU_triple;
              case 0x00000001:
                switch ((codeWord_0 >> 19) & 0x00000001) {
                case 0x00000000:
                  return Opcode_v2_SO_extend27_upper27_lower10_registerZ_registerV_triple;
                default:
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000001:
              return Opcode_v2_SV_extend27_upper27_lower10_registerZ_registerE_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ALCLRW_registerW_extend27_upper27_lower10_registerZ_triple;
            case 0x00000001:
              return Opcode_v2_ALCLRD_registerW_extend27_upper27_lower10_registerZ_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ALADDW_extend27_upper27_lower10_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v2_ALADDD_extend27_upper27_lower10_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LBZ_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LBS_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LHZ_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LHS_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 25) & 0x00000007) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LV_speculate_lsucond_registerY_registerA_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LV_column_speculate_lsucond_registerY_registerAq_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SB_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v2_SH_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_SW_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v2_SD_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 18) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_SQ_lsucond_registerY_extend27_offset27_registerZ_registerU_triple;
              case 0x00000001:
                switch ((codeWord_0 >> 19) & 0x00000001) {
                case 0x00000000:
                  return Opcode_v2_SO_lsucond_registerY_extend27_offset27_registerZ_registerV_triple;
                default:
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
            case 0x00000001:
              return Opcode_v2_SV_lsucond_registerY_extend27_offset27_registerZ_registerE_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ALCLRW_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
            case 0x00000001:
              return Opcode_v2_ALCLRD_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 24) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_ALADDW_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
            case 0x00000001:
              return Opcode_v2_ALADDD_lsucond_registerY_extend27_offset27_registerZ_registerT_triple;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000b:
    switch ((codeWord_1 >> 29) & 0x00000007) {
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LWZ_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LWS_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LD_variant_registerW_extend27_upper27_lower10_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_LQ_variant_registerM_extend27_upper27_lower10_registerZ_triple;
            case 0x00000001:
              switch ((codeWord_0 >> 19) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_LO_variant_registerN_extend27_upper27_lower10_registerZ_triple;
              case 0x00000001:
                switch ((codeWord_0 >> 20) & 0x0000000f) {
                case 0x00000000:
                  return Opcode_v2_DTOUCHL_cachelev_extend27_upper27_lower10_registerZ_triple;
                case 0x00000001:
                  return Opcode_v2_DINVALL_cachelev_extend27_upper27_lower10_registerZ_triple;
                case 0x00000005:
                  return Opcode_v2_IINVALS_cachelev_extend27_upper27_lower10_registerZ_triple;
                default:
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LWZ_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LWS_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_LD_variant_lsucond_registerY_registerW_extend27_offset27_registerZ_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              return Opcode_v2_LQ_variant_lsucond_registerY_registerM_extend27_offset27_registerZ_triple;
            case 0x00000001:
              switch ((codeWord_0 >> 19) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_LO_variant_lsucond_registerY_registerN_extend27_offset27_registerZ_triple;
              case 0x00000001:
                switch ((codeWord_0 >> 20) & 0x0000000f) {
                case 0x00000000:
                  return Opcode_v2_DTOUCHL_cachelev_lsucond_registerY_extend27_offset27_registerZ_triple;
                case 0x00000001:
                  return Opcode_v2_DINVALL_cachelev_lsucond_registerY_extend27_offset27_registerZ_triple;
                case 0x00000005:
                  return Opcode_v2_IINVALS_cachelev_lsucond_registerY_extend27_offset27_registerZ_triple;
                default:
                  break;
                }
                return Opcode__UNDEF;
                break;
              }
              return Opcode__UNDEF;
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000003:
        switch ((codeWord_0 >> 26) & 0x00000003) {
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            switch ((codeWord_0 >> 18) & 0x00000001) {
            case 0x00000000:
              switch ((codeWord_0 >> 24) & 0x00000001) {
              case 0x00000000:
                return Opcode_v2_ACSWAPW_boolcas_lsucond_registerY_extend27_offset27_registerZ_registerU_triple;
              case 0x00000001:
                return Opcode_v2_ACSWAPD_boolcas_lsucond_registerY_extend27_offset27_registerZ_registerU_triple;
                break;
              }
              return Opcode__UNDEF;
            default:
              break;
            }
            return Opcode__UNDEF;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000c:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MADDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000001:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MADDWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000002:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MADDHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000004:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MULD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000005:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MULWP_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000006:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MULHQ_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000007:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MULWC_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000008:
      switch ((codeWord_0 >> 16) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000007) {
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MADDDT_registerM_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000007) {
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MULDT_registerM_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x00000009:
      switch ((codeWord_0 >> 16) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000007) {
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MADDUDT_registerM_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000007) {
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MULUDT_registerM_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000a:
      switch ((codeWord_0 >> 16) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000007) {
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MADDSUDT_registerM_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000007) {
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MULSUDT_registerM_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000b:
      switch ((codeWord_0 >> 16) & 0x00000007) {
      case 0x00000000:
        switch ((codeWord_1 >> 29) & 0x00000007) {
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MADDUZDT_registerM_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000004:
        switch ((codeWord_1 >> 29) & 0x00000007) {
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_CMULDT_registerM_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000c:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_DOT2WD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000d:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_DOT2UWD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000e:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_DOT2SUWD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    case 0x0000000f:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_DOT2W_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000e:
    switch ((codeWord_1 >> 29) & 0x00000007) {
    case 0x00000004:
      switch ((codeWord_0 >> 16) & 0x00000003) {
      case 0x00000000:
        switch ((codeWord_0 >> 24) & 0x0000000f) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MAKE_registerW_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000001:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ADDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000002:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MIND_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000003:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MAXD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000004:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ABDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000005:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_SBFD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000006:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MINUD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000007:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_MAXUD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000008:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ANDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x00000009:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_NANDD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000a:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ORD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000b:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_NORD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000c:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_XORD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000d:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_NXORD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000e:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ANDND_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        case 0x0000000f:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_ORND_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
          break;
        }
        return Opcode__UNDEF;
      case 0x00000001:
        switch ((codeWord_2 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x0000000f) {
          case 0x0000000e:
            return Opcode_v2_ADDSD_registerW_registerZ_extend27_upper27_lower10_triple;
          case 0x0000000f:
            return Opcode_v2_SBFSD_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            return Opcode_v2_COMPD_comparison_registerW_registerZ_extend27_upper27_lower10_triple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      case 0x00000002:
        switch ((codeWord_2 >> 29) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_0 >> 24) & 0x0000000f) {
          case 0x0000000e:
            return Opcode_v2_SBMM8_registerW_registerZ_extend27_upper27_lower10_triple;
          case 0x0000000f:
            return Opcode_v2_SBMMT8_registerW_registerZ_extend27_upper27_lower10_triple;
          default:
            return Opcode_v2_CMOVED_scalarcond_registerZ_registerW_extend27_upper27_lower10_triple;
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  case 0x0000000f:
    switch ((codeWord_0 >> 24) & 0x0000000f) {
    case 0x00000000:
      switch ((codeWord_1 >> 29) & 0x00000007) {
      case 0x00000004:
        switch ((codeWord_0 >> 16) & 0x00000003) {
        case 0x00000000:
          switch ((codeWord_2 >> 29) & 0x00000003) {
          case 0x00000000:
            return Opcode_v2_PCREL_registerW_extend27_upper27_lower10_triple;
          default:
            break;
          }
          return Opcode__UNDEF;
        default:
          break;
        }
        return Opcode__UNDEF;
      default:
        break;
      }
      return Opcode__UNDEF;
    default:
      break;
    }
    return Opcode__UNDEF;
  default:
    break;
  }
  return Opcode__UNDEF;
}
#endif//NDECODE

