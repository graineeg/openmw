/*
 * androidcontrolshider.cpp
 *
 *  Created on: 4 окт. 2016 г.
 *      Author: sandstranger
 */
#include <jni.h>
#include <android/vmstorage.hpp>
#include <android/androidcontrolshider.hpp>

#define THREAD_SLEEP_TIME 1
static AndroidControlsHider *controlsHelper;

void AndroidControlsHider::sleep() {
	boost::this_thread::sleep_for(boost::chrono::seconds { THREAD_SLEEP_TIME });
}

void AndroidControlsHider::runnable() {
	while (!controlsHelper->needStopBackgroundTask) {
		sleep();
		controlsHelper->updateCursorVisibility();
	}
}

void AndroidControlsHider::updateCursorVisibility() {
	MWBase::WindowManager *mwWindow =
			MWBase::Environment::get().getWindowManager();
	if (mwWindow && cursorVisible != mwWindow->getCursorVisible()) {
		cursorVisible = mwWindow->getCursorVisible();
		if (!vmListener) {
			vmListener = new VMNativeListener();
		}
		vmListener->updateControlsState(cursorVisible);
	}

}

void AndroidControlsHider::startBackdroundTask() {
	this->thr = boost::thread(runnable);
}

VMNativeListener::VMNativeListener() {
  VMStorage::getVM()->AttachCurrentThread((JNIEnv **) &myEnv, NULL);
 _detach = true;
}

VMNativeListener::~VMNativeListener() {
	if (_detach) {
		VMStorage::getVM()->DetachCurrentThread();
	}
}

void VMNativeListener::updateControlsState(bool cursorVisibility) {
	myEnv->CallStaticVoidMethod(VMStorage::getCursorClass(), VMStorage::getUpdateControlsMethod(), cursorVisibility);
}

extern "C" {
JNIEXPORT void JNICALL Java_cursor_CursorVisibility_InitBackgroundTask(
		JNIEnv *env, jobject obj) {
	if (!controlsHelper) {
		controlsHelper = new AndroidControlsHider();
		controlsHelper->startBackdroundTask();
	}
}
}

extern "C" {
JNIEXPORT void JNICALL Java_cursor_CursorVisibility_StopBackgroundTask(
		JNIEnv *env, jobject obj) {
	if (controlsHelper) {
		controlsHelper->needStopBackgroundTask = true;
	}
}
}

