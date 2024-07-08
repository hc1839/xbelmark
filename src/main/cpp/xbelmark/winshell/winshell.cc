#include "xbelmark/winshell/winshell.h"

#include <codecvt>
#include <locale>
#include <stdexcept>
#include <utility>

namespace xbelmark {
namespace winshell {

using xbelmark::memory::UniquePtr;

ShellWindows::ShellWindows() {
  void *obj(nullptr);
  CoCreateInstance(
      CLSID_ShellWindows, nullptr, CLSCTX_ALL, IID_IShellWindows, &obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IShellWindows`.");
  }
  this->psw = WinUnknownPtr<IShellWindows>(obj);
  obj = nullptr;
}

ShellWindows::ShellWindows(ShellWindows &&) = default;

ShellWindows::~ShellWindows() = default;

ShellWindows &ShellWindows::operator=(ShellWindows &&) = default;

Dispatch::Dispatch(VARIANT index) {
  IDispatch *obj(nullptr);
  this->psw->Item(index, &obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IDispatch`.");
  }
  this->pd = WinUnknownPtr(obj);
  obj = nullptr;
}

Dispatch::Dispatch(Dispatch &&) = default;

Dispatch::~Dispatch() = default;

Dispatch &Dispatch::operator=(Dispatch &&) = default;

std::unique_ptr<WebBrowserApp> WebBrowserApp::New(const HWND &hwnd) {
  std::unique_ptr<WebBrowserApp> retval;
  VARIANT index;
  V_VT(&index) = VT_I4;
  for (V_I4(&index) = 0; ; ++V_I4(&index)) {
    try {
      retval.reset(new WebBrowserApp(index));
    } catch (const std::exception &) {
      throw std::runtime_error(
          "Cannot find the window with the given handle.");
    }
    if (retval->hwnd() == hwnd) {
      return retval;
    }
  }
}

WebBrowserApp::WebBrowserApp(VARIANT index) : Dispatch(index) {
  void *obj(nullptr);
  this->pd->QueryInterface(IID_IWebBrowserApp, &obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IWebBrowserApp`.");
  }
  this->pwba = WinUnknownPtr<IWebBrowserApp>(obj);
  obj = nullptr;
  HWND wba_hwnd(0);
  this->pwba->get_HWND(reinterpret_cast<LONG_PTR *>(&wba_hwnd));
  if (wba_hwnd == 0) {
    throw std::runtime_error(
        "Failed to get the window handle of an `IWebBrowserApp`.");
  }
  this->hwnd_ = wba_hwnd;
}

WebBrowserApp::WebBrowserApp(WebBrowserApp &&) = default;

WebBrowserApp::~WebBrowserApp() = default;

WebBrowserApp &WebBrowserApp::operator=(WebBrowserApp &&) = default;

const HWND &WebBrowserApp::hwnd() const {
  return hwnd_;
}

ServiceProvider::ServiceProvider(const HWND &hwnd)
    : WebBrowserApp(std::move(*WebBrowserApp::New(hwnd))) {
  void *obj(nullptr);
  this->pwba->QueryInterface(IID_IServiceProvider, &obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IServiceProvider`.");
  }
  this->psp = WinUnknownPtr<IServiceProvider>(obj);
  obj = nullptr;
}

ServiceProvider::ServiceProvider(ServiceProvider &&) = default;

ServiceProvider::~ServiceProvider() = default;

ServiceProvider &ServiceProvider::operator=(ServiceProvider &&) = default;

ShellBrowser::ShellBrowser(const HWND &hwnd, REFGUID guid_service)
    : ServiceProvider(hwnd) {
  void *obj(nullptr);
  this->psp->QueryService(guid_service, IID_IShellBrowser, &obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IShellBrowser`.");
  }
  this->psb = WinUnknownPtr<IShellBrowser>(obj);
  obj = nullptr;
}

ShellBrowser::ShellBrowser(ShellBrowser &&) = default;

ShellBrowser::~ShellBrowser() = default;

ShellBrowser &ShellBrowser::operator=(ShellBrowser &&) = default;

ShellView::ShellView(const HWND &hwnd) : ShellBrowser(hwnd) {
  IShellView *obj(nullptr);
  this->psb->QueryActiveShellView(&obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IShellView`.");
  }
  this->psv = WinUnknownPtr(obj);
  obj = nullptr;
}

ShellView::ShellView(ShellView &&) = default;

ShellView::~ShellView() = default;

ShellView &ShellView::operator=(ShellView &&) = default;

FolderView::FolderView(const HWND &hwnd) : ShellView(hwnd) {
  void *obj(nullptr);
  this->psv->QueryInterface(IID_IFolderView, &obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IFolderView`.");
  }
  this->pfv = WinUnknownPtr<IFolderView>(obj);
  obj = nullptr;
}

FolderView::FolderView(FolderView &&) = default;

FolderView::~FolderView() = default;

FolderView &FolderView::operator=(FolderView &&) = default;

PersistFolder2::PersistFolder2(const HWND &hwnd) : FolderView(hwnd) {
  void *obj(nullptr);
  this->pfv->GetFolder(IID_IPersistFolder2, &obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IPersistFolder2`.");
  }
  this->ppf2 = WinUnknownPtr<IPersistFolder2>(obj);
  obj = nullptr;
}

PersistFolder2::PersistFolder2(PersistFolder2 &&) = default;

PersistFolder2::~PersistFolder2() = default;

PersistFolder2 &PersistFolder2::operator=(PersistFolder2 &&) = default;

ShellFolder::ShellFolder(const HWND &hwnd) : PersistFolder2(hwnd) {
  void *obj(nullptr);
  this->ppf2->QueryInterface(IID_IShellFolder, &obj);
  if (!obj) {
    throw std::runtime_error("Failed to instantiate `IShellFolder`.");
  }
  this->psf = WinUnknownPtr<IShellFolder>(obj);
  obj = nullptr;
}

ShellFolder::ShellFolder(ShellFolder &&) = default;

ShellFolder::~ShellFolder() = default;

ShellFolder &ShellFolder::operator=(ShellFolder &&) = default;

ShellItemIterator::ShellItemIterator(const FolderView &fv) {
  UniquePtr<IShellItemArray> psia;
  {
    void *obj = nullptr;
    fv.pfv->Items(SVGIO_ALLVIEW, IID_IShellItemArray, &obj);
    if (!obj) {
      throw std::runtime_error("Failed to get `IShellItemArray`.");
    }
    psia = xbelmark::winshell::WinUnknownPtr<IShellItemArray>(obj);
    obj = nullptr;
  }
  {
    IEnumShellItems *obj = nullptr;
    psia->EnumItems(&obj);
    if (!obj) {
      throw std::runtime_error("Failed to get `IEnumShellItems`.");
    }
    this->pesi_ = xbelmark::winshell::WinUnknownPtr(obj);
    obj = nullptr;
  }
  IShellItem *psi;
  if (this->pesi_->Next(1, &psi, nullptr) == S_OK) {
    this->psi_ = WinUnknownPtr(psi);
  }
}

bool ShellItemIterator::HasNext() const {
  return psi_ != nullptr;
}

ShellItemPtr ShellItemIterator::Next() {
  if (psi_) {
    ShellItemPtr retval(std::move(psi_));
    IShellItem *psi;
    if (pesi_->Next(1, &psi, nullptr) == S_OK) {
      psi_ = WinUnknownPtr(psi);
    }
    return retval;
  } else {
    throw std::runtime_error("No more elements in the iteration.");
  }
}

} // namespace winshell
} // namespace xbelmark
