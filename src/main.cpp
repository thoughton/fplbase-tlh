// Copyright 2015 Google Inc. All rights reserved.
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

// This is a wrapper around main, as SDL needs to modify the main function
// directly.  Users of FPLBase should declare "FPL_main" which this will call.

#ifdef FPL_BASE_BACKEND_SDL
#include "SDL_main.h"
#endif

extern int FPL_main(int argc, char* argv[]);

int main(int argc, char* argv[]) { return FPL_main(argc, argv); }
