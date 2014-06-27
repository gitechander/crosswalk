// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FILE_PICKER_WEB_DIALOG_H_
#define CHROME_BROWSER_FILE_PICKER_WEB_DIALOG_H_


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/web_dialogs/web_dialog_delegate.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace content {
class WebContents;
}

namespace ui {

// Launches a web dialog for file picker with specified URL and title.
class FilePickerWebDialog : public ui::WebDialogDelegate {
 public:
  // Shows the dialog box.
  static void ShowDialog(gfx::NativeWindow owning_window, content::WebContents* contents,
      SelectFileDialog::Listener* listener_);
  // Closes the dialog, which will delete itself.
  void Close() const;
  const content::WebUI* GetWebUI() const { return webui_; }


 private:
  FilePickerWebDialog(SelectFileDialog::Listener*);

  // Overridden from ui::WebDialogDelegate:
  virtual ui::ModalType GetDialogModalType() const OVERRIDE;
  virtual base::string16 GetDialogTitle() const OVERRIDE;
  virtual GURL GetDialogContentURL() const OVERRIDE;
  virtual void GetWebUIMessageHandlers(
      std::vector<content::WebUIMessageHandler*>* handlers) const OVERRIDE;
  virtual void GetDialogSize(gfx::Size* size) const OVERRIDE;
  virtual std::string GetDialogArgs() const OVERRIDE;
  virtual void OnDialogClosed(const std::string& json_retval) OVERRIDE;
  virtual void OnCloseContents(
      content::WebContents* source, bool* out_close_dialog) OVERRIDE;
  virtual bool ShouldShowDialogTitle() const OVERRIDE;
  virtual bool HandleContextMenu(
      const content::ContextMenuParams& params) OVERRIDE;
  virtual void OnDialogShown(content::WebUI* webui,
                               content::RenderViewHost* render_view_host) OVERRIDE;

  SelectFileDialog::Listener* listener_;
  content::WebUI* webui_;

  DISALLOW_COPY_AND_ASSIGN(FilePickerWebDialog);
};

}  // namespace ui


#endif  // CHROME_BROWSER_FILE_PICKER_WEB_DIALOG_H_
