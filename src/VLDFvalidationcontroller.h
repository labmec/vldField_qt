#ifndef VLDFVALIDATIONCONTROLLER_H
#define VLDFVALIDATIONCONTROLLER_H

#include <QHash>
#include <QLineEdit>
#include <QTimer>
#include <QMetaObject>
#include <QAction>
#include <QStyle>
#include <QApplication>
#include <QObject>
#include "VLDFpopupmessage.h"

class QAbstractButton;
class VLDFValidationInput;

class VLDFValidationController : public QObject
{
    Q_OBJECT

public:
    explicit VLDFValidationController(QObject *parent);
    ~VLDFValidationController();
    bool addInput(VLDFValidationInput *input);
    bool registerLineEdit(QLineEdit *lineEdit, std::function<bool(void)> aditionalValidationRule = [] () -> bool { return true; }, const QString additionalErrorMessage = "");
    bool unregisterLineEdit(QLineEdit *lineEdit);
    bool addControlButton(QAbstractButton *button);
    void clearValidationInfo();
    void unregisterAllLineEdits();
    bool validateInputs();
    void showErrorMessage(VLDFValidationInput *input);
    void setPopupMessageEnabled(bool enable);
    bool appendDependency(QLineEdit *dependantField, QLineEdit *dependencyField);

    bool softRegisterLineEdit(QLineEdit *lineEdit);
    void softUnregisterAllLineEdits();
    void updateButtonsState(bool newValidState);

    bool isCurrentStateValid;


public slots:
    void checkInputsValidity(const bool inputBecameValid);
    void setControlButtonsEnabled(bool enable);



private:
    // TODO change this to a hash of LineEdit to TPZAutoPointer
    QHash<QLineEdit *, VLDFValidationInput*> mInputs;
    QList<QAbstractButton *> mControlButtons;
    VLDFPopupMessage mPopup;
    bool mIsPopupEnabled;


    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // VLDFVALIDATIONCONTROLLER_H
