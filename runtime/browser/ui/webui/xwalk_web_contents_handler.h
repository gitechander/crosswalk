// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_UI_WEBUI_CHROME_WEB_CONTENTS_HANDLER_H_
#define XWALK_UI_WEBUI_CHROME_WEB_CONTENTS_HANDLER_H_

#include "base/compiler_specific.h"
#include "ui/web_dialogs/web_dialog_web_contents_delegate.h"

class XWalkWebContentsHandler
    : public ui::WebDialogWebContentsDelegate::WebContentsHandler {
 public:
  XWalkWebContentsHandler();
  virtual ~XWalkWebContentsHandler();

  // Overridden from WebDialogWebContentsDelegate::WebContentsHandler:
  virtual content::WebContents* OpenURLFromTab(
      content::BrowserContext* context,
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual void AddNewContents(content::BrowserContext* context,
                              content::WebContents* source,
                              content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(XWalkWebContentsHandler);
};

#endif  // XWALK_UI_WEBUI_CHROME_WEB_CONTENTS_HANDLER_H_
