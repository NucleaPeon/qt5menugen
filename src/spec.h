/*!
 * Qt5 Menu Gen JSON File Specification
 *




 * The following attributes for menu_defs.json "actions" default to true, so only use if you don't want
something to show up on a certain os:

    "visible_on_mac": "true",
    "visible_on_win": "true",
    "visible_on_nix": "true",



The following attribute, if not specified, will not set checked state. If false or true, will
assume it's checkable but set false/true.

    "checked": "true" or "false"

If you have "checked" set, you can also add a {"group": "string"} where string is the name of the group
and all QActions that are part of that group will uncheck the others (ie: tool selection)

If you see a "comment" key, it is not used but allows for user readable comment strings
to help aid in understanding behavior.

Add separators by having a {"separator": "true"} object. If this key does not exist, it
will skip the rest of the menu generation. For Toolbars, this may be a spacing instead (such
as the case for osx toolbars)

While all defined items have the option to be toolbar-accessible, that may crowd it very quickly.
To disable visibility in the toolbar (but still have it avaialble for customization), use
the key/value of:

    "toolbar_hidden": "true"

To disable a button right away, set the {"enabled": "false"} key/value in the action object.
Use this when toolbar and menu actions are contextual based on a loaded document.

To add a shortcut, type in the string to a "shortcut" key:

    {
        "text": "&Print",
        "shortcut": "Ctrl+P"
    }

This uses QKeySequence::fromString() which defaults to a PortableText conversion so do not
use native strings for it. Basically it's Ctrl/Shift/Alt and the letter combination(s).
Qt5 on OSX will convert Ctrl to Command automatically so think in terms of Windows/Linux and
it will respect OS X's keyboard layout.


------------- CONNECTIONS ----------------

In order to connect these QActions/toolbar items to signals and slots, include a "slot"
key. ToolBar items will connect on 'activate()' or appropriate signal, QActions on
'clicked()'. We can add ability to change this later or make it more customizable,
but for now we just want click = do something functionality.

    "slot": "doThis()"

The slot object (container with "_____ slot:" in class and defined function) is set in the
LoadMenu call, so let's assume this will be mainwindow.cpp in most instances (or mymodal.cpp)
and define those signals there.

-------------==============---------------

*/
