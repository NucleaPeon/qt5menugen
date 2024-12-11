#ifndef QTMENUGEN_H
#define QTMENUGEN_H

#include "qt5menugen_global.h"

#include <QtWidgets/QMainWindow>
#include <QtCore/QMetaMethod>
#include <QtWidgets/QWidget>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonParseError>
#include <QFile>
#include <QUrl>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>
#include <QtCore/QMap>

#ifdef Q_OS_MAC
#include <QtMacExtras/QMacToolBar>
#include <QtMacExtras/QMacToolBarItem>
#endif

#ifdef Q_OS_LINUX
#include <QtWidgets/QToolBar>
#elif Q_OS_WIN
#include <QtWidgets/QToolBar>
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
 * in json to allow the appropriate \c QKeySequence::StandardKey as the shortcut.
 * Instead of "Ctrl+P", "QKeySequence::Print" can be used and is recommended as it's more platform-
 * independent and readable.
 *
 * I'm looking at getting \c Qt::Key enum to be utilized within this library but so far I've been
 * unsuccessful. Even with the appropriate hex values being given to \c QKeySequence, it doesn't work
 * for shortcuts; perhaps it's unsuitable for this purpose and will be ignored.
 *
 * Since the \c Qt::Key and \c QKeySequence::StandardKey enums are in namespaces and not \c QObject's, there is
 * no way to access them by a string repr of the enum key (at least respecting the < c++-11 standard.)
 * As there haven't been any differences between 5.3.2 and 5.15 between enums, I've copied the enums
 * and their values as a \c QMap<QString, int>.
 *
 * \sa QKeySequence
 */

class QT5MENUGENSHARED_EXPORT QtMenuGen
{
    Q_ENUMS(UpdateTypes)
    Q_ENUMS(Injection)
public:
    /*!
     * \brief QtMenuGen Object based on a QString path
     * \param path QString local file
     * \version 2.0.0
     */
    explicit QtMenuGen(QString path);
    /*!
     * \brief QtMenuGen Object based on a QUrl path
     *
     * Currently only local files are implemented
     *
     * \param path QUrl local file
     *
     * \version 2.0.0
     */
    QtMenuGen(QUrl path);
    ~QtMenuGen();


    /*!
     *  brief The UpdateTypes enum specifies whether to *additionally* allow \sa update() to add to various areas where menus are created
     *
     * \version 2.3.0
     */
    enum UpdateTypes { MENU, MENUBAR, TOOLBAR };
    /*!
     * \brief The Injection enum determines where the update() will occur if given a menu name
     *
     * For example, if you have a toolbar and you have a common set of menus and actions, you can
     * specify qtmenugen to add the resulting update as a submenu of the menu/action, before or after
     * the name, or if DEFAULT it will put it at the end.
     *
     * \version 2.3.0
     */
    enum Injection { DEFAULT, INSERT_AFTER, INSERT_BEFORE, SUBMENU };

    /*!
     * \brief loadFile will explicitly load the Json file, such as scenarios where no toolbar or menu setup is required.
     * \param path QString local file
     * \return bool whether file was loaded successfully
     *
     * \version 2.0.0
     */
    bool loadFile(QString path);
    /*!
     * \brief loadFile will explicitly load the Json file, such as scenarios where no toolbar or menu setup is required
     * \param path QUrl local file
     * \return bool whether file was loaded successfully
     * \version 2.0.0
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
     *      this->qtmg = QtMenuGen(":/files/menu.json");
     *      this->qtmg.setup(this, this);
     * }
     *
     * \endcode
     *
     * \param widget QWidget*
     * \param slotobj QObject*
     * \version 2.0.0
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
     *      this->qtmg = QtMenuGen(":/files/menu.json");
     *      this->qtmg.setup(this, this);
     * }
     *
     * \endcode
     *
     * \param widget QWidget*
     * \param slotobj QObject*
     * \version 2.0.0
     */
    void setup(QMainWindow *window, QObject *slotobj);

    /*!
     * \brief setup configures the Menu with a json file containing a json object with {name: "", actions: []}.
     *
     * QMenu* will be reassigned to the content in the json file and returned.
     * The json required to set just the menu is
     *
     * {
     *      "name": "....",
     *      "actions": [
     *          { ... }
     *      ]
     * }
     *
     * same as a json file that specifies multiple menus, but just the individual menu QJsonObject.
     *
     * \code{.cpp}
     * #include <qtmenugen.h>
     *
     * // ...
     * {
     *     // Title is specified in the json file
     *     QMenu *contextMenu = new QMenu();
     *     this->qtmg = QtMenuGen(":/files/just_one_menu.json");
     *     this->qtmg.setup(contextMenu, this);
     * }
     * \endcode
     *
     * \param menu QMenu*
     * \param slotobj QObject*
     * \version 2.1.0
     *
     */
    void setup(QMenu *menu, QObject *slotobj, QJsonObject obj = QJsonObject());

    /*!
     * \brief update will combine another \c QtMenuGen* object with this one
     * \param menugenobj QtMenuGen* object to copy and append
     * \param UpdateTypes will add the QtMenuGen object contents to the toolbar and/or menubar if specified.
     *
     * There may be scenarios where the user wants to have menus that appear in both the toolbar and in
     * other places, such as a list of tools in a drawing or editing program. By calling update() on
     * a \c QtMenuGen* object, we can create a copy of the containing menus and actions so the developer doesn't
     * have to manually do this.
     *
     * Menu updates should be reflected immediately.
     *
     * \returns QMenu* of the newly formed QMenu object
     *
     * \version 2.3.0
     */
    QMenu *update(QtMenuGen* menugenobj, QObject *slotobj, UpdateTypes type = MENU);

    QMenu* update(QtMenuGen* menugenobj, QObject *slotobj, QString append, UpdateTypes type = MENU, Injection inj = DEFAULT);

    /*!
     * \brief actionByName Return the QAction* object based on the name assigned to it in the json file
     *
     * \param name QString
     * \return QAction*
     * \version 2.0.0
     */
    QAction* actionByName(const QString name);
    /*!
     * \brief menuByName Return the QMenu* object based on the name assigned to it in the json file
     *
     * \param name QString
     * \return QMenu*
     * \version 2.0.0
     */
    QMenu* menuByName(const QString name);

	/*!
	 * \version 2.0.0
	 */
    bool isLoaded();

	/*!
	 * \version 2.0.0
     * \return QMap<QString, int>
	 */
    const QMap<QString, int> getShortcuts();

	/*!
	 * \brief Returns json that is read in or an invalid/empty json object
	 *
	 * In certain cases such as testing, it's useful to know what has been
	 * read in but no toolbars or menus are required. Calling this after a
	 * successful contructor call or \c loadFile() call will allow this to
	 * return a valid QJsonDocument object.
	 * \sa QJsonDocument
	 *
	 * \version 2.0.0
     * \return QJsonDocument
	 */
    const QJsonDocument jsonDocument();

    /*!
     * \brief actions returns the QMap of QActions
     * \return \c QMap<QString, QAction*>
     *
     * \version 2.3.0
     */
    const QMap<QString, QAction*> actions();
    /*!
     * \brief actionGroups returns the QMap of QActionGroups
     * \return \c QMap<QString, QActionGroup*>
     *
     * \version 2.3.0
     */
    const QMap<QString, QActionGroup*> actionGroups();
    /*!
     * \brief menus returns the QMap of QMenus
     * \return \c QMap<QString, QMenu*>
     *
     * \version 2.3.0
     */
    const QMap<QString, QMenu*> menus();

private:
	/*!
     * \brief Internal method to setup a single menu
     *
	 * Creates a new menu (default behaviour internally) and sets it up, then
     * returns it.
     *
     * \param QJsonObject obj: If empty, pulls from the jsonDocument() contents
     *
	 * \version 2.1.0
	 */
    QMenu* setupMenu(QObject* slotobj, QJsonObject obj = QJsonObject());
    /*!
     * \brief Internal method to setup a single menu
     *
     * For use with a single menu generation only (ie: for contextual menus) using \c setup(QMenu, QObject);
     * wherein it reuses the passed in QMenu* object.
	 *
	 * If \c QJsonObject \c obj is empty, we pull from \c this->jsonDocument(). This is set internally to work with json files that contain multiple menus.
	 * Normal users shouldn't have to set this at all.
	 *
	 * \version 2.1.0
	 */
    QMenu* setupMenu(QMenu* m, QObject *slotobj, QJsonObject obj = QJsonObject());

    QMenuBar* setupMenus(QWidget *widget);
    QMap<QString, int> load_shortcuts();
    /*!
     * \brief handleSignalSlot Connects the signal to the slot
     *
     * \param connector
     * \param signal Currently this is only set to "triggered()" from the calling method
     * \param caller
     * \param slot
     */
    void handleSignalSlot(QObject *connector, const char *signal, QObject *caller, const char *slot);
    
    bool isValid(const QJsonObject obj);

    /*!
     * \brief warn Convenience method to produce qWarnings() with QString % arg parameters
     * \param message to send to qWarning()
     *
     * This helps build strings with parameters for easier qWarning calls.
     *
     * \code{.cpp}
     *     warn(QString("This has %1 and %2 in it").arg("foo").arg("bar"));
     * \endcode
     *
     *
     * \version 2.1.1
     */
    void warn(QString message);

    /*!
     * \brief buildAction moved QAction* building code into this one method to compartmentalize it
     * \param obj
     * \param slotobj
     * \return QAction*
     *
     * \version 2.3.0
     */
    QAction *buildAction(QJsonObject obj, QObject* slotobj, QMenu* menu);

    /*!
     * \brief buildMenu moved QMenu* building code into this one method to compartmentalize it
     * \param obj
     * \param slotobj
     * \param menu a default empty menu. If you pass in an existing QMenu, its title will be overwritten,
     *        menus, separators and actions will be added to it on top of what might already exist.
     * \return QMenu*
     *
     * \version 2.3.0
     */
    QMenu *buildMenu(QJsonObject obj, QObject* slotobj, QMenu *menu = new QMenu());

    /*!
     * \brief ptrToString produces a unique name (memory address location) as a string for cases when a name is not provided
     * \param menu
     *
     * When we create menus by using update(), there may be cases where menus do not have names.
     * In this case, we set the name to a string memory address location so in any case it's possible
     * to retrieve the menu using its name.
     *
     * \return QString
     */
    const QString ptrToString(const QMenu *menu);

    bool loaded;
    bool configured;
    QMap<QString, int> shortcuts;
    QJsonDocument jdoc;
    QMap<QString, QAction*> action_map;
    QMap<QString, QActionGroup*> group_map;
    QMap<QString, QMenu*> menu_map;
    QMenuBar* mb;

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
