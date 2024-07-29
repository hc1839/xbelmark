#ifndef XBELMARK_WINSHELL_WINSHELL_H
#define XBELMARK_WINSHELL_WINSHELL_H

#include <memory>
#include <string>

#include <shlobj.h>

#include "xbelmark/iterator.h"
#include "xbelmark/memory/smart_ptr.h"

namespace xbelmark {
namespace winshell {

using ShellItemPtr = xbelmark::memory::UniquePtr<IShellItem>;

template <typename T>
xbelmark::memory::UniquePtr<T> WinUnknownPtr(T *obj) {
  return xbelmark::memory::UniquePtr<T>(
      obj,
      [](T *ptr) -> void {
        ptr->Release();
      });
}

template <typename T>
xbelmark::memory::UniquePtr<T> WinUnknownPtr(void *obj = nullptr) {
  return WinUnknownPtr(static_cast<T *>(obj));
}

struct ShellWindows {
 public:
  ShellWindows();

  ShellWindows(ShellWindows &&);

  virtual ~ShellWindows();

  ShellWindows &operator=(ShellWindows &&);

  xbelmark::memory::UniquePtr<IShellWindows> psw =
      WinUnknownPtr<IShellWindows>();
};

struct Dispatch : public ShellWindows {
 public:
  Dispatch(VARIANT index);

  Dispatch(Dispatch &&);

  virtual ~Dispatch();

  Dispatch &operator=(Dispatch &&);

  xbelmark::memory::UniquePtr<IDispatch> pd = WinUnknownPtr<IDispatch>();
};

struct WebBrowserApp : public Dispatch {
 public:
  static std::unique_ptr<WebBrowserApp> New(const HWND &hwnd);

  WebBrowserApp(VARIANT index);

  WebBrowserApp(WebBrowserApp &&);

  virtual ~WebBrowserApp();

  WebBrowserApp &operator=(WebBrowserApp &&);

  const HWND &hwnd() const;

  xbelmark::memory::UniquePtr<IWebBrowserApp> pwba =
      WinUnknownPtr<IWebBrowserApp>();

 private:
  HWND hwnd_;
};

struct ServiceProvider : public WebBrowserApp {
 public:
  ServiceProvider(const HWND &hwnd);

  ServiceProvider(ServiceProvider &&);

  virtual ~ServiceProvider();

  ServiceProvider &operator=(ServiceProvider &&);

  xbelmark::memory::UniquePtr<IServiceProvider> psp =
      WinUnknownPtr<IServiceProvider>();
};

struct ShellBrowser : public ServiceProvider {
 public:
  ShellBrowser(const HWND &hwnd, REFGUID guid_service = SID_STopLevelBrowser);

  ShellBrowser(ShellBrowser &&);

  virtual ~ShellBrowser();

  ShellBrowser &operator=(ShellBrowser &&);

  xbelmark::memory::UniquePtr<IShellBrowser> psb =
      WinUnknownPtr<IShellBrowser>();
};

struct ShellView : public ShellBrowser {
 public:
  ShellView(const HWND &hwnd);

  ShellView(ShellView &&);

  virtual ~ShellView();

  ShellView &operator=(ShellView &&);

  xbelmark::memory::UniquePtr<IShellView> psv = WinUnknownPtr<IShellView>();
};

struct FolderView : public ShellView {
 public:
  FolderView(const HWND &hwnd);

  FolderView(FolderView &&);

  virtual ~FolderView();

  FolderView &operator=(FolderView &&);

  xbelmark::memory::UniquePtr<IFolderView> pfv = WinUnknownPtr<IFolderView>();
};

struct PersistFolder2 : public FolderView {
 public:
  PersistFolder2(const HWND &hwnd);

  PersistFolder2(PersistFolder2 &&);

  virtual ~PersistFolder2();

  PersistFolder2 &operator=(PersistFolder2 &&);

  xbelmark::memory::UniquePtr<IPersistFolder2> ppf2 =
      WinUnknownPtr<IPersistFolder2>();
};

struct ShellFolder : public PersistFolder2 {
  ShellFolder(const HWND &hwnd);

  ShellFolder(ShellFolder &&);

  virtual ~ShellFolder();

  ShellFolder &operator=(ShellFolder &&);

  xbelmark::memory::UniquePtr<IShellFolder> psf =
      WinUnknownPtr<IShellFolder>();
};

class ShellItemIterator final : public xbelmark::Iterator<ShellItemPtr> {
 public:
  ShellItemIterator(const FolderView &fv);

  virtual ~ShellItemIterator() = default;

  bool HasNext() const override;

  ShellItemPtr Next() override;

 private:
  xbelmark::memory::UniquePtr<IEnumShellItems> pesi_;

  ShellItemPtr psi_;
};

} // namespace winshell
} // namespace xbelmark

#endif
