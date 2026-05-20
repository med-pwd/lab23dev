#include <jni.h>
#include <string>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <android/log.h>

#define LOG_TAG "AntiDebugNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/**
 * Check 1: Ptrace check.
 * If ptrace(PTRACE_TRACEME) fails, it usually means a debugger is already attached.
 */
bool isPtraceDetected() {
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
        LOGW("AntiDebug: Ptrace detection triggered!");
        return true;
    }
    // Detach if we succeeded in traceme (just in case, though usually not needed if we exit or don't continue)
    ptrace(PTRACE_DETACH, 0, 1, 0);
    return false;
}

/**
 * Check 2: TracerPid check in /proc/self/status.
 * If TracerPid is non-zero, a debugger is attached.
 */
bool isTracerPidDetected() {
    int status_fd = open("/proc/self/status", O_RDONLY);
    if (status_fd == -1) return false;

    char buffer[1024];
    ssize_t num_read = read(status_fd, buffer, sizeof(buffer) - 1);
    close(status_fd);

    if (num_read <= 0) return false;
    buffer[num_read] = '\0';

    const char* tracer_pid_str = "TracerPid:";
    char* pos = strstr(buffer, tracer_pid_str);
    if (pos) {
        int tracer_pid = atoi(pos + strlen(tracer_pid_str));
        if (tracer_pid != 0) {
            LOGW("AntiDebug: TracerPid detection triggered! TracerPid: %d", tracer_pid);
            return true;
        }
    }
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_jni_MainActivity_isDebugDetected(JNIEnv *env, jobject /* this */) {
    LOGI("AntiDebug: Running security checks...");

    bool ptraceDetected = isPtraceDetected();
    bool tracerPidDetected = isTracerPidDetected();

    if (ptraceDetected || tracerPidDetected) {
        LOGE("AntiDebug: DEBUGGER DETECTED!");
        return JNI_TRUE;
    }

    LOGI("AntiDebug: No debugger detected. System secure.");
    return JNI_FALSE;
}
