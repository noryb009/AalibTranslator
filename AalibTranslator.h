/*
 * Copyright 2013, Luke, noryb009@gmail.com. All rights reserved.
 * Based on TesseractTranslator by
 *    Gerasim Troeglazov, 3dEyes@gmail.com, Copyright 2013
 * Distributed under the terms of the MIT License.
 */

#ifndef AALIB_TRANSLATOR_H
#define AALIB_TRANSLATOR_H


#include "BaseTranslator.h"

#include <Translator.h>
#include <TranslatorFormats.h>
#include <TranslationDefs.h>
#include <GraphicsDefs.h>
#include <InterfaceDefs.h>
#include <DataIO.h>
#include <File.h>
#include <ByteOrder.h>
#include <OS.h>
#include <Notification.h>
#include <Resources.h>
#include <Roster.h>
#include <File.h>
#include <Bitmap.h>
#include <IconUtils.h>
#include <TranslationUtils.h>

#include <aalib.h>

#define DOCUMENT_COUNT "/documentCount"
#define DOCUMENT_INDEX "/documentIndex"

#define AALIB_TRANSLATOR_VERSION B_TRANSLATION_MAKE_VERSION(0, 0, 1)
#define AALIB_TEXT_FORMAT	'AALB'

#define BITS_IN_QUALITY		1
#define BITS_IN_CAPABILITY	1

#define AALIB_OUT_QUALITY	0.3
#define AALIB_OUT_CAPABILITY	0.3

class AalibTranslator : public BaseTranslator {
	public:
		AalibTranslator();

		virtual status_t DerivedIdentify(BPositionIO *inSource,
			const translation_format *inFormat, BMessage *ioExtension,
			translator_info *outInfo, uint32 outType);

		virtual status_t DerivedTranslate(BPositionIO *inSource,
			const translator_info *inInfo, BMessage *ioExtension,
			uint32 outType, BPositionIO *outDestination, int32 baseType);

		virtual status_t DerivedCanHandleImageSize(float width,
			float height) const;

		virtual BView *NewConfigView(TranslatorSettings *settings);	

	protected:
		virtual ~AalibTranslator();
};

// Extensions that ShowImage supports
extern const char *kDocumentCount;
extern const char *kDocumentIndex;

#endif	/* AALIB_TRANSLATOR_H */
