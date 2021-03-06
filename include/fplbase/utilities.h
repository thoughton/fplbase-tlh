// Copyright 2014 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef FPLBASE_UTILITIES_H
#define FPLBASE_UTILITIES_H

#include <string>
#include "fplbase/config.h"  // Must come first.

#include "mathfu/utilities.h"

#if defined(__ANDROID__)
#include <jni.h>
#if defined(FPL_BASE_BACKEND_STDLIB)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif  // defined(FPL_BASE_BACKEND_STDLIB)
#endif

namespace fplbase {

// General utility functions, used by FPLBase, and that might be of use to
// people using the library:
// Constants for use with LogInfo, LogError, etc.
#ifdef FPL_BASE_BACKEND_SDL
enum LogCategory {
  kApplication = 0,  // SDL_LOG_CATEGORY_APPLICATION
  kError = 1,        // SDL_LOG_CATEGORY_ERROR
  kSystem = 3,       // SDL_LOG_CATEGORY_SYSTEM
  kAudio = 4,        // SDL_LOG_CATEGORY_AUDIO
  kVideo = 5,        // SDL_LOG_CATEGORY_VIDEO
  kRender = 6,       // SDL_LOG_CATEGORY_RENDER
  kInput = 7,        // SDL_LOG_CATEGORY_INPUT
  kCustom = 19,      // SDL_LOG_CATEGORY_CUSTOM
};
#else
enum LogCategory {
  kApplication = 0,
  kError,
  kSystem,
  kAudio,
  kVideo,
  kRender,
  kInput,
  kCustom
};
#endif

// Called by LoadFile().
typedef bool (*LoadFileFunction)(const char *filename, std::string *dest);

// Enums for use with the Set/GetPerformanceMode() functions.
enum PerformanceMode {
  // Normal mode.  No special actions taken.
  kNormalPerformance = 0,

  // High performance mode.  Attempt to keep the CPU frequency up.
  kHighPerformance
};

#ifdef __ANDROID__
typedef void (*VsyncCallback)(void);

const int kDefaultAndroidKeycode = 115;  // F24, unavailable on most keyboards.
const double kDefaultTimeBetweenPresses = 1.0;  // Time in seconds

struct HighPerformanceParams {
  HighPerformanceParams()
      : android_key_code(kDefaultAndroidKeycode),
        time_between_presses(kDefaultTimeBetweenPresses) {}
  HighPerformanceParams(int keycode, int presses)
      : android_key_code(keycode), time_between_presses(presses) {}

  int android_key_code;
  double time_between_presses;
};
#endif

// Loads a file and returns its contents via string pointer.
// Returns false if the file couldn't be loaded (usually means it's not
// present, but can also mean there was a read error).
bool LoadFileRaw(const char *filename, std::string *dest);

// Loads a file and returns its contents via string pointer.
// In contrast to LoadFileRaw(), this method simply calls function set by
// SetLoadFileFunction() to read the specified file.
// Returns false if the file couldn't be loaded (usually means it's not
// present, but can also mean there was a read error).
bool LoadFile(const char *filename, std::string *dest);

// Set the function called by LoadFile().  If the specified function is nullptr,
// LoadFileRaw is set as the function.
// Returns the previously set LoadFileFunction.
LoadFileFunction SetLoadFileFunction(LoadFileFunction load_file_function);

// Load a file like LoadFile(), but scan for #include "filename" statements
// at the top of the file (only), and replace them with the contents of
// those files.
// Supports recursive includes, and only ever includes each file once.
// The first non-#include line will terminate #include scanning, though
// blank lines and // comments are correctly ignored.
// If this function returns false, failedfilename indicates which file
// caused the problem.
bool LoadFileWithIncludes(const char *filename, std::string *dest,
                          std::string *failedfilename);

// Save a string to a file, overwriting the existing contents.
// Returns false if the file could not be written.
bool SaveFile(const char *filename, const std::string &data);

// Save binary data to a file, overwriting the existing contents.
bool SaveFile(const char *filename, const void *data, size_t size);

// Load/Save a preference settings.
// The API uses a dedicated API when an optimal API is avaialable instead of
// regular file IO APIs.
// Returns false if the file couldn't be loaded (usually means it's not
// present, but can also mean there was a read error).
bool LoadPreferences(const char *filename, std::string *dest);
bool SavePreferences(const char *filename, const void *data, size_t size);

// Load/Save single integer value to a preference.
int32_t LoadPreference(const char *key, int32_t initial_value);
bool SavePreference(const char *key, int32_t value);

// Search up the directory tree from binary_dir for target_dir, changing the
// working directory to the target_dir and returning true if it's found,
// false otherwise.
bool ChangeToUpstreamDir(const char *const binary_dir,
                         const char *const target_dir);

// Return true if 16bpp MipMap generation is supported.
// (Basically always true, except on certain android devices.)
bool MipmapGeneration16bppSupported();

// Return the system RAM size in MB.
int32_t GetSystemRamSize();

// Basic logging functions.  They will output to the console.
void LogInfo(const char *fmt, va_list args);
void LogError(const char *fmt, va_list args);
void LogInfo(LogCategory category, const char *fmt, va_list args);
void LogError(LogCategory category, const char *fmt, va_list args);
void LogInfo(const char *fmt, ...);
void LogError(const char *fmt, ...);
void LogInfo(LogCategory category, const char *fmt, ...);
void LogError(LogCategory category, const char *fmt, ...);

#if defined(__ANDROID__)
// Returns a pointer to the Java instance of the activity class
// in an Android application.
jobject AndroidGetActivity();

// Returns a pointer to the Java native interface object (JNIEnv) of the
// current thread on an Android application.
JNIEnv *AndroidGetJNIEnv();

// Register for handling vsync callbacks on android.  As with most callbacks,
// this will normally be called on a separate thread, so you'll need to be
// careful about thread-safety with anything you do during the callback.
// Return value is whatever callback was previously registered.  (Or nullptr
// if none.)
VsyncCallback RegisterVsyncCallback(VsyncCallback callback);

// Blocks until the next time a VSync happens.
void WaitForVsync();

// Returns a unique ID representing the frame.  Guaranteed to change
// every time the frame increments.  (May eventually wrap.)
int GetVsyncFrameId();

// Triggers a keypress event on an Android device.
void SendKeypressEventToAndroid(int android_keycode);

// Get the name of the current activity class.  This can be used by C++ code
// to determine how the application was started.
std::string AndroidGetActivityName();

// Determine whether the activity was started with Intent.ACTION_VIEW and if
// so return the data the user wants to "view" in the application.
std::string AndroidGetViewIntentData();
#endif  // __ANDROID__

#if defined(__ANDROID__) && defined(FPL_BASE_BACKEND_STDLIB)
// Provide a pointer to an already-created instance of AAssetManager. Must call
// this function once before loading any assets.
void SetAAssetManager(AAssetManager *manager);
#endif

// Retrieve a path where an app can store data files.
bool GetStoragePath(const char *app_name, std::string *path);

// Checks whether Head Mounted Displays, such as Cardboard, are supported by
// the system being run on.
bool SupportsHeadMountedDisplay();

// Checks if the device has a touchscreen.
bool TouchScreenDevice();

// Checks whether the device we are running on is an Android TV device.
// Always returns false when not running on Android.
bool IsTvDevice();

// Sets the performance mode.
void SetPerformanceMode(PerformanceMode new_mode);

// Returns the current performance mode.
PerformanceMode GetPerformanceMode();

// Relaunch the application.
void RelaunchApplication();

#ifdef __ANDROID__
// Sets the specific parameters used by high-performance mode on Android.
// android_key_code is the key to press repeatedly, to keep the CPU active.
// time_between_presses is the time (in seconds) between keypress events.
void SetHighPerformanceParameters(const HighPerformanceParams &params);

// Returns the current performance parameters, in the form of a struct.
const HighPerformanceParams &GetHighPerformanceParameters();

// Returns the model of the Android device the app is currently being run on.
std::string DeviceModel();
#endif
}  // namespace fplbase

#endif  // FPLBASE_UTILITIES_H
