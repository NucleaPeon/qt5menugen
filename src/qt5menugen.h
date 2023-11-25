#ifndef QTMENUGEN_H
#define QTMENUGEN_H

#include "qt5menugen_global.h"

#include <QMainWindow>
#include <QMetaMethod>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QFile>
#include <QUrl>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QMap>

#ifdef Q_OS_MAC
#include <QMacToolBar>
#include <QMacToolBarItem>
#endif

#ifdef Q_OS_LINUX
#include <QToolBar>
#elif Q_OS_WIN
#include <QToolBar>
#endif

/*!
 * \brief The QtMenuGen class contains functionality to autoconfigure your toolbars and menus.
 *
 * A Json file defines how a menu is configured so it can be reproduced on all operating systems in similar fashion:
 *
 *   OS X = QMacToolBar
 *   Linux and Windows = QToolBar
 *   All: QMenu and QAction
 *
 * Shortcuts:
 *
 * To allow certain shortcuts to be accessed, we enable the string "QKeySequence::_______" to be used
 * in json to allow the appropriate \sa QKeySequence::StandardKey as the shortcut.
 * Instead of "Ctrl+P", "QKeySequence::Print" can be used and is recommended as it's more platform-
 * independent and readable.
 *
 * I'm looking at getting Qt::Key enum to be utilized within this library but so far I've been
 * unsuccessful. Even with the appropriate hex values being given to QKeySequence, it doesn't work
 * for shortcuts; perhaps it's unsuitable for this purpose and will be ignored.
 *
 * Since the Qt::Key and QKeySequence::StandardKey enums are in namespaces and not QObject's, there is
 * no way to access them by a string repr of the enum key (at least respecting the < c++-11 standard.)
 * As there haven't been any differences between 5.3.2 and 5.15 between enums, I've copied the enums
 * and their values as a QMap<QString, int>.
 */

class QT5MENUGENSHARED_EXPORT QtMenuGen
{

public:
    /*!
     * \brief QtMenuGen Object based on a QFile path
     * \param path QString local file
     */
    explicit QtMenuGen(QString path);
    /*!
     * \brief QtMenuGen Object based on a QUrl path
     *
     * Currently only local files are implemented
     *
     * \param path QUrl local file
     */
    QtMenuGen(QUrl path);
    ~QtMenuGen();

    /*!
     * \brief loadFile will explicitly load the Json file, such as scenarios where no toolbar or menu setup is required
     * \param path QString local file
     * \return bool whether file was loaded successfully
     */
    bool loadFile(QString path);
    /*!
     * \brief loadFile will explicitly load the Json file, such as scenarios where no toolbar or menu setup is required
     * \param path QUrl local file
     * \return bool whether file was loaded successfully
     */
    bool loadFile(QUrl path);

    /*!
     * \brief setup configures the Toolbar and Menus on the QWidget and looks for slot functions in the slotobj.
     *
     * This is used when displaying QWidgets, non-QMainWindow user interfaces.
     *
     * \code{.cpp}
     * #include <qtmenugen.h>
     *
     * // ...
     * {
     *      QFile path(":/files/menu.json");
     *      this->qtmg = QtMenuGen(&path);
     *      this->qtmg.setup(this, this);
     * }
     *
     * \endcode
     *
     * \param widget QWidget*
     * \param slotobj QObject*
     */
    void setup(QWidget *widget, QObject *slotobj);

    /*!
     * \brief setup configures the Toolbar and Menus on the QMainWindow and looks for slot functions in the slotobj.
     *
     * \code{.cpp}
     * #include <qtmenugen.h>
     *
     * // ...
     * {
     *      QFile path(":/files/menu.json");
     *      this->qtmg = QtMenuGen(&path);
     *      this->qtmg.setup(this, this);
     * }
     *
     * \endcode
     *
     * \param widget QWidget*
     * \param slotobj QObject*
     */
    void setup(QMainWindow *window, QObject *slotobj);

    /*!
     * \brief actionByName Return the QAction* object based on the name assigned to it in the json file
     *
     * \param name QString
     * \return QAction*
     */
    QAction* actionByName(const QString name);
    /*!
     * \brief menuByName Return the QMenu* object based on the name assigned to it in the json file
     *
     * \param name QString
     * \return QMenu*
     */
    QMenu* menuByName(const QString name);

    bool isLoaded();

    const QMap<QString, int> getShortcuts();


private:
    bool loaded;
    bool configured;
    QMap<QString, int> shortcuts;
    QJsonDocument jdoc;
    QMap<QString, QAction*> action_map;
    QMap<QString, QActionGroup*> group_map;
    QMap<QString, QMenu*> menu_map;
    QMenuBar* mb;

    QMenuBar* setupMenus(QWidget *widget);
    QMap<QString, int> load_shortcuts();
    void handleSignalSlot(QObject *connector, const char *signal, QObject *caller, const char *slot);
    bool isValid(const QJsonObject obj);

#ifdef Q_OS_MAC
    QMacToolBar *tb;
    QMacToolBar* setupOSXToolBar(QWidget *widget, QObject *slotobj);
    QMacToolBarItem* toolBarItemByText(QString text);
#else
    QToolBar *tb;
    QAction* toolBarItemByText(QString text);
    QToolBar* setupToolBar(QWidget *widget, QObject *slotobj);
#endif
};

#endif // QTMENUGEN_H
