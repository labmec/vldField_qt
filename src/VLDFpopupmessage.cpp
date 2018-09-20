#include "VLDFpopupmessage.h"

#include <QShowEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>

enum Opacity {
    MIN_OPACITY = 0,
    MAX_OPACITY = 1
};

VLDFPopupMessage::VLDFPopupMessage(QWidget *parent) :
    QWidget(parent),
    mTextMessage("Popup Message"),
    mBottomBandColor(QBrush(Qt::red)),
    mTextColor(QColor(Qt::white)),
    mTipHeight(8),
    mPadding(8),
    mBottomBandHeight(3),
    mFadeInAnimation(),
    mFadeOutAnimation(),
    mOpacityEffect(this)
{
    setVisible(false);
    mTextFont.setBold(false);
    mTextFont.setPointSize(10);
    setGeometry(x(), y(), width(), height());
    QApplication::instance()->installEventFilter(this);

    setGraphicsEffect(&mOpacityEffect);

    mFadeInAnimation.setTargetObject(&mOpacityEffect);
    mFadeInAnimation.setPropertyName("opacity");
    mFadeInAnimation.setEasingCurve(QEasingCurve::Linear);
    mFadeInAnimation.setDuration(150);
    mFadeInAnimation.setStartValue(0.0);
    mFadeInAnimation.setEndValue(1.0);

    mFadeOutAnimation.setTargetObject(&mOpacityEffect);
    mFadeOutAnimation.setPropertyName("opacity");
    mFadeOutAnimation.setEasingCurve(QEasingCurve::Linear);
    mFadeOutAnimation.setDuration(150);
    mFadeOutAnimation.setStartValue(1.0);
    mFadeOutAnimation.setEndValue(0.0);

    connect(&mFadeOutAnimation, &QPropertyAnimation::finished, [this] {
        QWidget::hide();
    });
}

VLDFPopupMessage::~VLDFPopupMessage() {

}

void VLDFPopupMessage::setMessage(const QString &message) {
    mTextMessage = message;
    setGeometry(x(), y(), width(), height());
    update();
}

void VLDFPopupMessage::setMessageType(MessageType type) {
    switch (type) {
    case NormalMessage:
        mBottomBandColor = QBrush(Qt::black);
        break;
    case ErrorMessage:
        mBottomBandColor = QBrush(Qt::red);
        break;
    case WarningMessage:
        mBottomBandColor = QBrush(Qt::yellow);
        break;
    default:
        //DebugStop();
        break;
    }
    update();
}

double VLDFPopupMessage::height() const {
    if(mTextMessage.contains('\n')) {
        return mTextMessage.count('\n') * QFontMetrics(mTextFont).height() + (2 * mPadding) + mTipHeight;
    } else {
        return QFontMetrics(mTextFont).height() + (2 * mPadding) + mTipHeight;
    }
}

double VLDFPopupMessage::width() const {
    double greaterLineWidth = 0.;
    QStringList lines = mTextMessage.split('\n');
    for(int i = 0;i < lines.size(); i++) {
        const double lineSize = QFontMetrics(mTextFont).width(lines.at(i));
        greaterLineWidth = std::max(greaterLineWidth, lineSize);
    }
    return greaterLineWidth + (2 * mPadding);
}

void VLDFPopupMessage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(Qt::black)));

    QVector<QPoint> triangleTipPoints;
    triangleTipPoints.append(QPoint(rect().x() + 32, rect().y() + height()));
    const int triangleTipTopY = rect().y() + height() - mTipHeight - 1;
    triangleTipPoints.append(QPoint(rect().x() + 16, triangleTipTopY));
    triangleTipPoints.append(QPoint(rect().x() + 48, triangleTipTopY));

    painter.setBrush(mBottomBandColor);
    painter.drawRect(rect().x(), rect().y(), width(), height() - mTipHeight);
    painter.drawPolygon(QPolygon(triangleTipPoints));

    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(rect().x(), rect().y(), width(), height() - mTipHeight - mBottomBandHeight);

    painter.restore();

    painter.translate(mPadding, mPadding);
    QRectF textRect = QRectF(0, 0, width() - mPadding, height() - mTipHeight - mPadding);
    painter.setFont(mTextFont);
    painter.setPen(mTextColor);
    painter.drawText(textRect, mTextMessage);
}

bool VLDFPopupMessage::eventFilter(QObject *watched, QEvent *event) {
    if(isVisible()) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(event->type() == QEvent::MouseButtonPress) {
            hide();
        } else if (keyEvent && keyEvent->key() == Qt::Key_Escape) {
            hide();
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}

void VLDFPopupMessage::showEvent(QShowEvent *event) {
    Q_UNUSED(event);

    setWindowOpacity(0.0);
    QWidget::show();

    mFadeInAnimation.start();
}

void VLDFPopupMessage::hideEvent(QHideEvent *event)
{
    event->ignore();
    mFadeOutAnimation.start();
}
