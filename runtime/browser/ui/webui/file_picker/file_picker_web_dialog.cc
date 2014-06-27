// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/runtime/browser/ui/webui/file_picker/file_picker_web_dialog.h"

#include "base/bind.h"
#include "base/values.h"
#include "base/json/json_writer.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "content/public/common/url_constants.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/gfx/size.h"
#include "xwalk/grit/xwalk_resources.h"
#include "xwalk/runtime/browser/ui/webui/browser_dialogs.h"

using content::WebContents;
using content::WebUIMessageHandler;

namespace {

// Default width/height of the dialog.
const int kDefaultWidth = 350;
const int kDefaultHeight = 225;
}

namespace ui {

// FilePickerMessageHandler --------------------------------------------
class FilePickerMessageHandler : public content::WebUIMessageHandler {
 public:
  FilePickerMessageHandler(
      const ui::FilePickerWebDialog* dialog);
  virtual ~FilePickerMessageHandler();
  virtual void RegisterMessages() OVERRIDE;

 private:
  // content::WebUIMessageHandler implementation.
  void OnCancelButtonClicked(const base::ListValue* args);
  void OnSaveButtonClicked(const base::ListValue* args);

  // Weak ptr to parent dialog.
  const ui::FilePickerWebDialog* dialog_;
};

FilePickerMessageHandler::FilePickerMessageHandler(
    const ui::FilePickerWebDialog* dialog)
  : dialog_(dialog) {
}

FilePickerMessageHandler::~FilePickerMessageHandler() {
}

void FilePickerMessageHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "cancel",
      base::Bind(&FilePickerMessageHandler::OnCancelButtonClicked,
                 base::Unretained(this)));

  web_ui()->RegisterMessageCallback(
      "save",
      base::Bind(&FilePickerMessageHandler::OnSaveButtonClicked,
                 base::Unretained(this)));
}

void FilePickerMessageHandler::OnCancelButtonClicked(
    const base::ListValue* args) {
  dialog_->Close();
}

void FilePickerMessageHandler::OnSaveButtonClicked(
    const base::ListValue* args) {
  dialog_->Close();
}

}  // namespace ui

namespace ui {

// static
void FilePickerWebDialog::ShowDialog(gfx::NativeWindow owning_window,
    content::WebContents* contents,  SelectFileDialog::Listener* listener) {
  fprintf(stdout, "FilePickerWebDialog::%s\n", __func__);
  xwalk::ShowWebDialog(owning_window, contents->GetBrowserContext(),
      new FilePickerWebDialog(listener));
}

void FilePickerWebDialog::Close() const {
}

FilePickerWebDialog::FilePickerWebDialog(SelectFileDialog::Listener* listener)
    : listener_(listener), webui_(NULL){
}

ui::ModalType FilePickerWebDialog::GetDialogModalType() const {
  return ui::MODAL_TYPE_SYSTEM;
}

base::string16 FilePickerWebDialog::GetDialogTitle() const {
  return l10n_util::GetStringUTF16(IDS_FILE_PICKER_TITLE);
}

GURL FilePickerWebDialog::GetDialogContentURL() const {
  return GURL (content::kChromeUIFilePickerURL);
}

void FilePickerWebDialog::GetWebUIMessageHandlers(
     std::vector<content::WebUIMessageHandler*>* handlers) const {
  handlers->push_back(new FilePickerMessageHandler(this));
}

void FilePickerWebDialog::GetDialogSize(gfx::Size* size) const {
  size->SetSize(kDefaultWidth, kDefaultHeight);
}

std::string FilePickerWebDialog::GetDialogArgs() const {
  std::string data;
  base::DictionaryValue file_info;
  // FIXME(joone): Pass the exact file path
  file_info.SetString("general.file_path", "test.text");
  base::JSONWriter::Write(&file_info, &data);
  return data;
}

void FilePickerWebDialog::OnDialogClosed(const std::string& json_retval) {
  listener_->FileSelectionCanceled(NULL);
  delete this;
}

void FilePickerWebDialog::OnCloseContents(WebContents* source,
                                                bool* out_close_dialog) {
  if (out_close_dialog)
    *out_close_dialog = true;
}

bool FilePickerWebDialog::ShouldShowDialogTitle() const {
  return true;
}

void FilePickerWebDialog::OnDialogShown(content::WebUI* webui,
                              content::RenderViewHost* render_view_host) {
  webui_ = webui;
}

bool FilePickerWebDialog::HandleContextMenu(
    const content::ContextMenuParams& params) {
  // Disable context menu.
  return true;
}

}  // namespace ui
