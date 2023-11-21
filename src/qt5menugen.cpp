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
    _shortcut_map = QMap<QString, int>();
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

void QtMenuGen::load_shortcuts()
{
	{
		// Use short name as we will be referring to it a LOT:
		// attr will be cleaned up after scope is ended
		QMap<QString, int> sm = _shortcut_map;
		sm["Qt::Key_Escape"] = 0x01000000;                // misc keys
        sm["Qt::Key_Tab"] = 0x01000001;
        sm["Qt::Key_Backtab"] = 0x01000002;
        sm["Qt::Key_Backspace"] = 0x01000003;
        sm["Qt::Key_Return"] = 0x01000004;
        sm["Qt::Key_Enter"] = 0x01000005;
        sm["Qt::Key_Insert"] = 0x01000006;
        sm["Qt::Key_Delete"] = 0x01000007;
        sm["Qt::Key_Pause"] = 0x01000008;
        sm["Qt::Key_Print"] = 0x01000009;               // print screen
        sm["Qt::Key_SysReq"] = 0x0100000a;
        sm["Qt::Key_Clear"] = 0x0100000b;
        sm["Qt::Key_Home"] = 0x01000010;                // cursor movement
        sm["Qt::Key_End"] = 0x01000011;
        sm["Qt::Key_Left"] = 0x01000012;
        sm["Qt::Key_Up"] = 0x01000013;
        sm["Qt::Key_Right"] = 0x01000014;
        sm["Qt::Key_Down"] = 0x01000015;
        sm["Qt::Key_PageUp"] = 0x01000016;
        sm["Qt::Key_PageDown"] = 0x01000017;
        sm["Qt::Key_Shift"] = 0x01000020;                // modifiers
        sm["Qt::Key_Control"] = 0x01000021;
        sm["Qt::Key_Meta"] = 0x01000022;
        sm["Qt::Key_Alt"] = 0x01000023;
        sm["Qt::Key_CapsLock"] = 0x01000024;
        sm["Qt::Key_NumLock"] = 0x01000025;
        sm["Qt::Key_ScrollLock"] = 0x01000026;
        sm["Qt::Key_F1"] = 0x01000030;                // function keys
        sm["Qt::Key_F2"] = 0x01000031;
        sm["Qt::Key_F3"] = 0x01000032;
        sm["Qt::Key_F4"] = 0x01000033;
        sm["Qt::Key_F5"] = 0x01000034;
        sm["Qt::Key_F6"] = 0x01000035;
        sm["Qt::Key_F7"] = 0x01000036;
        sm["Qt::Key_F8"] = 0x01000037;
        sm["Qt::Key_F9"] = 0x01000038;
        sm["Qt::Key_F10"] = 0x01000039;
        sm["Qt::Key_F11"] = 0x0100003a;
        sm["Qt::Key_F12"] = 0x0100003b;
        sm["Qt::Key_F13"] = 0x0100003c;
        sm["Qt::Key_F14"] = 0x0100003d;
        sm["Qt::Key_F15"] = 0x0100003e;
        sm["Qt::Key_F16"] = 0x0100003f;
        sm["Qt::Key_F17"] = 0x01000040;
        sm["Qt::Key_F18"] = 0x01000041;
        sm["Qt::Key_F19"] = 0x01000042;
        sm["Qt::Key_F20"] = 0x01000043;
        sm["Qt::Key_F21"] = 0x01000044;
        sm["Qt::Key_F22"] = 0x01000045;
        sm["Qt::Key_F23"] = 0x01000046;
        sm["Qt::Key_F24"] = 0x01000047;
        sm["Qt::Key_F25"] = 0x01000048;                // F25 .. F35 only on X11
        sm["Qt::Key_F26"] = 0x01000049;
        sm["Qt::Key_F27"] = 0x0100004a;
        sm["Qt::Key_F28"] = 0x0100004b;
        sm["Qt::Key_F29"] = 0x0100004c;
        sm["Qt::Key_F30"] = 0x0100004d;
        sm["Qt::Key_F31"] = 0x0100004e;
        sm["Qt::Key_F32"] = 0x0100004f;
        sm["Qt::Key_F33"] = 0x01000050;
        sm["Qt::Key_F34"] = 0x01000051;
        sm["Qt::Key_F35"] = 0x01000052;
        sm["Qt::Key_Super_L"] = 0x01000053;                 // extra keys
        sm["Qt::Key_Super_R"] = 0x01000054;
        sm["Qt::Key_Menu"] = 0x01000055;
        sm["Qt::Key_Hyper_L"] = 0x01000056;
        sm["Qt::Key_Hyper_R"] = 0x01000057;
        sm["Qt::Key_Help"] = 0x01000058;
        sm["Qt::Key_Direction_L"] = 0x01000059;
        sm["Qt::Key_Direction_R"] = 0x01000060;
        sm["Qt::Key_Space"] = 0x20;                // 7 bit printable ASCII
        sm["Qt::Key_Any"] = Key_Space;
        sm["Qt::Key_Exclam"] = 0x21;
        sm["Qt::Key_QuoteDbl"] = 0x22;
        sm["Qt::Key_NumberSign"] = 0x23;
        sm["Qt::Key_Dollar"] = 0x24;
        sm["Qt::Key_Percent"] = 0x25;
        sm["Qt::Key_Ampersand"] = 0x26;
        sm["Qt::Key_Apostrophe"] = 0x27;
        sm["Qt::Key_ParenLeft"] = 0x28;
        sm["Qt::Key_ParenRight"] = 0x29;
        sm["Qt::Key_Asterisk"] = 0x2a;
        sm["Qt::Key_Plus"] = 0x2b;
        sm["Qt::Key_Comma"] = 0x2c;
        sm["Qt::Key_Minus"] = 0x2d;
        sm["Qt::Key_Period"] = 0x2e;
        sm["Qt::Key_Slash"] = 0x2f;
        sm["Qt::Key_0"] = 0x30;
        sm["Qt::Key_1"] = 0x31;
        sm["Qt::Key_2"] = 0x32;
        sm["Qt::Key_3"] = 0x33;
        sm["Qt::Key_4"] = 0x34;
        sm["Qt::Key_5"] = 0x35;
        sm["Qt::Key_6"] = 0x36;
        sm["Qt::Key_7"] = 0x37;
        sm["Qt::Key_8"] = 0x38;
        sm["Qt::Key_9"] = 0x39;
        sm["Qt::Key_Colon"] = 0x3a;
        sm["Qt::Key_Semicolon"] = 0x3b;
        sm["Qt::Key_Less"] = 0x3c;
        sm["Qt::Key_Equal"] = 0x3d;
        sm["Qt::Key_Greater"] = 0x3e;
        sm["Qt::Key_Question"] = 0x3f;
        sm["Qt::Key_At"] = 0x40;
        sm["Qt::Key_A"] = 0x41;
        sm["Qt::Key_B"] = 0x42;
        sm["Qt::Key_C"] = 0x43;
        sm["Qt::Key_D"] = 0x44;
        sm["Qt::Key_E"] = 0x45;
        sm["Qt::Key_F"] = 0x46;
        sm["Qt::Key_G"] = 0x47;
        sm["Qt::Key_H"] = 0x48;
        sm["Qt::Key_I"] = 0x49;
        sm["Qt::Key_J"] = 0x4a;
        sm["Qt::Key_K"] = 0x4b;
        sm["Qt::Key_L"] = 0x4c;
        sm["Qt::Key_M"] = 0x4d;
        sm["Qt::Key_N"] = 0x4e;
        sm["Qt::Key_O"] = 0x4f;
        sm["Qt::Key_P"] = 0x50;
        sm["Qt::Key_Q"] = 0x51;
        sm["Qt::Key_R"] = 0x52;
        sm["Qt::Key_S"] = 0x53;
        sm["Qt::Key_T"] = 0x54;
        sm["Qt::Key_U"] = 0x55;
        sm["Qt::Key_V"] = 0x56;
        sm["Qt::Key_W"] = 0x57;
        sm["Qt::Key_X"] = 0x58;
        sm["Qt::Key_Y"] = 0x59;
        sm["Qt::Key_Z"] = 0x5a;
        sm["Qt::Key_BracketLeft"] = 0x5b;
        sm["Qt::Key_Backslash"] = 0x5c;
        sm["Qt::Key_BracketRight"] = 0x5d;
        sm["Qt::Key_AsciiCircum"] = 0x5e;
        sm["Qt::Key_Underscore"] = 0x5f;
        sm["Qt::Key_QuoteLeft"] = 0x60;
        sm["Qt::Key_BraceLeft"] = 0x7b;
        sm["Qt::Key_Bar"] = 0x7c;
        sm["Qt::Key_BraceRight"] = 0x7d;
        sm["Qt::Key_AsciiTilde"] = 0x7e;
        sm["Qt::Key_nobreakspace"] = 0x0a0;
        sm["Qt::Key_exclamdown"] = 0x0a1;
        sm["Qt::Key_cent"] = 0x0a2;
        sm["Qt::Key_sterling"] = 0x0a3;
        sm["Qt::Key_currency"] = 0x0a4;
        sm["Qt::Key_yen"] = 0x0a5;
        sm["Qt::Key_brokenbar"] = 0x0a6;
        sm["Qt::Key_section"] = 0x0a7;
        sm["Qt::Key_diaeresis"] = 0x0a8;
        sm["Qt::Key_copyright"] = 0x0a9;
        sm["Qt::Key_ordfeminine"] = 0x0aa;
        sm["Qt::Key_guillemotleft"] = 0x0ab;        // left angle quotation mark
        sm["Qt::Key_notsign"] = 0x0ac;
        sm["Qt::Key_hyphen"] = 0x0ad;
        sm["Qt::Key_registered"] = 0x0ae;
        sm["Qt::Key_macron"] = 0x0af;
        sm["Qt::Key_degree"] = 0x0b0;
        sm["Qt::Key_plusminus"] = 0x0b1;
        sm["Qt::Key_twosuperior"] = 0x0b2;
        sm["Qt::Key_threesuperior"] = 0x0b3;
        sm["Qt::Key_acute"] = 0x0b4;
        sm["Qt::Key_mu"] = 0x0b5;
        sm["Qt::Key_paragraph"] = 0x0b6;
        sm["Qt::Key_periodcentered"] = 0x0b7;
        sm["Qt::Key_cedilla"] = 0x0b8;
        sm["Qt::Key_onesuperior"] = 0x0b9;
        sm["Qt::Key_masculine"] = 0x0ba;
        sm["Qt::Key_guillemotright"] = 0x0bb;        // right angle quotation mark
        sm["Qt::Key_onequarter"] = 0x0bc;
        sm["Qt::Key_onehalf"] = 0x0bd;
        sm["Qt::Key_threequarters"] = 0x0be;
        sm["Qt::Key_questiondown"] = 0x0bf;
        sm["Qt::Key_Agrave"] = 0x0c0;
        sm["Qt::Key_Aacute"] = 0x0c1;
        sm["Qt::Key_Acircumflex"] = 0x0c2;
        sm["Qt::Key_Atilde"] = 0x0c3;
        sm["Qt::Key_Adiaeresis"] = 0x0c4;
        sm["Qt::Key_Aring"] = 0x0c5;
        sm["Qt::Key_AE"] = 0x0c6;
        sm["Qt::Key_Ccedilla"] = 0x0c7;
        sm["Qt::Key_Egrave"] = 0x0c8;
        sm["Qt::Key_Eacute"] = 0x0c9;
        sm["Qt::Key_Ecircumflex"] = 0x0ca;
        sm["Qt::Key_Ediaeresis"] = 0x0cb;
        sm["Qt::Key_Igrave"] = 0x0cc;
        sm["Qt::Key_Iacute"] = 0x0cd;
        sm["Qt::Key_Icircumflex"] = 0x0ce;
        sm["Qt::Key_Idiaeresis"] = 0x0cf;
        sm["Qt::Key_ETH"] = 0x0d0;
        sm["Qt::Key_Ntilde"] = 0x0d1;
        sm["Qt::Key_Ograve"] = 0x0d2;
        sm["Qt::Key_Oacute"] = 0x0d3;
        sm["Qt::Key_Ocircumflex"] = 0x0d4;
        sm["Qt::Key_Otilde"] = 0x0d5;
        sm["Qt::Key_Odiaeresis"] = 0x0d6;
        sm["Qt::Key_multiply"] = 0x0d7;
        sm["Qt::Key_Ooblique"] = 0x0d8;
        sm["Qt::Key_Ugrave"] = 0x0d9;
        sm["Qt::Key_Uacute"] = 0x0da;
        sm["Qt::Key_Ucircumflex"] = 0x0db;
        sm["Qt::Key_Udiaeresis"] = 0x0dc;
        sm["Qt::Key_Yacute"] = 0x0dd;
        sm["Qt::Key_THORN"] = 0x0de;
        sm["Qt::Key_ssharp"] = 0x0df;
        sm["Qt::Key_division"] = 0x0f7;
        sm["Qt::Key_ydiaeresis"] = 0x0ff;
        // International input method support (X keycode - 0xEE00; the
        // definition follows Qt/Embedded 2.3.7) Only interesting if
        // you are writing your own input method
        // International & multi-key character composition
        sm["Qt::Key_AltGr"]               = 0x01001103;
        sm["Qt::Key_Multi_key"]           = 0x01001120;  // Multi-key character compose
        sm["Qt::Key_Codeinput"]           = 0x01001137;
        sm["Qt::Key_SingleCandidate"]     = 0x0100113c;
        sm["Qt::Key_MultipleCandidate"]   = 0x0100113d;
        sm["Qt::Key_PreviousCandidate"]   = 0x0100113e;
        // Misc Functions
        sm["Qt::Key_Mode_switch"]         = 0x0100117e;  // Character set switch
        //Key_script_switch       = 0x0100117e;  // Alias for mode_switch
        // Japanese keyboard support
        sm["Qt::Key_Kanji"]               = 0x01001121;  // Kanji; Kanji convert
        sm["Qt::Key_Muhenkan"]            = 0x01001122;  // Cancel Conversion
        //Key_Henkan_Mode         = 0x01001123;  // Start/Stop Conversion
        sm["Qt::Key_Henkan"]              = 0x01001123;  // Alias for Henkan_Mode
        sm["Qt::Key_Romaji"]              = 0x01001124;  // to Romaji
        sm["Qt::Key_Hiragana"]            = 0x01001125;  // to Hiragana
        sm["Qt::Key_Katakana"]            = 0x01001126;  // to Katakana
        sm["Qt::Key_Hiragana_Katakana"]   = 0x01001127;  // Hiragana/Katakana toggle
        sm["Qt::Key_Zenkaku"]             = 0x01001128;  // to Zenkaku
        sm["Qt::Key_Hankaku"]             = 0x01001129;  // to Hankaku
        sm["Qt::Key_Zenkaku_Hankaku"]     = 0x0100112a;  // Zenkaku/Hankaku toggle
        sm["Qt::Key_Touroku"]             = 0x0100112b;  // Add to Dictionary
        sm["Qt::Key_Massyo"]              = 0x0100112c;  // Delete from Dictionary
        sm["Qt::Key_Kana_Lock"]           = 0x0100112d;  // Kana Lock
        sm["Qt::Key_Kana_Shift"]          = 0x0100112e;  // Kana Shift
        sm["Qt::Key_Eisu_Shift"]          = 0x0100112f;  // Alphanumeric Shift
        sm["Qt::Key_Eisu_toggle"]         = 0x01001130;  // Alphanumeric toggle
        //Key_Kanji_Bangou        = 0x01001137;  // Codeinput
        //Key_Zen_Koho            = 0x0100113d;  // Multiple/All Candidate(s)
        //Key_Mae_Koho            = 0x0100113e;  // Previous Candidate
        // Korean keyboard support
        //
        // In fact; many Korean users need only 2 keys; Key_Hangul and
        // Key_Hangul_Hanja. But rest of the keys are good for future.
        sm["Qt::Key_Hangul"]              = 0x01001131;  // Hangul start/stop(toggle)
        sm["Qt::Key_Hangul_Start"]        = 0x01001132;  // Hangul start
        sm["Qt::Key_Hangul_End"]          = 0x01001133;  // Hangul end; English start
        sm["Qt::Key_Hangul_Hanja"]        = 0x01001134;  // Start Hangul->Hanja Conversion
        sm["Qt::Key_Hangul_Jamo"]         = 0x01001135;  // Hangul Jamo mode
        sm["Qt::Key_Hangul_Romaja"]       = 0x01001136;  // Hangul Romaja mode
        //Key_Hangul_Codeinput    = 0x01001137;  // Hangul code input mode
        sm["Qt::Key_Hangul_Jeonja"]       = 0x01001138;  // Jeonja mode
        sm["Qt::Key_Hangul_Banja"]        = 0x01001139;  // Banja mode
        sm["Qt::Key_Hangul_PreHanja"]     = 0x0100113a;  // Pre Hanja conversion
        sm["Qt::Key_Hangul_PostHanja"]    = 0x0100113b;  // Post Hanja conversion
        //Key_Hangul_SingleCandidate   = 0x0100113c;  // Single candidate
        //Key_Hangul_MultipleCandidate = 0x0100113d;  // Multiple candidate
        //Key_Hangul_PreviousCandidate = 0x0100113e;  // Previous candidate
        sm["Qt::Key_Hangul_Special"]      = 0x0100113f;  // Special symbols
        //Key_Hangul_switch       = 0x0100117e;  // Alias for mode_switch
        // dead keys (X keycode - 0xED00 to avoid the conflict)
        sm["Qt::Key_Dead_Grave"]          = 0x01001250;
        sm["Qt::Key_Dead_Acute"]          = 0x01001251;
        sm["Qt::Key_Dead_Circumflex"]     = 0x01001252;
        sm["Qt::Key_Dead_Tilde"]          = 0x01001253;
        sm["Qt::Key_Dead_Macron"]         = 0x01001254;
        sm["Qt::Key_Dead_Breve"]          = 0x01001255;
        sm["Qt::Key_Dead_Abovedot"]       = 0x01001256;
        sm["Qt::Key_Dead_Diaeresis"]      = 0x01001257;
        sm["Qt::Key_Dead_Abovering"]      = 0x01001258;
        sm["Qt::Key_Dead_Doubleacute"]    = 0x01001259;
        sm["Qt::Key_Dead_Caron"]          = 0x0100125a;
        sm["Qt::Key_Dead_Cedilla"]        = 0x0100125b;
        sm["Qt::Key_Dead_Ogonek"]         = 0x0100125c;
        sm["Qt::Key_Dead_Iota"]           = 0x0100125d;
        sm["Qt::Key_Dead_Voiced_Sound"]   = 0x0100125e;
        sm["Qt::Key_Dead_Semivoiced_Sound"] = 0x0100125f;
        sm["Qt::Key_Dead_Belowdot"]       = 0x01001260;
        sm["Qt::Key_Dead_Hook"]           = 0x01001261;
        sm["Qt::Key_Dead_Horn"]           = 0x01001262;
        sm["Qt::Key_Dead_Stroke"]         = 0x01001263;
        sm["Qt::Key_Dead_Abovecomma"]     = 0x01001264;
        sm["Qt::Key_Dead_Abovereversedcomma"] = 0x01001265;
        sm["Qt::Key_Dead_Doublegrave"]    = 0x01001266;
        sm["Qt::Key_Dead_Belowring"]      = 0x01001267;
        sm["Qt::Key_Dead_Belowmacron"]    = 0x01001268;
        sm["Qt::Key_Dead_Belowcircumflex"] = 0x01001269;
        sm["Qt::Key_Dead_Belowtilde"]     = 0x0100126a;
        sm["Qt::Key_Dead_Belowbreve"]     = 0x0100126b;
        sm["Qt::Key_Dead_Belowdiaeresis"] = 0x0100126c;
        sm["Qt::Key_Dead_Invertedbreve"]  = 0x0100126d;
        sm["Qt::Key_Dead_Belowcomma"]     = 0x0100126e;
        sm["Qt::Key_Dead_Currency"]       = 0x0100126f;
        sm["Qt::Key_Dead_a"]              = 0x01001280;
        sm["Qt::Key_Dead_A"]              = 0x01001281;
        sm["Qt::Key_Dead_e"]              = 0x01001282;
        sm["Qt::Key_Dead_E"]              = 0x01001283;
        sm["Qt::Key_Dead_i"]              = 0x01001284;
        sm["Qt::Key_Dead_I"]              = 0x01001285;
        sm["Qt::Key_Dead_o"]              = 0x01001286;
        sm["Qt::Key_Dead_O"]              = 0x01001287;
        sm["Qt::Key_Dead_u"]              = 0x01001288;
        sm["Qt::Key_Dead_U"]              = 0x01001289;
        sm["Qt::Key_Dead_Small_Schwa"]    = 0x0100128a;
        sm["Qt::Key_Dead_Capital_Schwa"]  = 0x0100128b;
        sm["Qt::Key_Dead_Greek"]          = 0x0100128c;
        sm["Qt::Key_Dead_Lowline"]        = 0x01001290;
        sm["Qt::Key_Dead_Aboveverticalline"] = 0x01001291;
        sm["Qt::Key_Dead_Belowverticalline"] = 0x01001292;
        sm["Qt::Key_Dead_Longsolidusoverlay"] = 0x01001293;
        // multimedia/internet keys - ignored by default - see QKeyEvent c'tor
        sm["Qt::Key_Back"]  = 0x01000061;
        sm["Qt::Key_Forward"]  = 0x01000062;
        sm["Qt::Key_Stop"]  = 0x01000063;
        sm["Qt::Key_Refresh"]  = 0x01000064;
        sm["Qt::Key_VolumeDown"] = 0x01000070;
        sm["Qt::Key_VolumeMute"]  = 0x01000071;
        sm["Qt::Key_VolumeUp"] = 0x01000072;
        sm["Qt::Key_BassBoost"] = 0x01000073;
        sm["Qt::Key_BassUp"] = 0x01000074;
        sm["Qt::Key_BassDown"] = 0x01000075;
        sm["Qt::Key_TrebleUp"] = 0x01000076;
        sm["Qt::Key_TrebleDown"] = 0x01000077;
        sm["Qt::Key_MediaPlay"]  = 0x01000080;
        sm["Qt::Key_MediaStop"]  = 0x01000081;
        sm["Qt::Key_MediaPrevious"]  = 0x01000082;
        sm["Qt::Key_MediaNext"]  = 0x01000083;
        sm["Qt::Key_MediaRecord"] = 0x01000084;
        sm["Qt::Key_MediaPause"] = 0x1000085;
        sm["Qt::Key_MediaTogglePlayPause"] = 0x1000086;
        sm["Qt::Key_HomePage"]  = 0x01000090;
        sm["Qt::Key_Favorites"]  = 0x01000091;
        sm["Qt::Key_Search"]  = 0x01000092;
        sm["Qt::Key_Standby"] = 0x01000093;
        sm["Qt::Key_OpenUrl"] = 0x01000094;
        sm["Qt::Key_LaunchMail"]  = 0x010000a0;
        sm["Qt::Key_LaunchMedia"] = 0x010000a1;
        sm["Qt::Key_Launch0"]  = 0x010000a2;
        sm["Qt::Key_Launch1"]  = 0x010000a3;
        sm["Qt::Key_Launch2"]  = 0x010000a4;
        sm["Qt::Key_Launch3"]  = 0x010000a5;
        sm["Qt::Key_Launch4"]  = 0x010000a6;
        sm["Qt::Key_Launch5"]  = 0x010000a7;
        sm["Qt::Key_Launch6"]  = 0x010000a8;
        sm["Qt::Key_Launch7"]  = 0x010000a9;
        sm["Qt::Key_Launch8"]  = 0x010000aa;
        sm["Qt::Key_Launch9"]  = 0x010000ab;
        sm["Qt::Key_LaunchA"]  = 0x010000ac;
        sm["Qt::Key_LaunchB"]  = 0x010000ad;
        sm["Qt::Key_LaunchC"]  = 0x010000ae;
        sm["Qt::Key_LaunchD"]  = 0x010000af;
        sm["Qt::Key_LaunchE"]  = 0x010000b0;
        sm["Qt::Key_LaunchF"]  = 0x010000b1;
        sm["Qt::Key_MonBrightnessUp"] = 0x010000b2;
        sm["Qt::Key_MonBrightnessDown"] = 0x010000b3;
        sm["Qt::Key_KeyboardLightOnOff"] = 0x010000b4;
        sm["Qt::Key_KeyboardBrightnessUp"] = 0x010000b5;
        sm["Qt::Key_KeyboardBrightnessDown"] = 0x010000b6;
        sm["Qt::Key_PowerOff"] = 0x010000b7;
        sm["Qt::Key_WakeUp"] = 0x010000b8;
        sm["Qt::Key_Eject"] = 0x010000b9;
        sm["Qt::Key_ScreenSaver"] = 0x010000ba;
        sm["Qt::Key_WWW"] = 0x010000bb;
        sm["Qt::Key_Memo"] = 0x010000bc;
        sm["Qt::Key_LightBulb"] = 0x010000bd;
        sm["Qt::Key_Shop"] = 0x010000be;
        sm["Qt::Key_History"] = 0x010000bf;
        sm["Qt::Key_AddFavorite"] = 0x010000c0;
        sm["Qt::Key_HotLinks"] = 0x010000c1;
        sm["Qt::Key_BrightnessAdjust"] = 0x010000c2;
        sm["Qt::Key_Finance"] = 0x010000c3;
        sm["Qt::Key_Community"] = 0x010000c4;
        sm["Qt::Key_AudioRewind"] = 0x010000c5; // Media rewind
        sm["Qt::Key_BackForward"] = 0x010000c6;
        sm["Qt::Key_ApplicationLeft"] = 0x010000c7;
        sm["Qt::Key_ApplicationRight"] = 0x010000c8;
        sm["Qt::Key_Book"] = 0x010000c9;
        sm["Qt::Key_CD"] = 0x010000ca;
        sm["Qt::Key_Calculator"] = 0x010000cb;
        sm["Qt::Key_ToDoList"] = 0x010000cc;
        sm["Qt::Key_ClearGrab"] = 0x010000cd;
        sm["Qt::Key_Close"] = 0x010000ce;
        sm["Qt::Key_Copy"] = 0x010000cf;
        sm["Qt::Key_Cut"] = 0x010000d0;
        sm["Qt::Key_Display"] = 0x010000d1; // Output switch key
        sm["Qt::Key_DOS"] = 0x010000d2;
        sm["Qt::Key_Documents"] = 0x010000d3;
        sm["Qt::Key_Excel"] = 0x010000d4;
        sm["Qt::Key_Explorer"] = 0x010000d5;
        sm["Qt::Key_Game"] = 0x010000d6;
        sm["Qt::Key_Go"] = 0x010000d7;
        sm["Qt::Key_iTouch"] = 0x010000d8;
        sm["Qt::Key_LogOff"] = 0x010000d9;
        sm["Qt::Key_Market"] = 0x010000da;
        sm["Qt::Key_Meeting"] = 0x010000db;
        sm["Qt::Key_MenuKB"] = 0x010000dc;
        sm["Qt::Key_MenuPB"] = 0x010000dd;
        sm["Qt::Key_MySites"] = 0x010000de;
        sm["Qt::Key_News"] = 0x010000df;
        sm["Qt::Key_OfficeHome"] = 0x010000e0;
        sm["Qt::Key_Option"] = 0x010000e1;
        sm["Qt::Key_Paste"] = 0x010000e2;
        sm["Qt::Key_Phone"] = 0x010000e3;
        sm["Qt::Key_Calendar"] = 0x010000e4;
        sm["Qt::Key_Reply"] = 0x010000e5;
        sm["Qt::Key_Reload"] = 0x010000e6;
        sm["Qt::Key_RotateWindows"] = 0x010000e7;
        sm["Qt::Key_RotationPB"] = 0x010000e8;
        sm["Qt::Key_RotationKB"] = 0x010000e9;
        sm["Qt::Key_Save"] = 0x010000ea;
        sm["Qt::Key_Send"] = 0x010000eb;
        sm["Qt::Key_Spell"] = 0x010000ec;
        sm["Qt::Key_SplitScreen"] = 0x010000ed;
        sm["Qt::Key_Support"] = 0x010000ee;
        sm["Qt::Key_TaskPane"] = 0x010000ef;
        sm["Qt::Key_Terminal"] = 0x010000f0;
        sm["Qt::Key_Tools"] = 0x010000f1;
        sm["Qt::Key_Travel"] = 0x010000f2;
        sm["Qt::Key_Video"] = 0x010000f3;
        sm["Qt::Key_Word"] = 0x010000f4;
        sm["Qt::Key_Xfer"] = 0x010000f5;
        sm["Qt::Key_ZoomIn"] = 0x010000f6;
        sm["Qt::Key_ZoomOut"] = 0x010000f7;
        sm["Qt::Key_Away"] = 0x010000f8;
        sm["Qt::Key_Messenger"] = 0x010000f9;
        sm["Qt::Key_WebCam"] = 0x010000fa;
        sm["Qt::Key_MailForward"] = 0x010000fb;
        sm["Qt::Key_Pictures"] = 0x010000fc;
        sm["Qt::Key_Music"] = 0x010000fd;
        sm["Qt::Key_Battery"] = 0x010000fe;
        sm["Qt::Key_Bluetooth"] = 0x010000ff;
        sm["Qt::Key_WLAN"] = 0x01000100;
        sm["Qt::Key_UWB"] = 0x01000101;
        sm["Qt::Key_AudioForward"] = 0x01000102; // Media fast-forward
        sm["Qt::Key_AudioRepeat"] = 0x01000103; // Toggle repeat mode
        sm["Qt::Key_AudioRandomPlay"] = 0x01000104; // Toggle shuffle mode
        sm["Qt::Key_Subtitle"] = 0x01000105;
        sm["Qt::Key_AudioCycleTrack"] = 0x01000106;
        sm["Qt::Key_Time"] = 0x01000107;
        sm["Qt::Key_Hibernate"] = 0x01000108;
        sm["Qt::Key_View"] = 0x01000109;
        sm["Qt::Key_TopMenu"] = 0x0100010a;
        sm["Qt::Key_PowerDown"] = 0x0100010b;
        sm["Qt::Key_Suspend"] = 0x0100010c;
        sm["Qt::Key_ContrastAdjust"] = 0x0100010d;
        sm["Qt::Key_LaunchG"]  = 0x0100010e;
        sm["Qt::Key_LaunchH"]  = 0x0100010f;
        sm["Qt::Key_TouchpadToggle"] = 0x01000110;
        sm["Qt::Key_TouchpadOn"] = 0x01000111;
        sm["Qt::Key_TouchpadOff"] = 0x01000112;
        sm["Qt::Key_MicMute"] = 0x01000113;
        sm["Qt::Key_Red"] = 0x01000114;
        sm["Qt::Key_Green"] = 0x01000115;
        sm["Qt::Key_Yellow"] = 0x01000116;
        sm["Qt::Key_Blue"] = 0x01000117;
        sm["Qt::Key_ChannelUp"] = 0x01000118;
        sm["Qt::Key_ChannelDown"] = 0x01000119;
        sm["Qt::Key_Guide"]    = 0x0100011a;
        sm["Qt::Key_Info"]     = 0x0100011b;
        sm["Qt::Key_Settings"] = 0x0100011c;
        sm["Qt::Key_MicVolumeUp"]   = 0x0100011d;
        sm["Qt::Key_MicVolumeDown"] = 0x0100011e;
        sm["Qt::Key_New"]      = 0x01000120;
        sm["Qt::Key_Open"]     = 0x01000121;
        sm["Qt::Key_Find"]     = 0x01000122;
        sm["Qt::Key_Undo"]     = 0x01000123;
        sm["Qt::Key_Redo"]     = 0x01000124;
        sm["Qt::Key_MediaLast"] = 0x0100ffff;
        // Keypad navigation keys
        sm["Qt::Key_Select"] = 0x01010000;
        sm["Qt::Key_Yes"] = 0x01010001;
        sm["Qt::Key_No"] = 0x01010002;
        // Newer misc keys
        sm["Qt::Key_Cancel"]  = 0x01020001;
        sm["Qt::Key_Printer"] = 0x01020002;
        sm["Qt::Key_Execute"] = 0x01020003;
        sm["Qt::Key_Sleep"]   = 0x01020004;
        sm["Qt::Key_Play"]    = 0x01020005; // Not the same as Key_MediaPlay
        sm["Qt::Key_Zoom"]    = 0x01020006;
        //Key_Jisho   = 0x01020007; // IME: Dictionary key
        //Key_Oyayubi_Left = 0x01020008; // IME: Left Oyayubi key
        //Key_Oyayubi_Right = 0x01020009; // IME: Right Oyayubi key
        sm["Qt::Key_Exit"]    = 0x0102000a;
        // Device keys
        sm["Qt::Key_Context1"] = 0x01100000;
        sm["Qt::Key_Context2"] = 0x01100001;
        sm["Qt::Key_Context3"] = 0x01100002;
        sm["Qt::Key_Context4"] = 0x01100003;
        sm["Qt::Key_Call"] = 0x01100004;      // set absolute state to in a call (do not toggle state)
        sm["Qt::Key_Hangup"] = 0x01100005;    // set absolute state to hang up (do not toggle state)
        sm["Qt::Key_Flip"] = 0x01100006;
        sm["Qt::Key_ToggleCallHangup"] = 0x01100007; // a toggle key for answering; or hanging up; based on current call state
        sm["Qt::Key_VoiceDial"] = 0x01100008;
        sm["Qt::Key_LastNumberRedial"] = 0x01100009;
        sm["Qt::Key_Camera"] = 0x01100020;
        sm["Qt::Key_CameraFocus"] = 0x01100021;
        sm["Qt::Key_unknown"] = 0x01ffffff;


        // Now set QStandardKey::
        sm["QKeySequence::AddTab"] = 19; //	Add new tab.
		sm["QKeySequence::Back"]	= 13; //	Navigate back.
		sm["QKeySequence::Bold"]	= 27; //	Bold text.
		sm["QKeySequence::Close"] = 4; //	Close document/tab.
		sm["QKeySequence::Copy"]	= 9; //	Copy.
		sm["QKeySequence::Cut"]	= 8; //	Cut.
		sm["QKeySequence::Delete"]	= 7; //	Delete.
		sm["QKeySequence::DeleteEndOfLine"]	= 60; //	Delete end of line.
		sm["QKeySequence::DeleteEndOfWord"]	= 59; //	Delete word from the end of the cursor.
		sm["QKeySequence::DeleteStartOfWord"] = 58; //	Delete the beginning of a word up to the cursor.
		sm["QKeySequence::DeleteCompleteLine"]	= 68; //	Delete the entire line.
		sm["QKeySequence::Find"]	= 22; //	Find in document.
		sm["QKeySequence::FindNext"]	= 23; //	Find next result.
		sm["QKeySequence::FindPrevious"]	= 24; //	Find previous result.
		sm["QKeySequence::Forward"]	= 14; //	Navigate forward.
		sm["QKeySequence::HelpContents"]	= 1; //	Open help contents.
		sm["QKeySequence::InsertLineSeparator"]	= 62; //	Insert a new line.
		sm["QKeySequence::InsertParagraphSeparator"]	= 61; //	Insert a new paragraph.
		sm["QKeySequence::Italic"]	= 28; //	Italic text.
		sm["QKeySequence::MoveToEndOfBlock"]	= 41; //	Move cursor to end of block. This shortcut is only used on the OS X.
		sm["QKeySequence::MoveToEndOfDocument"]	= 43; //	Move cursor to end of document.
		sm["QKeySequence::MoveToEndOfLine"]	= 39; //	Move cursor to end of line.
		sm["QKeySequence::MoveToNextChar"]	= 30; //	Move cursor to next character.
		sm["QKeySequence::MoveToNextLine"]	= 34; //	Move cursor to next line.
		sm["QKeySequence::MoveToNextPage"]	= 36; //	Move cursor to next page.
		sm["QKeySequence::MoveToNextWord"]	= 32; //	Move cursor to next word.
		sm["QKeySequence::MoveToPreviousChar"]	= 31; //	Move cursor to previous character.
		sm["QKeySequence::MoveToPreviousLine"]	= 35; //	Move cursor to previous line.
		sm["QKeySequence::MoveToPreviousPage"]	= 37; //	Move cursor to previous page.
		sm["QKeySequence::MoveToPreviousWord"]	= 33; //	Move cursor to previous word.
		sm["QKeySequence::MoveToStartOfBlock"]	= 40; //	Move cursor to start of a block. This shortcut is only used on OS X.
		sm["QKeySequence::MoveToStartOfDocument"] = 42; //	Move cursor to start of document.
		sm["QKeySequence::MoveToStartOfLine"] = 38; //	Move cursor to start of line.
		sm["QKeySequence::New"]	= 6; //	Create new document.
		sm["QKeySequence::NextChild"] = 20; //	Navigate to next tab or child window.
		sm["QKeySequence::Open"]	= 3; //	Open document.
		sm["QKeySequence::Paste"] = 10; //	Paste.
		sm["QKeySequence::Preferences"]	= 64; //	Open the preferences dialog.
		sm["QKeySequence::PreviousChild"] = 21; //	Navigate to previous tab or child window.
		sm["QKeySequence::Print"] = 18; //	Print document.
		sm["QKeySequence::Quit"]	= 65; //	Quit the application.
		sm["QKeySequence::Redo"]	= 12; //	Redo.
		sm["QKeySequence::Refresh"]	= 15; //	Refresh or reload current document.
		sm["QKeySequence::Replace"]	= 25; //	Find and replace.
		sm["QKeySequence::SaveAs"]	= 63; //	Save document after prompting the user for a file name.
		sm["QKeySequence::Save"]	= 5; //	Save document.
		sm["QKeySequence::SelectAll"] = 26; //	Select all text.
		sm["QKeySequence::Deselect"]	= 67; //	Deselect text. Since 5.1
		sm["QKeySequence::SelectEndOfBlock"]	= 55; //	Extend selection to the end of a text block. This shortcut is only used on OS X.
		sm["QKeySequence::SelectEndOfDocument"]	= 57; //	Extend selection to end of document.
		sm["QKeySequence::SelectEndOfLine"]	= 53; //	Extend selection to end of line.
		sm["QKeySequence::SelectNextChar"]	= 44; //	Extend selection to next character.
		sm["QKeySequence::SelectNextLine"]	= 48; //	Extend selection to next line.
		sm["QKeySequence::SelectNextPage"]	= 50; //	Extend selection to next page.
		sm["QKeySequence::SelectNextWord"]	= 46; //	Extend selection to next word.
		sm["QKeySequence::SelectPreviousChar"]	= 45; //	Extend selection to previous character.
		sm["QKeySequence::SelectPreviousLine"]	= 49; //	Extend selection to previous line.
		sm["QKeySequence::SelectPreviousPage"]	= 51; //	Extend selection to previous page.
		sm["QKeySequence::SelectPreviousWord"]	= 47; //	Extend selection to previous word.
		sm["QKeySequence::SelectStartOfBlock"]	= 54; //	Extend selection to the start of a text block. This shortcut is only used on OS X.
		sm["QKeySequence::SelectStartOfDocument"] = 56; //	Extend selection to start of document.
		sm["QKeySequence::SelectStartOfLine"] = 52; //	Extend selection to start of line.
		sm["QKeySequence::Underline"] = 29; //	Underline text.
		sm["QKeySequence::Undo"]	= 11; //	Undo.
		sm["QKeySequence::UnknownKey"]	= 0; //	Unbound key.
		sm["QKeySequence::WhatsThis"] = 2; //	Activate "what's this".
		sm["QKeySequence::ZoomIn"]	= 16; //	Zoom in.
		sm["QKeySequence::ZoomOut"]	= 17; //	Zoom out.
		sm["QKeySequence::FullScreen"]	= 66; //	Toggle the window state to/from full screen.
		_shortcut_map = sm;
	}
	_shortcut_map[

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
