/* i18n/lang-en.h
 * lang2d language defines for English
 *
 */

#ifndef I18N_PRESENT
#define I18N_PRESENT

// Message types
#define MSG_TYP_CATASTROPHIC "RUN AWAY"
#define MSG_TYP_ERROR        "ERROR"
#define MSG_TYP_WARNING      "Warning"
#define MSG_TYP_INFO         "Info"
#define MSG_TYP_DEBUG        "debug"

// ERROR MESSAGES
#define MSG_ERR_NOSOURCE   "Feed me source file", 0x100
#define MSG_ERR_BADSOURCE  "Icky source file", 0x101
#define MSG_ERR_BADHEADER  "Icky header file", 0x102
#define MSG_ERR_SOURCEDIMS "Source dimensions too large", 0x103
// WARNING MESSAGES
#define MSG_WRN_BADOPTION  "Option may not exist:", 0x200
// INFO MESSAGES
// DEBUG MESSAGES

// MISC STRINGS
#define MISCSTR_BADERROR "Error message encountered an error. Seek help\n"

#endif
