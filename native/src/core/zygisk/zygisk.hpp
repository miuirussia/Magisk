#pragma once

#include <sys/mman.h>
#include <stdint.h>
#include <jni.h>
#include <vector>
#include <core.hpp>

namespace ZygiskRequest {
enum : int {
    GET_INFO,
    CONNECT_COMPANION,
    GET_MODDIR,
    SULIST_ROOT_NS,
    REVERT_UNMOUNT,
    END
};
}

#if defined(__LP64__)
#define ZLOGD(...) LOGD("zygisk64: " __VA_ARGS__)
#define ZLOGE(...) LOGE("zygisk64: " __VA_ARGS__)
#define ZLOGI(...) LOGI("zygisk64: " __VA_ARGS__)
#define ZLOGW(...) LOGW("zygisk64: " __VA_ARGS__)
#else
#define ZLOGD(...) LOGD("zygisk32: " __VA_ARGS__)
#define ZLOGE(...) LOGE("zygisk32: " __VA_ARGS__)
#define ZLOGI(...) LOGI("zygisk32: " __VA_ARGS__)
#define ZLOGW(...) LOGW("zygisk32: " __VA_ARGS__)
#endif

extern void *self_handle;

extern int system_server_fd;

void hook_functions();
int remote_get_info(int uid, const char *process, uint32_t *flags, std::vector<int> &fds);
int remote_request_sulist();
int remote_request_umount();

inline int zygisk_request(int req) {
    int fd = connect_daemon(+RequestCode::ZYGISK);
    if (fd < 0) return fd;
    write_int(fd, req);
    return fd;
}

#if USE_PTRACE != 1

// The reference of the following structs
// https://cs.android.com/android/platform/superproject/main/+/main:art/libnativebridge/include/nativebridge/native_bridge.h

struct NativeBridgeRuntimeCallbacks {
    const char* (*getMethodShorty)(JNIEnv* env, jmethodID mid);
    uint32_t (*getNativeMethodCount)(JNIEnv* env, jclass clazz);
    uint32_t (*getNativeMethods)(JNIEnv* env, jclass clazz, JNINativeMethod* methods,
                                 uint32_t method_count);
};

struct NativeBridgeCallbacks {
    uint32_t version;
    void *padding[5];
    bool (*isCompatibleWith)(uint32_t);
};

#endif

bool trace_zygote(int pid, const char *libpath);
