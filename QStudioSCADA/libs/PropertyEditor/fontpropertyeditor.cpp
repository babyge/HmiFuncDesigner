#include "fontpropertyeditor.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QFontDialog>
#include <QDebug>

FontPropertyEditor::FontPropertyEditor(QWidget *parent) :
    QWidget(parent) {
    // Create the tool button
    toolButton_ = new QToolButton(this);
    toolButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    toolButton_->setText(tr("..."));
    toolButton_->setFixedWidth(20);
    toolButton_->installEventFilter(this);
    setFocusProxy(toolButton_);
    setFocusPolicy(toolButton_->focusPolicy());
    connect(toolButton_, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    // Create the text label
    textLabel_ = new QLabel(this);
    textLabel_->setText(getFontString(font_));

    // Spacer (this is needed for proper display of the label and button)
    spacer_ = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    // The layout (a horizontal layout)
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addItem(spacer_);
    layout->addWidget(toolButton_);
    textLabel_->hide();
}

void FontPropertyEditor::setFont(const QFont& font) {
    if (font_ != font) {
        font_ = font;
        textLabel_->setText(getFontString(font_));
    }
}


QString FontPropertyEditor::getFontString(const QFont& font) {
    return font.toString();
}

void FontPropertyEditor::onToolButtonClicked() {
    bool ok = false;
    QFont oldFont = font_;
    QFont newFont = QFontDialog::getFont(&ok, oldFont, this);
    if (ok && oldFont != newFont) {
        setFont(newFont);
        emit dataChangedByUser(newFont, this);
    }
}

bool FontPropertyEditor::eventFilter(QObject *obj, QEvent *ev) {
    if(obj == toolButton_ && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress)) {
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

QFont FontPropertyEditor::getFont() const {
    return font_;
}

