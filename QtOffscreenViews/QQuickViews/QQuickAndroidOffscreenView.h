/*
  Offscreen Android Views library for Qt

  Author:
  Sergey A. Galin <sergey.galin@gmail.com>

  Distrbuted under The BSD License

  Copyright (c) 2014, DoubleGIS, LLC.
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

#pragma once
#include <QSharedPointer>
#include <QFocusEvent>
#include <QtQuick/QQuickFramebufferObject>
#include <QAndroidOffscreenView.h>

/*!
 * Base class for any Android offscreen view.
 */
class QQuickAndroidOffscreenView : public QQuickFramebufferObject
{
    Q_OBJECT
	Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
public:
	QQuickAndroidOffscreenView(QAndroidOffscreenView * aview);
	Renderer * createRenderer() const;

	QColor getBackgroundColor() const { return androidView()->fillColor(); }
	void setBackgroundColor(QColor color);

protected:
	QAndroidOffscreenView * androidView() { return aview_.data(); }
	const QAndroidOffscreenView * androidView() const { return aview_.data(); }

	virtual void focusInEvent(QFocusEvent * event);
	virtual void focusOutEvent(QFocusEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void itemChange(ItemChange change, const ItemChangeData & value);

public slots:
	void updateAndroidViewVisibility();
	void updateAndroidViewPosition();
	void updateAndroidEnabled();

signals:
	void textureUpdated();
	void backgroundColorChanged(QColor color);

protected slots:
	void onTextureUpdated();

private:
	QSharedPointer<QAndroidOffscreenView> aview_;
	bool is_interactive_;
	bool mouse_tracking_;
};

/*!
 * Base class for renderer of any Android offscreen view.
 */
class QAndroidOffscreenViewRenderer : public QObject, public QQuickFramebufferObject::Renderer
{
	Q_OBJECT
public:
	QAndroidOffscreenViewRenderer(QSharedPointer<QAndroidOffscreenView> aview);
	void render();
	QOpenGLFramebufferObject * createFramebufferObject(const QSize & size);
protected slots:
	void onTextureUpdated();
protected:
	QSharedPointer<QAndroidOffscreenView> aview_;
};

