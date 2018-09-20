#include "VLDFvalidationinput.h"

VLDFValidationInput::VLDFValidationInput(QLineEdit *lineEdit,
                                         VLDFValidationController *controller,
                                         std::function<bool(void)> aditionalValidationRule,
                                         const QString additionalErrorMessage) :
    QObject(Q_NULLPTR),
    mLineEdit(lineEdit),
    mValidationTimer(),
    mAdditionalValidationRule(aditionalValidationRule),
    mAdditionalErrorMessage(additionalErrorMessage)
{
	mMustValidate = true; //false will keep from being validated
	mValidationTimer.setSingleShot(true);

    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkValidity()));
    connect(&mValidationTimer, SIGNAL(timeout()), lineEdit, SIGNAL(editingFinished()));
    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(changeLineEditValidationInfo()));
    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(checkDependencies()));

    mTimerConnection = connect(lineEdit, &QLineEdit::textEdited, [this] {
        clearValidationInfo();
        const int halfSecondInMilliseconds = 500;
        mValidationTimer.start(halfSecondInMilliseconds);
    });

    mPopupConnection = connect(mLineEdit, &QLineEdit::editingFinished, [this, controller] {
        if(!isValid()) {
            controller->showErrorMessage(this);
        }
    });

    connect(this, &VLDFValidationInput::destroyed, [this, controller] {
        mLineEdit->removeEventFilter(controller);
    });
}

/**
 * @brief Slot called when the content of the line edit changes. it emits a signal
 * informing if the lineedit's content is valid according to the user defined rules.
 */
void VLDFValidationInput::checkValidity() {
	emit validityChanged(isValid()); // aciona SLOT VLDFValidationController::checkInputsValidity(bool) 
}

VLDFValidationInput::~VLDFValidationInput() {
    disconnect(mLineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkValidity()));
    disconnect(mLineEdit, SIGNAL(editingFinished()), this, SLOT(changeLineEditValidationInfo()));
    disconnect(mLineEdit, SIGNAL(editingFinished()), this, SLOT(checkDependencies()));
    QObject::disconnect(mTimerConnection);
    QObject::disconnect(mPopupConnection);
    clearValidationInfo();
}

/**
 * @brief helper function to check if the content of the lineedit follows the rules set by the user
 * @return true, if the content respect the user's rules, else false
 */
bool VLDFValidationInput::isValid() const {
	if (mMustValidate)
		return mLineEdit->hasAcceptableInput() && mAdditionalValidationRule();
	else
		return true; 
}

void VLDFValidationInput::checkDependencies() {
    for(VLDFValidationInput *depency: mDependencies) {
        depency->updateValidation();
    }
}

void VLDFValidationInput::updateValidation() {
    if(mLineEdit->actions().size() != 0) {
        changeLineEditValidationInfo();
    }
}

/**
 * @brief Removes the validation icons at this input
 */
void VLDFValidationInput::clearValidationInfo()  {
    QList<QAction *> actions = mLineEdit->actions();
    for(QAction *action : actions) {
        mLineEdit->removeAction(action);
    }
    qDeleteAll(actions);
}

bool VLDFValidationInput::attendsRecomendation() const {
    return true;
}

bool VLDFValidationInput::appendDependency(VLDFValidationInput *dependencyField) {
    if(!dependencyField)
        return false;
    mDependencies.append(dependencyField);
	return true;
}

/**
 * @brief changes the validation information/icons of an input based on it's current state.
 * It's called when QLineEdit::editingFinished signal is emited, either by the user changing
 * focus to another field or when the validation timer QTimer::timeOut signal is emited half
 * second after the user typed.
 */
bool VLDFValidationInput::changeLineEditValidationInfo() {
    if(mLineEdit)
	{
        if(mLineEdit->actions().size() != 0) 
		{
            clearValidationInfo();
        }
        QAction *newAction = new QAction();
        if(this->isValid())
		{
            if(attendsRecomendation())
			{
                newAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
				emit validityChanged(true); // invokes SLOT VLDFValidationController::checkInputsValidity(const bool)
			} 
			else
			{
                newAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            }
        } 
		else
		{
            newAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
			emit validityChanged(false); // aciona SLOT VLDFValidationController::checkInputsValidity(const bool)
		}
        mLineEdit->addAction(newAction, QLineEdit::TrailingPosition);
		emit validationInfoChanged();
		return true;
    }
	else 
        return false;
    
}