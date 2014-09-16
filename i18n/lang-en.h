/* i18n/lang-en.h
 * lang2d language defines for English
 *
 */

#ifndef I18N_PRESENT
#define I18N_PRESENT

// Message types
#define MSG_TYP_CATASTROPHIC "CATASTROPHIC"
#define MSG_TYP_ERROR        "ERROR"
#define MSG_TYP_WARNING      "Warning"
#define MSG_TYP_INFO         "Info"
#define MSG_TYP_DEBUG        "debug"

// ERROR MESSAGES
#define MSG_ERR_NOSOURCE     "Feed me source file", 0x100
#define MSG_ERR_BADSOURCE    "Icky source file", 0x101
#define MSG_ERR_BADHEADER    "Unpalatable header", 0x102
#define MSG_ERR_SOURCEDIMS   "Source dimensions too large", 0x103
#define MSG_ERR_BADINSTR     "Instruction does not exist:", 0x104
// WARNING MESSAGES
#define MSG_WRN_BADOPTION    "Option may not exist:", 0x200
#define MSG_WRN_HEADERTOKEN  "Header token parse failure", 0x201
// INFO MESSAGES
#define MSG_INFO_EOX         "Reached end of execution, halting.", 0x300
// DEBUG MESSAGES
#define MSG_DBG_ENABLED      "Debug messages enabled", 0x400
#define MSG_DBG_EOX          "End of execution, halting.", 0x401

// MISC STRINGS
#define MISCSTR_BADERROR "Illegal error, seek help"
#define MISCSTR_FLAGS    "Flags"
#define MISCSTR_WARP     "Warp"
#define MISCSTR_BEACON   "Beacon"
#define MISCSTR_IPTR     "Instruction pointer"
#define MISCSTR_IVEC     "Instruction vector"
#define MISCSTR_PGMSIZE  "Program size"
#define MISCSTR_PGMSPACE "Program space"
#define MISCSTR_OUCH     "Ouch!"

#endif
