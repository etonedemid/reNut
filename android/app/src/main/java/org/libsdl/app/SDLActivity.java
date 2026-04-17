package org.libsdl.app;

import android.app.Activity;
import android.view.Surface;

/**
 * Minimal SDLActivity stub.
 *
 * SDL3's native JNI layer (SDL_android.c) looks up static methods on a class
 * named "org/libsdl/app/SDLActivity" and caches them via nativeSetupJNI().
 * We don't use SDLActivity as our activity, but the native joystick / gamepad
 * subsystem still dereferences the cached class pointer (mActivityClass) when
 * it needs the Android Context (e.g. for internal-storage path lookups).
 *
 * Only the methods actually *called* at runtime need real implementations;
 * everything else can be a no-op / null-return so that GetStaticMethodID
 * succeeds and the jmethodID cache is populated.
 */
public class SDLActivity {

    private static Activity mActivity;

    /** Call once from your real Activity.onCreate() to wire things up. */
    public static void setActivity(Activity activity) {
        mActivity = activity;
    }

    // ── Called by SDL_android.c nativeSetupJNI() ──────────────────────
    public static native void nativeSetupJNI();

    // ── Methods looked up by GetStaticMethodID ────────────────────────

    public static Activity getContext() {
        return mActivity;
    }

    public static Surface getNativeSurface() {
        return null;            // we manage the surface ourselves
    }

    public static boolean getManifestEnvironmentVariables() {
        return false;
    }

    public static void initTouch() {}

    public static boolean isAndroidTV() { return false; }
    public static boolean isChromebook() { return false; }
    public static boolean isDeXMode() { return false; }
    public static boolean isTablet() { return false; }

    public static String clipboardGetText() { return ""; }
    public static boolean clipboardHasText() { return false; }
    public static void clipboardSetText(String text) {}

    public static int createCustomCursor(int[] colors, int w, int h,
                                          int hotX, int hotY) { return 0; }
    public static void destroyCustomCursor(int id) {}
    public static boolean setCustomCursor(int id) { return false; }
    public static boolean setSystemCursor(int id) { return false; }

    public static void manualBackButton() {}
    public static void minimizeWindow() {}
    public static boolean openURL(String url) { return false; }
    public static void requestPermission(String perm, int reqCode) {}
    public static boolean showToast(String msg, int dur, int grav,
                                     int xOff, int yOff) { return false; }
    public static boolean sendMessage(int cmd, int param) { return false; }
    public static boolean setActivityTitle(String title) { return false; }
    public static void setOrientation(int w, int h, boolean resizable,
                                       String hint) {}
    public static boolean setRelativeMouseEnabled(boolean enabled) { return false; }
    public static void setWindowStyle(boolean fullscreen) {}
    public static boolean shouldMinimizeOnFocusLoss() { return false; }
    public static boolean showTextInput(int x, int y, int w, int h,
                                         int type) { return false; }
    public static boolean supportsRelativeMouse() { return false; }
    public static int openFileDescriptor(String path, String mode) { return -1; }
    public static boolean showFileDialog(String[] filters, boolean allowAll,
                                          boolean multiSelect, int mode) { return false; }
    public static String getPreferredLocales() { return "en"; }
}
