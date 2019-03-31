/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This source code is licensed under the MIT License,
 * and the license can be found at: https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * This source code belongs to project Mojoc which is a pure C game engine hosted on GitHub,
 * and the details can be found at: https://github.com/scottcgi/Mojoc
 *
 * The author information can be found at:
 * https://github.com/scottcgi
 *
 * The project Mojoc is a serious project with extreme code pursuit,
 * and will continue to iterate.
 *
 * Since : 2013-3-12
 * Update: 2019-2-23
 * Author: scott.cgi
 */


#include "Engine/Toolkit/Platform/Platform.h"


#ifdef IS_PLATFORM_ANDROID
    #include <EGL/egl.h>
#elif defined(IS_PLATFORM_IOS)
    // coding by swift
#endif
