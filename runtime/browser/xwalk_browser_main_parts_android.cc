// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "xwalk/runtime/browser/xwalk_browser_main_parts_android.h"

#include "base/android/path_utils.h"
#include "base/base_paths_android.h"
#include "base/files/file_path.h"
#include "base/file_util.h"
#include "base/message_loop/message_loop.h"
#include "base/path_service.h"
#include "base/strings/string_number_conversions.h"
#include "base/sys_info.h"
#include "base/threading/sequenced_worker_pool.h"
#include "cc/base/switches.h"
#include "content/public/browser/android/compositor.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/cookie_store_factory.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/result_codes.h"
#include "grit/net_resources.h"
#include "net/android/network_change_notifier_factory_android.h"
#include "net/base/network_change_notifier.h"
#include "net/base/net_module.h"
#include "net/base/net_util.h"
#include "ui/base/layout.h"
#include "ui/base/l10n/l10n_util_android.h"
#include "ui/base/resource/resource_bundle.h"
#include "xwalk/application/browser/application_system.h"
#include "xwalk/extensions/browser/xwalk_extension_service.h"
#include "xwalk/extensions/common/xwalk_extension.h"
#include "xwalk/extensions/common/xwalk_extension_switches.h"
#include "xwalk/runtime/browser/android/cookie_manager.h"
#include "xwalk/runtime/browser/runtime_context.h"
#include "xwalk/runtime/browser/xwalk_runner.h"
#include "xwalk/runtime/common/xwalk_runtime_features.h"
#include "xwalk/runtime/extension/runtime_extension.h"
#include "xwalk/sysapps/raw_socket/raw_socket_extension.h"

namespace {

base::StringPiece PlatformResourceProvider(int key) {
  if (key == IDR_DIR_HEADER_HTML) {
    base::StringPiece html_data =
        ui::ResourceBundle::GetSharedInstance().GetRawDataResource(
            IDR_DIR_HEADER_HTML);
    return html_data;
  }
  return base::StringPiece();
}

}  // namespace

namespace xwalk {

using content::BrowserThread;
using extensions::XWalkExtension;

XWalkBrowserMainPartsAndroid::XWalkBrowserMainPartsAndroid(
    const content::MainFunctionParams& parameters)
    : XWalkBrowserMainParts(parameters) {
}

XWalkBrowserMainPartsAndroid::~XWalkBrowserMainPartsAndroid() {
}

void XWalkBrowserMainPartsAndroid::PreEarlyInitialization() {
  net::NetworkChangeNotifier::SetFactory(
      new net::NetworkChangeNotifierFactoryAndroid());

  CommandLine::ForCurrentProcess()->AppendSwitch(
      cc::switches::kCompositeToMailbox);

  // Initialize the Compositor.
  content::Compositor::Initialize();

  XWalkBrowserMainParts::PreEarlyInitialization();
}

void XWalkBrowserMainPartsAndroid::PreMainMessageLoopStart() {
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  // Disable ExtensionProcess for Android.
  // External extensions will run in the BrowserProcess (in process mode).
  command_line->AppendSwitch(switches::kXWalkDisableExtensionProcess);

  // Enable WebGL for Android.
  command_line->AppendSwitch(switches::kIgnoreGpuBlacklist);

  // Disable HW encoding/decoding acceleration for WebRTC on Android.
  // FIXME: Remove these switches for Android when Android OS is removed from
  // GPU accelerated_video_decode blacklist or we stop ignoring the GPU
  // blacklist.
  command_line->AppendSwitch(switches::kDisableWebRtcHWDecoding);
  command_line->AppendSwitch(switches::kDisableWebRtcHWEncoding);

  if (!command_line->HasSwitch(cc::switches::kNumRasterThreads)) {
    // Enable multiple threads for rasterization to take advantage of multi CPU
    // cores. Only valid when ImplSidePainting is enabled.
    const int num_of_cpu_cores = base::SysInfo::NumberOfProcessors();
    command_line->AppendSwitchASCII(cc::switches::kNumRasterThreads,
                                    base::IntToString(num_of_cpu_cores));
  }

  XWalkBrowserMainParts::PreMainMessageLoopStart();

  startup_url_ = GURL();
}

void XWalkBrowserMainPartsAndroid::PostMainMessageLoopStart() {
  base::MessageLoopForUI::current()->Start();
}

void XWalkBrowserMainPartsAndroid::PreMainMessageLoopRun() {
  net::NetModule::SetResourceProvider(PlatformResourceProvider);
  if (parameters_.ui_task) {
    parameters_.ui_task->Run();
    delete parameters_.ui_task;
    run_default_message_loop_ = false;
  }

  xwalk_runner_->PreMainMessageLoopRun();

  runtime_context_ = xwalk_runner_->runtime_context();
  extension_service_ = xwalk_runner_->extension_service();

  // Prepare the cookie store.
  base::FilePath user_data_dir;
  if (!PathService::Get(base::DIR_ANDROID_APP_DATA, &user_data_dir)) {
    NOTREACHED() << "Failed to get app data directory for Crosswalk";
  }

  base::FilePath cookie_store_path = user_data_dir.Append(
      FILE_PATH_LITERAL("Cookies"));
  scoped_refptr<base::SequencedTaskRunner> background_task_runner =
      BrowserThread::GetBlockingPool()->GetSequencedTaskRunner(
          BrowserThread::GetBlockingPool()->GetSequenceToken());

  cookie_store_ = content::CreatePersistentCookieStore(
      cookie_store_path,
      true,
      NULL,
      NULL,
      BrowserThread::GetMessageLoopProxyForThread(BrowserThread::IO),
      background_task_runner);

  cookie_store_->GetCookieMonster()->SetPersistSessionCookies(true);
  SetCookieMonsterOnNetworkStackInit(cookie_store_->GetCookieMonster());
}

void XWalkBrowserMainPartsAndroid::PostMainMessageLoopRun() {
  XWalkBrowserMainParts::PostMainMessageLoopRun();

  base::MessageLoopForUI::current()->Start();
}

void XWalkBrowserMainPartsAndroid::CreateInternalExtensionsForExtensionThread(
    content::RenderProcessHost* host,
    extensions::XWalkExtensionVector* extensions) {
  // FIXME(cmarcelo): Android port keeps the ownership of extensions
  // here in the XWalkBrowserMainParts for some reason, this is incorrect use
  // of extensions system.
  ScopedVector<XWalkExtension>::const_iterator it = extensions_.begin();
  for (; it != extensions_.end(); ++it)
    extensions->push_back(*it);

  if (XWalkRuntimeFeatures::isRawSocketsAPIEnabled())
    extensions->push_back(new sysapps::RawSocketExtension());
}

void XWalkBrowserMainPartsAndroid::CreateInternalExtensionsForUIThread(
    content::RenderProcessHost* host,
    extensions::XWalkExtensionVector* extensions) {
  // This empty function overrides the implementation in the class
  // XWalkBrowserMainParts. It's because application runtime and event related
  // extensions are not ready for Android port. Need to re-design these 2
  // mechanisms on Android. Please see JIRA issue:
  // https://crosswalk-project.org/jira/browse/XWALK-674
  // TODO(yongsheng): Remove this implementation once above 2 features
  // are ready for Android.
}

void XWalkBrowserMainPartsAndroid::RegisterExtension(
    scoped_ptr<XWalkExtension> extension) {
  extensions_.push_back(extension.release());
}

void XWalkBrowserMainPartsAndroid::UnregisterExtension(
    scoped_ptr<XWalkExtension> extension) {
  ScopedVector<XWalkExtension>::iterator it = extensions_.begin();
  for (; it != extensions_.end(); ++it) {
    // FIXME: how is that possible we have 2 scoped_ptrs to the same pointer?
    if (*it == extension.release()) break;
  }

  if (it != extensions_.end())
    extensions_.erase(it);
}

}  // namespace xwalk
