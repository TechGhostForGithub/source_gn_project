#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_RW_LOCK_WRAPPER_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_RW_LOCK_WRAPPER_H_

// Note, Windows pre-Vista version of RW locks are not supported natively. For
// these OSs regular critical sections have been used to approximate RW lock
// functionality and will therefore have worse performance.

namespace gn {

class RWLockWrapper {
 public:
  static RWLockWrapper* CreateRWLock();
  virtual ~RWLockWrapper() {}

  virtual void AcquireLockExclusive() = 0;
  virtual void ReleaseLockExclusive() = 0;

  virtual void AcquireLockShared() = 0;
  virtual void ReleaseLockShared() = 0;
};

// RAII extensions of the RW lock. Prevents Acquire/Release missmatches and
// provides more compact locking syntax.
class ReadLockScoped {
 public:
  ReadLockScoped(RWLockWrapper& rw_lock)
    :
    rw_lock_(rw_lock) {
    rw_lock_.AcquireLockShared();
  }

  ~ReadLockScoped() {
    rw_lock_.ReleaseLockShared();
  }

 private:
  RWLockWrapper& rw_lock_;
};

class WriteLockScoped {
 public:
  WriteLockScoped(RWLockWrapper& rw_lock)
    :
    rw_lock_(rw_lock) {
    rw_lock_.AcquireLockExclusive();
  }

  ~WriteLockScoped() {
    rw_lock_.ReleaseLockExclusive();
  }

 private:
  RWLockWrapper& rw_lock_;
};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_RW_LOCK_WRAPPER_H_
