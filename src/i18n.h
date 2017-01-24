/*****************************************************************************\
 * i18n.h                                                                    *
 * Stone-age internationalization for the Xusto interpreter                  *
 *                                                                           *
 * Language files are simply a collection of #defines of all the strings for *
 * each language. Languages are identified by their two-letter ISO 639-1     *
 * code, when possible, or their full name when not.                         *
\*****************************************************************************/
#ifndef I18N_H
#define I18N_H

#ifdef LANG_EN
#include "i18n/lang-en.h"
#endif

#ifdef LANG_ES
#include "i18n/lang-es.h"
#endif

#ifdef LANG_FURBISH
#include "i18n/lang-xx-furbish.h"
#endif

#endif /* I18N_H */
