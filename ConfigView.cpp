/*
 * Copyright 2013, Luke, noryb009@gmail.com. All rights reserved.
 * Based on TesseractTranslator by
 *    Gerasim Troeglazov, 3dEyes@gmail.com, Copyright 2013
 * Distributed under the terms of the MIT License.
 */

#include "ConfigView.h"
#include "AalibTranslator.h"

#include <StringView.h>
#include <SpaceLayoutItem.h>
#include <ControlLook.h>

#include <stdio.h>


ConfigView::ConfigView(TranslatorSettings *settings)
	: BGroupView("ICNSTranslator Settings", B_VERTICAL, 0)
{
	fSettings = settings;
	
	BAlignment leftAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_UNSET);

	BStringView *stringView = new BStringView("title", "aalib Translator");
	stringView->SetFont(be_bold_font);
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	float spacing = be_control_look->DefaultItemSpacing();
	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));

	char version[256];
	sprintf(version, "Version %d.%d.%d, %s",
		int(B_TRANSLATION_MAJOR_VERSION(AALIB_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_MINOR_VERSION(AALIB_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_REVISION_VERSION(AALIB_TRANSLATOR_VERSION)),
		__DATE__);
	stringView = new BStringView("version", version);
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	stringView = new BStringView("my_copyright",
		B_UTF8_COPYRIGHT "2013 Luke <noryb009@gmail.com>.");
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));
	
	AddChild(BSpaceLayoutItem::CreateGlue());
	GroupLayout()->SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, 
		B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING);

	SetExplicitPreferredSize(GroupLayout()->MinSize());		
}


ConfigView::~ConfigView()
{
	fSettings->Release();
}
