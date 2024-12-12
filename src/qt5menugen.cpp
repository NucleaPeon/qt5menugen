#include "qt5menugen.h"

QtMenuGen::QtMenuGen(QString path)
{
    this->action_map = QMap<QString, QAction*>();
    this->group_map = QMap<QString, QActionGroup*>();
    this->menu_map = QMap<QString, QMenu*>();
    this->shortcuts = QMap<QString, int>();
    this->loaded = false;
    this->configured = false;
    this->loadFile(path);
}

QtMenuGen::QtMenuGen(QUrl path)
{
    this->action_map = QMap<QString, QAction*>();
    this->group_map = QMap<QString, QActionGroup*>();
    this->menu_map = QMap<QString, QMenu*>();
    this->shortcuts = QMap<QString, int>();
    this->loaded = false;
    this->configured = false;
    this->loadFile(path);
}

QtMenuGen::~QtMenuGen()
{

}

bool QtMenuGen::loadFile(QString path)
{
    QFile def(path);
    if (def.exists()) {
        if (def.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QJsonParseError err;
            this->jdoc = QJsonDocument::fromJson(def.readAll(), &err);
            if(err.error != QJsonParseError::NoError) {
                QString msg = QString("Unable to parse json file (%1)").arg(err.errorString());
                qWarning(msg.toLatin1());
            }
            def.close();
        }
        load_shortcuts();
        this->loaded = true;
    } else {
        // If a previous loadFile() succeeds, loaded can be true. This only returns the current call result.
        return false;
    }
    return this->loaded;
}

bool QtMenuGen::loadFile(QUrl path)
{
    if (path.isLocalFile()) {
        return this->loadFile(path.toLocalFile());
    } else {
        qWarning("Remote QUrl loadFile() Not implemented");
        return false;
    }
}

const QJsonDocument QtMenuGen::jsonDocument()
{
    return this->jdoc;
}

const QMap<QString, QAction *> QtMenuGen::actions()
{
    return this->action_map;
}

const QMap<QString, QActionGroup *> QtMenuGen::actionGroups()
{
    return this->group_map;
}

const QMap<QString, QMenu *> QtMenuGen::menus()
{
    return this->menu_map;
}

void QtMenuGen::setup(QWidget *widget, QObject *slotobj)
{
    mb = setupMenus(widget);
#ifdef Q_OS_MAC
    tb = setupOSXToolBar(widget, slotobj);
#endif
#ifdef Q_OS_LINUX
    tb = setupToolBar(widget, slotobj);
#endif
}

void QtMenuGen::setup(QMainWindow *window, QObject *slotobj)
{
    mb = setupMenus(window);
    window->setMenuBar(mb);
#ifdef Q_OS_MAC
    tb = setupOSXToolBar(window, slotobj);
#endif
#ifdef Q_OS_LINUX
    tb = setupToolBar(window, slotobj);
    window->addToolBar(tb);
#endif
}

void QtMenuGen::setup(QMenu *menu, QObject *slotobj, QJsonObject obj)
{
    if (obj.isEmpty()) { obj = this->jsonDocument().object(); }
    // const QMetaObject *metaConn = slotobj->metaObject();
	menu = setupMenu(menu, slotobj, obj);
    menu_map[obj.value("name").toString("").toLower().replace("&", "")] = menu;
}

QMenu* QtMenuGen::update(QtMenuGen *menugenobj, QObject *slotobj, UpdateTypes type)
{
    // If it's not loaded, that usually implies a badly formatted json file.
    if (! menugenobj->isLoaded()) {
        qWarning("Cannot update() on a QtMenuGen* that hasn't been set up correctly.");
        return NULL;
    }

    QJsonDocument doc = menugenobj->jsonDocument();
    QMenu* menu;
    if (doc.isArray()) {
        // It's something like a menu bar format with multi-menus

    } else if (doc.isObject()) {
        // It's a single menu we can add immediately. Must have actions[], otherwise we convert it
        // to a single QAction.
        const QJsonObject obj = doc.object();
        bool hasactions = obj.contains("actions");
        if (! hasactions) {
            menu = new QMenu();
            menu->setTitle(ptrToString(menu));
            QList<QAction*> acts = QList<QAction*>();
            acts.append(buildAction(obj, slotobj, menu));
            menu->addActions(acts);
        } else {
            menu = buildMenu(obj, slotobj);
        }
        menu_map[obj.value("name").toString("").toLower().replace("&", "")] = menu;
        if (type == TOOLBAR) {
            QJsonArray arr;
            // Update the toolbar with our menu
            if (hasactions) {
                arr = obj.value("actions").toArray();
                foreach(QJsonValue val, arr) {
#ifdef Q_OS_MAC
                    updateToolBar(this->tb, val, slotobj);
#endif
#ifdef Q_OS_LINUX
                    updateToolBar(this->tb, val, slotobj);
#endif
                }
            } else {

            }
        } else if (type == MENUBAR) {
            this->mb->addMenu(menu);
        } // If MENU, simply return the menu.
    }
    return menu;
}

QMenu *QtMenuGen::update(QtMenuGen *menugenobj, QObject *slotobj, QString append, QtMenuGen::UpdateTypes type, QtMenuGen::InjectionTypes inj)
{
    return new QMenu();
}


QAction *QtMenuGen::actionByName(const QString name)
{
    return action_map.value(name.toLower().replace("&", ""), NULL);
}

QMenu *QtMenuGen::menuByName(const QString name)
{
    return menu_map.value(name.toLower().replace("&", ""), NULL);
}

bool QtMenuGen::isLoaded()
{
    return this->loaded;
}

const QMap<QString, int> QtMenuGen::getShortcuts()
{
    return this->shortcuts;
}

QMenu* QtMenuGen::setupMenu(QObject *slotobj, QJsonObject obj)
{
    QMenu* m = new QMenu();
    return setupMenu(m, slotobj, obj);
}

QMenu* QtMenuGen::setupMenu(QMenu* m, QObject *slotobj,  QJsonObject obj)
{
    // setupMenu isn't a proper name for creating menus, so simply call the buildMenu() method.
    return buildMenu(obj, slotobj, m);
}

QMenuBar* QtMenuGen::setupMenus(QWidget *widget)
{
    QMenuBar *mb = new QMenuBar(widget);
    QJsonArray arr = jdoc.array();
    foreach(QJsonValue val, arr) {
        QJsonObject obj = val.toObject();
        QMenu *m = setupMenu(widget, obj);
        menu_map[obj.value("name").toString("").toLower().replace("&", "")] = m;
        mb->addMenu(m);
    }
    return mb;
}

#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)

void QtMenuGen::updateToolBar(QToolBar *toolbar, QJsonValue val, QObject *slotobj, QString name, InjectionTypes type)
{
    QJsonObject actobj = actval.toObject();
    bool toolbar_hidden = actobj.value("toolbar_hidden").toBool(false);
    if (actobj.contains("separator") && ! toolbar_hidden) {
        tb->addSeparator();
        return;
    }
    if (! isValid(actobj)) { return; }
    const QString _name = actobj.value("name").toString();
    QAction *act = action_map.value(_name.toLower(), NULL);
    if (act != NULL) {
        // Just don't add. As long as OS X is working, or until we impl our own toolbar visibility ui widget,
        // they can use the menus.
        if (toolbar_hidden) { return; }
        tb->addAction(act);
    }
}

QToolBar* QtMenuGen::setupToolBar(QWidget *widget, QObject *slotobj)
{
    QJsonArray arr = jdoc.array();
    QToolBar *tb = new QToolBar(widget);
    foreach(QJsonValue val, arr) {
        // QActions already set up and configured
        QJsonObject obj = val.toObject();
        foreach(QJsonValue actval, obj.value("actions").toArray()) {
            updateToolBar(tb, actval, slotobj);
        }
    }
    return tb;
}

#endif

#ifdef Q_OS_MAC
void QtMenuGen::updateToolBar(QMacToolBar* toolbar, QJsonValue val, QObject *slotobj, QString name, InjectionTypes type)
{
    QJsonObject actobj = val.toObject();
    // Allow hiding of separators on toolbars (but not menus), set toolbar_hidden.
    bool toolbar_hidden = actobj.value("toolbar_hidden").toBool(false);
    if (actobj.contains("separator") && ! toolbar_hidden) {
        toolbar->addSeparator();
        return;
    }
    if (! this->isValid(actobj)) { return; }
    // FIXME: enabled currently doesn't work on QMacToolBarItem
    const bool enabled = actobj.value("enabled").toBool(true);
    QIcon icon = QIcon(actobj.value("icon").toString());
    const QString _name = actobj.value("text").toString().remove('&');

    QMacToolBarItem *tbitem;
    if (toolbar_hidden) {
        tbitem = toolbar->addAllowedItem(icon, _name);
    } else {
        tbitem = toolbar->addItem(icon, _name);
    }
    QString slot = actobj.value("slot").toString();
    if (! slot.isEmpty()) {
        handleSignalSlot(tbitem, "activated()", slotobj, slot.toLocal8Bit().data());
    }
}

QMacToolBar* QtMenuGen::setupOSXToolBar(QWidget *widget, QObject *slotobj)
{
    QJsonArray arr = jdoc.array();
    QMacToolBar *tb = new QMacToolBar();
    foreach(QJsonValue val, arr) {
        QJsonObject obj = val.toObject();
        foreach(QJsonValue actval, obj.value("actions").toArray()) {
            updateToolBar(tb, actval, slotobj);
        }
    }
    tb->attachToWindow(widget->windowHandle());
    return tb;
}

QMacToolBarItem *QtMenuGen::toolBarItemByText(QString text)
{
    const QString name = text.remove('&');
    foreach(QMacToolBarItem* item, tb->items()) {
        if (item->text() == name)
            return item;
    }
    return NULL;
}

#endif

void QtMenuGen::handleSignalSlot(QObject *connector, const char *signal, QObject *caller, const char *slot)
{
    const QMetaObject *metaConn = connector->metaObject();
    int sigIdx = metaConn->indexOfSignal(signal);
    if (sigIdx < 0) { warn(QString("qt5menugen: %1: %2").arg("signal method not found").arg(signal)); return; }
    const QMetaMethod sigMethod = metaConn->method(sigIdx);

    const QMetaObject *metaCall = caller->metaObject();
    int slotIdx = metaCall->indexOfSlot(slot);
    if (slotIdx < 0) { warn(QString("qt5menugen: %1: %2 on %3").arg("slot method not found").arg(slot).arg(metaCall->className())); return; }
    const QMetaMethod slotMethod = metaCall->method(slotIdx);

    QObject::connect(connector, sigMethod, caller, slotMethod);
}

bool QtMenuGen::isValid(const QJsonObject obj)
{
    return (obj.contains("name") && (
            ! obj.value("icon").toString().isEmpty() ||
                ! obj.value("text").toString().isEmpty() ||
                	obj.contains("actions")) );
}

void QtMenuGen::warn(QString message)
{
    qWarning(message.toStdString().c_str());
}

QAction *QtMenuGen::buildAction(QJsonObject obj, QObject *slotobj, QMenu* menu)
{
    const bool has_checked = obj.contains("checked");
    const bool has_group = obj.contains("group");

    const QIcon icon = QIcon(obj.value("icon").toString(""));
    QAction* act = new QAction(icon, obj.value("text").toString(""), menu->parent());
    act->setData(QVariant(obj.value("name").toString()));
    if (has_checked) {
        act->setCheckable(true);
        act->setChecked(obj.value("checked").toBool());
        if (has_group) {
            QActionGroup* group;
            const QString groupname = obj.value("group").toString();
            if (group_map.contains(groupname)) {
                group = group_map.value(groupname);
            } else {
                group = new QActionGroup(0);
                group_map[groupname] = group;
            }
            group->addAction(act);
        }
    }
    // Assign shortcut
    const QString sc = QObject::tr(obj.value("shortcut").toString().toLatin1().data());
    if (! sc.isNull() && ! sc.isEmpty()) {
        if (sc.contains("Qt::") || sc.contains("QKeySequence::")) {
            if (sc.contains("Qt::")) {
                qWarning("Qt:: enum detected for key, but this is not yet supported");
            }
            // Split by +, use entire string (____::____) as key for int value, then use it to generate the QKeySequence.
            QStringList lst = sc.split("+");
            QKeySequence seq;
            switch(lst.size()) {
            case 1:
                if (shortcuts.contains(lst.at(0)))
                    seq = QKeySequence((QKeySequence::StandardKey) shortcuts.value(lst.at(0)));

                break;
            case 2:
                if (shortcuts.contains(lst.at(0)) && shortcuts.contains(lst.at(1)))
                    seq = QKeySequence((int) shortcuts.value(lst.at(0)),
                                    (int) shortcuts.value(lst.at(1)));

                break;
            case 3:
                if (shortcuts.contains(lst.at(0)) && shortcuts.contains(lst.at(1))
                        && shortcuts.contains(lst.at(2)))
                    seq = QKeySequence((int) shortcuts.value(lst.at(0)),
                                    (int) shortcuts.value(lst.at(1)),
                                    (int) shortcuts.value(lst.at(2)));

                break;
            case 4:
                if (shortcuts.contains(lst.at(0)) && shortcuts.contains(lst.at(1))
                        && shortcuts.contains(lst.at(2)) && shortcuts.contains(lst.at(3)))
                    seq = QKeySequence((int) shortcuts.value(lst.at(0)),
                                    (int) shortcuts.value(lst.at(1)),
                                    (int) shortcuts.value(lst.at(2)),
                                    (int) shortcuts.value(lst.at(3)));

                break;
            default:
                break;
            }
            act->setShortcut(seq);
        } else {
            act->setShortcut(QKeySequence::fromString(sc));
        }
    }
    act->setEnabled(obj.value("enabled").toBool(true));
    QString slot = obj.value("slot").toString();
    if (! slot.isEmpty()) {
        handleSignalSlot(act, "triggered()", slotobj, slot.toLocal8Bit().data());
    }
    action_map[obj.value("name").toString().toLower().replace("&", "")] = act;
    return act;
}

QMenu *QtMenuGen::buildMenu(QJsonObject obj, QObject *slotobj, QMenu* menu)
{
    if (obj.isEmpty()) {
        obj = this->jsonDocument().object();
    }
    menu->setTitle(obj.value("name").toString(""));
    QJsonArray arr = obj.value("actions").toArray();
    foreach(QJsonValue actval, arr) {
        QJsonObject actobj = actval.toObject();
        if (actobj.contains("separator")) {
            menu->addSeparator();
            continue;
        }
        if (! isValid(actobj)) { continue; }

        if (actobj.contains("actions")) {
            QMenu *submenu = new QMenu();
            submenu = buildMenu(actobj, slotobj, submenu);
            menu->addMenu(submenu);
            continue;
        }
        // This builds out the entire QAction with shortcuts, slots, everything etc.
        menu->addAction(buildAction(actobj, slotobj, menu));
    }
    menu_map[obj.value("name").toString("").toLower().replace("&", "")] = menu;
    return menu;
}

const QString QtMenuGen::ptrToString(const QMenu *menu)
{
    // Found at https://stackoverflow.com/questions/8881923/how-to-convert-a-pointer-value-to-qstring
    return QString( "0x%1" ).arg( reinterpret_cast<quintptr>(menu),
                                  QT_POINTER_SIZE * 2, 16, QChar('0') );
}


QMap<QString, int> QtMenuGen::load_shortcuts()
{
    // attr will be cleaned up after scope is ended
    // shortcuts["Qt::Key_Escape"] = 0x01000000;                // misc keys
//     shortcuts["Qt::Key_Tab"] = 0x01000001;
//     shortcuts["Qt::Key_Backtab"] = 0x01000002;
//     shortcuts["Qt::Key_Backspace"] = 0x01000003;
//     shortcuts["Qt::Key_Return"] = 0x01000004;
//     shortcuts["Qt::Key_Enter"] = 0x01000005;
//     shortcuts["Qt::Key_Insert"] = 0x01000006;
//     shortcuts["Qt::Key_Delete"] = 0x01000007;
//     shortcuts["Qt::Key_Pause"] = 0x01000008;
//     shortcuts["Qt::Key_Print"] = 0x01000009;               // print screen
//     shortcuts["Qt::Key_SysReq"] = 0x0100000a;
//     shortcuts["Qt::Key_Clear"] = 0x0100000b;
//     shortcuts["Qt::Key_Home"] = 0x01000010;                // cursor movement
//     shortcuts["Qt::Key_End"] = 0x01000011;
//     shortcuts["Qt::Key_Left"] = 0x01000012;
//     shortcuts["Qt::Key_Up"] = 0x01000013;
//     shortcuts["Qt::Key_Right"] = 0x01000014;
//     shortcuts["Qt::Key_Down"] = 0x01000015;
//     shortcuts["Qt::Key_PageUp"] = 0x01000016;
//     shortcuts["Qt::Key_PageDown"] = 0x01000017;
//     shortcuts["Qt::Key_Shift"] = 0x01000020;                // modifiers
//     shortcuts["Qt::Key_Control"] = Qt::Key_Control;
//     shortcuts["Qt::Key_Meta"] = 0x01000022;
//     shortcuts["Qt::Key_Alt"] = 0x01000023;
//     shortcuts["Qt::Key_CapsLock"] = 0x01000024;
//     shortcuts["Qt::Key_NumLock"] = 0x01000025;
//     shortcuts["Qt::Key_ScrollLock"] = 0x01000026;
//     shortcuts["Qt::Key_F1"] = 0x01000030;                // function keys
//     shortcuts["Qt::Key_F2"] = 0x01000031;
//     shortcuts["Qt::Key_F3"] = 0x01000032;
//     shortcuts["Qt::Key_F4"] = 0x01000033;
//     shortcuts["Qt::Key_F5"] = 0x01000034;
//     shortcuts["Qt::Key_F6"] = 0x01000035;
//     shortcuts["Qt::Key_F7"] = 0x01000036;
//     shortcuts["Qt::Key_F8"] = 0x01000037;
//     shortcuts["Qt::Key_F9"] = 0x01000038;
//     shortcuts["Qt::Key_F10"] = 0x01000039;
//     shortcuts["Qt::Key_F11"] = 0x0100003a;
//     shortcuts["Qt::Key_F12"] = 0x0100003b;
//     shortcuts["Qt::Key_F13"] = 0x0100003c;
//     shortcuts["Qt::Key_F14"] = 0x0100003d;
//     shortcuts["Qt::Key_F15"] = 0x0100003e;
//     shortcuts["Qt::Key_F16"] = 0x0100003f;
//     shortcuts["Qt::Key_F17"] = 0x01000040;
//     shortcuts["Qt::Key_F18"] = 0x01000041;
//     shortcuts["Qt::Key_F19"] = 0x01000042;
//     shortcuts["Qt::Key_F20"] = 0x01000043;
//     shortcuts["Qt::Key_F21"] = 0x01000044;
//     shortcuts["Qt::Key_F22"] = 0x01000045;
//     shortcuts["Qt::Key_F23"] = 0x01000046;
//     shortcuts["Qt::Key_F24"] = 0x01000047;
//     shortcuts["Qt::Key_F25"] = 0x01000048;                // F25 .. F35 only on X11
//     shortcuts["Qt::Key_F26"] = 0x01000049;
//     shortcuts["Qt::Key_F27"] = 0x0100004a;
//     shortcuts["Qt::Key_F28"] = 0x0100004b;
//     shortcuts["Qt::Key_F29"] = 0x0100004c;
//     shortcuts["Qt::Key_F30"] = 0x0100004d;
//     shortcuts["Qt::Key_F31"] = 0x0100004e;
//     shortcuts["Qt::Key_F32"] = 0x0100004f;
//     shortcuts["Qt::Key_F33"] = 0x01000050;
//     shortcuts["Qt::Key_F34"] = 0x01000051;
//     shortcuts["Qt::Key_F35"] = 0x01000052;
//     shortcuts["Qt::Key_Super_L"] = 0x01000053;                 // extra keys
//     shortcuts["Qt::Key_Super_R"] = 0x01000054;
//     shortcuts["Qt::Key_Menu"] = 0x01000055;
//     shortcuts["Qt::Key_Hyper_L"] = 0x01000056;
//     shortcuts["Qt::Key_Hyper_R"] = 0x01000057;
//     shortcuts["Qt::Key_Help"] = 0x01000058;
//     shortcuts["Qt::Key_Direction_L"] = 0x01000059;
//     shortcuts["Qt::Key_Direction_R"] = 0x01000060;
//     shortcuts["Qt::Key_Space"] = Qt::Key_Space;                // 7 bit printable ASCII
//     shortcuts["Qt::Key_Any"] = shortcuts.value("Qt::Key_Space", 0);
//     shortcuts["Qt::Key_Exclam"] = 0x21;
//     shortcuts["Qt::Key_QuoteDbl"] = 0x22;
//     shortcuts["Qt::Key_NumberSign"] = 0x23;
//     shortcuts["Qt::Key_Dollar"] = 0x24;
//     shortcuts["Qt::Key_Percent"] = 0x25;
//     shortcuts["Qt::Key_Ampersand"] = 0x26;
//     shortcuts["Qt::Key_Apostrophe"] = 0x27;
//     shortcuts["Qt::Key_ParenLeft"] = 0x28;
//     shortcuts["Qt::Key_ParenRight"] = 0x29;
//     shortcuts["Qt::Key_Asterisk"] = 0x2a;
//     shortcuts["Qt::Key_Plus"] = 0x2b;
//     shortcuts["Qt::Key_Comma"] = 0x2c;
//     shortcuts["Qt::Key_Minus"] = 0x2d;
//     shortcuts["Qt::Key_Period"] = 0x2e;
//     shortcuts["Qt::Key_Slash"] = 0x2f;
//     shortcuts["Qt::Key_0"] = 0x30;
//     shortcuts["Qt::Key_1"] = 0x31;
//     shortcuts["Qt::Key_2"] = 0x32;
//     shortcuts["Qt::Key_3"] = 0x33;
//     shortcuts["Qt::Key_4"] = 0x34;
//     shortcuts["Qt::Key_5"] = 0x35;
//     shortcuts["Qt::Key_6"] = 0x36;
//     shortcuts["Qt::Key_7"] = 0x37;
//     shortcuts["Qt::Key_8"] = 0x38;
//     shortcuts["Qt::Key_9"] = 0x39;
//     shortcuts["Qt::Key_Colon"] = 0x3a;
//     shortcuts["Qt::Key_Semicolon"] = 0x3b;
//     shortcuts["Qt::Key_Less"] = 0x3c;
//     shortcuts["Qt::Key_Equal"] = 0x3d;
//     shortcuts["Qt::Key_Greater"] = 0x3e;
//     shortcuts["Qt::Key_Question"] = 0x3f;
//     shortcuts["Qt::Key_At"] = 0x40;
//     shortcuts["Qt::Key_A"] = 0x41;
//     shortcuts["Qt::Key_B"] = 0x42;
//     shortcuts["Qt::Key_C"] = 0x43;
//     shortcuts["Qt::Key_D"] = 0x44;
//     shortcuts["Qt::Key_E"] = Qt::Key_E;
//     shortcuts["Qt::Key_F"] = 0x46;
//     shortcuts["Qt::Key_G"] = 0x47;
//     shortcuts["Qt::Key_H"] = 0x48;
//     shortcuts["Qt::Key_I"] = 0x49;
//     shortcuts["Qt::Key_J"] = 0x4a;
//     shortcuts["Qt::Key_K"] = 0x4b;
//     shortcuts["Qt::Key_L"] = 0x4c;
//     shortcuts["Qt::Key_M"] = 0x4d;
//     shortcuts["Qt::Key_N"] = 0x4e;
//     shortcuts["Qt::Key_O"] = 0x4f;
//     shortcuts["Qt::Key_P"] = 0x50;
//     shortcuts["Qt::Key_Q"] = 0x51;
//     shortcuts["Qt::Key_R"] = 0x52;
//     shortcuts["Qt::Key_S"] = 0x53;
//     shortcuts["Qt::Key_T"] = 0x54;
//     shortcuts["Qt::Key_U"] = 0x55;
//     shortcuts["Qt::Key_V"] = 0x56;
//     shortcuts["Qt::Key_W"] = 0x57;
//     shortcuts["Qt::Key_X"] = 0x58;
//     shortcuts["Qt::Key_Y"] = 0x59;
//     shortcuts["Qt::Key_Z"] = 0x5a;
//     shortcuts["Qt::Key_BracketLeft"] = 0x5b;
//     shortcuts["Qt::Key_Backslash"] = 0x5c;
//     shortcuts["Qt::Key_BracketRight"] = 0x5d;
//     shortcuts["Qt::Key_AsciiCircum"] = 0x5e;
//     shortcuts["Qt::Key_Underscore"] = 0x5f;
//     shortcuts["Qt::Key_QuoteLeft"] = 0x60;
//     shortcuts["Qt::Key_BraceLeft"] = 0x7b;
//     shortcuts["Qt::Key_Bar"] = 0x7c;
//     shortcuts["Qt::Key_BraceRight"] = 0x7d;
//     shortcuts["Qt::Key_AsciiTilde"] = 0x7e;
//     shortcuts["Qt::Key_nobreakspace"] = 0x0a0;
//     shortcuts["Qt::Key_exclamdown"] = 0x0a1;
//     shortcuts["Qt::Key_cent"] = 0x0a2;
//     shortcuts["Qt::Key_sterling"] = 0x0a3;
//     shortcuts["Qt::Key_currency"] = 0x0a4;
//     shortcuts["Qt::Key_yen"] = 0x0a5;
//     shortcuts["Qt::Key_brokenbar"] = 0x0a6;
//     shortcuts["Qt::Key_section"] = 0x0a7;
//     shortcuts["Qt::Key_diaeresis"] = 0x0a8;
//     shortcuts["Qt::Key_copyright"] = 0x0a9;
//     shortcuts["Qt::Key_ordfeminine"] = 0x0aa;
//     shortcuts["Qt::Key_guillemotleft"] = 0x0ab;        // left angle quotation mark
//     shortcuts["Qt::Key_notsign"] = 0x0ac;
//     shortcuts["Qt::Key_hyphen"] = 0x0ad;
//     shortcuts["Qt::Key_registered"] = 0x0ae;
//     shortcuts["Qt::Key_macron"] = 0x0af;
//     shortcuts["Qt::Key_degree"] = 0x0b0;
//     shortcuts["Qt::Key_plusminus"] = 0x0b1;
//     shortcuts["Qt::Key_twosuperior"] = 0x0b2;
//     shortcuts["Qt::Key_threesuperior"] = 0x0b3;
//     shortcuts["Qt::Key_acute"] = 0x0b4;
//     shortcuts["Qt::Key_mu"] = 0x0b5;
//     shortcuts["Qt::Key_paragraph"] = 0x0b6;
//     shortcuts["Qt::Key_periodcentered"] = 0x0b7;
//     shortcuts["Qt::Key_cedilla"] = 0x0b8;
//     shortcuts["Qt::Key_onesuperior"] = 0x0b9;
//     shortcuts["Qt::Key_masculine"] = 0x0ba;
//     shortcuts["Qt::Key_guillemotright"] = 0x0bb;        // right angle quotation mark
//     shortcuts["Qt::Key_onequarter"] = 0x0bc;
//     shortcuts["Qt::Key_onehalf"] = 0x0bd;
//     shortcuts["Qt::Key_threequarters"] = 0x0be;
//     shortcuts["Qt::Key_questiondown"] = 0x0bf;
//     shortcuts["Qt::Key_Agrave"] = 0x0c0;
//     shortcuts["Qt::Key_Aacute"] = 0x0c1;
//     shortcuts["Qt::Key_Acircumflex"] = 0x0c2;
//     shortcuts["Qt::Key_Atilde"] = 0x0c3;
//     shortcuts["Qt::Key_Adiaeresis"] = 0x0c4;
//     shortcuts["Qt::Key_Aring"] = 0x0c5;
//     shortcuts["Qt::Key_AE"] = 0x0c6;
//     shortcuts["Qt::Key_Ccedilla"] = 0x0c7;
//     shortcuts["Qt::Key_Egrave"] = 0x0c8;
//     shortcuts["Qt::Key_Eacute"] = 0x0c9;
//     shortcuts["Qt::Key_Ecircumflex"] = 0x0ca;
//     shortcuts["Qt::Key_Ediaeresis"] = 0x0cb;
//     shortcuts["Qt::Key_Igrave"] = 0x0cc;
//     shortcuts["Qt::Key_Iacute"] = 0x0cd;
//     shortcuts["Qt::Key_Icircumflex"] = 0x0ce;
//     shortcuts["Qt::Key_Idiaeresis"] = 0x0cf;
//     shortcuts["Qt::Key_ETH"] = 0x0d0;
//     shortcuts["Qt::Key_Ntilde"] = 0x0d1;
//     shortcuts["Qt::Key_Ograve"] = 0x0d2;
//     shortcuts["Qt::Key_Oacute"] = 0x0d3;
//     shortcuts["Qt::Key_Ocircumflex"] = 0x0d4;
//     shortcuts["Qt::Key_Otilde"] = 0x0d5;
//     shortcuts["Qt::Key_Odiaeresis"] = 0x0d6;
//     shortcuts["Qt::Key_multiply"] = 0x0d7;
//     shortcuts["Qt::Key_Ooblique"] = 0x0d8;
//     shortcuts["Qt::Key_Ugrave"] = 0x0d9;
//     shortcuts["Qt::Key_Uacute"] = 0x0da;
//     shortcuts["Qt::Key_Ucircumflex"] = 0x0db;
//     shortcuts["Qt::Key_Udiaeresis"] = 0x0dc;
//     shortcuts["Qt::Key_Yacute"] = 0x0dd;
//     shortcuts["Qt::Key_THORN"] = 0x0de;
//     shortcuts["Qt::Key_ssharp"] = 0x0df;
//     shortcuts["Qt::Key_division"] = 0x0f7;
//     shortcuts["Qt::Key_ydiaeresis"] = 0x0ff;
//     // International input method support (X keycode - 0xEE00; the
//     // definition follows Qt/Embedded 2.3.7) Only interesting if
//     // you are writing your own input method
//     // International & multi-key character composition
//     shortcuts["Qt::Key_AltGr"]               = 0x01001103;
//     shortcuts["Qt::Key_Multi_key"]           = 0x01001120;  // Multi-key character compose
//     shortcuts["Qt::Key_Codeinput"]           = 0x01001137;
//     shortcuts["Qt::Key_SingleCandidate"]     = 0x0100113c;
//     shortcuts["Qt::Key_MultipleCandidate"]   = 0x0100113d;
//     shortcuts["Qt::Key_PreviousCandidate"]   = 0x0100113e;
//     // Misc Functions
//     shortcuts["Qt::Key_Mode_switch"]         = 0x0100117e;  // Character set switch
//     //Key_script_switch       = 0x0100117e;  // Alias for mode_switch
//     // Japanese keyboard support
//     shortcuts["Qt::Key_Kanji"]               = 0x01001121;  // Kanji; Kanji convert
//     shortcuts["Qt::Key_Muhenkan"]            = 0x01001122;  // Cancel Conversion
//     //Key_Henkan_Mode         = 0x01001123;  // Start/Stop Conversion
//     shortcuts["Qt::Key_Henkan"]              = 0x01001123;  // Alias for Henkan_Mode
//     shortcuts["Qt::Key_Romaji"]              = 0x01001124;  // to Romaji
//     shortcuts["Qt::Key_Hiragana"]            = 0x01001125;  // to Hiragana
//     shortcuts["Qt::Key_Katakana"]            = 0x01001126;  // to Katakana
//     shortcuts["Qt::Key_Hiragana_Katakana"]   = 0x01001127;  // Hiragana/Katakana toggle
//     shortcuts["Qt::Key_Zenkaku"]             = 0x01001128;  // to Zenkaku
//     shortcuts["Qt::Key_Hankaku"]             = 0x01001129;  // to Hankaku
//     shortcuts["Qt::Key_Zenkaku_Hankaku"]     = 0x0100112a;  // Zenkaku/Hankaku toggle
//     shortcuts["Qt::Key_Touroku"]             = 0x0100112b;  // Add to Dictionary
//     shortcuts["Qt::Key_Massyo"]              = 0x0100112c;  // Delete from Dictionary
//     shortcuts["Qt::Key_Kana_Lock"]           = 0x0100112d;  // Kana Lock
//     shortcuts["Qt::Key_Kana_Shift"]          = 0x0100112e;  // Kana Shift
//     shortcuts["Qt::Key_Eisu_Shift"]          = 0x0100112f;  // Alphanumeric Shift
//     shortcuts["Qt::Key_Eisu_toggle"]         = 0x01001130;  // Alphanumeric toggle
//     //Key_Kanji_Bangou        = 0x01001137;  // Codeinput
//     //Key_Zen_Koho            = 0x0100113d;  // Multiple/All Candidate(s)
//     //Key_Mae_Koho            = 0x0100113e;  // Previous Candidate
//     // Korean keyboard support
//     //
//     // In fact; many Korean users need only 2 keys; Key_Hangul and
//     // Key_Hangul_Hanja. But rest of the keys are good for future.
//     shortcuts["Qt::Key_Hangul"]              = 0x01001131;  // Hangul start/stop(toggle)
//     shortcuts["Qt::Key_Hangul_Start"]        = 0x01001132;  // Hangul start
//     shortcuts["Qt::Key_Hangul_End"]          = 0x01001133;  // Hangul end; English start
//     shortcuts["Qt::Key_Hangul_Hanja"]        = 0x01001134;  // Start Hangul->Hanja Conversion
//     shortcuts["Qt::Key_Hangul_Jamo"]         = 0x01001135;  // Hangul Jamo mode
//     shortcuts["Qt::Key_Hangul_Romaja"]       = 0x01001136;  // Hangul Romaja mode
//     //Key_Hangul_Codeinput    = 0x01001137;  // Hangul code input mode
//     shortcuts["Qt::Key_Hangul_Jeonja"]       = 0x01001138;  // Jeonja mode
//     shortcuts["Qt::Key_Hangul_Banja"]        = 0x01001139;  // Banja mode
//     shortcuts["Qt::Key_Hangul_PreHanja"]     = 0x0100113a;  // Pre Hanja conversion
//     shortcuts["Qt::Key_Hangul_PostHanja"]    = 0x0100113b;  // Post Hanja conversion
//     //Key_Hangul_SingleCandidate   = 0x0100113c;  // Single candidate
//     //Key_Hangul_MultipleCandidate = 0x0100113d;  // Multiple candidate
//     //Key_Hangul_PreviousCandidate = 0x0100113e;  // Previous candidate
//     shortcuts["Qt::Key_Hangul_Special"]      = 0x0100113f;  // Special symbols
//     //Key_Hangul_switch       = 0x0100117e;  // Alias for mode_switch
//     // dead keys (X keycode - 0xED00 to avoid the conflict)
//     shortcuts["Qt::Key_Dead_Grave"]          = 0x01001250;
//     shortcuts["Qt::Key_Dead_Acute"]          = 0x01001251;
//     shortcuts["Qt::Key_Dead_Circumflex"]     = 0x01001252;
//     shortcuts["Qt::Key_Dead_Tilde"]          = 0x01001253;
//     shortcuts["Qt::Key_Dead_Macron"]         = 0x01001254;
//     shortcuts["Qt::Key_Dead_Breve"]          = 0x01001255;
//     shortcuts["Qt::Key_Dead_Abovedot"]       = 0x01001256;
//     shortcuts["Qt::Key_Dead_Diaeresis"]      = 0x01001257;
//     shortcuts["Qt::Key_Dead_Abovering"]      = 0x01001258;
//     shortcuts["Qt::Key_Dead_Doubleacute"]    = 0x01001259;
//     shortcuts["Qt::Key_Dead_Caron"]          = 0x0100125a;
//     shortcuts["Qt::Key_Dead_Cedilla"]        = 0x0100125b;
//     shortcuts["Qt::Key_Dead_Ogonek"]         = 0x0100125c;
//     shortcuts["Qt::Key_Dead_Iota"]           = 0x0100125d;
//     shortcuts["Qt::Key_Dead_Voiced_Sound"]   = 0x0100125e;
//     shortcuts["Qt::Key_Dead_Semivoiced_Sound"] = 0x0100125f;
//     shortcuts["Qt::Key_Dead_Belowdot"]       = 0x01001260;
//     shortcuts["Qt::Key_Dead_Hook"]           = 0x01001261;
//     shortcuts["Qt::Key_Dead_Horn"]           = 0x01001262;
//     shortcuts["Qt::Key_Dead_Stroke"]         = 0x01001263;
//     shortcuts["Qt::Key_Dead_Abovecomma"]     = 0x01001264;
//     shortcuts["Qt::Key_Dead_Abovereversedcomma"] = 0x01001265;
//     shortcuts["Qt::Key_Dead_Doublegrave"]    = 0x01001266;
//     shortcuts["Qt::Key_Dead_Belowring"]      = 0x01001267;
//     shortcuts["Qt::Key_Dead_Belowmacron"]    = 0x01001268;
//     shortcuts["Qt::Key_Dead_Belowcircumflex"] = 0x01001269;
//     shortcuts["Qt::Key_Dead_Belowtilde"]     = 0x0100126a;
//     shortcuts["Qt::Key_Dead_Belowbreve"]     = 0x0100126b;
//     shortcuts["Qt::Key_Dead_Belowdiaeresis"] = 0x0100126c;
//     shortcuts["Qt::Key_Dead_Invertedbreve"]  = 0x0100126d;
//     shortcuts["Qt::Key_Dead_Belowcomma"]     = 0x0100126e;
//     shortcuts["Qt::Key_Dead_Currency"]       = 0x0100126f;
//     shortcuts["Qt::Key_Dead_a"]              = 0x01001280;
//     shortcuts["Qt::Key_Dead_A"]              = 0x01001281;
//     shortcuts["Qt::Key_Dead_e"]              = 0x01001282;
//     shortcuts["Qt::Key_Dead_E"]              = 0x01001283;
//     shortcuts["Qt::Key_Dead_i"]              = 0x01001284;
//     shortcuts["Qt::Key_Dead_I"]              = 0x01001285;
//     shortcuts["Qt::Key_Dead_o"]              = 0x01001286;
//     shortcuts["Qt::Key_Dead_O"]              = 0x01001287;
//     shortcuts["Qt::Key_Dead_u"]              = 0x01001288;
//     shortcuts["Qt::Key_Dead_U"]              = 0x01001289;
//     shortcuts["Qt::Key_Dead_Small_Schwa"]    = 0x0100128a;
//     shortcuts["Qt::Key_Dead_Capital_Schwa"]  = 0x0100128b;
//     shortcuts["Qt::Key_Dead_Greek"]          = 0x0100128c;
//     shortcuts["Qt::Key_Dead_Lowline"]        = 0x01001290;
//     shortcuts["Qt::Key_Dead_Aboveverticalline"] = 0x01001291;
//     shortcuts["Qt::Key_Dead_Belowverticalline"] = 0x01001292;
//     shortcuts["Qt::Key_Dead_Longsolidusoverlay"] = 0x01001293;
//     // multimedia/internet keys - ignored by default - see QKeyEvent c'tor
//     shortcuts["Qt::Key_Back"]  = 0x01000061;
//     shortcuts["Qt::Key_Forward"]  = 0x01000062;
//     shortcuts["Qt::Key_Stop"]  = 0x01000063;
//     shortcuts["Qt::Key_Refresh"]  = 0x01000064;
//     shortcuts["Qt::Key_VolumeDown"] = 0x01000070;
//     shortcuts["Qt::Key_VolumeMute"]  = 0x01000071;
//     shortcuts["Qt::Key_VolumeUp"] = 0x01000072;
//     shortcuts["Qt::Key_BassBoost"] = 0x01000073;
//     shortcuts["Qt::Key_BassUp"] = 0x01000074;
//     shortcuts["Qt::Key_BassDown"] = 0x01000075;
//     shortcuts["Qt::Key_TrebleUp"] = 0x01000076;
//     shortcuts["Qt::Key_TrebleDown"] = 0x01000077;
//     shortcuts["Qt::Key_MediaPlay"]  = 0x01000080;
//     shortcuts["Qt::Key_MediaStop"]  = 0x01000081;
//     shortcuts["Qt::Key_MediaPrevious"]  = 0x01000082;
//     shortcuts["Qt::Key_MediaNext"]  = 0x01000083;
//     shortcuts["Qt::Key_MediaRecord"] = 0x01000084;
//     shortcuts["Qt::Key_MediaPause"] = 0x1000085;
//     shortcuts["Qt::Key_MediaTogglePlayPause"] = 0x1000086;
//     shortcuts["Qt::Key_HomePage"]  = 0x01000090;
//     shortcuts["Qt::Key_Favorites"]  = 0x01000091;
//     shortcuts["Qt::Key_Search"]  = 0x01000092;
//     shortcuts["Qt::Key_Standby"] = 0x01000093;
//     shortcuts["Qt::Key_OpenUrl"] = 0x01000094;
//     shortcuts["Qt::Key_LaunchMail"]  = 0x010000a0;
//     shortcuts["Qt::Key_LaunchMedia"] = 0x010000a1;
//     shortcuts["Qt::Key_Launch0"]  = 0x010000a2;
//     shortcuts["Qt::Key_Launch1"]  = 0x010000a3;
//     shortcuts["Qt::Key_Launch2"]  = 0x010000a4;
//     shortcuts["Qt::Key_Launch3"]  = 0x010000a5;
//     shortcuts["Qt::Key_Launch4"]  = 0x010000a6;
//     shortcuts["Qt::Key_Launch5"]  = 0x010000a7;
//     shortcuts["Qt::Key_Launch6"]  = 0x010000a8;
//     shortcuts["Qt::Key_Launch7"]  = 0x010000a9;
//     shortcuts["Qt::Key_Launch8"]  = 0x010000aa;
//     shortcuts["Qt::Key_Launch9"]  = 0x010000ab;
//     shortcuts["Qt::Key_LaunchA"]  = 0x010000ac;
//     shortcuts["Qt::Key_LaunchB"]  = 0x010000ad;
//     shortcuts["Qt::Key_LaunchC"]  = 0x010000ae;
//     shortcuts["Qt::Key_LaunchD"]  = 0x010000af;
//     shortcuts["Qt::Key_LaunchE"]  = 0x010000b0;
//     shortcuts["Qt::Key_LaunchF"]  = 0x010000b1;
//     shortcuts["Qt::Key_MonBrightnessUp"] = 0x010000b2;
//     shortcuts["Qt::Key_MonBrightnessDown"] = 0x010000b3;
//     shortcuts["Qt::Key_KeyboardLightOnOff"] = 0x010000b4;
//     shortcuts["Qt::Key_KeyboardBrightnessUp"] = 0x010000b5;
//     shortcuts["Qt::Key_KeyboardBrightnessDown"] = 0x010000b6;
//     shortcuts["Qt::Key_PowerOff"] = 0x010000b7;
//     shortcuts["Qt::Key_WakeUp"] = 0x010000b8;
//     shortcuts["Qt::Key_Eject"] = 0x010000b9;
//     shortcuts["Qt::Key_ScreenSaver"] = 0x010000ba;
//     shortcuts["Qt::Key_WWW"] = 0x010000bb;
//     shortcuts["Qt::Key_Memo"] = 0x010000bc;
//     shortcuts["Qt::Key_LightBulb"] = 0x010000bd;
//     shortcuts["Qt::Key_Shop"] = 0x010000be;
//     shortcuts["Qt::Key_History"] = 0x010000bf;
//     shortcuts["Qt::Key_AddFavorite"] = 0x010000c0;
//     shortcuts["Qt::Key_HotLinks"] = 0x010000c1;
//     shortcuts["Qt::Key_BrightnessAdjust"] = 0x010000c2;
//     shortcuts["Qt::Key_Finance"] = 0x010000c3;
//     shortcuts["Qt::Key_Community"] = 0x010000c4;
//     shortcuts["Qt::Key_AudioRewind"] = 0x010000c5; // Media rewind
//     shortcuts["Qt::Key_BackForward"] = 0x010000c6;
//     shortcuts["Qt::Key_ApplicationLeft"] = 0x010000c7;
//     shortcuts["Qt::Key_ApplicationRight"] = 0x010000c8;
//     shortcuts["Qt::Key_Book"] = 0x010000c9;
//     shortcuts["Qt::Key_CD"] = 0x010000ca;
//     shortcuts["Qt::Key_Calculator"] = 0x010000cb;
//     shortcuts["Qt::Key_ToDoList"] = 0x010000cc;
//     shortcuts["Qt::Key_ClearGrab"] = 0x010000cd;
//     shortcuts["Qt::Key_Close"] = 0x010000ce;
//     shortcuts["Qt::Key_Copy"] = 0x010000cf;
//     shortcuts["Qt::Key_Cut"] = 0x010000d0;
//     shortcuts["Qt::Key_Display"] = 0x010000d1; // Output switch key
//     shortcuts["Qt::Key_DOS"] = 0x010000d2;
//     shortcuts["Qt::Key_Documents"] = 0x010000d3;
//     shortcuts["Qt::Key_Excel"] = 0x010000d4;
//     shortcuts["Qt::Key_Explorer"] = 0x010000d5;
//     shortcuts["Qt::Key_Game"] = 0x010000d6;
//     shortcuts["Qt::Key_Go"] = 0x010000d7;
//     shortcuts["Qt::Key_iTouch"] = 0x010000d8;
//     shortcuts["Qt::Key_LogOff"] = 0x010000d9;
//     shortcuts["Qt::Key_Market"] = 0x010000da;
//     shortcuts["Qt::Key_Meeting"] = 0x010000db;
//     shortcuts["Qt::Key_MenuKB"] = 0x010000dc;
//     shortcuts["Qt::Key_MenuPB"] = 0x010000dd;
//     shortcuts["Qt::Key_MySites"] = 0x010000de;
//     shortcuts["Qt::Key_News"] = 0x010000df;
//     shortcuts["Qt::Key_OfficeHome"] = 0x010000e0;
//     shortcuts["Qt::Key_Option"] = 0x010000e1;
//     shortcuts["Qt::Key_Paste"] = 0x010000e2;
//     shortcuts["Qt::Key_Phone"] = 0x010000e3;
//     shortcuts["Qt::Key_Calendar"] = 0x010000e4;
//     shortcuts["Qt::Key_Reply"] = 0x010000e5;
//     shortcuts["Qt::Key_Reload"] = 0x010000e6;
//     shortcuts["Qt::Key_RotateWindows"] = 0x010000e7;
//     shortcuts["Qt::Key_RotationPB"] = 0x010000e8;
//     shortcuts["Qt::Key_RotationKB"] = 0x010000e9;
//     shortcuts["Qt::Key_Save"] = 0x010000ea;
//     shortcuts["Qt::Key_Send"] = 0x010000eb;
//     shortcuts["Qt::Key_Spell"] = 0x010000ec;
//     shortcuts["Qt::Key_SplitScreen"] = 0x010000ed;
//     shortcuts["Qt::Key_Support"] = 0x010000ee;
//     shortcuts["Qt::Key_TaskPane"] = 0x010000ef;
//     shortcuts["Qt::Key_Terminal"] = 0x010000f0;
//     shortcuts["Qt::Key_Tools"] = 0x010000f1;
//     shortcuts["Qt::Key_Travel"] = 0x010000f2;
//     shortcuts["Qt::Key_Video"] = 0x010000f3;
//     shortcuts["Qt::Key_Word"] = 0x010000f4;
//     shortcuts["Qt::Key_Xfer"] = 0x010000f5;
//     shortcuts["Qt::Key_ZoomIn"] = 0x010000f6;
//     shortcuts["Qt::Key_ZoomOut"] = 0x010000f7;
//     shortcuts["Qt::Key_Away"] = 0x010000f8;
//     shortcuts["Qt::Key_Messenger"] = 0x010000f9;
//     shortcuts["Qt::Key_WebCam"] = 0x010000fa;
//     shortcuts["Qt::Key_MailForward"] = 0x010000fb;
//     shortcuts["Qt::Key_Pictures"] = 0x010000fc;
//     shortcuts["Qt::Key_Music"] = 0x010000fd;
//     shortcuts["Qt::Key_Battery"] = 0x010000fe;
//     shortcuts["Qt::Key_Bluetooth"] = 0x010000ff;
//     shortcuts["Qt::Key_WLAN"] = 0x01000100;
//     shortcuts["Qt::Key_UWB"] = 0x01000101;
//     shortcuts["Qt::Key_AudioForward"] = 0x01000102; // Media fast-forward
//     shortcuts["Qt::Key_AudioRepeat"] = 0x01000103; // Toggle repeat mode
//     shortcuts["Qt::Key_AudioRandomPlay"] = 0x01000104; // Toggle shuffle mode
//     shortcuts["Qt::Key_Subtitle"] = 0x01000105;
//     shortcuts["Qt::Key_AudioCycleTrack"] = 0x01000106;
//     shortcuts["Qt::Key_Time"] = 0x01000107;
//     shortcuts["Qt::Key_Hibernate"] = 0x01000108;
//     shortcuts["Qt::Key_View"] = 0x01000109;
//     shortcuts["Qt::Key_TopMenu"] = 0x0100010a;
//     shortcuts["Qt::Key_PowerDown"] = 0x0100010b;
//     shortcuts["Qt::Key_Suspend"] = 0x0100010c;
//     shortcuts["Qt::Key_ContrastAdjust"] = 0x0100010d;
//     shortcuts["Qt::Key_LaunchG"]  = 0x0100010e;
//     shortcuts["Qt::Key_LaunchH"]  = 0x0100010f;
//     shortcuts["Qt::Key_TouchpadToggle"] = 0x01000110;
//     shortcuts["Qt::Key_TouchpadOn"] = 0x01000111;
//     shortcuts["Qt::Key_TouchpadOff"] = 0x01000112;
//     shortcuts["Qt::Key_MicMute"] = 0x01000113;
//     shortcuts["Qt::Key_Red"] = 0x01000114;
//     shortcuts["Qt::Key_Green"] = 0x01000115;
//     shortcuts["Qt::Key_Yellow"] = 0x01000116;
//     shortcuts["Qt::Key_Blue"] = 0x01000117;
//     shortcuts["Qt::Key_ChannelUp"] = 0x01000118;
//     shortcuts["Qt::Key_ChannelDown"] = 0x01000119;
//     shortcuts["Qt::Key_Guide"]    = 0x0100011a;
//     shortcuts["Qt::Key_Info"]     = 0x0100011b;
//     shortcuts["Qt::Key_Settings"] = 0x0100011c;
//     shortcuts["Qt::Key_MicVolumeUp"]   = 0x0100011d;
//     shortcuts["Qt::Key_MicVolumeDown"] = 0x0100011e;
//     shortcuts["Qt::Key_New"]      = 0x01000120;
//     shortcuts["Qt::Key_Open"]     = 0x01000121;
//     shortcuts["Qt::Key_Find"]     = 0x01000122;
//     shortcuts["Qt::Key_Undo"]     = 0x01000123;
//     shortcuts["Qt::Key_Redo"]     = 0x01000124;
//     shortcuts["Qt::Key_MediaLast"] = 0x0100ffff;
//     // Keypad navigation keys
//     shortcuts["Qt::Key_Select"] = 0x01010000;
//     shortcuts["Qt::Key_Yes"] = 0x01010001;
//     shortcuts["Qt::Key_No"] = 0x01010002;
//     // Newer misc keys
//     shortcuts["Qt::Key_Cancel"]  = 0x01020001;
//     shortcuts["Qt::Key_Printer"] = 0x01020002;
//     shortcuts["Qt::Key_Execute"] = 0x01020003;
//     shortcuts["Qt::Key_Sleep"]   = 0x01020004;
//     shortcuts["Qt::Key_Play"]    = 0x01020005; // Not the same as Key_MediaPlay
//     shortcuts["Qt::Key_Zoom"]    = 0x01020006;
//     //Key_Jisho   = 0x01020007; // IME: Dictionary key
//     //Key_Oyayubi_Left = 0x01020008; // IME: Left Oyayubi key
//     //Key_Oyayubi_Right = 0x01020009; // IME: Right Oyayubi key
//     shortcuts["Qt::Key_Exit"]    = 0x0102000a;
//     // Device keys
//     shortcuts["Qt::Key_Context1"] = 0x01100000;
//     shortcuts["Qt::Key_Context2"] = 0x01100001;
//     shortcuts["Qt::Key_Context3"] = 0x01100002;
//     shortcuts["Qt::Key_Context4"] = 0x01100003;
//     shortcuts["Qt::Key_Call"] = 0x01100004;      // set absolute state to in a call (do not toggle state)
//     shortcuts["Qt::Key_Hangup"] = 0x01100005;    // set absolute state to hang up (do not toggle state)
//     shortcuts["Qt::Key_Flip"] = 0x01100006;
//     shortcuts["Qt::Key_ToggleCallHangup"] = 0x01100007; // a toggle key for answering; or hanging up; based on current call state
//     shortcuts["Qt::Key_VoiceDial"] = 0x01100008;
//     shortcuts["Qt::Key_LastNumberRedial"] = 0x01100009;
//     shortcuts["Qt::Key_Camera"] = 0x01100020;
//     shortcuts["Qt::Key_CameraFocus"] = 0x01100021;
//     shortcuts["Qt::Key_unknown"] = 0x01ffffff;


    // Now set QStandardKey::
    shortcuts["QKeySequence::AddTab"] = 19; //	Add new tab.
    shortcuts["QKeySequence::Back"]	= 13; //	Navigate back.
    shortcuts["QKeySequence::Bold"]	= 27; //	Bold text.
    shortcuts["QKeySequence::Close"] = 4; //	Close document/tab.
    shortcuts["QKeySequence::Copy"]	= 9; //	Copy.
    shortcuts["QKeySequence::Cut"]	= 8; //	Cut.
    shortcuts["QKeySequence::Delete"]	= 7; //	Delete.
    shortcuts["QKeySequence::DeleteEndOfLine"]	= 60; //	Delete end of line.
    shortcuts["QKeySequence::DeleteEndOfWord"]	= 59; //	Delete word from the end of the cursor.
    shortcuts["QKeySequence::DeleteStartOfWord"] = 58; //	Delete the beginning of a word up to the cursor.
    shortcuts["QKeySequence::DeleteCompleteLine"]	= 68; //	Delete the entire line.
    shortcuts["QKeySequence::Find"]	= 22; //	Find in document.
    shortcuts["QKeySequence::FindNext"]	= 23; //	Find next result.
    shortcuts["QKeySequence::FindPrevious"]	= 24; //	Find previous result.
    shortcuts["QKeySequence::Forward"]	= 14; //	Navigate forward.
    shortcuts["QKeySequence::HelpContents"]	= 1; //	Open help contents.
    shortcuts["QKeySequence::InsertLineSeparator"]	= 62; //	Insert a new line.
    shortcuts["QKeySequence::InsertParagraphSeparator"]	= 61; //	Insert a new paragraph.
    shortcuts["QKeySequence::Italic"]	= 28; //	Italic text.
    shortcuts["QKeySequence::MoveToEndOfBlock"]	= 41; //	Move cursor to end of block. This shortcut is only used on the OS X.
    shortcuts["QKeySequence::MoveToEndOfDocument"]	= 43; //	Move cursor to end of document.
    shortcuts["QKeySequence::MoveToEndOfLine"]	= 39; //	Move cursor to end of line.
    shortcuts["QKeySequence::MoveToNextChar"]	= 30; //	Move cursor to next character.
    shortcuts["QKeySequence::MoveToNextLine"]	= 34; //	Move cursor to next line.
    shortcuts["QKeySequence::MoveToNextPage"]	= 36; //	Move cursor to next page.
    shortcuts["QKeySequence::MoveToNextWord"]	= 32; //	Move cursor to next word.
    shortcuts["QKeySequence::MoveToPreviousChar"]	= 31; //	Move cursor to previous character.
    shortcuts["QKeySequence::MoveToPreviousLine"]	= 35; //	Move cursor to previous line.
    shortcuts["QKeySequence::MoveToPreviousPage"]	= 37; //	Move cursor to previous page.
    shortcuts["QKeySequence::MoveToPreviousWord"]	= 33; //	Move cursor to previous word.
    shortcuts["QKeySequence::MoveToStartOfBlock"]	= 40; //	Move cursor to start of a block. This shortcut is only used on OS X.
    shortcuts["QKeySequence::MoveToStartOfDocument"] = 42; //	Move cursor to start of document.
    shortcuts["QKeySequence::MoveToStartOfLine"] = 38; //	Move cursor to start of line.
    shortcuts["QKeySequence::New"]	= 6; //	Create new document.
    shortcuts["QKeySequence::NextChild"] = 20; //	Navigate to next tab or child window.
    shortcuts["QKeySequence::Open"]	= 3; //	Open document.
    shortcuts["QKeySequence::Paste"] = 10; //	Paste.
    shortcuts["QKeySequence::Preferences"]	= 64; //	Open the preferences dialog.
    shortcuts["QKeySequence::PreviousChild"] = 21; //	Navigate to previous tab or child window.
    shortcuts["QKeySequence::Print"] = 18; //	Print document.
    shortcuts["QKeySequence::Quit"]	= 65; //	Quit the application.
    shortcuts["QKeySequence::Redo"]	= 12; //	Redo.
    shortcuts["QKeySequence::Refresh"]	= 15; //	Refresh or reload current document.
    shortcuts["QKeySequence::Replace"]	= 25; //	Find and replace.
    shortcuts["QKeySequence::SaveAs"]	= 63; //	Save document after prompting the user for a file name.
    shortcuts["QKeySequence::Save"]	= 5; //	Save document.
    shortcuts["QKeySequence::SelectAll"] = 26; //	Select all text.
    shortcuts["QKeySequence::Deselect"]	= 67; //	Deselect text. Since 5.1
    shortcuts["QKeySequence::SelectEndOfBlock"]	= 55; //	Extend selection to the end of a text block. This shortcut is only used on OS X.
    shortcuts["QKeySequence::SelectEndOfDocument"]	= 57; //	Extend selection to end of document.
    shortcuts["QKeySequence::SelectEndOfLine"]	= 53; //	Extend selection to end of line.
    shortcuts["QKeySequence::SelectNextChar"]	= 44; //	Extend selection to next character.
    shortcuts["QKeySequence::SelectNextLine"]	= 48; //	Extend selection to next line.
    shortcuts["QKeySequence::SelectNextPage"]	= 50; //	Extend selection to next page.
    shortcuts["QKeySequence::SelectNextWord"]	= 46; //	Extend selection to next word.
    shortcuts["QKeySequence::SelectPreviousChar"]	= 45; //	Extend selection to previous character.
    shortcuts["QKeySequence::SelectPreviousLine"]	= 49; //	Extend selection to previous line.
    shortcuts["QKeySequence::SelectPreviousPage"]	= 51; //	Extend selection to previous page.
    shortcuts["QKeySequence::SelectPreviousWord"]	= 47; //	Extend selection to previous word.
    shortcuts["QKeySequence::SelectStartOfBlock"]	= 54; //	Extend selection to the start of a text block. This shortcut is only used on OS X.
    shortcuts["QKeySequence::SelectStartOfDocument"] = 56; //	Extend selection to start of document.
    shortcuts["QKeySequence::SelectStartOfLine"] = 52; //	Extend selection to start of line.
    shortcuts["QKeySequence::Underline"] = 29; //	Underline text.
    shortcuts["QKeySequence::Undo"]	= 11; //	Undo.
    shortcuts["QKeySequence::UnknownKey"]	= 0; //	Unbound key.
    shortcuts["QKeySequence::WhatsThis"] = 2; //	Activate "what's this".
    shortcuts["QKeySequence::ZoomIn"]	= 16; //	Zoom in.
    shortcuts["QKeySequence::ZoomOut"]	= 17; //	Zoom out.
    shortcuts["QKeySequence::FullScreen"]	= 66; //	Toggle the window state to/from full screen.
    return shortcuts;
}
