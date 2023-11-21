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
 * \brief The LoadMenu class is a file spec loader for menu setup.
 *
 * Unlike sbs and sbp files, this is internal only and handles consistency and
 * convenience by using a .json file that defines how a menu is configured so
 * it can be reproduced on all operating systems in similar fashion.
 * OS X = QMacToolBar
 * Linux and Windows = QToolBar
 * Windows 7+ jump lists and toolbar thumbnail buttons: QWinThumbnailToolBar
 *
 *
 * Once a defined spec file is read in, it is cached so menus can be generated repeatedly
 * without having to re-read the file.
 */

class QT5MENUGENSHARED_EXPORT QtMenuGen
{

public:
    QtMenuGen();

    static bool loadFile(QUrl def);
    static void setupToolBarOn(QFile *definition, QWidget *widget, QObject *slotobj = 0);
    static void setupToolBarOn(QFile *definition, QMainWindow *window, QObject *slotobj = 0);

    /*!
     * \brief actionByName Retrieves a QAction based on name defined in the menu_defs.json
     * \param name QString
     * \return QAction* or NULL if not found
     */
    static QAction* actionByName(const QString name);
    static QMenu* menuByName(const QString name);

    static QMenuBar *setupMenus(QWidget *widget);
#ifdef Q_OS_WIN
    static QWinThumbnailToolBar* setupWindowsToolBar(QWidget *widget, QObject *slotobj);
#endif
#ifdef Q_OS_LINUX
    static QToolBar* setupNixToolBar(QWidget *widget, QObject *slotobj);
#endif
#ifdef Q_OS_MAC
    static QMacToolBar* setupOSXToolBar(QWidget *widget, QObject *slotobj);
    static QMacToolBarItem* toolBarItemByText(QString text);
#else
    static QAction* toolBarItemByText(QString text);
#endif

private:
    static void handleSignalSlot(QObject *connector, const char *signal, QObject *caller, const char *slot);
    static bool isValid(const QJsonObject obj);
    /*!
     * \brief To allow certain shortcuts to be accessed, load in Qt::Key enum for parsing
     * Due to the age of c++ standard (pre-11) that this library is compatible with,
     * as well as the enum being defined in a namespace and not a QObject,
     * there is no convenient way to look up a key from an enum as a string.
     * Looking at the evolution of Qt::Key from 5.3.2 to 5.15, there is no
     * difference aside from some spacing and comments, so we've directly copied the
     * entire enum as a string to int map so we can provide the exact values given
     * the standard enum string name.
     * We also incorporate QKeySequence shortcuts into this map for convenience.
     * Any shortcut that has "::" in it will evaluate to an int value, otherwise
     * using "Ctrl+Z" for example, stays a string -- these are not compatible!
     * You cannot use something like "Ctrl+Qt::Key_A".
     * For as much as you can, use the QKeySequence::StandardKey enum for basic
     * shortcuts (ie: "QKeySequence::Copy") until you have need of something more custom.
     */
    static void load_shortcuts();
};

static bool _loaded;
static QJsonDocument _json;
static QMap<QString, QAction*> _action_map;
static QMap<QString, QActionGroup*> _group_map;
static QMap<QString, QMenu*> _menu_map;
static QMap<QString, int> _shortcut_map;
static QMenuBar* mb;
#ifdef Q_OS_MAC
static QMacToolBar *tb;
#else
static QToolBar *tb;
#endif



#endif // QTMENUGEN_H
