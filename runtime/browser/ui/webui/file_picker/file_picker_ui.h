// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_FILE_PICKER_UI_H_
#define CHROME_BROWSER_UI_WEBUI_FILE_PICKER_UI_H_

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/web_dialogs/web_dialog_ui.h"

namespace content {
class BrowserContext;
class WebContents;
}

namespace ui {
class WebDialogDelegate;
}

namespace ui {

class FilePickerUI : public WebDialogUI {
 public:
  explicit FilePickerUI(content::WebUI* web_ui);
  virtual ~FilePickerUI();

 private:
  DISALLOW_COPY_AND_ASSIGN(FilePickerUI);
};

}  // namespace ui

#endif  // CHROME_BROWSER_UI_WEBUI_FILE_PICKER_UI_H_
