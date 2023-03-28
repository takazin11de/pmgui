#include "pmconv.h"

#include <stdlib.h>

#define PM_HIGH_SURROGATE_BEGIN (0xD800)
#define PM_HIGH_SURROGATE_END (0xDBFF)
#define PM_LOW_SURROGATE_BEGIN (0xDC00)
#define PM_LOW_SURROGATE_END (0xDFFF)

unsigned short* utf8to16(unsigned short* org_str, char* str) {
  unsigned int i, count, count1, len, code, code1;
  int mode;
  unsigned short* out_str = 0;
  if (org_str) {
    for (i = 0; org_str[i]; i++) {
    }
    count1 = i;
  } else {
    count1 = 0;
  }
  for (mode = 0; mode < 2; mode++) {
    count = 0;
    if (mode) {
      if (count1 < len) {
        free(org_str);
        out_str = malloc(sizeof(unsigned short) * len);
        if (!out_str) {
          return NULL;
        }
      } else {
        out_str = org_str;
      }
    }
    for (i = 0;;) {
      code = 0;
      if (mode && (count >= len)) {
        out_str[count - 1] = 0;
        break;
      }
      if ((str[i] & 0x80) == 0) {  // 0***_****
        code = str[i];
        i++;
      } else if ((str[i] & 0xE0) == 0xC0) {  // 110*_****
        code = (str[i] & 0x1F) << 6;         // 110a_bcde -> 0000_0000_0abc_de00_0000
        if ((code & 0x780) == 0) {           // 0000_0000_0abc_d*00_0000, abcd==0000 -> illegal code
          goto error;
        }
        i++;
        if ((str[i] & 0xC0) == 0x80) {  // 10**_****
          code |= (str[i] & 0x3F);      // 10fg_hijk -> 0000_0000_0abc_defg_hijk
          i++;
        } else {
          goto error;
        }
      } else if ((str[i] & 0xF0) == 0xE0) {  // 1110_****
        code = (str[i] & 0x0F) << 12;        // 1110_abcd -> 0000_abcd_0000_0000_0000
        i++;
        if ((str[i] & 0xC0) == 0x80) {   // 10**_****
          code |= (str[i] & 0x3F) << 6;  // 10ef_ghij -> 0000_abcd_efgh_ij00_0000
          if ((code & 0xF800) == 0) {    // 0000_abcd_e***_**00_0000, abcde==00000 -> illegal code
            goto error;
          }
          i++;
        } else {
          goto error;
        }
        if ((str[i] & 0xC0) == 0x80) {  // 10**_****
          code |= (str[i] & 0x3F);      // 10kl_mnop -> 0000_abcd_efgh_ijkl_mnop
          i++;
        } else {
          goto error;
        }
      } else if ((str[i] & 0xF8) == 0xF0) {  // 1111_0***
        code = (str[i] & 0x07) << 18;        // 1111_0abc -> 000a_bc00_0000_0000_0000_0000
        i++;
        if ((str[i] & 0xC0) == 0x80) {    // 10**_****
          code |= (str[i] & 0x3F) << 12;  // 10de_fghi -> 000a_bcde_fghi_0000_0000_0000
          if ((code & 0x1F0000) == 0) {   // 000a_bcde_0000_0000_0000_0000, abcde==00000 -> illegal code
            goto error;
          }
          i++;
        } else {
          goto error;
        }
        if ((str[i] & 0xC0) == 0x80) {   // 10**_****
          code |= (str[i] & 0x3F) << 6;  // 10jk_lmno -> 000a_bcde_fghi_jklm_no00_0000
          i++;
        } else {
          goto error;
        }
        if ((str[i] & 0xC0) == 0x80) {  // 10**_****
          code |= (str[i] & 0x3F);      // 10pq_rstu -> 000a_bcde_fghi_jklm_nopq_rstu
          i++;
        } else {
          goto error;
        }
      } else {
        goto error;
      }
      if (PM_HIGH_SURROGATE_BEGIN <= code && code <= PM_LOW_SURROGATE_END) {
        goto error;
      }
      if (0x10000 <= code) {
        if (mode) {
          code1 = code - 0x10000;
          out_str[count] = (code1 >> 10) + PM_HIGH_SURROGATE_BEGIN;
          out_str[count + 1] = (code1 & 0x3FF) + PM_LOW_SURROGATE_BEGIN;
        }
        count += 2;
      } else {
        if (mode) {
          out_str[count] = code;
        }
        count++;
      }
      if (code == 0) {
        len = count;
        break;
      }
    }
  }
  return out_str;
error:
  if (out_str) {
    free(out_str);
  }
  return 0;
}

char* utf16to8(char* org_str, unsigned short* str) {
  unsigned int i, code, count, count1, len;
  int mode;
  char* out_str = 0;

  if (org_str) {
    for (i = 0; org_str[i]; i++) {
    }
    count1 = i;
  } else {
    count1 = 0;
  }

  for (mode = 0; mode < 2; mode++) {
    count = 0;

    if (mode) {
      if (count1 < len) {
        free(org_str);
        out_str = malloc(sizeof(char) * len);
        if (!out_str) {
          return 0;
        }
      } else {
        out_str = org_str;
      }
    }

    for (i = 0;;) {
      code = 0;

      if (mode && (count >= len)) {
        out_str[count - 1] = 0;
        break;
      }

      if (PM_HIGH_SURROGATE_BEGIN <= str[i] && str[i] <= PM_HIGH_SURROGATE_END) {
        if (PM_LOW_SURROGATE_BEGIN <= str[i + 1] && str[i + 1] <= PM_LOW_SURROGATE_END) {
          code = (((str[i] - PM_HIGH_SURROGATE_BEGIN) << 10) + (str[i + 1] - PM_LOW_SURROGATE_BEGIN)) + 0x10000;
          i += 2;
        } else {
          goto error;
        }

      } else if (PM_LOW_SURROGATE_BEGIN <= str[i] && str[i] <= PM_LOW_SURROGATE_END) {
        goto error;
      } else {
        code = str[i];
        i++;
      }
      if ((code & 0x1F0000) != 0) {  // 000a_bcde_0000_0000_0000_0000, abcde!=00000
        if (mode) {
          out_str[count] = 0xF0 | (code >> 18);
          out_str[count + 1] = 0x80 | (0x3F & (code >> 12));
          out_str[count + 2] = 0x80 | (0x3F & (code >> 6));
          out_str[count + 3] = 0x80 | (0x3F & (code));
        }
        count += 4;
      } else if ((code & 0xF800) != 0) {  // 0000_abcd_e000_0000_0000, abcde!=00000
        if (mode) {
          out_str[count] = 0xE0 | (code >> 12);
          out_str[count + 1] = 0x80 | (0x3F & (code >> 6));
          out_str[count + 2] = 0x80 | (0x3F & (code));
        }
        count += 3;
      } else if ((code & 0x780) != 0) {  // 0000_0000_0abc_d**0_0000, abcd!=00000
        if (mode) {
          out_str[count] = 0xC0 | (code >> 6);
          out_str[count + 1] = 0x80 | (0x3F & code);
        }
        count += 2;
      } else {
        if (mode) {
          out_str[count] = 0x7F & code;
        }
        count++;
      }

      if (code == 0) {
        len = count;
        break;
      }
    }
  }
  return out_str;
error:
  if (out_str) {
    free(out_str);
  }
  return 0;
}
