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

QMap<QString, int> QtMenuGen::load_shortcuts()
{
    _shortcut_map = QMap<QString, int>();
    // attr will be cleaned up after scope is ended
    _shortcut_map["Qt::Key_Escape"] = 0x01000000;                // misc keys
    _shortcut_map["Qt::Key_Tab"] = 0x01000001;
    _shortcut_map["Qt::Key_Backtab"] = 0x01000002;
    _shortcut_map["Qt::Key_Backspace"] = 0x01000003;
    _shortcut_map["Qt::Key_Return"] = 0x01000004;
    _shortcut_map["Qt::Key_Enter"] = 0x01000005;
    _shortcut_map["Qt::Key_Insert"] = 0x01000006;
    _shortcut_map["Qt::Key_Delete"] = 0x01000007;
    _shortcut_map["Qt::Key_Pause"] = 0x01000008;
    _shortcut_map["Qt::Key_Print"] = 0x01000009;               // print screen
    _shortcut_map["Qt::Key_SysReq"] = 0x0100000a;
    _shortcut_map["Qt::Key_Clear"] = 0x0100000b;
    _shortcut_map["Qt::Key_Home"] = 0x01000010;                // cursor movement
    _shortcut_map["Qt::Key_End"] = 0x01000011;
    _shortcut_map["Qt::Key_Left"] = 0x01000012;
    _shortcut_map["Qt::Key_Up"] = 0x01000013;
    _shortcut_map["Qt::Key_Right"] = 0x01000014;
    _shortcut_map["Qt::Key_Down"] = 0x01000015;
    _shortcut_map["Qt::Key_PageUp"] = 0x01000016;
    _shortcut_map["Qt::Key_PageDown"] = 0x01000017;
    _shortcut_map["Qt::Key_Shift"] = 0x01000020;                // modifiers
    _shortcut_map["Qt::Key_Control"] = 0x01000021;
    _shortcut_map["Qt::Key_Meta"] = 0x01000022;
    _shortcut_map["Qt::Key_Alt"] = 0x01000023;
    _shortcut_map["Qt::Key_CapsLock"] = 0x01000024;
    _shortcut_map["Qt::Key_NumLock"] = 0x01000025;
    _shortcut_map["Qt::Key_ScrollLock"] = 0x01000026;
    _shortcut_map["Qt::Key_F1"] = 0x01000030;                // function keys
    _shortcut_map["Qt::Key_F2"] = 0x01000031;
    _shortcut_map["Qt::Key_F3"] = 0x01000032;
    _shortcut_map["Qt::Key_F4"] = 0x01000033;
    _shortcut_map["Qt::Key_F5"] = 0x01000034;
    _shortcut_map["Qt::Key_F6"] = 0x01000035;
    _shortcut_map["Qt::Key_F7"] = 0x01000036;
    _shortcut_map["Qt::Key_F8"] = 0x01000037;
    _shortcut_map["Qt::Key_F9"] = 0x01000038;
    _shortcut_map["Qt::Key_F10"] = 0x01000039;
    _shortcut_map["Qt::Key_F11"] = 0x0100003a;
    _shortcut_map["Qt::Key_F12"] = 0x0100003b;
    _shortcut_map["Qt::Key_F13"] = 0x0100003c;
    _shortcut_map["Qt::Key_F14"] = 0x0100003d;
    _shortcut_map["Qt::Key_F15"] = 0x0100003e;
    _shortcut_map["Qt::Key_F16"] = 0x0100003f;
    _shortcut_map["Qt::Key_F17"] = 0x01000040;
    _shortcut_map["Qt::Key_F18"] = 0x01000041;
    _shortcut_map["Qt::Key_F19"] = 0x01000042;
    _shortcut_map["Qt::Key_F20"] = 0x01000043;
    _shortcut_map["Qt::Key_F21"] = 0x01000044;
    _shortcut_map["Qt::Key_F22"] = 0x01000045;
    _shortcut_map["Qt::Key_F23"] = 0x01000046;
    _shortcut_map["Qt::Key_F24"] = 0x01000047;
    _shortcut_map["Qt::Key_F25"] = 0x01000048;                // F25 .. F35 only on X11
    _shortcut_map["Qt::Key_F26"] = 0x01000049;
    _shortcut_map["Qt::Key_F27"] = 0x0100004a;
    _shortcut_map["Qt::Key_F28"] = 0x0100004b;
    _shortcut_map["Qt::Key_F29"] = 0x0100004c;
    _shortcut_map["Qt::Key_F30"] = 0x0100004d;
    _shortcut_map["Qt::Key_F31"] = 0x0100004e;
    _shortcut_map["Qt::Key_F32"] = 0x0100004f;
    _shortcut_map["Qt::Key_F33"] = 0x01000050;
    _shortcut_map["Qt::Key_F34"] = 0x01000051;
    _shortcut_map["Qt::Key_F35"] = 0x01000052;
    _shortcut_map["Qt::Key_Super_L"] = 0x01000053;                 // extra keys
    _shortcut_map["Qt::Key_Super_R"] = 0x01000054;
    _shortcut_map["Qt::Key_Menu"] = 0x01000055;
    _shortcut_map["Qt::Key_Hyper_L"] = 0x01000056;
    _shortcut_map["Qt::Key_Hyper_R"] = 0x01000057;
    _shortcut_map["Qt::Key_Help"] = 0x01000058;
    _shortcut_map["Qt::Key_Direction_L"] = 0x01000059;
    _shortcut_map["Qt::Key_Direction_R"] = 0x01000060;
    _shortcut_map["Qt::Key_Space"] = 0x20;                // 7 bit printable ASCII
    _shortcut_map["Qt::Key_Any"] = _shortcut_map.value("Qt::Key_Space", 0);
    _shortcut_map["Qt::Key_Exclam"] = 0x21;
    _shortcut_map["Qt::Key_QuoteDbl"] = 0x22;
    _shortcut_map["Qt::Key_NumberSign"] = 0x23;
    _shortcut_map["Qt::Key_Dollar"] = 0x24;
    _shortcut_map["Qt::Key_Percent"] = 0x25;
    _shortcut_map["Qt::Key_Ampersand"] = 0x26;
    _shortcut_map["Qt::Key_Apostrophe"] = 0x27;
    _shortcut_map["Qt::Key_ParenLeft"] = 0x28;
    _shortcut_map["Qt::Key_ParenRight"] = 0x29;
    _shortcut_map["Qt::Key_Asterisk"] = 0x2a;
    _shortcut_map["Qt::Key_Plus"] = 0x2b;
    _shortcut_map["Qt::Key_Comma"] = 0x2c;
    _shortcut_map["Qt::Key_Minus"] = 0x2d;
    _shortcut_map["Qt::Key_Period"] = 0x2e;
    _shortcut_map["Qt::Key_Slash"] = 0x2f;
    _shortcut_map["Qt::Key_0"] = 0x30;
    _shortcut_map["Qt::Key_1"] = 0x31;
    _shortcut_map["Qt::Key_2"] = 0x32;
    _shortcut_map["Qt::Key_3"] = 0x33;
    _shortcut_map["Qt::Key_4"] = 0x34;
    _shortcut_map["Qt::Key_5"] = 0x35;
    _shortcut_map["Qt::Key_6"] = 0x36;
    _shortcut_map["Qt::Key_7"] = 0x37;
    _shortcut_map["Qt::Key_8"] = 0x38;
    _shortcut_map["Qt::Key_9"] = 0x39;
    _shortcut_map["Qt::Key_Colon"] = 0x3a;
    _shortcut_map["Qt::Key_Semicolon"] = 0x3b;
    _shortcut_map["Qt::Key_Less"] = 0x3c;
    _shortcut_map["Qt::Key_Equal"] = 0x3d;
    _shortcut_map["Qt::Key_Greater"] = 0x3e;
    _shortcut_map["Qt::Key_Question"] = 0x3f;
    _shortcut_map["Qt::Key_At"] = 0x40;
    _shortcut_map["Qt::Key_A"] = 0x41;
    _shortcut_map["Qt::Key_B"] = 0x42;
    _shortcut_map["Qt::Key_C"] = 0x43;
    _shortcut_map["Qt::Key_D"] = 0x44;
    _shortcut_map["Qt::Key_E"] = 0x45;
    _shortcut_map["Qt::Key_F"] = 0x46;
    _shortcut_map["Qt::Key_G"] = 0x47;
    _shortcut_map["Qt::Key_H"] = 0x48;
    _shortcut_map["Qt::Key_I"] = 0x49;
    _shortcut_map["Qt::Key_J"] = 0x4a;
    _shortcut_map["Qt::Key_K"] = 0x4b;
    _shortcut_map["Qt::Key_L"] = 0x4c;
    _shortcut_map["Qt::Key_M"] = 0x4d;
    _shortcut_map["Qt::Key_N"] = 0x4e;
    _shortcut_map["Qt::Key_O"] = 0x4f;
    _shortcut_map["Qt::Key_P"] = 0x50;
    _shortcut_map["Qt::Key_Q"] = 0x51;
    _shortcut_map["Qt::Key_R"] = 0x52;
    _shortcut_map["Qt::Key_S"] = 0x53;
    _shortcut_map["Qt::Key_T"] = 0x54;
    _shortcut_map["Qt::Key_U"] = 0x55;
    _shortcut_map["Qt::Key_V"] = 0x56;
    _shortcut_map["Qt::Key_W"] = 0x57;
    _shortcut_map["Qt::Key_X"] = 0x58;
    _shortcut_map["Qt::Key_Y"] = 0x59;
    _shortcut_map["Qt::Key_Z"] = 0x5a;
    _shortcut_map["Qt::Key_BracketLeft"] = 0x5b;
    _shortcut_map["Qt::Key_Backslash"] = 0x5c;
    _shortcut_map["Qt::Key_BracketRight"] = 0x5d;
    _shortcut_map["Qt::Key_AsciiCircum"] = 0x5e;
    _shortcut_map["Qt::Key_Underscore"] = 0x5f;
    _shortcut_map["Qt::Key_QuoteLeft"] = 0x60;
    _shortcut_map["Qt::Key_BraceLeft"] = 0x7b;
    _shortcut_map["Qt::Key_Bar"] = 0x7c;
    _shortcut_map["Qt::Key_BraceRight"] = 0x7d;
    _shortcut_map["Qt::Key_AsciiTilde"] = 0x7e;
    _shortcut_map["Qt::Key_nobreakspace"] = 0x0a0;
    _shortcut_map["Qt::Key_exclamdown"] = 0x0a1;
    _shortcut_map["Qt::Key_cent"] = 0x0a2;
    _shortcut_map["Qt::Key_sterling"] = 0x0a3;
    _shortcut_map["Qt::Key_currency"] = 0x0a4;
    _shortcut_map["Qt::Key_yen"] = 0x0a5;
    _shortcut_map["Qt::Key_brokenbar"] = 0x0a6;
    _shortcut_map["Qt::Key_section"] = 0x0a7;
    _shortcut_map["Qt::Key_diaeresis"] = 0x0a8;
    _shortcut_map["Qt::Key_copyright"] = 0x0a9;
    _shortcut_map["Qt::Key_ordfeminine"] = 0x0aa;
    _shortcut_map["Qt::Key_guillemotleft"] = 0x0ab;        // left angle quotation mark
    _shortcut_map["Qt::Key_notsign"] = 0x0ac;
    _shortcut_map["Qt::Key_hyphen"] = 0x0ad;
    _shortcut_map["Qt::Key_registered"] = 0x0ae;
    _shortcut_map["Qt::Key_macron"] = 0x0af;
    _shortcut_map["Qt::Key_degree"] = 0x0b0;
    _shortcut_map["Qt::Key_plusminus"] = 0x0b1;
    _shortcut_map["Qt::Key_twosuperior"] = 0x0b2;
    _shortcut_map["Qt::Key_threesuperior"] = 0x0b3;
    _shortcut_map["Qt::Key_acute"] = 0x0b4;
    _shortcut_map["Qt::Key_mu"] = 0x0b5;
    _shortcut_map["Qt::Key_paragraph"] = 0x0b6;
    _shortcut_map["Qt::Key_periodcentered"] = 0x0b7;
    _shortcut_map["Qt::Key_cedilla"] = 0x0b8;
    _shortcut_map["Qt::Key_onesuperior"] = 0x0b9;
    _shortcut_map["Qt::Key_masculine"] = 0x0ba;
    _shortcut_map["Qt::Key_guillemotright"] = 0x0bb;        // right angle quotation mark
    _shortcut_map["Qt::Key_onequarter"] = 0x0bc;
    _shortcut_map["Qt::Key_onehalf"] = 0x0bd;
    _shortcut_map["Qt::Key_threequarters"] = 0x0be;
    _shortcut_map["Qt::Key_questiondown"] = 0x0bf;
    _shortcut_map["Qt::Key_Agrave"] = 0x0c0;
    _shortcut_map["Qt::Key_Aacute"] = 0x0c1;
    _shortcut_map["Qt::Key_Acircumflex"] = 0x0c2;
    _shortcut_map["Qt::Key_Atilde"] = 0x0c3;
    _shortcut_map["Qt::Key_Adiaeresis"] = 0x0c4;
    _shortcut_map["Qt::Key_Aring"] = 0x0c5;
    _shortcut_map["Qt::Key_AE"] = 0x0c6;
    _shortcut_map["Qt::Key_Ccedilla"] = 0x0c7;
    _shortcut_map["Qt::Key_Egrave"] = 0x0c8;
    _shortcut_map["Qt::Key_Eacute"] = 0x0c9;
    _shortcut_map["Qt::Key_Ecircumflex"] = 0x0ca;
    _shortcut_map["Qt::Key_Ediaeresis"] = 0x0cb;
    _shortcut_map["Qt::Key_Igrave"] = 0x0cc;
    _shortcut_map["Qt::Key_Iacute"] = 0x0cd;
    _shortcut_map["Qt::Key_Icircumflex"] = 0x0ce;
    _shortcut_map["Qt::Key_Idiaeresis"] = 0x0cf;
    _shortcut_map["Qt::Key_ETH"] = 0x0d0;
    _shortcut_map["Qt::Key_Ntilde"] = 0x0d1;
    _shortcut_map["Qt::Key_Ograve"] = 0x0d2;
    _shortcut_map["Qt::Key_Oacute"] = 0x0d3;
    _shortcut_map["Qt::Key_Ocircumflex"] = 0x0d4;
    _shortcut_map["Qt::Key_Otilde"] = 0x0d5;
    _shortcut_map["Qt::Key_Odiaeresis"] = 0x0d6;
    _shortcut_map["Qt::Key_multiply"] = 0x0d7;
    _shortcut_map["Qt::Key_Ooblique"] = 0x0d8;
    _shortcut_map["Qt::Key_Ugrave"] = 0x0d9;
    _shortcut_map["Qt::Key_Uacute"] = 0x0da;
    _shortcut_map["Qt::Key_Ucircumflex"] = 0x0db;
    _shortcut_map["Qt::Key_Udiaeresis"] = 0x0dc;
    _shortcut_map["Qt::Key_Yacute"] = 0x0dd;
    _shortcut_map["Qt::Key_THORN"] = 0x0de;
    _shortcut_map["Qt::Key_ssharp"] = 0x0df;
    _shortcut_map["Qt::Key_division"] = 0x0f7;
    _shortcut_map["Qt::Key_ydiaeresis"] = 0x0ff;
    // International input method support (X keycode - 0xEE00; the
    // definition follows Qt/Embedded 2.3.7) Only interesting if
    // you are writing your own input method
    // International & multi-key character composition
    _shortcut_map["Qt::Key_AltGr"]               = 0x01001103;
    _shortcut_map["Qt::Key_Multi_key"]           = 0x01001120;  // Multi-key character compose
    _shortcut_map["Qt::Key_Codeinput"]           = 0x01001137;
    _shortcut_map["Qt::Key_SingleCandidate"]     = 0x0100113c;
    _shortcut_map["Qt::Key_MultipleCandidate"]   = 0x0100113d;
    _shortcut_map["Qt::Key_PreviousCandidate"]   = 0x0100113e;
    // Misc Functions
    _shortcut_map["Qt::Key_Mode_switch"]         = 0x0100117e;  // Character set switch
    //Key_script_switch       = 0x0100117e;  // Alias for mode_switch
    // Japanese keyboard support
    _shortcut_map["Qt::Key_Kanji"]               = 0x01001121;  // Kanji; Kanji convert
    _shortcut_map["Qt::Key_Muhenkan"]            = 0x01001122;  // Cancel Conversion
    //Key_Henkan_Mode         = 0x01001123;  // Start/Stop Conversion
    _shortcut_map["Qt::Key_Henkan"]              = 0x01001123;  // Alias for Henkan_Mode
    _shortcut_map["Qt::Key_Romaji"]              = 0x01001124;  // to Romaji
    _shortcut_map["Qt::Key_Hiragana"]            = 0x01001125;  // to Hiragana
    _shortcut_map["Qt::Key_Katakana"]            = 0x01001126;  // to Katakana
    _shortcut_map["Qt::Key_Hiragana_Katakana"]   = 0x01001127;  // Hiragana/Katakana toggle
    _shortcut_map["Qt::Key_Zenkaku"]             = 0x01001128;  // to Zenkaku
    _shortcut_map["Qt::Key_Hankaku"]             = 0x01001129;  // to Hankaku
    _shortcut_map["Qt::Key_Zenkaku_Hankaku"]     = 0x0100112a;  // Zenkaku/Hankaku toggle
    _shortcut_map["Qt::Key_Touroku"]             = 0x0100112b;  // Add to Dictionary
    _shortcut_map["Qt::Key_Massyo"]              = 0x0100112c;  // Delete from Dictionary
    _shortcut_map["Qt::Key_Kana_Lock"]           = 0x0100112d;  // Kana Lock
    _shortcut_map["Qt::Key_Kana_Shift"]          = 0x0100112e;  // Kana Shift
    _shortcut_map["Qt::Key_Eisu_Shift"]          = 0x0100112f;  // Alphanumeric Shift
    _shortcut_map["Qt::Key_Eisu_toggle"]         = 0x01001130;  // Alphanumeric toggle
    //Key_Kanji_Bangou        = 0x01001137;  // Codeinput
    //Key_Zen_Koho            = 0x0100113d;  // Multiple/All Candidate(s)
    //Key_Mae_Koho            = 0x0100113e;  // Previous Candidate
    // Korean keyboard support
    //
    // In fact; many Korean users need only 2 keys; Key_Hangul and
    // Key_Hangul_Hanja. But rest of the keys are good for future.
    _shortcut_map["Qt::Key_Hangul"]              = 0x01001131;  // Hangul start/stop(toggle)
    _shortcut_map["Qt::Key_Hangul_Start"]        = 0x01001132;  // Hangul start
    _shortcut_map["Qt::Key_Hangul_End"]          = 0x01001133;  // Hangul end; English start
    _shortcut_map["Qt::Key_Hangul_Hanja"]        = 0x01001134;  // Start Hangul->Hanja Conversion
    _shortcut_map["Qt::Key_Hangul_Jamo"]         = 0x01001135;  // Hangul Jamo mode
    _shortcut_map["Qt::Key_Hangul_Romaja"]       = 0x01001136;  // Hangul Romaja mode
    //Key_Hangul_Codeinput    = 0x01001137;  // Hangul code input mode
    _shortcut_map["Qt::Key_Hangul_Jeonja"]       = 0x01001138;  // Jeonja mode
    _shortcut_map["Qt::Key_Hangul_Banja"]        = 0x01001139;  // Banja mode
    _shortcut_map["Qt::Key_Hangul_PreHanja"]     = 0x0100113a;  // Pre Hanja conversion
    _shortcut_map["Qt::Key_Hangul_PostHanja"]    = 0x0100113b;  // Post Hanja conversion
    //Key_Hangul_SingleCandidate   = 0x0100113c;  // Single candidate
    //Key_Hangul_MultipleCandidate = 0x0100113d;  // Multiple candidate
    //Key_Hangul_PreviousCandidate = 0x0100113e;  // Previous candidate
    _shortcut_map["Qt::Key_Hangul_Special"]      = 0x0100113f;  // Special symbols
    //Key_Hangul_switch       = 0x0100117e;  // Alias for mode_switch
    // dead keys (X keycode - 0xED00 to avoid the conflict)
    _shortcut_map["Qt::Key_Dead_Grave"]          = 0x01001250;
    _shortcut_map["Qt::Key_Dead_Acute"]          = 0x01001251;
    _shortcut_map["Qt::Key_Dead_Circumflex"]     = 0x01001252;
    _shortcut_map["Qt::Key_Dead_Tilde"]          = 0x01001253;
    _shortcut_map["Qt::Key_Dead_Macron"]         = 0x01001254;
    _shortcut_map["Qt::Key_Dead_Breve"]          = 0x01001255;
    _shortcut_map["Qt::Key_Dead_Abovedot"]       = 0x01001256;
    _shortcut_map["Qt::Key_Dead_Diaeresis"]      = 0x01001257;
    _shortcut_map["Qt::Key_Dead_Abovering"]      = 0x01001258;
    _shortcut_map["Qt::Key_Dead_Doubleacute"]    = 0x01001259;
    _shortcut_map["Qt::Key_Dead_Caron"]          = 0x0100125a;
    _shortcut_map["Qt::Key_Dead_Cedilla"]        = 0x0100125b;
    _shortcut_map["Qt::Key_Dead_Ogonek"]         = 0x0100125c;
    _shortcut_map["Qt::Key_Dead_Iota"]           = 0x0100125d;
    _shortcut_map["Qt::Key_Dead_Voiced_Sound"]   = 0x0100125e;
    _shortcut_map["Qt::Key_Dead_Semivoiced_Sound"] = 0x0100125f;
    _shortcut_map["Qt::Key_Dead_Belowdot"]       = 0x01001260;
    _shortcut_map["Qt::Key_Dead_Hook"]           = 0x01001261;
    _shortcut_map["Qt::Key_Dead_Horn"]           = 0x01001262;
    _shortcut_map["Qt::Key_Dead_Stroke"]         = 0x01001263;
    _shortcut_map["Qt::Key_Dead_Abovecomma"]     = 0x01001264;
    _shortcut_map["Qt::Key_Dead_Abovereversedcomma"] = 0x01001265;
    _shortcut_map["Qt::Key_Dead_Doublegrave"]    = 0x01001266;
    _shortcut_map["Qt::Key_Dead_Belowring"]      = 0x01001267;
    _shortcut_map["Qt::Key_Dead_Belowmacron"]    = 0x01001268;
    _shortcut_map["Qt::Key_Dead_Belowcircumflex"] = 0x01001269;
    _shortcut_map["Qt::Key_Dead_Belowtilde"]     = 0x0100126a;
    _shortcut_map["Qt::Key_Dead_Belowbreve"]     = 0x0100126b;
    _shortcut_map["Qt::Key_Dead_Belowdiaeresis"] = 0x0100126c;
    _shortcut_map["Qt::Key_Dead_Invertedbreve"]  = 0x0100126d;
    _shortcut_map["Qt::Key_Dead_Belowcomma"]     = 0x0100126e;
    _shortcut_map["Qt::Key_Dead_Currency"]       = 0x0100126f;
    _shortcut_map["Qt::Key_Dead_a"]              = 0x01001280;
    _shortcut_map["Qt::Key_Dead_A"]              = 0x01001281;
    _shortcut_map["Qt::Key_Dead_e"]              = 0x01001282;
    _shortcut_map["Qt::Key_Dead_E"]              = 0x01001283;
    _shortcut_map["Qt::Key_Dead_i"]              = 0x01001284;
    _shortcut_map["Qt::Key_Dead_I"]              = 0x01001285;
    _shortcut_map["Qt::Key_Dead_o"]              = 0x01001286;
    _shortcut_map["Qt::Key_Dead_O"]              = 0x01001287;
    _shortcut_map["Qt::Key_Dead_u"]              = 0x01001288;
    _shortcut_map["Qt::Key_Dead_U"]              = 0x01001289;
    _shortcut_map["Qt::Key_Dead_Small_Schwa"]    = 0x0100128a;
    _shortcut_map["Qt::Key_Dead_Capital_Schwa"]  = 0x0100128b;
    _shortcut_map["Qt::Key_Dead_Greek"]          = 0x0100128c;
    _shortcut_map["Qt::Key_Dead_Lowline"]        = 0x01001290;
    _shortcut_map["Qt::Key_Dead_Aboveverticalline"] = 0x01001291;
    _shortcut_map["Qt::Key_Dead_Belowverticalline"] = 0x01001292;
    _shortcut_map["Qt::Key_Dead_Longsolidusoverlay"] = 0x01001293;
    // multimedia/internet keys - ignored by default - see QKeyEvent c'tor
    _shortcut_map["Qt::Key_Back"]  = 0x01000061;
    _shortcut_map["Qt::Key_Forward"]  = 0x01000062;
    _shortcut_map["Qt::Key_Stop"]  = 0x01000063;
    _shortcut_map["Qt::Key_Refresh"]  = 0x01000064;
    _shortcut_map["Qt::Key_VolumeDown"] = 0x01000070;
    _shortcut_map["Qt::Key_VolumeMute"]  = 0x01000071;
    _shortcut_map["Qt::Key_VolumeUp"] = 0x01000072;
    _shortcut_map["Qt::Key_BassBoost"] = 0x01000073;
    _shortcut_map["Qt::Key_BassUp"] = 0x01000074;
    _shortcut_map["Qt::Key_BassDown"] = 0x01000075;
    _shortcut_map["Qt::Key_TrebleUp"] = 0x01000076;
    _shortcut_map["Qt::Key_TrebleDown"] = 0x01000077;
    _shortcut_map["Qt::Key_MediaPlay"]  = 0x01000080;
    _shortcut_map["Qt::Key_MediaStop"]  = 0x01000081;
    _shortcut_map["Qt::Key_MediaPrevious"]  = 0x01000082;
    _shortcut_map["Qt::Key_MediaNext"]  = 0x01000083;
    _shortcut_map["Qt::Key_MediaRecord"] = 0x01000084;
    _shortcut_map["Qt::Key_MediaPause"] = 0x1000085;
    _shortcut_map["Qt::Key_MediaTogglePlayPause"] = 0x1000086;
    _shortcut_map["Qt::Key_HomePage"]  = 0x01000090;
    _shortcut_map["Qt::Key_Favorites"]  = 0x01000091;
    _shortcut_map["Qt::Key_Search"]  = 0x01000092;
    _shortcut_map["Qt::Key_Standby"] = 0x01000093;
    _shortcut_map["Qt::Key_OpenUrl"] = 0x01000094;
    _shortcut_map["Qt::Key_LaunchMail"]  = 0x010000a0;
    _shortcut_map["Qt::Key_LaunchMedia"] = 0x010000a1;
    _shortcut_map["Qt::Key_Launch0"]  = 0x010000a2;
    _shortcut_map["Qt::Key_Launch1"]  = 0x010000a3;
    _shortcut_map["Qt::Key_Launch2"]  = 0x010000a4;
    _shortcut_map["Qt::Key_Launch3"]  = 0x010000a5;
    _shortcut_map["Qt::Key_Launch4"]  = 0x010000a6;
    _shortcut_map["Qt::Key_Launch5"]  = 0x010000a7;
    _shortcut_map["Qt::Key_Launch6"]  = 0x010000a8;
    _shortcut_map["Qt::Key_Launch7"]  = 0x010000a9;
    _shortcut_map["Qt::Key_Launch8"]  = 0x010000aa;
    _shortcut_map["Qt::Key_Launch9"]  = 0x010000ab;
    _shortcut_map["Qt::Key_LaunchA"]  = 0x010000ac;
    _shortcut_map["Qt::Key_LaunchB"]  = 0x010000ad;
    _shortcut_map["Qt::Key_LaunchC"]  = 0x010000ae;
    _shortcut_map["Qt::Key_LaunchD"]  = 0x010000af;
    _shortcut_map["Qt::Key_LaunchE"]  = 0x010000b0;
    _shortcut_map["Qt::Key_LaunchF"]  = 0x010000b1;
    _shortcut_map["Qt::Key_MonBrightnessUp"] = 0x010000b2;
    _shortcut_map["Qt::Key_MonBrightnessDown"] = 0x010000b3;
    _shortcut_map["Qt::Key_KeyboardLightOnOff"] = 0x010000b4;
    _shortcut_map["Qt::Key_KeyboardBrightnessUp"] = 0x010000b5;
    _shortcut_map["Qt::Key_KeyboardBrightnessDown"] = 0x010000b6;
    _shortcut_map["Qt::Key_PowerOff"] = 0x010000b7;
    _shortcut_map["Qt::Key_WakeUp"] = 0x010000b8;
    _shortcut_map["Qt::Key_Eject"] = 0x010000b9;
    _shortcut_map["Qt::Key_ScreenSaver"] = 0x010000ba;
    _shortcut_map["Qt::Key_WWW"] = 0x010000bb;
    _shortcut_map["Qt::Key_Memo"] = 0x010000bc;
    _shortcut_map["Qt::Key_LightBulb"] = 0x010000bd;
    _shortcut_map["Qt::Key_Shop"] = 0x010000be;
    _shortcut_map["Qt::Key_History"] = 0x010000bf;
    _shortcut_map["Qt::Key_AddFavorite"] = 0x010000c0;
    _shortcut_map["Qt::Key_HotLinks"] = 0x010000c1;
    _shortcut_map["Qt::Key_BrightnessAdjust"] = 0x010000c2;
    _shortcut_map["Qt::Key_Finance"] = 0x010000c3;
    _shortcut_map["Qt::Key_Community"] = 0x010000c4;
    _shortcut_map["Qt::Key_AudioRewind"] = 0x010000c5; // Media rewind
    _shortcut_map["Qt::Key_BackForward"] = 0x010000c6;
    _shortcut_map["Qt::Key_ApplicationLeft"] = 0x010000c7;
    _shortcut_map["Qt::Key_ApplicationRight"] = 0x010000c8;
    _shortcut_map["Qt::Key_Book"] = 0x010000c9;
    _shortcut_map["Qt::Key_CD"] = 0x010000ca;
    _shortcut_map["Qt::Key_Calculator"] = 0x010000cb;
    _shortcut_map["Qt::Key_ToDoList"] = 0x010000cc;
    _shortcut_map["Qt::Key_ClearGrab"] = 0x010000cd;
    _shortcut_map["Qt::Key_Close"] = 0x010000ce;
    _shortcut_map["Qt::Key_Copy"] = 0x010000cf;
    _shortcut_map["Qt::Key_Cut"] = 0x010000d0;
    _shortcut_map["Qt::Key_Display"] = 0x010000d1; // Output switch key
    _shortcut_map["Qt::Key_DOS"] = 0x010000d2;
    _shortcut_map["Qt::Key_Documents"] = 0x010000d3;
    _shortcut_map["Qt::Key_Excel"] = 0x010000d4;
    _shortcut_map["Qt::Key_Explorer"] = 0x010000d5;
    _shortcut_map["Qt::Key_Game"] = 0x010000d6;
    _shortcut_map["Qt::Key_Go"] = 0x010000d7;
    _shortcut_map["Qt::Key_iTouch"] = 0x010000d8;
    _shortcut_map["Qt::Key_LogOff"] = 0x010000d9;
    _shortcut_map["Qt::Key_Market"] = 0x010000da;
    _shortcut_map["Qt::Key_Meeting"] = 0x010000db;
    _shortcut_map["Qt::Key_MenuKB"] = 0x010000dc;
    _shortcut_map["Qt::Key_MenuPB"] = 0x010000dd;
    _shortcut_map["Qt::Key_MySites"] = 0x010000de;
    _shortcut_map["Qt::Key_News"] = 0x010000df;
    _shortcut_map["Qt::Key_OfficeHome"] = 0x010000e0;
    _shortcut_map["Qt::Key_Option"] = 0x010000e1;
    _shortcut_map["Qt::Key_Paste"] = 0x010000e2;
    _shortcut_map["Qt::Key_Phone"] = 0x010000e3;
    _shortcut_map["Qt::Key_Calendar"] = 0x010000e4;
    _shortcut_map["Qt::Key_Reply"] = 0x010000e5;
    _shortcut_map["Qt::Key_Reload"] = 0x010000e6;
    _shortcut_map["Qt::Key_RotateWindows"] = 0x010000e7;
    _shortcut_map["Qt::Key_RotationPB"] = 0x010000e8;
    _shortcut_map["Qt::Key_RotationKB"] = 0x010000e9;
    _shortcut_map["Qt::Key_Save"] = 0x010000ea;
    _shortcut_map["Qt::Key_Send"] = 0x010000eb;
    _shortcut_map["Qt::Key_Spell"] = 0x010000ec;
    _shortcut_map["Qt::Key_SplitScreen"] = 0x010000ed;
    _shortcut_map["Qt::Key_Support"] = 0x010000ee;
    _shortcut_map["Qt::Key_TaskPane"] = 0x010000ef;
    _shortcut_map["Qt::Key_Terminal"] = 0x010000f0;
    _shortcut_map["Qt::Key_Tools"] = 0x010000f1;
    _shortcut_map["Qt::Key_Travel"] = 0x010000f2;
    _shortcut_map["Qt::Key_Video"] = 0x010000f3;
    _shortcut_map["Qt::Key_Word"] = 0x010000f4;
    _shortcut_map["Qt::Key_Xfer"] = 0x010000f5;
    _shortcut_map["Qt::Key_ZoomIn"] = 0x010000f6;
    _shortcut_map["Qt::Key_ZoomOut"] = 0x010000f7;
    _shortcut_map["Qt::Key_Away"] = 0x010000f8;
    _shortcut_map["Qt::Key_Messenger"] = 0x010000f9;
    _shortcut_map["Qt::Key_WebCam"] = 0x010000fa;
    _shortcut_map["Qt::Key_MailForward"] = 0x010000fb;
    _shortcut_map["Qt::Key_Pictures"] = 0x010000fc;
    _shortcut_map["Qt::Key_Music"] = 0x010000fd;
    _shortcut_map["Qt::Key_Battery"] = 0x010000fe;
    _shortcut_map["Qt::Key_Bluetooth"] = 0x010000ff;
    _shortcut_map["Qt::Key_WLAN"] = 0x01000100;
    _shortcut_map["Qt::Key_UWB"] = 0x01000101;
    _shortcut_map["Qt::Key_AudioForward"] = 0x01000102; // Media fast-forward
    _shortcut_map["Qt::Key_AudioRepeat"] = 0x01000103; // Toggle repeat mode
    _shortcut_map["Qt::Key_AudioRandomPlay"] = 0x01000104; // Toggle shuffle mode
    _shortcut_map["Qt::Key_Subtitle"] = 0x01000105;
    _shortcut_map["Qt::Key_AudioCycleTrack"] = 0x01000106;
    _shortcut_map["Qt::Key_Time"] = 0x01000107;
    _shortcut_map["Qt::Key_Hibernate"] = 0x01000108;
    _shortcut_map["Qt::Key_View"] = 0x01000109;
    _shortcut_map["Qt::Key_TopMenu"] = 0x0100010a;
    _shortcut_map["Qt::Key_PowerDown"] = 0x0100010b;
    _shortcut_map["Qt::Key_Suspend"] = 0x0100010c;
    _shortcut_map["Qt::Key_ContrastAdjust"] = 0x0100010d;
    _shortcut_map["Qt::Key_LaunchG"]  = 0x0100010e;
    _shortcut_map["Qt::Key_LaunchH"]  = 0x0100010f;
    _shortcut_map["Qt::Key_TouchpadToggle"] = 0x01000110;
    _shortcut_map["Qt::Key_TouchpadOn"] = 0x01000111;
    _shortcut_map["Qt::Key_TouchpadOff"] = 0x01000112;
    _shortcut_map["Qt::Key_MicMute"] = 0x01000113;
    _shortcut_map["Qt::Key_Red"] = 0x01000114;
    _shortcut_map["Qt::Key_Green"] = 0x01000115;
    _shortcut_map["Qt::Key_Yellow"] = 0x01000116;
    _shortcut_map["Qt::Key_Blue"] = 0x01000117;
    _shortcut_map["Qt::Key_ChannelUp"] = 0x01000118;
    _shortcut_map["Qt::Key_ChannelDown"] = 0x01000119;
    _shortcut_map["Qt::Key_Guide"]    = 0x0100011a;
    _shortcut_map["Qt::Key_Info"]     = 0x0100011b;
    _shortcut_map["Qt::Key_Settings"] = 0x0100011c;
    _shortcut_map["Qt::Key_MicVolumeUp"]   = 0x0100011d;
    _shortcut_map["Qt::Key_MicVolumeDown"] = 0x0100011e;
    _shortcut_map["Qt::Key_New"]      = 0x01000120;
    _shortcut_map["Qt::Key_Open"]     = 0x01000121;
    _shortcut_map["Qt::Key_Find"]     = 0x01000122;
    _shortcut_map["Qt::Key_Undo"]     = 0x01000123;
    _shortcut_map["Qt::Key_Redo"]     = 0x01000124;
    _shortcut_map["Qt::Key_MediaLast"] = 0x0100ffff;
    // Keypad navigation keys
    _shortcut_map["Qt::Key_Select"] = 0x01010000;
    _shortcut_map["Qt::Key_Yes"] = 0x01010001;
    _shortcut_map["Qt::Key_No"] = 0x01010002;
    // Newer misc keys
    _shortcut_map["Qt::Key_Cancel"]  = 0x01020001;
    _shortcut_map["Qt::Key_Printer"] = 0x01020002;
    _shortcut_map["Qt::Key_Execute"] = 0x01020003;
    _shortcut_map["Qt::Key_Sleep"]   = 0x01020004;
    _shortcut_map["Qt::Key_Play"]    = 0x01020005; // Not the same as Key_MediaPlay
    _shortcut_map["Qt::Key_Zoom"]    = 0x01020006;
    //Key_Jisho   = 0x01020007; // IME: Dictionary key
    //Key_Oyayubi_Left = 0x01020008; // IME: Left Oyayubi key
    //Key_Oyayubi_Right = 0x01020009; // IME: Right Oyayubi key
    _shortcut_map["Qt::Key_Exit"]    = 0x0102000a;
    // Device keys
    _shortcut_map["Qt::Key_Context1"] = 0x01100000;
    _shortcut_map["Qt::Key_Context2"] = 0x01100001;
    _shortcut_map["Qt::Key_Context3"] = 0x01100002;
    _shortcut_map["Qt::Key_Context4"] = 0x01100003;
    _shortcut_map["Qt::Key_Call"] = 0x01100004;      // set absolute state to in a call (do not toggle state)
    _shortcut_map["Qt::Key_Hangup"] = 0x01100005;    // set absolute state to hang up (do not toggle state)
    _shortcut_map["Qt::Key_Flip"] = 0x01100006;
    _shortcut_map["Qt::Key_ToggleCallHangup"] = 0x01100007; // a toggle key for answering; or hanging up; based on current call state
    _shortcut_map["Qt::Key_VoiceDial"] = 0x01100008;
    _shortcut_map["Qt::Key_LastNumberRedial"] = 0x01100009;
    _shortcut_map["Qt::Key_Camera"] = 0x01100020;
    _shortcut_map["Qt::Key_CameraFocus"] = 0x01100021;
    _shortcut_map["Qt::Key_unknown"] = 0x01ffffff;


    // Now set QStandardKey::
    _shortcut_map["QKeySequence::AddTab"] = 19; //	Add new tab.
    _shortcut_map["QKeySequence::Back"]	= 13; //	Navigate back.
    _shortcut_map["QKeySequence::Bold"]	= 27; //	Bold text.
    _shortcut_map["QKeySequence::Close"] = 4; //	Close document/tab.
    _shortcut_map["QKeySequence::Copy"]	= 9; //	Copy.
    _shortcut_map["QKeySequence::Cut"]	= 8; //	Cut.
    _shortcut_map["QKeySequence::Delete"]	= 7; //	Delete.
    _shortcut_map["QKeySequence::DeleteEndOfLine"]	= 60; //	Delete end of line.
    _shortcut_map["QKeySequence::DeleteEndOfWord"]	= 59; //	Delete word from the end of the cursor.
    _shortcut_map["QKeySequence::DeleteStartOfWord"] = 58; //	Delete the beginning of a word up to the cursor.
    _shortcut_map["QKeySequence::DeleteCompleteLine"]	= 68; //	Delete the entire line.
    _shortcut_map["QKeySequence::Find"]	= 22; //	Find in document.
    _shortcut_map["QKeySequence::FindNext"]	= 23; //	Find next result.
    _shortcut_map["QKeySequence::FindPrevious"]	= 24; //	Find previous result.
    _shortcut_map["QKeySequence::Forward"]	= 14; //	Navigate forward.
    _shortcut_map["QKeySequence::HelpContents"]	= 1; //	Open help contents.
    _shortcut_map["QKeySequence::InsertLineSeparator"]	= 62; //	Insert a new line.
    _shortcut_map["QKeySequence::InsertParagraphSeparator"]	= 61; //	Insert a new paragraph.
    _shortcut_map["QKeySequence::Italic"]	= 28; //	Italic text.
    _shortcut_map["QKeySequence::MoveToEndOfBlock"]	= 41; //	Move cursor to end of block. This shortcut is only used on the OS X.
    _shortcut_map["QKeySequence::MoveToEndOfDocument"]	= 43; //	Move cursor to end of document.
    _shortcut_map["QKeySequence::MoveToEndOfLine"]	= 39; //	Move cursor to end of line.
    _shortcut_map["QKeySequence::MoveToNextChar"]	= 30; //	Move cursor to next character.
    _shortcut_map["QKeySequence::MoveToNextLine"]	= 34; //	Move cursor to next line.
    _shortcut_map["QKeySequence::MoveToNextPage"]	= 36; //	Move cursor to next page.
    _shortcut_map["QKeySequence::MoveToNextWord"]	= 32; //	Move cursor to next word.
    _shortcut_map["QKeySequence::MoveToPreviousChar"]	= 31; //	Move cursor to previous character.
    _shortcut_map["QKeySequence::MoveToPreviousLine"]	= 35; //	Move cursor to previous line.
    _shortcut_map["QKeySequence::MoveToPreviousPage"]	= 37; //	Move cursor to previous page.
    _shortcut_map["QKeySequence::MoveToPreviousWord"]	= 33; //	Move cursor to previous word.
    _shortcut_map["QKeySequence::MoveToStartOfBlock"]	= 40; //	Move cursor to start of a block. This shortcut is only used on OS X.
    _shortcut_map["QKeySequence::MoveToStartOfDocument"] = 42; //	Move cursor to start of document.
    _shortcut_map["QKeySequence::MoveToStartOfLine"] = 38; //	Move cursor to start of line.
    _shortcut_map["QKeySequence::New"]	= 6; //	Create new document.
    _shortcut_map["QKeySequence::NextChild"] = 20; //	Navigate to next tab or child window.
    _shortcut_map["QKeySequence::Open"]	= 3; //	Open document.
    _shortcut_map["QKeySequence::Paste"] = 10; //	Paste.
    _shortcut_map["QKeySequence::Preferences"]	= 64; //	Open the preferences dialog.
    _shortcut_map["QKeySequence::PreviousChild"] = 21; //	Navigate to previous tab or child window.
    _shortcut_map["QKeySequence::Print"] = 18; //	Print document.
    _shortcut_map["QKeySequence::Quit"]	= 65; //	Quit the application.
    _shortcut_map["QKeySequence::Redo"]	= 12; //	Redo.
    _shortcut_map["QKeySequence::Refresh"]	= 15; //	Refresh or reload current document.
    _shortcut_map["QKeySequence::Replace"]	= 25; //	Find and replace.
    _shortcut_map["QKeySequence::SaveAs"]	= 63; //	Save document after prompting the user for a file name.
    _shortcut_map["QKeySequence::Save"]	= 5; //	Save document.
    _shortcut_map["QKeySequence::SelectAll"] = 26; //	Select all text.
    _shortcut_map["QKeySequence::Deselect"]	= 67; //	Deselect text. Since 5.1
    _shortcut_map["QKeySequence::SelectEndOfBlock"]	= 55; //	Extend selection to the end of a text block. This shortcut is only used on OS X.
    _shortcut_map["QKeySequence::SelectEndOfDocument"]	= 57; //	Extend selection to end of document.
    _shortcut_map["QKeySequence::SelectEndOfLine"]	= 53; //	Extend selection to end of line.
    _shortcut_map["QKeySequence::SelectNextChar"]	= 44; //	Extend selection to next character.
    _shortcut_map["QKeySequence::SelectNextLine"]	= 48; //	Extend selection to next line.
    _shortcut_map["QKeySequence::SelectNextPage"]	= 50; //	Extend selection to next page.
    _shortcut_map["QKeySequence::SelectNextWord"]	= 46; //	Extend selection to next word.
    _shortcut_map["QKeySequence::SelectPreviousChar"]	= 45; //	Extend selection to previous character.
    _shortcut_map["QKeySequence::SelectPreviousLine"]	= 49; //	Extend selection to previous line.
    _shortcut_map["QKeySequence::SelectPreviousPage"]	= 51; //	Extend selection to previous page.
    _shortcut_map["QKeySequence::SelectPreviousWord"]	= 47; //	Extend selection to previous word.
    _shortcut_map["QKeySequence::SelectStartOfBlock"]	= 54; //	Extend selection to the start of a text block. This shortcut is only used on OS X.
    _shortcut_map["QKeySequence::SelectStartOfDocument"] = 56; //	Extend selection to start of document.
    _shortcut_map["QKeySequence::SelectStartOfLine"] = 52; //	Extend selection to start of line.
    _shortcut_map["QKeySequence::Underline"] = 29; //	Underline text.
    _shortcut_map["QKeySequence::Undo"]	= 11; //	Undo.
    _shortcut_map["QKeySequence::UnknownKey"]	= 0; //	Unbound key.
    _shortcut_map["QKeySequence::WhatsThis"] = 2; //	Activate "what's this".
    _shortcut_map["QKeySequence::ZoomIn"]	= 16; //	Zoom in.
    _shortcut_map["QKeySequence::ZoomOut"]	= 17; //	Zoom out.
    _shortcut_map["QKeySequence::FullScreen"]	= 66; //	Toggle the window state to/from full screen.
    return _shortcut_map;
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
            const int invalidkey = 0;
            const QString sc = QObject::tr(actobj.value("shortcut").toString().toLatin1().data());
            if (! sc.isNull() && ! sc.isEmpty()) {
            	if (sc.contains("Qt::") || sc.contains("QKeySequence::")) {
					// Split by +, use entire string (____::____) as key for int value, then use it to generate the QKeySequence.
					QStringList lst = sc.split("+");
					QKeySequence seq;
					switch(lst.size()) {
					case 1:
                        if (_shortcut_map.value(lst.at(0), invalidkey) == invalidkey) {
                            qWarning(lst.at(0).toLatin1() + " is not a recognized shortcut value");
                        }
						seq = QKeySequence(_shortcut_map.value(lst.at(0), invalidkey));
						break;
					case 2:
                        if (_shortcut_map.value(lst.at(0), invalidkey) == invalidkey) {
                            qWarning(lst.at(0).toLatin1() + " is not a recognized shortcut value");
                        }
                        if (_shortcut_map.value(lst.at(1), invalidkey) == invalidkey) {
                            qWarning(lst.at(1).toLatin1() + " is not a recognized shortcut value");
                        }
						seq = QKeySequence(
							_shortcut_map.value(lst.at(0), invalidkey),
							_shortcut_map.value(lst.at(1), invalidkey));
						break;
					case 3:
                        if (_shortcut_map.value(lst.at(0), invalidkey) == invalidkey) {
                            qWarning(lst.at(0).toLatin1() + " is not a recognized shortcut value");
                        }
                        if (_shortcut_map.value(lst.at(1), invalidkey) == invalidkey) {
                            qWarning(lst.at(1).toLatin1() + " is not a recognized shortcut value");
                        }
                        if (_shortcut_map.value(lst.at(2), invalidkey) == invalidkey) {
                            qWarning(lst.at(2).toLatin1() + " is not a recognized shortcut value");
                        }
						seq = QKeySequence(
							_shortcut_map.value(lst.at(0), invalidkey),
							_shortcut_map.value(lst.at(1), invalidkey),
							_shortcut_map.value(lst.at(2), invalidkey));
						break;
					case 4:
                        if (_shortcut_map.value(lst.at(0), invalidkey) == invalidkey) {
                            qWarning(lst.at(0).toLatin1() + " is not a recognized shortcut value");
                        }
                        if (_shortcut_map.value(lst.at(1), invalidkey) == invalidkey) {
                            qWarning(lst.at(1).toLatin1() + " is not a recognized shortcut value");
                        }
                        if (_shortcut_map.value(lst.at(2), invalidkey) == invalidkey) {
                            qWarning(lst.at(2).toLatin1() + " is not a recognized shortcut value");
                        }
                        if (_shortcut_map.value(lst.at(3), invalidkey) == invalidkey) {
                            qWarning(lst.at(3).toLatin1() + " is not a recognized shortcut value");
                        }

						seq = QKeySequence(
							_shortcut_map.value(lst.at(0), invalidkey),
							_shortcut_map.value(lst.at(1), invalidkey),
							_shortcut_map.value(lst.at(2), invalidkey),
							_shortcut_map.value(lst.at(3), invalidkey));
						break;
					default:
						break;
					}
					act->setShortcut(seq);
	          	} else {
	                act->setShortcut(QKeySequence::fromString(sc));
	            }
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
