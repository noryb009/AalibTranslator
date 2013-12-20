/*
 * Copyright 2013, Luke, noryb009@gmail.com. All rights reserved.
 * Based on TesseractTranslator by
 *    Gerasim Troeglazov, 3dEyes@gmail.com, Copyright 2013
 * Distributed under the terms of the MIT License.
 */

#include "AalibTranslator.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ConfigView.h"

const char *kDocumentCount = "/documentCount";
const char *kDocumentIndex = "/documentIndex";

#define kTextMimeType "text/plain"
#define kTextName "ASCII Art"

// The input formats that this translator supports.
static const translation_format sInputFormats[] = {
	{
		B_TRANSLATOR_BITMAP,
		B_TRANSLATOR_BITMAP,
		BITS_IN_QUALITY,
		BITS_IN_CAPABILITY,
		"image/x-be-bitmap",
		"Be Bitmap Format (TesseractTranslator)"
	},
};

// The output formats that this translator supports.
static const translation_format sOutputFormats[] = {
	{
		AALIB_TEXT_FORMAT,
		B_TRANSLATOR_BITMAP,
		AALIB_OUT_QUALITY,
		AALIB_OUT_CAPABILITY,
		kTextMimeType,
		kTextName
	},
};

// Default settings for the Translator
static const TranSetting sDefaultSettings[] = {};

const uint32 kNumInputFormats = sizeof(sInputFormats)
	/ sizeof(translation_format);
const uint32 kNumOutputFormats = sizeof(sOutputFormats)
	/ sizeof(translation_format);
const uint32 kNumDefaultSettings = sizeof(sDefaultSettings)
	/ sizeof(TranSetting);

AalibTranslator::AalibTranslator()
	: BaseTranslator("Aalib", 
		"aalib ASCII Translator",
		AALIB_TRANSLATOR_VERSION,
		sInputFormats, kNumInputFormats,
		sOutputFormats, kNumOutputFormats,
		"AalibTranslator",
		sDefaultSettings, kNumDefaultSettings,
		B_TRANSLATOR_BITMAP, AALIB_TEXT_FORMAT)
{
}


AalibTranslator::~AalibTranslator()
{
}


status_t
AalibTranslator::DerivedIdentify(BPositionIO *source,
	const translation_format *format, BMessage *ioExtension,
	translator_info *info, uint32 outType)
{
	if (outType != AALIB_TEXT_FORMAT)
		return B_NO_TRANSLATOR;

	BBitmap *originalbmp;
	originalbmp = BTranslationUtils::GetBitmap(source);
	if(originalbmp == NULL) {
		return B_NO_TRANSLATOR;
	}
	
	info->type = sInputFormats[0].type;
	info->translator = 0;
	info->group = sInputFormats[0].group;
	info->quality = sInputFormats[0].quality;
	info->capability = sInputFormats[0].capability;
	strcpy(info->name, sInputFormats[0].name);
	strcpy(info->MIME, sInputFormats[0].MIME);

	return B_OK;
}


status_t
AalibTranslator::DerivedTranslate(BPositionIO *source,
	const translator_info *info, BMessage *ioExtension,
	uint32 outType, BPositionIO *target, int32 baseType)
{
	if(baseType == 1 && outType == AALIB_TEXT_FORMAT) {
		BBitmap *originalbmp, *greyscalebmp;
		BRect bounds;
		
		int imgWidth;
		int imgHeight;
		int imgHalfWidth;
		int imgHalfHeight;
		aa_context *context;
		aa_renderparams *params;
		aa_palette palette;
		struct aa_hardware_params hwparams;
		
		// get the image
		originalbmp = BTranslationUtils::GetBitmap(source);
		if(originalbmp == NULL) {
			return B_ERROR;
		}
		
		// get the image size
		bounds = originalbmp->Bounds();
		imgWidth = bounds.IntegerWidth()+1;
		imgHeight = bounds.IntegerHeight()+1;
		
		// convert the bitmap to greyscale
		greyscalebmp = new BBitmap(bounds, B_GRAY8);
		if(greyscalebmp->ImportBits(originalbmp) != B_OK) {
			return B_ERROR;
		}
		
		// get half the height and width, rounded up
		// aalib outputs half the height and width of the original
		if(imgWidth%2 == 1)
			imgHalfWidth = (imgWidth+1)/2;
		else
			imgHalfWidth = imgWidth/2;
		
		if(imgHeight%2 == 1)
			imgHalfHeight = (imgHeight+1)/2;
		else
			imgHalfHeight = imgHeight/2;
		
		// use some custom settings
		memcpy(&hwparams, &aa_defparams, sizeof(struct aa_hardware_params));
		hwparams.font = NULL; // default font
		hwparams.width = imgHalfWidth; // output is half of original width and height
		hwparams.height = imgHalfHeight;
		
		// new aalib context, use mem_d (memory drive) as we will get the output ourselves
		context = aa_init(&mem_d, &hwparams, NULL);
		if(context == NULL)
			return B_ERROR;
		
		// we can't use memcpy, as the image width might not be equal to the bytes per row
		//memcpy(context->imagebuffer, greyscalebmp->Bits(), imgWidth*imgHeight);
		
		// get the location of the bitmap bits, and the bytes per row
		unsigned char *bitsLocation = (unsigned char*)greyscalebmp->Bits();
		int bytesPerRow = greyscalebmp->BytesPerRow();
		for(int y=0; y<imgHeight; y++) { // for each row and column
			for(int x=0; x<imgWidth; x++) {
				// set the pixel
				// 255- is to invert the greyscale image
				aa_putpixel(context, x, y, 255-(bitsLocation[y*bytesPerRow+x]));
			}
		}
		
		// render the image
		params = aa_getrenderparams();
		aa_render(context, params, 0, 0, imgWidth, imgHeight);
		
		for(int i=0; i<imgHalfHeight; i++) { // for each row
			if(i != 0) { // after first line, write newline
				target->Write("\n",1);
			}
			// output that line
			target->Write(context->textbuffer+i*imgHalfWidth,imgHalfWidth);
		}
		
		aa_close(context);
		free(originalbmp);
		delete greyscalebmp;
		return B_OK;
	}
	return B_NO_TRANSLATOR;
}


status_t
AalibTranslator::DerivedCanHandleImageSize(float width, float height) const
{
	return B_OK;
}


BView *
AalibTranslator::NewConfigView(TranslatorSettings *settings)
{
	return new ConfigView(settings);
}


//	#pragma mark -


BTranslator *
make_nth_translator(int32 n, image_id you, uint32 flags, ...)
{
	if (n != 0)
		return NULL;

	return new AalibTranslator();
}

