/*
* Copyright Disney Enterprises, Inc.  All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License
* and the following modification to it: Section 6 Trademarks.
* deleted and replaced with:
*
* 6. Trademarks. This License does not grant permission to use the
* trade names, trademarks, service marks, or product names of the
* Licensor and its affiliates, except as required for reproducing
* the content of the NOTICE file.
*
* You may obtain a copy of the License at
* http://www.apache.org/licenses/LICENSE-2.0
*
* @file ExprEditor.h
* @brief This provides an expression editor for SeExpr syntax with auto ui features
* @author  aselle
*/
#ifndef ExprEditor_h
#define ExprEditor_h

#include <vector>

#include <QTextBrowser>
#include <QPlainTextEdit>
#include <QDialog>
#include <QTimer>
#include <QRegExp>
#include <QLineEdit>
#include <QCheckBox>
#include <QSlider>

class QLabel;
class QPushButton;
class QLineEdit;
class QMouseEvent;
class QPaintEvent;
class QKeyEvent;
class QCompleter;
class QToolTip;
class QListWidget;
class QListWidgetItem;
class ExprCompletionModel;
class ExprControl;
class ExprControlCollection;

class ExprEditor;
class ExprCompletionModel;
class ExprHighlighter;
class ExprPopupDoc;

class ExprTextEdit : public QTextEdit {
    Q_OBJECT

    QToolTip* functionTip;
    std::map<std::string, std::string> functionTooltips;
    ExprHighlighter* highlighter;
    QStyle* lastStyleForHighlighter;
    ExprPopupDoc* _tip;
    QAction* _popupEnabledAction;

  public:
    QCompleter* completer;
    ExprCompletionModel* completionModel;

  public:
    ExprTextEdit(QWidget* parent = 0);
    ~ExprTextEdit();
    void updateStyle();

  protected:
    void showTip(const QString& string);
    void hideTip();

    virtual void keyPressEvent(QKeyEvent* e);
    void focusInEvent(QFocusEvent* e);
    void focusOutEvent(QFocusEvent* e);
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* e);
    void wheelEvent(QWheelEvent* e);
    void contextMenuEvent(QContextMenuEvent* event);

  private
slots:
    void insertCompletion(const QString& completion);
signals:
    void applyShortcut();
    void nextError();
};

class ExprEditor : public QWidget {
    Q_OBJECT

  public:
    ExprEditor(QWidget* parent, ExprControlCollection* controls);
    virtual ~ExprEditor();

  public
slots:
    void exprChanged();
    void rebuildControls();
    void controlChanged(int id);
    void nextError();
    void selectError();
    void sendApply();
    void sendPreview();
// void handlePreviewTimer();
signals:
    void apply();
    void preview();

  public:
    // Get the expression that is in the editor
    std::string getExpr();
    // Sets the expression that is in the editor
    void setExpr(const std::string& expression, const bool apply = false);
    // Append string
    void appendStr(const std::string& str);
  public
slots:
    // Insert string
    void insertStr(const std::string& str);

  public:
    // Adds an error and its associated position
    void addError(const int startPos, const int endPos, const std::string& error);
    // Removes all errors and hides the completion widget
    void clearErrors();
    // Removes all extra completion symbols
    void clearExtraCompleters();
    // Registers an extra function and associated do cstring
    void registerExtraFunction(const std::string& name, const std::string& docString);
    // Register an extra variable (i.e. $P, or $u, something provided by resolveVar)
    void registerExtraVariable(const std::string& name, const std::string& docString);
    // Replace extras
    void replaceExtras(const ExprCompletionModel& completer);
    // Updates the completion widget, must call after registering any new functions/variables
    void updateCompleter();
    // Updates style
    void updateStyle();

    // Get underlaying text editor
    ExprTextEdit* getExprTextEditor() const { return exprTe; }

  private:
    ExprTextEdit* exprTe;
    ExprControlCollection* controls;
    QListWidget* errorWidget;

    QTimer* controlRebuildTimer;
    QTimer* previewTimer;

    bool _updatingText;
    int errorHeight;
};

#endif
