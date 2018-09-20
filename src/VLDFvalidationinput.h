#ifndef VLDFVALIDATIONINPUT_H
#define VLDFVALIDATIONINPUT_H

#include "VLDFvalidationcontroller.h"
#include <QObject>
#include <QLineEdit>
#include <functional>

class VLDFValidationInput : public QObject
{
    Q_OBJECT

public:
    VLDFValidationInput(QLineEdit *lineEdit,
                        VLDFValidationController *controller,
                        std::function<bool(void)> aditionalValidationRule = [] () -> bool { return true; },
                        const QString additionalErrorMessage = "");

    ~VLDFValidationInput();

    QString &additionalErrorMessage() { return mAdditionalErrorMessage; }
    QLineEdit *lineEdit() const { return mLineEdit; }
    void clearValidationInfo();
    bool isValid() const;
    bool attendsRecomendation() const;
    bool appendDependency(VLDFValidationInput *dependencyField);
	std::function<bool(void)> mAdditionalValidationRule;
	QString mAdditionalErrorMessage;
	bool mMustValidate;

signals:
    void validityChanged(const bool isValid);
    void validationInfoChanged();

private:
    void updateValidation();

private slots:
    bool changeLineEditValidationInfo();
    void checkValidity();
    void checkDependencies();

private:
    QLineEdit *mLineEdit;
    QTimer mValidationTimer;
    QMetaObject::Connection mTimerConnection;
    QMetaObject::Connection mPopupConnection;
    QList<VLDFValidationInput*> mDependencies;

};

#endif // VLDFVALIDATIONINPUT_H
