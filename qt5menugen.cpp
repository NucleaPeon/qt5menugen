#include "qt5menugen.h"


QtMenuGen::QtMenuGen()
{
}


bool QtMenuGen::loadFile(QUrl url)
{
    QFile file(url.path());
    if (! file.exists())
        return _loaded;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonParseError err;
        _json = QJsonDocument::fromJson(file.readAll(), &err);
        if(err.error != QJsonParseError::NoError) {
            QString msg = QString("Unable to parse slide menu json file (%1)").arg(err.errorString());
            qWarning(msg.toLatin1());
        }
        file.close();
        _loaded = true;
    }
    return _loaded;
}

void QtMenuGen::setupToolBarOn(QFile* definition, QWidget *widget, QObject *slotobj)
{
    _action_map = QMap<QString, QAction*>();
    _group_map = QMap<QString, QActionGroup*>();
    if (_loaded == false)
        loadFile(QUrl(definition->fileName()));

    mb = setupMenus(widget);
#ifdef Q_OS_MAC
    tb = setupOSXToolBar(widget, slotobj);
#endif
#ifdef Q_OS_LINUX
    tb = setupNixToolBar(widget, slotobj);
#endif
}

void QtMenuGen::setupToolBarOn(QFile *definition, QMainWindow *window, QObject *slotobj)
{
    _action_map = QMap<QString, QAction*>();
    _group_map = QMap<QString, QActionGroup*>();
    if (_loaded == false)
        loadFile(QUrl(definition->fileName()));

   mb = setupMenus(window);
   window->setMenuBar(mb);
#ifdef Q_OS_MAC
    tb = setupOSXToolBar(window, slotobj);
#endif
#ifdef Q_OS_LINUX
    tb = setupNixToolBar(window, slotobj);
    window->addToolBar(tb);
#endif
}

QAction *QtMenuGen::actionByName(const QString name)
{
    return _action_map.value(name.toLower().replace("&", ""), NULL);
}

QMenu *QtMenuGen::menuByName(const QString name)
{
    return _menu_map.value(name.toLower().replace("&", ""), NULL);
}

QMenuBar* QtMenuGen::setupMenus(QWidget *widget)
{
    QMenuBar *mb = new QMenuBar(widget);
    QJsonArray arr = _json.array();
    foreach(QJsonValue val, arr) {
        QJsonObject obj = val.toObject();
        QMenu *m = new QMenu(obj.value("name").toString(""));
        foreach(QJsonValue actval, obj.value("actions").toArray()) {
            QJsonObject actobj = actval.toObject();
            if (actobj.contains("separator")) {
                m->addSeparator();
                continue;
            }
            if (! isValid(actobj)) { continue; }
            const bool has_checked = actobj.contains("checked");
            const bool has_group = actobj.contains("group");
            const QIcon icon = QIcon(actobj.value("icon").toString());
            QAction *act = new QAction(icon, actobj.value("text").toString(), m->parent());
            act->setData(QVariant(actobj.value("name")));

            if (has_checked) {
                act->setCheckable(true);
                act->setChecked(actobj.value("checked").toBool());
                if (has_group) {
                    QActionGroup* group;
                    const QString groupname = actobj.value("group").toString();
                    if (_group_map.contains(groupname)) {
                        group = _group_map.value(groupname);
                    } else {
                        group = new QActionGroup(0);
                        _group_map[groupname] = group;
                    }
                    group->addAction(act);
                }
            }
            const QString sc = QObject::tr(actobj.value("shortcut").toString().toLatin1().data());
            if (! sc.isNull() && ! sc.isEmpty()) {
                act->setShortcut(QKeySequence::fromString(sc));
            }
            act->setEnabled(actobj.value("enabled").toBool(true));
            m->addAction(act);
            QString slot = actobj.value("slot").toString();
            if (! slot.isEmpty()) {
                handleSignalSlot(act, "triggered()", widget, slot.toLocal8Bit().data());
            }
            _action_map[actobj.value("name").toString().toLower().replace("&", "")] = act;
        }
        _menu_map[obj.value("name").toString("").toLower().replace("&", "")] = m;
        mb->addMenu(m);
    }
    return mb;
}
#ifdef Q_OS_WIN
QWinThumbnailToolBar* QtMenuGen::setupWindowsToolBar(QWidget *widget, QObject *slotobj)
{
    QJsonArray arr = _json.array();
    foreach(QJsonValue val, arr) {
        qWarning("TODO: Windows Ribbon Toolbar");
    }
}
#endif
#ifdef Q_OS_LINUX
QToolBar* QtMenuGen::setupNixToolBar(QWidget *widget, QObject *slotobj)
{
    QJsonArray arr = _json.array();
    QToolBar *tb = new QToolBar(widget);

    foreach(QJsonValue val, arr) {
        // QActions already set up and configured
        QJsonObject obj = val.toObject();
        foreach(QJsonValue actval, obj.value("actions").toArray()) {
            QJsonObject actobj = actval.toObject();
            bool toolbar_hidden = actobj.value("toolbar_hidden").toBool(false);
            if (actobj.contains("separator") && ! toolbar_hidden) {
                tb->addSeparator();
                continue;
            }
            if (! isValid(actobj)) { continue; }
            const QString name = actobj.value("name").toString();
            QAction *act = _action_map.value(name.toLower(), NULL);
            if (act != NULL) {
                // Just don't add. As long as OS X is working, or until we impl our own toolbar visibility ui widget,
                // they can use the menus.
                if (toolbar_hidden) { continue; }
                tb->addAction(act);
            }

        }
    }
    return tb;
}
#endif

#ifdef Q_OS_MAC
QMacToolBar* QtMenuGen::setupOSXToolBar(QWidget *widget, QObject *slotobj)
{
    QJsonArray arr = _json.array();
    QMacToolBar *tb = new QMacToolBar();
    foreach(QJsonValue val, arr) {
        QJsonObject obj = val.toObject();
        foreach(QJsonValue actval, obj.value("actions").toArray()) {
            QJsonObject actobj = actval.toObject();
            // Allow hiding of separators on toolbars (but not menus), set toolbar_hidden.
            bool toolbar_hidden = actobj.value("toolbar_hidden").toBool(false);
            if (actobj.contains("separator") && ! toolbar_hidden) {
                tb->addSeparator();
                continue;
            }
            if (! isValid(actobj)) { continue; }
            const bool enabled = actobj.value("enabled").toBool(true);
            QIcon icon = QIcon(actobj.value("icon").toString());
            // FIXME: enabled currently doesn't work on QMacToolBarItem
            const QString name = actobj.value("text").toString().remove('&');

            QMacToolBarItem *tbitem;
            if (toolbar_hidden) {
                tbitem = tb->addAllowedItem(icon, name);
            } else {
                tbitem = tb->addItem(icon, name);
            }
            QString slot = actobj.value("slot").toString();
            if (! slot.isEmpty()) {
                handleSignalSlot(tbitem, "activated()", widget, slot.toLocal8Bit().data());
            }
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
    if (sigIdx < 0) { qWarning("Menu/Toolbar signal method not found"); return; }
    const QMetaMethod sigMethod = metaConn->method(sigIdx);

    const QMetaObject *metaCall = caller->metaObject();
    int slotIdx = metaCall->indexOfSlot(slot);
    if (sigIdx < 0) { qWarning("Menu/Toolbar slot method not found"); return; }
    const QMetaMethod slotMethod = metaCall->method(slotIdx);
    QObject::connect(connector, sigMethod, caller, slotMethod);
}

bool QtMenuGen::isValid(const QJsonObject obj)
{
    return (obj.contains("name") && (
            ! obj.value("icon").toString().isEmpty() ||
            ! obj.value("text").toString().isEmpty()) );
}
