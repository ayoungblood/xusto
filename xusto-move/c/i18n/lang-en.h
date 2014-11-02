/* i18n/lang-en.h
 * English language defines for xusto
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
#define MSG_ERR_SOURCEDIMS   "Walked off edge of world, help!", 0x103
#define MSG_ERR_BADINSTR     "You are making up instructions:", 0x104
#define MSG_ERR_STACKALLOC   "No more memory, probable lossage", 0x105
#define MSG_ERR_EXITEXC      "Exiting with exception; definite badness", 0x106
#define MSG_ERR_INSTRDIM     "Not enough dimensions for instruction:", 0x107
// WARNING MESSAGES
#define MSG_WRN_BADOPTION    "Option may not exist:", 0x200
#define MSG_WRN_HEADERTOKEN  "Header token parse failure", 0x201
#define MSG_WRN_MOONPHASE    "Solar system may be misconfigured", 0x202
// INFO MESSAGES
#define MSG_INFO_EOX         "Reached end of execution, halting.", 0x300
// DEBUG MESSAGES
#define MSG_DBG_ENABLED      "Debug messages enabled", 0x400
#define MSG_DBG_EOX          "End of execution, dumping stack:", 0x401
#define MSG_DBG_STACKRESIZE  "Embiggening stack", 0x402

// MISC STRINGS
#define MISCSTR_BADERROR "Illegal error, seek help"
#define MISCSTR_FLAGS    "Flags"
#define MISCSTR_WARP     "Warp"
#define MISCSTR_PORTAL   "Portal"
#define MISCSTR_IPTR     "Instruction pointer"
#define MISCSTR_IVEC     "Instruction vector"
#define MISCSTR_PGMSIZE  "Program size"
#define MISCSTR_PGMSPACE "Program space"
#define MISCSTR_OUCH     "Ouch!\a"

#endif
