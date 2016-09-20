/*
  Offscreen Android Views library for Qt

  Author:
  Vyacheslav O. Koscheev <vok1980@gmail.com>

  Distrbuted under The BSD License

  Copyright (c) 2015, DoubleGIS, LLC.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.
  * Neither the name of the DoubleGIS, LLC nor the names of its contributors
	may be used to endorse or promote products derived from this software
	without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "QAndroidCompass.h"

#include <QAndroidQPAPluginGap.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QMutexLocker>



Q_DECL_EXPORT void JNICALL Java_setAzimuth(JNIEnv * env, jobject, jlong inst, jfloat azimuth)
{
	Q_UNUSED(env);

	try
	{
		if (inst)
		{
			void * vp = reinterpret_cast<void*>(inst);
			QAndroidCompass * proxy = reinterpret_cast<QAndroidCompass*>(vp);
			proxy->setAzimuth(azimuth);
			return;
		}
		else
		{
			qWarning() << __FUNCTION__ << "Zero param!";
		}
	}
	catch (std::exception & e)
	{
		qWarning() << __FUNCTION__ << " exception: " << e.what();
	}
}


static const JNINativeMethod methods[] = {
	{"getContext", "()Landroid/content/Context;", (void*)QAndroidQPAPluginGap::getCurrentContext},
	{"setAzimuth", "(JF)V", (void*)Java_setAzimuth},
};


QAndroidCompass::QAndroidCompass()
	: JniObjectLinker(reinterpret_cast<void*>(this), "ru/dublgis/androidcompass/CompassProvider", methods, sizeof(methods))
{
}


QAndroidCompass::~QAndroidCompass()
{
}


void QAndroidCompass::start(int32_t delayUs /*= -1*/, int32_t latencyUs /*= -1*/)
{
	QJniObject * hdl = handler();

	if (Q_NULLPTR != hdl)
	{
		hdl->callParamVoid("start", "II", (jint)delayUs, (jint)latencyUs);
	}
}


void QAndroidCompass::stop()
{
	QJniObject * hdl = handler();

	if (Q_NULLPTR != hdl)
	{
		hdl->callVoid("stop");
	}
}


void QAndroidCompass::resetAzimuthListener(const QWeakPointer<AzimuthListener> & azimuth_listener)
{
	QMutexLocker lock(&send_mutex_);
	azimuth_listener_ = azimuth_listener;
}


void QAndroidCompass::setAzimuth(float azimuth)
{
	QSharedPointer<AzimuthListener> azimuth_listener;

	{
		QMutexLocker lock(&send_mutex_);
		azimuth_listener = azimuth_listener_.lock();
	}

	if (azimuth_listener)
	{
		azimuth_listener->azimuthChanged(azimuth);
	}
	else
	{
		stop();
	}
}


