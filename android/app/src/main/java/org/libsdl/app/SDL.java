package org.libsdl.app;

import android.content.Context;

public class SDL {
    private static Context mContext;

    public static void setContext(Context context) {
        mContext = context;
    }

    public static Context getContext() {
        return mContext;
    }
}
