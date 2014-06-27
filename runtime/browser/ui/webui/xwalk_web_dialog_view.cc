// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/runtime/browser/ui/webui/browser_dialogs.h"

#include "ui/views/controls/webview/web_dialog_view.h"
#include "ui/views/widget/widget.h"
#include "xwalk/runtime/browser/ui/webui/xwalk_web_contents_handler.h"

namespace xwalk {

// Declared in browser_dialogs.h so that others don't need to depend on our .h.
// Copied from chrome/browser/ui/views/chrome_web_dialog_view.cc
gfx::NativeWindow ShowWebDialog(gfx::NativeWindow parent,
                                content::BrowserContext* context,
                                ui::WebDialogDelegate* delegate) {
  views::Widget* widget = NULL;
  views::WebDialogView* view =
      new views::WebDialogView(context, delegate, new XWalkWebContentsHandler);
  if (parent) {
    widget = views::Widget::CreateWindowWithParent(view, parent);
  } else {
    // We shouldn't be called with a NULL parent, but sometimes are.
    widget = views::Widget::CreateWindow(view);
  }

  widget->Show();
  return widget->GetNativeWindow();
}

}  // namespace xwalk
